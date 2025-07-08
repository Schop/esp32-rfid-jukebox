/*
   ESP32 RFID Jukebox - Multi-Function Music Player
   Based on original Arduino code by ryand1011 and Ananords
   Converted to ESP32 with PlatformIO by GitHub Copilot
   
   Features:
   - RFID card-triggered MP3 playback
   - Physical button controls (play/pause, next, prev, shuffle, reset)
   - Playlist support with negative numbers (-1 to -6 for folders, -7 for shuffle)
   - Automatic error recovery and reset functionality
   - Hardware serial communication with DFPlayer Mini
   - WiFi web interface for remote control
   
   Hardware Setup:
   -----------------------------------------------------------------------------------------
   MFRC522 RFID Reader    ESP32        Description
   SDA                    GPIO 5       SPI signal input
   SCK                    GPIO 18      SPI clock
   MOSI                   GPIO 23      SPI data input
   MISO                   GPIO 19      SPI master-in-slave-out
   IRQ                    Don't connect Interrupt pin
   GND                    GND          Ground
   RST                    GPIO 21      Reset pin
   3.3V                   3.3V         Power supply (2.5-3.3V)
   
   DFPlayer Mini          ESP32        Description
   TX                     GPIO 16      Serial receive (ESP32 RX2)
   RX                     GPIO 17      Serial transmit (ESP32 TX2)
   VCC                    5V           Power supply
   GND                    GND          Ground
   SPK1/SPK2              Speaker      Audio output
   
   Control Buttons        ESP32        Description
   Play/Pause             GPIO 26      Play/pause toggle
   Next                   GPIO 25      Next track
   Previous               GPIO 33      Previous track
   Shuffle                GPIO 27      Shuffle/random play
   Reset                  GPIO 32      System reset
   
   Volume Control         ESP32        Description
   Software Control       N/A          Volume control via serial/web commands (0-30, default 25)
   -----------------------------------------------------------------------------------------
*/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// ESP32 Pin definitions for RC522 (same as RFID programmer)
#define RST_PIN         21          // Reset pin
#define SS_PIN          5           // SDA (SS) pin

// ESP32 Pin definitions for buttons (using safe GPIO pins)
#define RESET_BUTTON    32          // Reset button (safe pin)
#define PREV_BUTTON     33          // Previous track button (safe pin)
#define NEXT_BUTTON     25          // Next track button (safe pin)
#define PLAY_PAUSE_BUTTON 26        // Play/pause button (safe pin)
#define SHUFFLE_BUTTON  27          // Shuffle button (safe pin)

// Volume control constants
#define MAX_VOLUME      30          // Maximum volume
#define MIN_VOLUME      0           // Minimum volume

// WiFi credentials for web interface
// TODO: Replace with your actual WiFi credentials
const char* ssid = "Odido-16EBE7";
const char* password = "WHJYVE7LUUSV37YL";

// ESP32 Hardware Serial for DFPlayer Mini (Serial2)
HardwareSerial dfPlayerSerial(2);   // Use Serial2 (GPIO16=RX, GPIO17=TX)

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);       // Create MFRC522 instance
DFRobotDFPlayerMini myDFPlayer;         // Create DFPlayer instance

// Web server for WiFi commands
AsyncWebServer server(80);              // Web server on port 80

// WiFi command response buffer
String wifiResponse = "";               // Buffer for web interface responses

// Function prototypes
void handleButtons();
void handleRFID();
void handleSerialCommands();
void performSystemCheck();
void playCardNumber(int number);
String getSongInfo(int trackNumber);
boolean TimePeriodIsOver(unsigned long &startOfPeriod, unsigned long TimePeriod);

// WiFi functions
void setupWiFi();
void handleWiFiConnection();
bool wifiSetupComplete = false;

// WiFi web interface functions
void setupWebServer();
void handleWebCommand(char command);
String processCommand(char command);
String processJukeboxCommand();

// RFID Programming functions
void programmerMode();
void autoModus();
void manualModus();
void readModus();

