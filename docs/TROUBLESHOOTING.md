# Troubleshooting Guide

## Common Issues and Solutions

### 🔌 Power and Connection Issues

#### ESP32 Won't Start
**Symptoms**: No serial output, no LED activity
**Solutions**:
- Check USB cable quality (data + power)
- Try different USB port
- Verify power supply (5V, 2A minimum)
- Check for short circuits
- Press EN button on ESP32

#### Serial Monitor Shows Garbage
**Symptoms**: Random characters, no readable output
**Solutions**:
- Set baud rate to 115200
- Check COM port selection
- Try different USB cable
- Reset ESP32 after opening monitor

#### Brown-out Detector Triggered
**Symptoms**: ESP32 resets repeatedly, "Brownout detector" messages
**Solutions**:
- Use higher capacity power supply
- Add bulk capacitors (1000µF)
- Check for power-hungry components
- Reduce WiFi transmission power

### 📡 WiFi Connection Problems

#### Cannot Connect to WiFi
**Symptoms**: "WiFi connection failed" message
**Solutions**:
```cpp
// Check these common issues:
1. Verify SSID and password spelling
2. Ensure 2.4GHz network (not 5GHz)
3. Check special characters in password
4. Move closer to router
5. Try different WiFi channel
```

#### Frequent WiFi Disconnections
**Symptoms**: Intermittent web interface access
**Solutions**:
- Check signal strength
- Reduce WiFi power management
- Add reconnection logic
- Use static IP configuration

#### Web Interface Not Accessible
**Symptoms**: Browser can't reach ESP32 IP
**Solutions**:
1. Verify IP address in serial output
2. Check firewall settings
3. Ensure same network subnet
4. Try `http://` prefix explicitly
5. Clear browser cache

### 🎵 Audio System Issues

#### DFPlayer Mini Not Responding
**Symptoms**: "DFPlayer initialization failed" error
**Solutions**:
```cpp
// Check wiring:
ESP32 TX2 (GPIO 17) → DFPlayer RX
ESP32 RX2 (GPIO 16) → DFPlayer TX
5V → DFPlayer VCC
GND → DFPlayer GND

// Verify SD card:
- Format as FAT32
- Check file naming (001.mp3, 002.mp3)
- Try different SD card
- Ensure files are valid MP3
```

#### No Audio Output
**Symptoms**: DFPlayer initializes but no sound
**Solutions**:
- Check speaker connections (SPK1/SPK2)
- Verify speaker impedance (8Ω recommended)
- Test with headphones/different speaker
- Check volume level (not zero)
- Verify MP3 file integrity

#### Audio Distortion
**Symptoms**: Crackling, static, or distorted sound
**Solutions**:
- Reduce volume limit (lower MAX_VOLUME)
- Check power supply capacity
- Use proper speaker impedance
- Verify clean audio connections
- Re-encode MP3 files at lower bit rate

#### Playback Stops Randomly
**Symptoms**: Music cuts out, player becomes unresponsive
**Solutions**:
- Check SD card quality/speed
- Verify file format compatibility
- Add error recovery in software
- Monitor system health checks

### 🏷️ RFID Reader Problems

#### Cards Not Detected
**Symptoms**: No "Card Detected" messages
**Solutions**:
```cpp
// Verify SPI connections:
ESP32 GPIO 5  → RC522 SDA
ESP32 GPIO 18 → RC522 SCK
ESP32 GPIO 23 → RC522 MOSI
ESP32 GPIO 19 → RC522 MISO
ESP32 GPIO 21 → RC522 RST
3.3V → RC522 VCC (NOT 5V!)
```

#### Intermittent Card Reading
**Symptoms**: Cards detected sometimes, not always
**Solutions**:
- Check antenna positioning
- Verify steady power supply
- Clean card contacts
- Try different cards
- Adjust antenna gain in software

#### Card Programming Fails
**Symptoms**: "Authentication failed" or "Write failed" errors
**Solutions**:
- Use MIFARE Classic cards only
- Check card is writable (not locked)
- Verify proper programming mode
- Try factory-fresh cards

#### Reading Wrong Data
**Symptoms**: Card shows unexpected numbers
**Solutions**:
- Check block number in read/write functions
- Verify data format and encoding
- Clear card data before reprogramming
- Use card reading mode to debug

### 🎚️ Volume Control Issues

#### Potentiometer Not Working
**Symptoms**: Volume doesn't change when turning pot
**Solutions**:
- Check wiper connection to GPIO 34
- Verify 3.3V supply (not 5V!)
- Test potentiometer with multimeter
- Check for loose connections

#### Volume Changes Erratically
**Symptoms**: Volume jumps randomly
**Solutions**:
- Clean potentiometer contacts
- Add debouncing in software
- Check for electrical interference
- Use higher quality potentiometer

#### Volume Limited to Low Values
**Symptoms**: Can't reach maximum volume
**Solutions**:
- Check MAX_VOLUME setting (should be 25)
- Verify potentiometer value (50kΩ)
- Test analog input with serial output
- Check ADC reference voltage

