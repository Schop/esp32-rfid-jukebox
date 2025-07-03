# Software Configuration Guide

## Development Environment Setup

### Prerequisites
1. **Visual Studio Code** - Main IDE
2. **PlatformIO Extension** - ESP32 development platform
3. **Git** - Version control (optional but recommended)

### Installation Steps
1. Install VS Code from [code.visualstudio.com](https://code.visualstudio.com/)
2. Install PlatformIO extension from VS Code marketplace
3. Clone/download the project repository
4. Open project folder in VS Code

## Project Configuration

### PlatformIO Settings
The `platformio.ini` file contains all build configurations:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
lib_deps = 
    miguelbalboa/MFRC522@^1.4.10
    dfrobot/DFRobotDFPlayerMini@^1.0.6
    ottowinter/ESPAsyncWebServer-esphome@^3.0.0
upload_protocol = esptool
upload_port = COM5  ; Change to your port
```

### Key Configuration Options

#### Library Dependencies
- **MFRC522**: RFID reader communication
- **DFRobotDFPlayerMini**: MP3 player control
- **ESPAsyncWebServer**: WiFi web interface

#### Upload Settings
- **Protocol**: `esptool` for USB upload
- **Port**: Change `COM5` to your actual COM port
- **Speed**: 921600 baud for faster uploads

## WiFi Configuration

### Method 1: Direct Code Edit
Edit `src/main.cpp` lines 69-70:
```cpp
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";
```

### Method 2: Separate Config File (Recommended)
1. Create `wifi_config.h` in project root
2. Add credentials:
```cpp
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

#endif
```
3. Include in main.cpp and update .gitignore

### WiFi Troubleshooting
- Use 2.4GHz networks only (ESP32 doesn't support 5GHz)
- Avoid special characters in passwords
- Check signal strength and distance
- Try different channels if interference

## Pin Configuration

All pin assignments are defined in `main.cpp`:

```cpp
// RFID Reader Pins
#define RST_PIN         21
#define SS_PIN          5

// Button Pins
#define RESET_BUTTON    32
#define PREV_BUTTON     33
#define NEXT_BUTTON     25
#define PLAY_PAUSE_BUTTON 26
#define SHUFFLE_BUTTON  27

// Volume Control
#define VOLUME_POT_PIN  34

// Volume Limits
#define MAX_VOLUME      25  // Prevent distortion
#define MIN_VOLUME      0
```

### Customizing Pins
To change pin assignments:
1. Verify pin capabilities (some are input-only)
2. Update `#define` statements
3. Check for conflicts with SPI/Serial pins
4. Update hardware documentation

## Audio Configuration

### SD Card Setup
1. Format as FAT32
2. Use quality Class 10 card (minimum)
3. File naming: `001.mp3`, `002.mp3`, etc.
4. Maximum ~255 files per folder

### Supported Audio Formats
- **MP3**: Primary format (recommended)
- **WAV**: Supported but larger files
- **Sample rates**: 8-48kHz
- **Bit rates**: 8-320kbps

### Volume Settings
```cpp
// Initial volume (0-25)
int currentVolume = 15;

// Update frequency (milliseconds)
unsigned long volumeCheckInterval = 100;
```

### Audio Quality Tips
- Use consistent bit rate (128kbps recommended)
- Normalize volume levels
- Use proper ID3 tags
- Avoid very long filenames

## Song Database Configuration

Edit the `getSongInfo()` function to match your music:

```cpp
String getSongInfo(int trackNumber) {
  switch (trackNumber) {
    case 1: return "Song Title - Artist Name";
    case 2: return "Another Song - Another Artist";
    // Add your songs here...
    default: return "Unknown Track #" + String(trackNumber);
  }
}
```

### Tips for Song Database
- Keep descriptions concise
- Include artist and title
- Use consistent formatting
- Update when adding new tracks

## Web Interface Customization

### Styling Changes
Edit the HTML in `setupWebServer()` function:
- Modify CSS for different colors/layout
- Add new buttons or controls
- Change responsive behavior

### Adding New Commands
1. Add case to `processCommand()` function
2. Update web interface HTML
3. Add command to help text
4. Test via serial and web

### API Extensions
Add new endpoints in `setupWebServer()`:
```cpp
server.on("/api/newfeature", HTTP_GET, [](AsyncWebServerRequest *request){
  // Handle new API endpoint
});
```

## Serial Communication

### Debug Output Levels
Control verbosity with build flags:
```ini
build_flags = 
    -DCORE_DEBUG_LEVEL=3  ; 0=None, 1=Error, 2=Warn, 3=Info, 4=Debug, 5=Verbose
```

### Serial Commands
Available debug commands:
- `s` - System status
- `v` - Volume level
- `+/-` - Volume control
- `l` - List songs
- `p` - Programming mode
- `r` - Reset system

### Custom Serial Commands
Add new commands in `handleSerialCommands()`:
```cpp
case 'x':  // New command
  // Your code here
  Serial.println("Command executed");
  break;
```

## Programming Mode Configuration

### Auto Mode Settings
```cpp
// Starting number for auto programming
int programmerCurrentNumber = 1;

// Auto mode ready flag
bool programmerAutoReady = false;
```

### Manual Mode Timeout
```cpp
const unsigned long timeout = 30000;  // 30 seconds
```

### Programming Workflow
1. Enter programming mode (`p` command)
2. Choose mode (`auto`, `manual`, `read`)
3. Follow prompts for card programming
4. Return to jukebox mode (`jukebox` command)

## Performance Tuning

### Timing Intervals
```cpp
unsigned long checkInterval = 5000;        // System health check
unsigned long volumeCheckInterval = 100;   // Volume pot reading
```

### Memory Optimization
- Use `F()` macro for string literals
- Minimize global variables
- Use appropriate data types
- Consider PROGMEM for large constants

### Responsiveness
- Keep loop() function lightweight
- Use non-blocking delays
- Prioritize critical functions
- Implement proper debouncing

## Building and Deployment

### Build Process
```bash
# Clean build
pio run --target clean

# Build only
pio run

# Build and upload
pio run --target upload

# Open serial monitor
pio device monitor
```

### Upload Methods
1. **USB**: Direct connection via USB cable
2. **Manual Boot**: Hold BOOT button while uploading
3. **Auto Reset**: Automatic if hardware supports it

### Troubleshooting Uploads
- Check COM port in device manager
- Try different USB cables
- Use manual boot mode
- Reduce upload speed if needed

## Security Considerations

### WiFi Security
- Use WPA2/WPA3 networks
- Avoid public WiFi
- Consider network isolation
- Use strong passwords

### Web Interface Security
- Runs on local network only
- No authentication implemented
- Consider adding basic auth if needed
- Monitor access logs

### RFID Security
- MIFARE Classic has known vulnerabilities
- Consider encryption for sensitive data
- Physical security of reader important
- Regular key rotation recommended

## Backup and Version Control

### Code Backup
- Use Git for version control
- Regular commits with descriptive messages
- Tag stable releases
- Backup project files regularly

### Configuration Backup
- Export PlatformIO settings
- Document custom pin assignments
- Save WiFi configurations separately
- Keep hardware documentation updated

### Recovery Procedures
- Keep bootloader backup
- Document recovery procedures
- Test restore procedures
- Maintain multiple development boards
