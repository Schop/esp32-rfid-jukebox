# Hardware Setup Guide

## Component List

### Required Components
| Component | Specification | Quantity | Notes |
|-----------|--------------|----------|--------|
| ESP32 Dev Module | 38-pin version | 1 | Main microcontroller |
| RC522 RFID Reader | 13.56MHz | 1 | Includes antenna |
| DFPlayer Mini | MP3 module | 1 | Audio playback |
| MicroSD Card | Class 10, FAT32 | 1 | Music storage |
| Potentiometer | 50kΩ linear | 1 | Volume control |
| Speaker | 8Ω, 3W | 1 | Audio output |
| Push Buttons | Momentary | 5 | Manual controls |
| Breadboard | Half-size+ | 1 | Prototyping |
| Jumper Wires | M-M, M-F | 20+ | Connections |

### Optional Components
| Component | Specification | Quantity | Notes |
|-----------|--------------|----------|--------|
| Pull-up Resistors | 10kΩ | 5 | If not using internal |
| Power Supply | 5V, 2A | 1 | External power |
| PCB | Custom/Protoboard | 1 | Permanent build |
| Enclosure | Project box | 1 | Protection |

## Detailed Wiring

### ESP32 Pin Usage
```
GPIO 5   - RC522 SDA (SPI Chip Select)
GPIO 16  - DFPlayer RX (Serial2 TX)
GPIO 17  - DFPlayer TX (Serial2 RX)
GPIO 18  - RC522 SCK (SPI Clock)
GPIO 19  - RC522 MISO (SPI Data In)
GPIO 21  - RC522 RST (Reset)
GPIO 23  - RC522 MOSI (SPI Data Out)
GPIO 25  - Next Track Button
GPIO 26  - Play/Pause Button
GPIO 27  - Shuffle Button
GPIO 32  - Reset Button
GPIO 33  - Previous Track Button
GPIO 34  - Volume Potentiometer (Analog Input)
```

### Power Distribution
```
ESP32 3.3V  → RC522 VCC, Potentiometer VCC
ESP32 5V    → DFPlayer VCC
ESP32 GND   → All component grounds
```

## Step-by-Step Assembly

### 1. ESP32 Preparation
1. Mount ESP32 on breadboard
2. Connect power rails (3.3V, 5V, GND)
3. Verify power LED illumination

### 2. RC522 RFID Module
1. Connect SPI bus (MOSI, MISO, SCK)
2. Connect SDA to GPIO 5
3. Connect RST to GPIO 21
4. Connect VCC to 3.3V (NOT 5V!)
5. Connect GND to ground rail

### 3. DFPlayer Mini
1. Connect VCC to 5V
2. Connect GND to ground rail
3. Connect TX to GPIO 16 (ESP32 RX2)
4. Connect RX to GPIO 17 (ESP32 TX2)
5. Connect speaker to SPK1/SPK2

### 4. Volume Potentiometer
⚠️ **CRITICAL**: Connect to 3.3V only!
1. Terminal 1 → GND
2. Wiper (center) → GPIO 34
3. Terminal 3 → 3.3V
4. **Never connect to 5V** (will damage ESP32)

### 5. Control Buttons
Connect each button between GPIO pin and GND:
- GPIO 26 ↔ Play/Pause Button ↔ GND
- GPIO 25 ↔ Next Button ↔ GND
- GPIO 33 ↔ Previous Button ↔ GND
- GPIO 27 ↔ Shuffle Button ↔ GND
- GPIO 32 ↔ Reset Button ↔ GND

Note: Using internal pull-up resistors (configured in software)

### 6. Audio Setup
1. Connect speaker to DFPlayer SPK1/SPK2
2. Insert microSD card with MP3 files
3. Ensure proper file naming (001.mp3, 002.mp3, etc.)

## Testing Checklist

### Power-On Tests
- [ ] ESP32 power LED illuminated
- [ ] Serial output visible (115200 baud)
- [ ] WiFi connection successful
- [ ] DFPlayer initialization successful

### Hardware Tests
- [ ] RFID reader detects cards
- [ ] All buttons respond
- [ ] Potentiometer changes volume smoothly
- [ ] Audio plays through speaker
- [ ] Web interface accessible

### Safety Checks
- [ ] No short circuits
- [ ] Proper voltage levels (3.3V/5V)
- [ ] Secure connections
- [ ] No overheating components

## Common Wiring Issues

### DFPlayer Problems
- **No audio**: Check TX/RX wiring (common to swap)
- **Distorted audio**: Verify 5V power supply
- **File not found**: Check SD card format (FAT32) and file naming

### RFID Issues
- **Cards not detected**: Verify SPI connections
- **Intermittent reads**: Check antenna positioning
- **Power issues**: Ensure 3.3V supply (not 5V)

### Potentiometer Issues
- **No volume change**: Check wiper connection to GPIO 34
- **Jumpy volume**: Verify clean connections
- **ESP32 damage**: If connected to 5V, GPIO 34 may be damaged

### Button Problems
- **Buttons not responding**: Check pull-up configuration
- **Multiple triggers**: Add debounce capacitors (100nF)
- **Stuck buttons**: Verify mechanical switches

## PCB Design Considerations

For permanent installations:
1. **Trace width**: Minimum 0.2mm for signals, 0.5mm for power
2. **Ground plane**: Use continuous ground plane
3. **Decoupling**: Add 100nF capacitors near each IC
4. **Antenna clearance**: Keep RC522 antenna away from power traces
5. **Component placement**: Group by function, minimize trace lengths

## Enclosure Guidelines

1. **Ventilation**: Ensure airflow for ESP32 and DFPlayer
2. **Antenna access**: RC522 needs unobstructed area
3. **Button access**: Panel-mount switches or cutouts
4. **Volume control**: Panel-mount potentiometer
5. **USB access**: For programming and debugging
6. **Speaker mounting**: Proper acoustic design

## Performance Optimization

### Power Management
- Use quality 5V supply (minimum 2A capacity)
- Add bulk capacitors (1000µF) for stable power
- Consider battery backup for portable use

### Audio Quality
- Use shielded audio cables
- Keep digital signals away from audio paths
- Add ferrite beads for EMI suppression
- Use quality speaker for best sound

### Reliability
- Secure all connections (solder for permanent)
- Use strain relief for cables
- Add protection diodes for inductive loads
- Implement watchdog timers in software