#### ESP32 Damaged by 5V
**Symptoms**: GPIO 34 not responding, erratic readings
**Solutions**:
- **Prevention**: Always use 3.3V for potentiometer
- **Recovery**: GPIO 34 may be permanently damaged
- **Workaround**: Use different analog pin and update code

### 🔘 Button Control Problems

#### Buttons Not Responding
**Symptoms**: Physical buttons don't trigger actions
**Solutions**:
- Check button connections to GPIO pins
- Verify pull-up resistor configuration
- Test button continuity with multimeter
- Check for proper debouncing

#### Multiple Button Triggers
**Symptoms**: Single press triggers multiple actions
**Solutions**:
- Add hardware debouncing (100nF capacitor)
- Increase software debounce delay
- Use interrupt-based button handling
- Check for mechanical switch bouncing

#### Button Stuck/Always Active
**Symptoms**: Continuous button press detected
**Solutions**:
- Check for short circuit to ground
- Verify button mechanical operation
- Test GPIO pin with different button
- Check for software pull-up configuration

### 💾 SD Card and File Issues

#### SD Card Not Recognized
**Symptoms**: DFPlayer can't read files
**Solutions**:
- Format card as FAT32 (not exFAT or NTFS)
- Use Class 10 or better SD card
- Check card size (32GB maximum recommended)
- Try different SD card

#### Files Not Playing
**Symptoms**: Card detected but no audio
**Solutions**:
- Check file naming (001.mp3, 002.mp3)
- Verify MP3 format compatibility
- Test files on computer first
- Use standard MP3 encoding

#### Playlist Folders Not Working
**Symptoms**: Negative card numbers don't work
**Solutions**:
- Create folders: 01/, 02/, 03/, etc.
- Put MP3 files in folders
- Check folder naming convention
- Verify playlist card programming

### 🌐 Web Interface Issues

#### Web Page Won't Load
**Symptoms**: Browser shows "site can't be reached"
**Solutions**:
- Check ESP32 IP address in serial output
- Verify network connectivity
- Try different browser
- Clear browser cache and cookies

#### Commands Don't Work
**Symptoms**: Buttons click but no response
**Solutions**:
- Check serial output for errors
- Verify command processing logic
- Test same commands via serial
- Check for JavaScript errors

#### Slow Response Time
**Symptoms**: Web interface very slow
**Solutions**:
- Reduce HTML page size
- Optimize CSS/JavaScript
- Check WiFi signal strength
- Monitor ESP32 memory usage

### 🔧 Programming Mode Issues

#### Can't Enter Programming Mode
**Symptoms**: 'p' command not working
**Solutions**:
- Verify serial input working
- Check mode switching logic
- Try from fresh boot
- Check for mode conflicts

#### Auto Programming Skips Numbers
**Symptoms**: Auto mode doesn't increment properly
**Solutions**:
- Check starting number setting
- Verify card write success
- Monitor serial output for errors
- Reset auto mode counter

#### Manual Programming Timeout
**Symptoms**: Manual mode exits before input
**Solutions**:
- Increase timeout value (30 seconds default)
- Type faster after card detection
- Check serial input buffering
- Use auto mode instead

### 🏥 System Recovery

#### Complete System Reset
When all else fails:
```cpp
// Hardware reset methods:
1. Press ESP32 EN button
2. Power cycle the system
3. Use reset button (GPIO 32)
4. Send 'r' serial command
```

#### Factory Reset
To return to clean state:
1. Re-upload firmware
2. Clear all RFID cards
3. Reset WiFi configuration
4. Check all hardware connections

#### Emergency Recovery
If ESP32 won't accept new code:
1. Hold BOOT button while powering on
2. Try esptool.py with erase_flash
3. Use manual flash mode
4. Check for hardware damage

### 📊 Diagnostic Tools

#### Serial Output Analysis
Monitor these indicators:
```
✅ Normal: "ESP32 RFID Jukebox Starting"
✅ Normal: "WiFi connected!"
✅ Normal: "DFPlayer Mini online!"
⚠️  Warning: "DFPlayer initialization failed"
❌ Error: "Brownout detector triggered"
```

#### Hardware Testing
Use multimeter to check:
- Power supply voltages (3.3V, 5V)
- GPIO pin continuity
- Button switch operation
- Potentiometer resistance

#### Software Debugging
Add debug output:
```cpp
Serial.print("Debug: Variable value = ");
Serial.println(variableName);
```

### 📞 Getting Help

#### Before Asking for Help
1. Check this troubleshooting guide
2. Review serial output for errors
3. Verify hardware connections
4. Test with minimal configuration

#### Information to Include
When reporting issues:
- Complete serial output
- Hardware configuration
- Exact steps to reproduce
- What you expected vs. what happened

#### Resources
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [MFRC522 Library](https://github.com/miguelbalboa/rfid)
- [DFPlayer Mini Documentation](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299)