// Player state variables
boolean isPlaying = false;
boolean autoPlay = false;
boolean msgSent = false;
boolean msgSent2 = false;
int currentSong = 0;

// System mode variables
bool jukeboxMode = true;            // true = jukebox, false = programmer
String programmerModeType = "";     // "auto", "manual", "read"
int programmerCurrentNumber = 1;    // For auto programming mode
bool programmerAutoReady = false;   // Flag for auto mode ready state

// Timing variables
unsigned long myBlinktimer;
unsigned long starttime;
unsigned long endtime;

// System monitoring variables
unsigned long checkTimer = 0;          // Timer for system health check
unsigned long checkInterval = 5000;    // Check every 5 seconds

// Volume management
int currentVolume = 25;                 // Track current volume (0-30, default 25)

// WiFi management
bool wifiConnected = false;            // Track WiFi connection status
unsigned long wifiStartTime = 0;      // WiFi connection start time
unsigned long wifiTimeout = 10000;    // WiFi connection timeout (10 seconds)
bool wifiSetupStarted = false;         // Track if WiFi setup has started

// Button state variables
bool previousNextButtonState = HIGH;
bool previousPrevButtonState = HIGH;
bool previousPlayPauseButtonState = HIGH;
bool previousShuffleButtonState = HIGH;

