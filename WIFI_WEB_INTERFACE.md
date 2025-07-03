# ESP32 Jukebox - WiFi Web Interface 🌐

## What's New?
Your ESP32 RFID Jukebox now has a **WiFi Web Interface**! You can now send all serial commands through a web browser or HTTP requests instead of needing a USB serial connection.

## 🌟 Features

### 🖥️ Web Interface
- **Beautiful web interface** accessible from any device on your network
- **Button-based controls** for easy operation
- **Real-time responses** displayed in the browser
- **Mobile-friendly** design that works on phones and tablets

### 📡 WiFi Commands
- **All serial commands** now available over WiFi
- **HTTP API** for programmatic control
- **No USB cable needed** for commands after initial setup
- **Works from anywhere** on your local network

### 🔧 Control Options
- **Volume control** (check, up, down)
- **Music control** (status, song list)
- **Programming mode** (enter/exit programming)
- **System control** (restart ESP32)
- **Custom commands** (send any single character command)

## 🚀 How to Use

### 1. Setup (One-time)
1. **Update WiFi credentials** in `main.cpp`:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
2. **Upload firmware** via USB (first time only)
3. **Check Serial Monitor** for WiFi connection confirmation

### 2. Access Web Interface
Once connected to WiFi, you can access the web interface in multiple ways:

#### Method 1: Using hostname (recommended)
- Open browser and go to: `http://esp32-jukebox.local`

#### Method 2: Using IP address
- Check Serial Monitor for IP address (e.g., `192.168.1.100`)
- Open browser and go to: `http://192.168.1.100`

### 3. Using the Web Interface
The web interface provides several sections:

#### 🎛️ Volume Control
- **Check Volume**: See current volume level
- **Volume Up**: Increase volume (max 30)
- **Volume Down**: Decrease volume (min 0)

#### 🎵 Music Control
- **Player Status**: Check DFPlayer state
- **List Songs**: See all available songs with track numbers

#### 🔧 Programming Mode
- **Enter Programming**: Switch to RFID programming mode
- **Return to Jukebox**: Switch back to music mode

#### 🔧 System Control
- **Reset ESP32**: Restart the device

#### 💬 Custom Command
- **Text input**: Send any single character command
- **Send Button**: Execute the command

## 🔌 HTTP API

For programmatic control, you can use direct HTTP requests:

### Web Interface Commands
```
GET http://esp32-jukebox.local/cmd?c=v     # Check volume
GET http://esp32-jukebox.local/cmd?c=+     # Volume up
GET http://esp32-jukebox.local/cmd?c=-     # Volume down
GET http://esp32-jukebox.local/cmd?c=s     # Player status
GET http://esp32-jukebox.local/cmd?c=l     # List songs
GET http://esp32-jukebox.local/cmd?c=p     # Enter programming
GET http://esp32-jukebox.local/cmd?c=r     # Reset ESP32
```

### JSON API
```
GET http://esp32-jukebox.local/api/command?cmd=v
```
Returns:
```json
{
  "command": "v",
  "response": "Current volume: 25"
}
```

## 📱 Mobile Usage

The web interface is mobile-friendly and works great on:
- **Smartphones** (iOS/Android)
- **Tablets** (iPad/Android tablets)
- **Laptops** (Windows/Mac/Linux)
- **Any device** with a web browser

## 🛠️ Programming Integration

### Python Example
```python
import requests

# Check volume
response = requests.get('http://esp32-jukebox.local/api/command?cmd=v')
print(response.json())

# Volume up
response = requests.get('http://esp32-jukebox.local/api/command?cmd=+')
print(response.json())

# Get song list
response = requests.get('http://esp32-jukebox.local/api/command?cmd=l')
print(response.json())
```

### JavaScript Example
```javascript
// Check volume
fetch('http://esp32-jukebox.local/api/command?cmd=v')
  .then(response => response.json())
  .then(data => console.log(data));

// Volume up
fetch('http://esp32-jukebox.local/api/command?cmd=+')
  .then(response => response.json())
  .then(data => console.log(data));
```

### cURL Example
```bash
# Check volume
curl "http://esp32-jukebox.local/api/command?cmd=v"

# Volume up
curl "http://esp32-jukebox.local/api/command?cmd=+"

# Get song list
curl "http://esp32-jukebox.local/api/command?cmd=l"
```

## 🔐 Security Notes

### Local Network Only
- The web interface only works on your local network
- Not accessible from the internet (for security)
- Same network as your ESP32 required

### No Authentication
- Currently no password protection on web interface
- Anyone on your network can control the jukebox
- Consider this when setting up on shared networks

## 📝 Available Commands

All the same commands that work via Serial Monitor now work via WiFi:

| Command | Function | Web Button |
|---------|----------|------------|
| `v` | Check volume | ✅ Check Volume |
| `+` | Volume up | ✅ Volume Up |
| `-` | Volume down | ✅ Volume Down |
| `s` | Player status | ✅ Player Status |
| `l` | List songs | ✅ List Songs |
| `p` | Enter programming | ✅ Enter Programming |
| `r` | Reset ESP32 | ✅ Reset ESP32 |

## 🎯 Use Cases

### 1. Remote Control
- Control volume from your phone while music is playing
- Check what song is playing without going to the device
- Restart the device remotely if needed

### 2. Programming Cards
- Enter programming mode from your computer
- Check current volume before programming sessions
- List available songs for reference

### 3. Home Automation
- Integrate with home automation systems
- Control via HTTP requests from other devices
- Monitor jukebox status programmatically

### 4. Party/Event Control
- Let guests control volume from their phones
- Share the web interface URL for easy access
- No need to explain physical button locations

## 🚨 Troubleshooting

### Web Interface Not Loading
1. Check ESP32 WiFi connection (Serial Monitor)
2. Verify you're on the same network
3. Try IP address instead of hostname
4. Check firewall settings

### Commands Not Working
1. Verify web interface loads correctly
2. Check Serial Monitor for error messages
3. Ensure DFPlayer is properly connected
4. Try the same command via Serial Monitor

### Network Issues
1. Check WiFi credentials in code
2. Verify network allows device-to-device communication
3. Try different WiFi network
4. Check router settings

## 🎉 Benefits

### ✅ Advantages
- **No USB cable needed** for commands
- **Works from anywhere** on your network
- **Mobile-friendly** interface
- **Real-time responses** in browser
- **API integration** possible
- **Multiple access methods** (web, HTTP, JSON)

### 🔄 Fallback
- **Serial Monitor still works** via USB
- **Web interface is additional**, not replacement
- **All original functionality preserved**
- **USB connection for emergencies**

Your jukebox now has full WiFi command capability while maintaining all original functionality!
