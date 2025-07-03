# 🎵 RFID Jukebox Song Reference Card

## Quick Song Lookup
Use this reference to know which RFID card corresponds to which song:

| Card # | Song Title | Artist |
|--------|------------|--------|
| 1 | Did Jesus Have a Baby Sister | Dory Previn |
| 2 | That's All Right | Elvis Presley |
| 3 | Hey Joe | Jimi Hendrix |
| 4 | Delia's Gone | Johnny Cash |
| 5 | In Da Club | 50 Cent |
| 6 | Keep the Customer Satisfied | Simon & Garfunkel |
| 7 | Thrift Shop | Macklemore & Ryan Lewis |
| 8 | Old Man | Neil Young |
| 9 | Never Going Back Again | Fleetwood Mac |
| 10 | Norwegian Wood (This Bird Has Flown) | The Beatles |
| 11 | Chain Gang | Sam Cooke |
| 12 | Yakety Yak | The Coasters |
| 13 | I've Been Everywhere | Johnny Cash |
| 14 | Thunderstruck | AC/DC |
| 15 | Duurt Te Lang | Davina Michelle |
| 16 | Alles Gaat Voorbij | Doe Maar |
| 17 | The Painter | William Ben |
| 18 | Think | Aretha Franklin |
| 19 | Scotland the Brave | Auld Town Band & Pipes |
| 20 | Single Ladies | Beyoncé |
| 21 | Grandma's Hands | Bill Withers |
| 22 | Without Me | Eminem |
| 23 | Spraakwater | Extince |
| 24 | King of the World | First Aid Kit |
| 25 | Komodovaraan | Yentl en De Boer |
| 26 | Look What They've Done To My Song, Ma | Melanie |
| 27 | The Man Who Sold The World | Nirvana |
| 28 | Rotterdam | Pokey LaFarge |
| 29 | 't Roeie Klied | Rowwen Heze |
| 30 | You Never Can Tell | Chuck Berry |
| 31 | Sit Still, Look Pretty | Daya |

## Special Cards (Playlists)
| Card # | Function |
|--------|----------|
| -1 | Play from folder 01/ |
| -2 | Play from folder 02/ |
| -3 | Play from folder 03/ |
| -4 | Play from folder 04/ |
| -5 | Play from folder 05/ |
| -6 | Play from folder 06/ |
| -7 | Master card - return to main folder |

## Programming Your Cards
**The jukebox now includes integrated RFID programming!**

### Programming Mode Commands:
1. **Enter Programming Mode**: Type `p` in Serial Monitor
2. **Choose Programming Type**:
   - `auto` - Sequential numbering (enter starting number)
   - `manual` - Enter specific numbers for each card
   - `read` - Read existing card numbers
3. **Return to Jukebox**: Type `jukebox` from any programming mode

### Quick Programming Workflow:
1. Connect ESP32 to computer
2. Open Serial Monitor (115200 baud)
3. Type `p` to enter programming mode
4. Type `auto` and enter starting number (e.g., `1`)
5. Place cards on reader one by one
6. Cards are automatically programmed 1, 2, 3, etc.
7. Type `jukebox` when done

### Serial Commands
**Jukebox Mode:**
- `l` - List all songs
- `p` - Enter programming mode  
- `s` - Check DFPlayer status
- `v` - Show volume level
- `+`/`-` - Adjust volume

**Programming Mode:**
- `auto` - Auto-increment programming
- `manual` - Manual number entry
- `read` - Read card contents
- `jukebox` - Return to music mode

Print this card and keep it handy while organizing your RFID cards! 🎶
