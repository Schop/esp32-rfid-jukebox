# Getting Started with ESP32 RFID Jukebox

## Quick Start Guide

### 1. Hardware Setup
Follow the wiring diagram in the README.md to connect:
- RC522 RFID reader to ESP32 SPI pins
- DFPlayer Mini to ESP32 Serial2 pins (GPIO 16/17)
- 5 control buttons to specified GPIO pins
- Speaker to DFPlayer Mini

### 2. SD Card Preparation
1. Format a microSD card as FAT32
2. Copy your MP3 files named as: `001.mp3`, `002.mp3`, `003.mp3`, etc.
3. Optionally create folders `01/`, `02/`, `03/` for playlists
4. Insert SD card into DFPlayer Mini

### 3. RFID Card Programming
Use the companion RFID Programmer project to write numbers to your cards:
- Cards with numbers 1-999: Play specific tracks
- Cards with -1 to -6: Play from folders 01/ to 06/
- Card with -7: Master card (return to main folder)

### 4. Building and Uploading
1. Open this project in VS Code with PlatformIO
2. Connect your ESP32 via USB
3. Build: `Ctrl+Alt+B` or click ✓ button
4. Upload: `Ctrl+Alt+U` or click → button
5. Monitor: `Ctrl+Alt+S` or click 🔌 button

### 5. Testing
1. Open Serial Monitor (115200 baud)
2. Wait for "ESP32 RFID Jukebox Ready" message
3. Scan an RFID card to test playback
4. Try the physical buttons for control

### 6. Troubleshooting
- If DFPlayer doesn't initialize: Check wiring and SD card
- If no audio: Verify speaker connections and volume
- If cards aren't detected: Check RC522 connections
- For system errors: Use serial commands (s, r, v, +, -)

## Serial Commands for Testing
- `s` - Check DFPlayer status
- `v` - Show current volume
- `+` - Increase volume
- `-` - Decrease volume  
- `r` - Reset system

## File Organization
```
Your SD Card:
├── 001.mp3          # Song 1
├── 002.mp3          # Song 2
├── 003.mp3          # Song 3
├── 01/              # Playlist folder 1
│   ├── 001.mp3
│   └── 002.mp3
├── 02/              # Playlist folder 2
│   ├── 001.mp3
│   └── 002.mp3
└── ...
```

Happy listening! 🎵
