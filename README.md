# ESP32 RFID Jukebox

An ESP32-based RFID jukebox that plays MP3 files when RFID cards are scanned. Converted from Arduino IDE to PlatformIO for enhanced development experience and ESP32 optimization.

## Features

### 🎵 Music Playbook
- **RFID-triggered playbook**: Scan a card to instantly play the corresponding song
- **Physical button controls**: Play/pause, next, previous, shuffle, and reset
- **Playlist support**: Special cards (negative numbers) trigger folder-based playlists
- **Volume control**: Serial commands for volume adjustment (0-30 range)
- **Integrated RFID programming**: Built-in card programming functionality

### 🔧 Technical Features
- **Hardware Serial**: Uses ESP32's Serial2 for reliable DFPlayer communication
- **Error recovery**: Automatic system health monitoring and recovery
- **Non-blocking operation**: Responsive button handling and RFID detection
- **Debug interface**: Serial commands for testing and diagnostics
- **Dual-mode operation**: Switch between jukebox and programming modes
- **🕸️ WiFi Web Interface**: Control via web browser on any device

### 🎛️ Control Options
- **RFID Cards**: Primary music selection method
- **Physical Buttons**: Manual playback control
- **Serial Commands**: Debug and volume control interface
- **📡 Wireless Updates**: Upload new firmware over WiFi
- **🌐 Web Interface**: Control via browser (http://esp32-jukebox.local)
- **📱 Mobile Control**: Works on phones, tablets, and computers

## Hardware Requirements

### Components
- ESP32 development board
- RC522 RFID reader module
- DFPlayer Mini MP3 module
- MicroSD card (for MP3 files)
- 5 push buttons
- Speaker (3W 4Ω recommended)
- Jumper wires and breadboard/PCB

### Wiring Diagram

#### RC522 RFID Reader → ESP32
| RC522 Pin | ESP32 Pin | Description |
|-----------|-----------|-------------|
| SDA       | GPIO 5    | SPI Slave Select |
| SCK       | GPIO 18   | SPI Clock |
| MOSI      | GPIO 23   | SPI Master Out |
| MISO      | GPIO 19   | SPI Master In |
| RST       | GPIO 21   | Reset |
| 3.3V      | 3.3V      | Power |
| GND       | GND       | Ground |

#### DFPlayer Mini → ESP32
| DFPlayer Pin | ESP32 Pin | Description |
|--------------|-----------|-------------|
| TX           | GPIO 16   | Serial Receive (ESP32 RX2) |
| RX           | GPIO 17   | Serial Transmit (ESP32 TX2) |
| VCC          | 5V        | Power |
| GND          | GND       | Ground |
| SPK1/SPK2    | Speaker   | Audio Output |

#### Control Buttons → ESP32
| Button Function | ESP32 Pin | Description |
|----------------|-----------|-------------|
| Play/Pause     | GPIO 26   | Toggle playback |
| Next Track     | GPIO 25   | Next song |
| Previous Track | GPIO 33   | Previous song |
| Shuffle        | GPIO 27   | Random play |
| Reset          | GPIO 32   | System reset |

*All buttons use internal pull-up resistors (INPUT_PULLUP)*
*These are "safe" GPIO pins that won't interfere with ESP32 boot process*

## Software Setup

### Prerequisites
- VS Code with PlatformIO extension
- ESP32 board package
- USB cable for programming

### Installation

#### Initial Setup (USB Required)
1. **Clone or download** this project
2. **Open in VS Code** with PlatformIO
3. **Configure WiFi**: Edit `main.cpp` and update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
4. **Build the project**: Click the build button (✓) or use `Ctrl+Alt+B`
5. **Upload to ESP32**: Click upload button (→) or use `Ctrl+Alt+U`

### SD Card Setup
1. **Format** a microSD card as FAT32
2. **Create folders** (optional, for playlist cards):
   - `01/` - Folder 1 (triggered by card with number -1)
   - `02/` - Folder 2 (triggered by card with number -2)
   - `03/` - Folder 3 (triggered by card with number -3)
   - etc.
3. **Copy MP3 files**:
   - Root directory: `001.mp3`, `002.mp3`, `003.mp3`, etc.
   - In folders: `001.mp3`, `002.mp3`, etc.
4. **Insert SD card** into DFPlayer Mini

### RFID Card Programming
**No separate programmer needed!** The jukebox includes integrated RFID programming:

#### Programming Mode:
1. **Enter programming mode**: Type `p` in Serial Monitor
2. **Choose programming type**:
   - `auto` - Sequential numbering starting from your chosen number
   - `manual` - Enter specific numbers for each card
   - `read` - Read and display existing card numbers
3. **Program your cards**: Place cards on reader as prompted
4. **Return to jukebox**: Type `jukebox` when done

#### Quick Setup for New Cards:
```
1. Type: p                 (enter programming mode)
2. Type: auto              (automatic sequential programming)  
3. Type: 1                 (start numbering from 1)
4. Place card 1 on reader  (writes number 1)
5. Place card 2 on reader  (writes number 2)
6. Continue for all cards...
7. Type: jukebox          (return to music mode)
```

## Usage

### Basic Operation
1. **Power on** the ESP32
2. **Wait for initialization** (DFPlayer setup takes 3-5 seconds)
3. **Scan an RFID card** to play the corresponding song
4. **Use buttons** for manual control during playback

### Button Controls
- **Play/Pause**: Toggle between play and pause
- **Next**: Skip to next track
- **Previous**: Go to previous track  
- **Shuffle**: Start random playback from all songs
- **Reset**: Restart the ESP32 system

### Serial Commands (Debug)
Connect to Serial Monitor (115200 baud) and send:

**Jukebox Mode:**
- `s` - Check DFPlayer status
- `r` - Manual system reset
- `v` - Display current volume
- `+` - Increase volume
- `-` - Decrease volume
- `l` - List all available songs with track numbers
- `p` - Enter RFID programming mode

**Programming Mode:**
- `auto` - Start automatic sequential programming
- `manual` - Manual number entry for each card
- `read` - Read and display card contents
- `jukebox` - Return to jukebox mode

### 🌐 WiFi Web Interface
Access the web interface on any device connected to your network:

**Web Access:**
- **URL**: `http://esp32-jukebox.local` (or use IP address from Serial Monitor)
- **Mobile-friendly**: Works on phones, tablets, computers
- **Real-time control**: All serial commands available via web buttons
- **API access**: HTTP endpoints for programmatic control

**Available via Web:**
- 🎛️ **Volume Control**: Check volume, volume up/down
- 🎵 **Music Control**: Player status, song list
- 🔧 **Programming Mode**: Enter/exit programming mode
- 🔧 **System Control**: Reset ESP32
- 💬 **Custom Commands**: Send any single character command

**📋 For detailed web interface guide, see [WIFI_WEB_INTERFACE.md](WIFI_WEB_INTERFACE.md)**

### Card Number System
- **1-999**: Play specific track number from root directory
- **-1 to -6**: Play from corresponding folder (01/ to 06/)
- **-7**: Master card - return to main MP3 folder

## Project Structure

```
jukebox/
├── src/
│   └── main.cpp           # Main application code
├── lib/                   # Project libraries (empty)
├── include/               # Header files (empty)
├── .github/
│   └── copilot-instructions.md  # Copilot customization
├── platformio.ini         # PlatformIO configuration
└── README.md              # This file
```

## Configuration

### PlatformIO Settings
The `platformio.ini` file is configured for:
- **Platform**: Espressif32
- **Board**: ESP32 Dev Module
- **Framework**: Arduino
- **Monitor Speed**: 115200 baud
- **Upload Speed**: 921600 baud

### Libraries
- `miguelbalboa/MFRC522@^1.4.10` - RFID operations
- `dfrobot/DFRobotDFPlayerMini@^1.0.6` - MP3 player control

### Build Flags
- `CORE_DEBUG_LEVEL=3` - Enhanced debugging output

## Troubleshooting

### Common Issues

**DFPlayer not responding:**
- Check wiring connections (especially TX/RX)
- Ensure SD card is properly formatted (FAT32)
- Verify MP3 files are numbered correctly (001.mp3, 002.mp3, etc.)
- Check power supply (DFPlayer needs 5V)

**RFID cards not detected:**
- Verify RC522 connections
- Check antenna gain setting in code
- Ensure cards are properly programmed
- Test with known working cards

**System resets frequently:**
- Check power supply stability
- Monitor Serial output for error messages
- Verify all connections are secure
- Check for interference from other devices

**No audio output:**
- Verify speaker connections
- Check volume settings (use `v`, `+`, `-` commands)
- Test with headphones first
- Ensure MP3 files are valid

### Debug Mode
Enable additional debug output by:
1. Setting `CORE_DEBUG_LEVEL=3` in platformio.ini (already set)
2. Monitoring Serial output at 115200 baud
3. Using serial commands to check system status

## Advanced Features

### Custom Playlists
Create themed playlists by organizing MP3 files in folders:
- `01/` - Kids songs
- `02/` - Classical music  
- `03/` - Rock/Pop
- `04/` - Ambient/Relaxation
- etc.

Program RFID cards with negative numbers to trigger these playlists.

### Volume Presets
Modify the code to set different volume levels for different card types or times of day.

### WiFi Integration
The ESP32's WiFi capabilities can be added for:
- Web-based control interface
- Streaming from online sources
- Remote playlist management

## Contributing

This project was converted from Arduino IDE to ESP32/PlatformIO. Contributions welcome for:
- Additional button functions
- WiFi/Bluetooth features
- Web interface
- Enhanced error handling
- Performance optimizations

## License

Based on original work by ryand1011 and Ananords. Converted and enhanced for ESP32 platform.

## Related Projects

- [RFID Programmer](../rfid-programmer/) - Companion project for programming RFID cards
