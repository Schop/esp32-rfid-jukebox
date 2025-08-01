# ESP32 RFID Jukebox

A powerful, feature-rich RFID-controlled music player built with ESP32, featuring physical controls, WiFi web interface, and analog volume control.

## 🎵 Features

- **RFID Control**: Place cards on reader to instantly play specific songs or playlists
- **Physical Controls**: Buttons for play/pause, next/previous, shuffle, and system reset
- **Analog Volume Control**: Smooth potentiometer-based volume adjustment (0-25, distortion-limited)
- **WiFi Web Interface**: Control via web browser from any device on your network
- **Programming Mode**: Write new cards with automatic, manual, or read-only modes
- **Playlist Support**: Special playlist cards for folder-based music organization
- **Auto Recovery**: System health monitoring with automatic error recovery
- **Hardware Serial**: Reliable ESP32 Serial2 communication with DFPlayer Mini

## 🔧 Hardware Requirements

### Core Components
- **ESP32 Dev Module** (38-pin version recommended)
- **RC522 RFID Reader** with cards/tags
- **DFPlayer Mini MP3 Module** with microSD card
- **50kΩ Potentiometer** for volume control
- **Speaker** (8Ω, 3W recommended)
- **Breadboard/PCB** for prototyping
- **Jumper wires** and connectors

### Optional Components
- **Push buttons** (5x) for manual control
- **Pull-up resistors** (10kΩ) if not using internal pull-ups
- **Power supply** (5V, 2A minimum)

## 📋 Hardware Connections

### RFID Reader (RC522)
```
RC522 Pin    ESP32 Pin    Description
SDA          GPIO 5       SPI Chip Select
SCK          GPIO 18      SPI Clock
MOSI         GPIO 23      SPI Data Out
MISO         GPIO 19      SPI Data In
IRQ          (unused)     Interrupt (not connected)
GND          GND          Ground
RST          GPIO 21      Reset
3.3V         3.3V         Power (2.5-3.3V)
```

### DFPlayer Mini
```
DFPlayer Pin ESP32 Pin    Description
TX           GPIO 16      Serial receive (ESP32 RX2)
RX           GPIO 17      Serial transmit (ESP32 TX2)
VCC          5V           Power supply
GND          GND          Ground
SPK1         Speaker +    Audio output positive
SPK2         Speaker -    Audio output negative
```

### Control Buttons
```
Button       ESP32 Pin    Description
Play/Pause   GPIO 26      Play/pause toggle
Next Track   GPIO 25      Next track
Previous     GPIO 33      Previous track
Shuffle      GPIO 27      Shuffle/random play
Reset        GPIO 32      System reset
```

### Volume Control
```
Potentiometer ESP32 Pin   Description
Terminal 1    GND         Ground
Wiper         GPIO 34     Analog input (ADC1_CH6)
Terminal 3    3.3V        Power (⚠️ NOT 5V!)
```

## 🚀 Quick Start

### 1. Hardware Setup
1. Connect all components according to the wiring diagram above
2. **Important**: Connect potentiometer to 3.3V, NOT 5V (would damage ESP32)
3. Insert formatted microSD card into DFPlayer Mini
4. Add MP3 files to SD card (001.mp3, 002.mp3, etc.)

