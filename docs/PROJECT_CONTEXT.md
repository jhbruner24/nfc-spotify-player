# Project: NFC "Cardstock Album" Spotify Player

## Concept
A physical tap-to-play music player. Small cardstock "albums" each have an
embedded NFC tag. Tapping an album on a reader box triggers the currently
active Spotify device to start playing that album/playlist/track.

## Hardware
- **Microcontroller**: ESP32 DevKit (WiFi built in, no separate Arduino needed)
- **NFC reader**: PN532 breakout, running in **I2C mode** (mode switch/jumpers
  on the board must be set to I2C, not SPI/UART)
- **Tags**: NTAG213 NFC stickers/cards embedded in cardstock albums
- **Alternative reader considered but not used**: MFRC522 (SPI, more wires,
  RFID not full NFC) — went with PN532 instead

## Wiring (ESP32 -> PN532, I2C)
| ESP32 pin | PN532 pin |
|-----------|-----------|
| 3V3       | VCC       |
| GND       | GND       |
| GPIO21    | SDA       |
| GPIO22    | SCL       |

Currently prototyping on a breadboard with jumper wires. Plan to solder the
final wiring once logic is validated (soldering directly to board pin holes
or headers, with hot glue for strain relief since albums will be handled).

## Software / toolchain
- Using **Arduino IDE** for now (ESP32 board package + "Adafruit PN532"
  library via Library Manager). Considering switching to VS Code +
  PlatformIO once the project grows past simple prototyping.
- Firmware language: C++ (.ino sketches), not MicroPython.

## Progress so far
1. Wiring diagrammed and confirmed (see table above).
2. Wrote and tested `nfc_tag_reader.ino` — a starter sketch that:
   - Initializes the PN532 over I2C
   - Polls for a tag every loop
   - On tap, prints the tag's UID (hex bytes + a clean string) to Serial
     Monitor at 115200 baud
   - This validates that tag reads work before adding WiFi/Spotify logic
3. Next planned step (not yet built): record UIDs of several physical tags,
   then build a lookup table mapping UID -> Spotify URI (album/playlist/track).

## Not yet built
- WiFi connection logic on the ESP32
- HTTP call from ESP32 to a backend (or directly to Spotify Web API)
- Spotify OAuth flow (Authorization Code flow, refresh token stored
  server-side so the ESP32 doesn't need to handle login)
- Backend endpoint that receives a tag UID and calls Spotify's
  `/me/player/play` endpoint with the matching `context_uri`
- Note: Spotify's playback control endpoints require a **Premium** account
- Physical builds: cardstock album housings, tag placement/testing for
  reliable reads through cardstock thickness

## Suggested architecture (not yet implemented)
```
ESP32 (WiFi) --tag UID--> small backend (Node/Flask, serverless is fine)
                                |
                                v
                    Spotify Web API (/me/player/play)
                    using stored refresh token for OAuth
```

## Open questions / decisions to make
- Where to host the backend (serverless function vs. small always-on server)
- Whether tag-to-URI mapping lives in backend code/DB, or is written onto
  the NFC tag itself as NDEF text
- Final enclosure/soldering approach for the reader box