//*****************************************************************************
void setup() {
  Serial.begin(115200);                     // ESP32 standard serial speed
  delay(1000);                              // Give serial time to start
  
  Serial.println(F("\n=== ESP32 RFID Jukebox Starting ==="));
  Serial.println(F("Step 1: Serial initialized"));
  
  // Initialize SPI first
  SPI.begin();                              // Init SPI bus
  Serial.println(F("Step 2: SPI initialized"));
  delay(100);
  
  // Initialize RFID
  mfrc522.PCD_Init();                       // Init MFRC522 card
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  Serial.println(F("Step 3: RFID initialized"));
  
  // Initialize button pins with internal pull-up resistors
  pinMode(PLAY_PAUSE_BUTTON, INPUT_PULLUP);
  pinMode(SHUFFLE_BUTTON, INPUT_PULLUP);
  pinMode(PREV_BUTTON, INPUT_PULLUP);
  pinMode(NEXT_BUTTON, INPUT_PULLUP);
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  Serial.println(F("Step 4: Buttons initialized"));

  // Initialize DFPlayer (this might be the problem)
  Serial.println(F("Step 5: Initializing DFPlayer Mini... (May take 3~5 seconds)"));
  dfPlayerSerial.begin(9600, SERIAL_8N1, 16, 17); // Hardware Serial2: RX=16, TX=17
  delay(500);

  if (!myDFPlayer.begin(dfPlayerSerial)) {  // Use Hardware Serial
    Serial.println(F("WARNING: DFPlayer initialization failed:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    Serial.println(F("3. Check if DFPlayer Mini is powered correctly!"));
    Serial.println(F("WARNING: Continuing without DFPlayer..."));
  } else {
    Serial.println(F("SUCCESS: DFPlayer Mini online!"));
    myDFPlayer.setTimeOut(1000);
    myDFPlayer.EQ(DFPLAYER_EQ_BASS);
    myDFPlayer.volume(currentVolume);  // Set volume using our variable
    Serial.print(F("VOLUME: Volume set to: "));
    Serial.println(currentVolume);
  }
  
  // Start WiFi connection in non-blocking mode
  Serial.println(F("Step 6: Starting WiFi connection (non-blocking)..."));
  setupWiFi();
  
  Serial.println(F("=== ESP32 RFID Jukebox Ready ==="));
  Serial.println(F("JUKEBOX: Place an RFID card on the reader to play a song"));
  Serial.println(F("Use buttons for manual control:"));
  Serial.println(F("- Play/Pause: GPIO 26"));
  Serial.println(F("- Next: GPIO 25"));
  Serial.println(F("- Previous: GPIO 33"));
  Serial.println(F("- Shuffle: GPIO 27"));
  Serial.println(F("- Reset: GPIO 32"));
  Serial.println(F("VOLUME: Software control via serial/web commands (0-30, default 25)"));
  Serial.println(F("\nPROGRAMMING: Type 'program' to enter card programming mode"));
  Serial.println(F("COMMANDS: l=song list, v=volume, +=vol up, -=vol down, s=status"));
  Serial.println(F("WEB: Web Interface will be available once WiFi connects..."));
}

//*****************************************************************************
void loop() {
  // Handle WiFi connection in background
  if (!wifiSetupComplete) {
    handleWiFiConnection();
  }
  
  if (jukeboxMode) {
    // Jukebox mode - normal operation
    handleButtons();
    handleRFID();
    handleSerialCommands();
    // performSystemCheck();
  } else {
    // Programming mode - RFID card programming
    programmerMode();
  }
}

//*****************************************************************************
void handleButtons() {
  // Read current button states
  bool currentNextButtonState = digitalRead(NEXT_BUTTON);
  bool currentPrevButtonState = digitalRead(PREV_BUTTON);
  bool currentPlayPauseButtonState = digitalRead(PLAY_PAUSE_BUTTON);
  bool currentShuffleButtonState = digitalRead(SHUFFLE_BUTTON);
  bool currentResetButtonState = digitalRead(RESET_BUTTON);

  // Check for falling edge on play/pause button
  if (currentPlayPauseButtonState == LOW && previousPlayPauseButtonState == HIGH) {
    if (isPlaying) {
      myDFPlayer.pause();
      isPlaying = false;
      Serial.println("PAUSE: Paused");
    } else {
      myDFPlayer.start();
      isPlaying = true;
      Serial.println("PLAY: Playing");
    }
    delay(50); // Debounce delay
  }

  // Check for falling edge on shuffle button
  if (currentShuffleButtonState == LOW && previousShuffleButtonState == HIGH) {
    myDFPlayer.randomAll();
    Serial.println("SHUFFLE: Shuffle Play");
    isPlaying = true;
    delay(50); // Debounce delay
  }

  // Check for falling edge on next button
  if (currentNextButtonState == LOW && previousNextButtonState == HIGH) {
    myDFPlayer.next();
    currentSong = currentSong + 1;
    Serial.println("NEXT: Next track");
    delay(50); // Debounce delay
  }

  // Check for falling edge on prev button
  if (currentPrevButtonState == LOW && previousPrevButtonState == HIGH) {
    myDFPlayer.previous();
    currentSong = currentSong - 1;
    Serial.println("PREVIOUS: Previous track");
    delay(50); // Debounce delay
  }

  // Check for reset button
  if (currentResetButtonState == LOW) {
    Serial.println("Reset button pressed - Restarting ESP32...");
    delay(1000);
    ESP.restart(); // ESP32 restart function
  }

  // Update previous button states
  previousNextButtonState = currentNextButtonState;
  previousPrevButtonState = currentPrevButtonState;
  previousPlayPauseButtonState = currentPlayPauseButtonState;
  previousShuffleButtonState = currentShuffleButtonState;
}

//*****************************************************************************
void handleRFID() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Variables for RFID operations
  byte block;
  byte len;
  MFRC522::StatusCode status;

  // Check if a new card is present on the sensor/reader
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    
    Serial.println(F("\nCARD: **Card Detected**"));
    
    // Dump some details about the card
    Serial.print(F("Card UID: "));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    //-------------------------------------------
    // Read the number stored on the card
    Serial.print(F("Reading number: "));

    byte buffer2[18];
    block = 1;
    len = 18;

    // Authenticate using key A
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    // Read the block
    status = mfrc522.MIFARE_Read(block, buffer2, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    // Convert buffer to string
    String number = "";
    for (uint8_t i = 0; i < 16; i++) {
      if (buffer2[i] != ' ' && buffer2[i] != 0) {
        number += (char)buffer2[i];
      }
    }
    number.trim();
    
    if (number.length() == 0) {
      Serial.println("No number found on card");
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      return;
    }
    
    Serial.print(number);
    Serial.print(" -> ");

    // Handle playlist cards (negative numbers) and regular song cards
    playCardNumber(number.toInt());

    Serial.println(F("**End Reading**\n"));
    delay(250); // Delay to prevent rapid re-reading
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

//*****************************************************************************
void playCardNumber(int number) {
  // Handle special playlist cards (negative numbers)
  if (number >= -7 && number <= -1) {
    int folderNumber = abs(number);
    
    if (number == -7) {
      // Shuffle card - start random play mode
      myDFPlayer.randomAll();
      Serial.println("SHUFFLE: Shuffle mode activated - Playing random tracks");
      isPlaying = true;
    } else {
      // Play from specific folder
      myDFPlayer.playLargeFolder(folderNumber, 1);
      Serial.print("Playing from folder ");
      Serial.println(folderNumber);
      isPlaying = true;
    }
    
    Serial.print(F("Volume: "));
    Serial.println(currentVolume);
  }
  else if (number > 0) {
    // Regular song card - play specific track number
    myDFPlayer.stop();
    delay(100);
    myDFPlayer.play(number);
    currentSong = number;
    isPlaying = true;
    
    Serial.print("PLAY: Playing track #");
    Serial.println(number);
    Serial.print("TRACK: ");
    Serial.println(getSongInfo(number));
    Serial.print(F("Volume: "));
    Serial.println(currentVolume);
  }
  else {
    Serial.println("Invalid card number");
  }
}

//*****************************************************************************
void handleSerialCommands() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 's':
        // Check player state
        {
          uint8_t state = myDFPlayer.readState();
          Serial.print("DFPlayer state: ");
          Serial.println(state);
        }
        break;
        
      case 'r':
        // Manual reset command
        Serial.println("Manual reset command received");
        ESP.restart();
        break;
        
      case 'v':
        // Volume info
        Serial.print("Current volume: ");
        Serial.println(currentVolume);
        break;
        
      case '+':
        // Volume up
        if (currentVolume < MAX_VOLUME) {
          currentVolume++;
          myDFPlayer.volume(currentVolume);
          Serial.print("VOLUME: Volume up: ");
          Serial.println(currentVolume);
        } else {
          Serial.print("VOLUME: Volume already at maximum (");
          Serial.print(MAX_VOLUME);
          Serial.println(")");
        }
        break;
        
      case '-':
        // Volume down
        if (currentVolume > MIN_VOLUME) {
          currentVolume--;
          myDFPlayer.volume(currentVolume);
          Serial.print("VOLUME: Volume down: ");
          Serial.println(currentVolume);
        } else {
          Serial.print("VOLUME: Volume already at minimum (");
          Serial.print(MIN_VOLUME);
          Serial.println(")");
        }
        break;
        
      case 'l':
        // List all songs
        Serial.println(F("\n=== Song List ==="));
        for (int i = 1; i <= 31; i++) {
          Serial.print("Track ");
          if (i < 10) Serial.print("0");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(getSongInfo(i));
        }
        Serial.println(F("===================\n"));
        break;
        
      case 'p':
        // Enter programming mode
        if (jukeboxMode) {
          jukeboxMode = false;
          programmerModeType = "";
          programmerAutoReady = false;
          Serial.println(F("\n=== PROGRAMMING MODE ACTIVATED ==="));
          Serial.println(F("Write 'auto' for Automatic mode, 'manual' for Manual mode, or 'read' to read cards"));
          Serial.println(F("Type 'jukebox' to return to jukebox mode"));
        }
        break;
        
      default:
        if (jukeboxMode) {
          Serial.println("Commands: s=state, r=reset, v=volume, +=vol up, -=vol down, l=list songs, p=program mode");
        }
        break;
    }
  }
}

