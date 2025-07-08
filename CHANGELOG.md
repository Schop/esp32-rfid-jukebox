# Changelog

All notable changes to the ESP32 RFID Jukebox project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2025-01-08

### Added
- 🎨 **Bootstrap CSS framework** - Modern, responsive web interface design
- 📂 **SPIFFS filesystem** - HTML files served from flash memory for better maintainability
- 📄 **Separate HTML file** - Web interface moved to `data/index.html` for easier editing
- 🎵 **Dropdown song selection** - Clean interface with song titles and artists
- 🎨 **Gradient design** - Beautiful visual design with Bootstrap components and custom gradients
- 📱 **Mobile responsive** - Optimized for mobile, tablet, and desktop devices
- 🔧 **Enhanced controls** - Status, Stop, Shuffle, and List Songs buttons in main interface
- ⚡ **Improved user experience** - Loading indicators and better error handling

### Changed
- 🗑️ **Removed individual song buttons** - Simplified interface with dropdown selection only
- ⚡ **Cleaner code structure** - HTML separated from C++ for better maintainability
- 🎚️ **Software-only volume** - Removed potentiometer dependency, volume via commands only
- 📍 **Fixed static IP** - Consistent 192.168.1.251 address for reliable access

### Removed
- 🎛️ **Potentiometer volume control** - Now software-controlled only
- 🔘 **Per-song grid buttons** - Replaced with cleaner dropdown interface
- 📝 **Embedded HTML** - Moved to external file for better organization

### Technical Changes
- Added SPIFFS support to platformio.ini
- Implemented file serving from flash memory
- Bootstrap 5.3.0 integration with CDN
- Enhanced web interface with icons and animations
- Improved build process with SPIFFS filesystem compilation

## [1.0.0] - 2025-01-03

### Added
- 🎚️ **Potentiometer volume control** - Smooth analog volume adjustment (0-25)
- 🕸️ **WiFi web interface** - Browser-based control from any device
- 📱 **RESTful API** - HTTP endpoints for programmatic control
- 🔒 **Volume limiting** - Maximum volume capped at 25 to prevent distortion
- 📡 **Real-time volume monitoring** - 100ms update frequency for responsive control
- 🛡️ **Hardware protection** - Prevents ESP32 damage from incorrect wiring
- 📚 **Comprehensive documentation** - Hardware guide, software guide, troubleshooting
- 🔧 **Programming modes** - Auto, manual, and read modes for RFID card setup
- 🎵 **Playlist support** - Negative card numbers for folder-based playlists
- 🔄 **Auto recovery** - System health monitoring with automatic error recovery
- 📋 **Serial commands** - Debug interface for testing and diagnostics
- 🏷️ **Song database** - Track information display and management

### Technical Features
- ESP32 hardware serial communication (Serial2) with DFPlayer Mini
- Non-blocking operation with TimePeriodIsOver utility function
- SPI communication with RC522 RFID reader
- Internal pull-up resistors for button controls
- ADC-based potentiometer reading with jitter prevention
- Async web server for responsive interface
- MIFARE Classic RFID card support

### Hardware Support
- ESP32 Dev Module (38-pin recommended)
- RC522 RFID Reader (13.56MHz)
- DFPlayer Mini MP3 Module
- 50kΩ linear potentiometer
- 5x momentary push buttons
- 8Ω speakers
- MicroSD card (FAT32, Class 10+)

### Documentation
- Complete hardware wiring guide
- Software configuration instructions
- Comprehensive troubleshooting guide
- API reference documentation
- Safety guidelines and best practices

### Security
- Local network operation only
- No external internet dependencies
- WiFi credentials stored locally
- RFID data encryption support

## [Unreleased]

### Planned Features
- Battery level monitoring
- Sleep/wake functionality
- Multi-language support
- Advanced playlist management
- Mobile app companion
- Audio equalizer controls
- Scheduled playback
- User access controls

---

## Version History Summary

- **v1.0.0** (2025-01-03): Initial stable release with potentiometer control
- **v0.x.x** (Development): Previous Arduino IDE versions (legacy)

## Migration Guide

### From Arduino IDE Version
1. Install PlatformIO in VS Code
2. Copy your WiFi credentials to new format
3. Update pin definitions if customized
4. Rebuild and upload firmware
5. Re-program RFID cards if needed

### Hardware Changes
- Added potentiometer on GPIO 34 (connect to 3.3V, NOT 5V)
- All other connections remain the same
- Volume now limited to 25 (was 30) for better audio quality

## Support

For questions, issues, or contributions:
1. Check the [troubleshooting guide](docs/TROUBLESHOOTING.md)
2. Search existing GitHub issues
3. Create new issue with detailed information
4. Join the community discussions

## Contributors

- **Primary Development**: GitHub Copilot assisted development
- **Original Concept**: Based on Arduino RFID jukebox by ryand1011 and Ananords
- **ESP32 Port**: Complete rewrite for ESP32 platform with enhanced features

## License

This project is released under the MIT License. See LICENSE file for details.