### 2. Software Setup
1. Install [PlatformIO](https://platformio.org/) in VS Code
2. Clone this repository
3. Open project in VS Code with PlatformIO
4. Update WiFi credentials in `src/main.cpp`:
   ```cpp
   const char* ssid = "Your_WiFi_Name";
   const char* password = "Your_WiFi_Password";
   ```
5. Connect ESP32 via USB and upload code
6. Open Serial Monitor (115200 baud) to see status

### 3. SD Card Setup
1. Format microSD card as FAT32
2. Add MP3 files named: `001.mp3`, `002.mp3`, etc.
3. Optional: Create folders for playlists:
   - `01/` - Folder 1 (triggered by card -1)
   - `02/` - Folder 2 (triggered by card -2)
   - etc.

### 4. Programming RFID Cards
1. Upload code and open Serial Monitor
2. Type `p` to enter programming mode
3. Choose mode:
   - `auto` - Auto-increment card programming
   - `manual` - Manually specify track numbers
   - `read` - Read existing card values
4. Follow on-screen instructions

## 🕸️ Web Interface

Once connected to WiFi, access the web interface at: `http://[ESP32-IP-ADDRESS]/`

### Available Controls
- **Volume**: Check current volume, increase/decrease
- **Playback**: Check player status, list all songs
- **Programming**: Enter/exit programming mode
- **System**: Reset ESP32

### API Access
Programmatic control via HTTP GET requests:
```
http://[ESP32-IP]/api/command?cmd=v  # Check volume
http://[ESP32-IP]/api/command?cmd=+  # Volume up
http://[ESP32-IP]/api/command?cmd=-  # Volume down
http://[ESP32-IP]/api/command?cmd=s  # Player status
http://[ESP32-IP]/api/command?cmd=l  # List songs
```

## 🎚️ Volume Control

The system includes multiple volume control methods:

### Potentiometer (Primary)
- **Real-time control**: Smooth analog adjustment
- **Range**: 0-25 (limited to prevent distortion)
- **Update rate**: 100ms for responsive control
- **Safety**: Hardware limits prevent damage

### Button Controls
- **Serial commands**: `+` (up), `-` (down), `v` (check)
- **Web interface**: Volume up/down buttons
- **Limits**: Same 0-25 range as potentiometer

### Why Volume Limit?
Volume is capped at 25 (instead of 30) because higher levels cause audio distortion on most speakers. This provides optimal sound quality while preventing damage.

## 🔧 Serial Commands

Connect to Serial Monitor (115200 baud) for debug commands:

```
s  - Check DFPlayer status
v  - Check current volume
+  - Increase volume
-  - Decrease volume
l  - List all songs
p  - Enter programming mode
r  - Reset ESP32
```

## 📁 Project Structure

```
esp32-rfid-jukebox/
├── src/
│   └── main.cpp              # Main application code
├── include/                  # Header files (if any)
├── lib/                      # Private libraries
├── test/                     # Unit tests
├── docs/                     # Documentation
│   ├── HARDWARE_GUIDE.md     # Detailed hardware setup
│   ├── SOFTWARE_GUIDE.md     # Software configuration
│   └── TROUBLESHOOTING.md    # Common issues and solutions
├── .vscode/                  # VS Code settings
├── .gitignore               # Git ignore rules
├── platformio.ini           # PlatformIO configuration
└── README.md               # This file
```

## 🛠️ Development

### Building
```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Serial monitor
pio device monitor
```

### Adding Songs
1. Name files as `001.mp3`, `002.mp3`, etc.
2. Update `getSongInfo()` function in `main.cpp` with track information
3. Program RFID cards with corresponding numbers

### Customization
- **Pin assignments**: Modify `#define` statements in `main.cpp`
- **Volume limits**: Change `MAX_VOLUME` constant
- **WiFi settings**: Update credentials in `main.cpp`
- **Song database**: Modify `getSongInfo()` function

## 🔧 Troubleshooting

### Common Issues

**DFPlayer not responding**
- Check wiring (especially TX/RX swap)
- Verify SD card format (FAT32)
- Ensure MP3 files are properly named

**Volume distortion**
- Reduce maximum volume limit
- Check speaker impedance (8Ω recommended)
- Verify power supply capacity

**WiFi connection failed**
- Double-check credentials
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Check signal strength

**RFID cards not reading**
- Verify SPI connections
- Check antenna positioning
- Ensure cards are MIFARE Classic compatible

## 📜 License

This project is open source and available under the MIT License.

## 🙏 Acknowledgments

- Based on original Arduino RFID jukebox by ryand1011 and Ananords
- Uses libraries: MFRC522, DFRobotDFPlayerMini, ESPAsyncWebServer
- Converted to ESP32 with enhanced features

## 🤝 Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss what you would like to change.

## 📧 Support

For questions or issues:
1. Check the [Troubleshooting Guide](docs/TROUBLESHOOTING.md)
2. Search existing GitHub issues
3. Create a new issue with detailed description

---

**⚠️ Safety Note**: Always connect the potentiometer to 3.3V, never 5V, to avoid damaging the ESP32's analog input pins.