//*****************************************************************************
void performSystemCheck() {
  // Periodic system health check
  if (TimePeriodIsOver(checkTimer, checkInterval)) {
    uint8_t state = myDFPlayer.readState();
    Serial.print("CHECK: System check - DFPlayer state: ");
    Serial.println(state);
    
    if (state == 255) {
      // Error state detected - attempt recovery
      unsigned long startTime = millis();
      int count = 1;
      
      Serial.println("Error state detected, attempting recovery...");
      
      while (millis() - startTime < 1000) { // Wait for 1 second
        state = myDFPlayer.readState();
        Serial.print("Recovery attempt: ");
        Serial.println(count);
        count++;
        
        if (state != 255) {
          Serial.println("Recovery successful!");
          break;
        }
        delay(100);
      }

      if (state == 255) {
        Serial.println("Recovery failed - Restarting ESP32...");
        delay(1000);
        ESP.restart();
      }
    }
  }
}

//*****************************************************************************
// RFID Programming Mode Functions
//*****************************************************************************

void programmerMode() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Handle auto mode setup
    if (programmerModeType == "auto" && !programmerAutoReady) {
      int startingNumber = input.toInt();
      if (startingNumber > 0 || input == "0") {
        programmerCurrentNumber = startingNumber;
        programmerAutoReady = true;
        Serial.println("Auto mode ready! Starting number: " + String(programmerCurrentNumber));
        Serial.println(F("Place the card on the reader and hold it there to write song number data to the card"));
        Serial.println(F("(Type 'jukebox' to return to jukebox mode)"));
      } else {
        Serial.println("Please enter a valid number (0 or greater):");
      }
      return;
    }

    // Handle mode changes
    if (input == "auto" || input == "manual" || input == "read") {
      programmerModeType = input;
      programmerAutoReady = false;
      
      Serial.println("Device is now in " + programmerModeType + " programming mode");
      
      if (programmerModeType == "auto") {
        Serial.println(F("Please enter the starting number:"));
      } else if (programmerModeType == "read") {
        Serial.println(F("Place the card on the reader to read its number"));
      } else {
        Serial.println(F("Place the card on the reader and hold it there to write song number data to the card"));
      }
    } else if (input == "jukebox") {
      // Return to jukebox mode
      jukeboxMode = true;
      programmerModeType = "";
      programmerAutoReady = false;
      Serial.println(F("\n=== JUKEBOX MODE ACTIVATED ==="));
      Serial.println(F("Place an RFID card on the reader to play a song"));
    } else if (programmerModeType != "auto" && programmerModeType != "manual" && programmerModeType != "read") {
      Serial.println("Programming commands: 'auto', 'manual', 'read', or 'jukebox'");
    }
  }

  // Execute the current programming mode functions
  if (programmerModeType == "auto" && programmerAutoReady) {
    autoModus();
  } else if (programmerModeType == "manual") {
    manualModus();
  } else if (programmerModeType == "read") {
    readModus();
  }
}

