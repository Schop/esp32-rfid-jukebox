<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

# ESP32 RFID Jukebox - Copilot Instructions

This is an ESP32-based RFID jukebox project using PlatformIO in VS Code. The project converts an Arduino IDE project to ESP32 platform.

## Project Context
- **Platform**: ESP32 (Espressif32)
- **Framework**: Arduino
- **IDE**: VS Code with PlatformIO
- **Hardware**: RC522 RFID Reader, DFPlayer Mini MP3 Module, Control Buttons
- **Communication**: Hardware Serial (Serial2) for DFPlayer, SPI for RFID

## Key Technical Details
- Use ESP32 GPIO pins and HardwareSerial instead of Arduino Uno pins and SoftwareSerial
- ESP32 restart function: `ESP.restart()` (not Arduino's `asm volatile("jmp 0")`)
- Serial2 configuration: `HardwareSerial dfPlayerSerial(2)` with pins RX=16, TX=17
- RFID pins match the programmer project: SDA=5, RST=21, SCK=18, MOSI=23, MISO=19
- Button pins: Play/Pause=4, Next=3, Prev=7, Shuffle=2, Reset=8

## Libraries Used
- MFRC522 for RFID operations
- DFRobotDFPlayerMini for MP3 playback
- Built-in ESP32 libraries (Arduino.h, SPI.h, HardwareSerial.h)

## Code Style Preferences
- Use modern C++ features where appropriate
- Include helpful Serial.print statements with emojis for user feedback
- Maintain error handling and recovery mechanisms
- Use proper ESP32 pin definitions and hardware features
- Keep timing functions non-blocking
- Add clear comments explaining hardware connections and functionality

## Hardware Integration Notes
- DFPlayer Mini uses Hardware Serial2 (not SoftwareSerial)
- All buttons use INPUT_PULLUP configuration
- RFID reader shares SPI bus, uses same pins as programmer project
- Volume control range: 0-30
- Support for folder-based playlists using negative card numbers (-1 to -7)

When suggesting code improvements or additions, consider ESP32-specific features like dual-core processing, WiFi capabilities, and expanded GPIO options.
