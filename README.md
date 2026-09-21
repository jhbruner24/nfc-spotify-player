# NFC Cardstock Album Spotify Player

A physical tap-to-play music player. Small cardstock "albums" each carry an
embedded NFC tag. Tapping one on the reader box starts that album, playlist,
or track on your currently active Spotify device.

## How it works

```
cardstock album (NTAG213)
        |  tap
        v
ESP32 + PN532 (I2C)  --tag UID-->  backend  --/me/player/play-->  Spotify
                                  (holds OAuth refresh token)
```

The ESP32 only ever sends a tag UID over WiFi. The backend owns the Spotify
credentials and does the OAuth work, so the microcontroller never handles
a login.

## Hardware

| Part | Notes |
|------|-------|
| ESP32 DevKit | Built-in WiFi |
| PN532 breakout | Set to **I2C mode** via the board's switch/jumpers |
| NTAG213 tags | Stickers or cards embedded in cardstock |

### Wiring (ESP32 -> PN532)

| ESP32 | PN532 |
|-------|-------|
| 3V3   | VCC   |
| GND   | GND   |
| GPIO21 | SDA  |
| GPIO22 | SCL  |

Currently on a breadboard. Final build will be soldered with hot glue for
strain relief.

## Repo layout

```
firmware/nfc_tag_reader/   Arduino sketch (ESP32 + PN532)
backend/                   Tag UID -> Spotify play service (not yet built)
docs/                      Project context, tag map example
```

## Getting started (firmware)

1. Arduino IDE: install the ESP32 board package and the **Adafruit PN532**
   library from Library Manager.
2. Open `firmware/nfc_tag_reader/nfc_tag_reader.ino`, select your ESP32
   board and port, upload.
3. Open Serial Monitor at **115200** baud and tap a tag. The UID prints as
   hex bytes and as a clean string.
4. Record the UID of each physical album in a copy of
   `docs/tags.example.json` -> `docs/tags.json`.

When WiFi gets added, copy `firmware/nfc_tag_reader/secrets.example.h` to
`secrets.h` and fill it in. `secrets.h` is gitignored.

## Secrets

Nothing sensitive is committed. Real values live only in:

- `firmware/nfc_tag_reader/secrets.h` (WiFi, backend URL)
- `backend/.env` (Spotify client ID/secret, refresh token)

Both are gitignored; `.example` templates show the expected shape.

## Status

- [x] Wiring confirmed
- [x] Tag UID read over I2C, printed to Serial
- [ ] Record UIDs for physical tags, build UID -> URI map
- [ ] ESP32 WiFi + HTTP call to backend
- [ ] Backend: Spotify Authorization Code flow, refresh token storage
- [ ] Backend: `/play` endpoint calling `/me/player/play`
- [ ] Cardstock album housings, read-through-cardstock testing
- [ ] Final enclosure and soldered wiring

Spotify playback control requires a **Premium** account.

See `docs/PROJECT_CONTEXT.md` for full notes and open decisions.