void autoModus() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if (mfrc522.PICC_IsNewCardPresent()) {
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    Serial.print(F("Card UID:"));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.print(F(" PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    byte buffer[16];
    MFRC522::StatusCode status;
    byte block = 1;
    byte len;

    len = sprintf((char*)buffer, "%d", programmerCurrentNumber);
    for (byte i = len; i < 16; i++) buffer[i] = ' ';

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Write failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    programmerCurrentNumber++;
    Serial.println("SUCCESS: Card written successfully! Number: " + String(programmerCurrentNumber - 1) + " (" + getSongInfo(programmerCurrentNumber - 1) + ")");
    Serial.println("Next number: " + String(programmerCurrentNumber));
    Serial.println("Put a new card on the reader...");
  }
}

void manualModus() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  Serial.println(F("Type any number and hit send/enter (or type 'jukebox' to return):"));
  
  String userInput = "";
  unsigned long startTime = millis();
  const unsigned long timeout = 30000;
  
  while (userInput == "" && (millis() - startTime) < timeout) {
    if (Serial.available()) {
      userInput = Serial.readStringUntil('\n');
      userInput.trim();
      break;
    }
    delay(10);
  }
  
  if (userInput == "jukebox") {
    jukeboxMode = true;
    programmerModeType = "";
    Serial.println("Returning to jukebox mode");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }
  
  if (userInput == "") {
    Serial.println("Timeout - no number entered");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  byte buffer[16];
  byte block = 1;
  MFRC522::StatusCode status;
  
  for (int i = 0; i < 16; i++) buffer[i] = ' ';
  int copyLength = (userInput.length() + 1 < 16) ? userInput.length() + 1 : 16;
  userInput.toCharArray((char*)buffer, copyLength);

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Write failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.println("SUCCESS: Card written successfully with: " + userInput + " (" + getSongInfo(userInput.toInt()) + ")");
  Serial.println("Put a new card on the reader to write another number");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void readModus() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.print(F(" PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte block = 1;
  byte size = sizeof(buffer);
  MFRC522::StatusCode status;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.print(F("Number stored on card: "));
    
    String cardData = "";
    for (byte i = 0; i < 16; i++) {
      if (buffer[i] != ' ' && buffer[i] != 0) {
        cardData += (char)buffer[i];
      }
    }
    
    if (cardData.length() > 0) {
      Serial.print(cardData);
      Serial.print(" (");
      Serial.print(getSongInfo(cardData.toInt()));
      Serial.println(")");
    } else {
      Serial.println("No number found or card is empty");
    }
  }

  Serial.println("Place another card to read, or type 'jukebox' to return");
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

//*****************************************************************************
// Get song title and artist information
String getSongInfo(int trackNumber) {
  switch (trackNumber) {
    case 1: return "Did Jesus Have a Baby Sister - Dory Previn";
    case 2: return "That's All Right - Elvis Presley";
    case 3: return "Hey Joe - Jimi Hendrix";
    case 4: return "Delia's Gone - Johnny Cash";
    case 5: return "In Da Club - 50 Cent";
    case 6: return "Keep the Customer Satisfied - Simon & Garfunkel";
    case 7: return "Thrift Shop - Macklemore & Ryan Lewis";
    case 8: return "Old Man - Neil Young";
    case 9: return "Never Going Back Again - Fleetwood Mac";
    case 10: return "Norwegian Wood (This Bird Has Flown) - The Beatles";
    case 11: return "Chain Gang - Sam Cooke";
    case 12: return "Yakety Yak - The Coasters";
    case 13: return "I've Been Everywhere - Johnny Cash";
    case 14: return "Thunderstruck - AC/DC";
    case 15: return "Duurt Te Lang - Davina Michelle";
    case 16: return "Alles Gaat Voorbij - Doe Maar";
    case 17: return "The Painter - William Ben";
    case 18: return "Think - Aretha Franklin";
    case 19: return "Scotland the Brave - Auld Town Band & Pipes";
    case 20: return "Single Ladies - Beyoncé";
    case 21: return "Grandma's Hands - Bill Withers";
    case 22: return "Without Me - Eminem";
    case 23: return "Spraakwater - Extince";
    case 24: return "King of the World - First Aid Kit";
    case 25: return "Komodovaraan - Yentl en De Boer";
    case 26: return "Look What They've Done To My Song, Ma - Melanie";
    case 27: return "The Man Who Sold The World - Nirvana";
    case 28: return "Rotterdam - Pokey LaFarge";
    case 29: return "'t Roeie Klied - Rowwen Heze";
    case 30: return "You Never Can Tell - Chuck Berry";
    case 31: return "Sit Still, Look Pretty - Daya";
    default: return "Unknown Track #" + String(trackNumber);
  }
}

//*****************************************************************************
// Helper function for non-blocking timing
boolean TimePeriodIsOver(unsigned long &startOfPeriod, unsigned long TimePeriod) {
  unsigned long currentMillis = millis();
  if (currentMillis - startOfPeriod >= TimePeriod) {
    startOfPeriod = currentMillis; // Start new period
    return true;
  }
  return false;
}

//*****************************************************************************
// WiFi Functions
//*****************************************************************************

void setupWiFi() {
  Serial.print(F("WIFI: Starting WiFi connection to: "));
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  wifiStartTime = millis();
  wifiSetupStarted = true;
  Serial.println(F("WIFI: WiFi connecting in background..."));
}

void handleWiFiConnection() {
  if (!wifiSetupStarted) return;
  
  unsigned long currentTime = millis();
  
  // Check WiFi status
  if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
    // WiFi just connected
    wifiConnected = true;
    wifiSetupComplete = true;
    
    Serial.println();
    Serial.println(F("SUCCESS: WiFi connected!"));
    Serial.print(F("NETWORK: IP address: "));
    Serial.println(WiFi.localIP());
    
    // Now setup the web server
    setupWebServer();
    Serial.print(F("WEB: Web Interface: Available at http://"));
    Serial.print(WiFi.localIP());
    Serial.println(F("/"));
    
  } else if (currentTime - wifiStartTime > wifiTimeout && !wifiConnected) {
    // WiFi connection timeout
    wifiSetupComplete = true;
    Serial.println();
    Serial.println(F("WARNING: WiFi connection timeout - continuing without web interface"));
    Serial.println(F("STATUS: Music playback is fully functional without WiFi"));
    
  } else if (!wifiConnected && (currentTime - wifiStartTime) % 1000 == 0) {
    // Print a dot every second while connecting (non-blocking)
    static unsigned long lastDotTime = 0;
    if (currentTime - lastDotTime >= 1000) {
      Serial.print(".");
      lastDotTime = currentTime;
    }
  }
}

//*****************************************************************************
// WiFi Web Interface Functions
//*****************************************************************************

void setupWebServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WARNING: Cannot setup Web Server - WiFi not connected"));
    return;
  }
  
  // Main web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 RFID Jukebox</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; margin: 20px; background-color: #f0f0f0; }
    .container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
    h1 { color: #333; text-align: center; }
    .section { margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }
    .button { display: inline-block; margin: 5px; padding: 10px 20px; background-color: #007bff; color: white; text-decoration: none; border-radius: 5px; border: none; cursor: pointer; }
    .button:hover { background-color: #0056b3; }
    .control { background-color: #17a2b8; }
    .system { background-color: #dc3545; }
    .response { background-color: #f8f9fa; padding: 10px; border-radius: 5px; margin-top: 10px; min-height: 50px; border: 1px solid #dee2e6; }
    .command-input { width: 100%; padding: 10px; margin: 5px 0; border: 1px solid #ddd; border-radius: 5px; }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP32 RFID Jukebox</h1>
    
    <div class="section">
      <h3>Music Control</h3>
      <button onclick="sendCommand('s')" class="button control">Player Status</button>
      <button onclick="sendCommand('l')" class="button control">List Songs</button>
    </div>
    
    <div class="section">
      <h3>Programming Mode</h3>
      <button onclick="sendCommand('p')" class="button control">Enter Programming</button>
      <button onclick="sendCommand('jukebox')" class="button control">Return to Jukebox</button>
    </div>
    
    <div class="section">
      <h3>System Control</h3>
      <button onclick="sendCommand('r')" class="button system">Reset ESP32</button>
    </div>
    
    <div class="section">
      <h3>Custom Command</h3>
      <input type="text" id="customCmd" class="command-input" placeholder="Enter single character command (s, l, p, r)">
      <button onclick="sendCustomCommand()" class="button">Send Command</button>
    </div>
    
    <div class="section">
      <h3>Response</h3>
      <div id="response" class="response">Click a button to see the response...</div>
    </div>
  </div>
  
  <script>
    function sendCommand(cmd) {
      const responseDiv = document.getElementById('response');
      responseDiv.innerHTML = 'Sending command...';
      
      fetch('/cmd?c=' + cmd)
        .then(response => response.text())
        .then(data => {
          // Wait a moment for the command to be processed
          setTimeout(() => {
            fetch('/response')
              .then(response => response.text())
              .then(data => {
                responseDiv.innerHTML = data.replace(/\n/g, '<br>');
              })
              .catch(error => {
                responseDiv.innerHTML = 'Error fetching response: ' + error;
              });
          }, 500);
        })
        .catch(error => {
          responseDiv.innerHTML = 'Error sending command: ' + error;
        });
    }
    
    function sendCustomCommand() {
      const cmd = document.getElementById('customCmd').value;
      if (cmd.length >= 1) {
        sendCommand(cmd);
      } else {
        alert('Please enter a command');
      }
    }
  </script>
</body>
</html>
)rawliteral";
    request->send(200, "text/html", html);
  });
  
  // Command endpoint
  server.on("/cmd", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("c")) {
      String command = request->getParam("c")->value();
      if (command.length() >= 1) {
        // Handle 'jukebox' command specially
        if (command == "jukebox") {
          wifiResponse = processJukeboxCommand();
        } else {
          char cmd = command.charAt(0);
          wifiResponse = processCommand(cmd);
        }
        request->send(200, "text/plain", "Command processed");
      } else {
        request->send(400, "text/plain", "Invalid command");
      }
    } else {
      request->send(400, "text/plain", "Missing command parameter");
    }
  });
  
  // Response endpoint
  server.on("/response", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", wifiResponse);
  });
  
  // API endpoint for programmatic access
  server.on("/api/command", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("cmd")) {
      String command = request->getParam("cmd")->value();
      if (command.length() == 1) {
        char cmd = command.charAt(0);
        String response = processCommand(cmd);
        request->send(200, "application/json", 
          "{\"command\":\"" + command + "\",\"response\":\"" + response + "\"}");
      } else {
        request->send(400, "application/json", 
          "{\"error\":\"Command must be a single character\"}");
      }
    } else {
      request->send(400, "application/json", 
        "{\"error\":\"Missing cmd parameter\"}");
    }
  });
  
  server.begin();
  Serial.println(F("SUCCESS: Web Server Ready"));
  Serial.print(F("WEB: Web Interface: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/"));
}

String processCommand(char command) {
  wifiResponse = ""; // Clear previous response
  
  switch (command) {
    case 's':
      {
        uint8_t state = myDFPlayer.readState();
        wifiResponse = "DFPlayer state: " + String(state);
      }
      break;
      
    case 'r':
      wifiResponse = "Manual reset command received - ESP32 will restart";
      delay(1000);
      ESP.restart();
      break;
      
    case 'l':
      wifiResponse = "=== Song List ===\n";
      for (int i = 1; i <= 31; i++) {
        wifiResponse += "Track ";
        if (i < 10) wifiResponse += "0";
        wifiResponse += String(i) + ": " + getSongInfo(i) + "\n";
      }
      wifiResponse += "===================";
      break;
      
    case 'p':
      if (jukeboxMode) {
        jukeboxMode = false;
        programmerModeType = "";
        programmerAutoReady = false;
        wifiResponse = "PROGRAM: === PROGRAMMING MODE ACTIVATED ===\n";
        wifiResponse += "Note: Programming mode works best with Serial Monitor.\n";
        wifiResponse += "Use Serial Monitor for: 'auto', 'manual', 'read', or 'jukebox'";
      } else {
        wifiResponse = "Already in programming mode";
      }
      break;
      
    default:
      wifiResponse = "Unknown command: " + String(command) + "\n";
      wifiResponse += "Available commands: s=state, r=reset, l=list songs, p=program mode";
      break;
  }
  
  return wifiResponse;
}

String processJukeboxCommand() {
  if (!jukeboxMode) {
    jukeboxMode = true;
    programmerModeType = "";
    programmerAutoReady = false;
    return "JUKEBOX: === JUKEBOX MODE ACTIVATED ===\nPlace an RFID card on the reader to play a song";
  } else {
    return "Already in jukebox mode";
  }
}
