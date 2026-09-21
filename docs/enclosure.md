# Enclosure design: display stand + storage

Form factor: an angled display face where the currently playing album
rests (the tap surface), with a vertical open-top slot behind it that
holds the rest of the collection like a mini record crate.

Albums: 3 x 3 in (76 x 76 mm) cardstock, NTAG213 tag sandwiched inside.
No LED/buzzer feedback; Spotify starting is the feedback.

## Side view

```
               album on display
                 /
                /  <- face panel, leans back ~22 deg
   ____________/_____________
  |   /|      |              |   <- open-top storage slot
  |  / |PN532 |  ||||||||||  |      (albums stand upright, face-out)
  | /  |      |  ||||||||||  |
  |/___|______|__||||||||||__|
  | ESP32     |              |
  |___________|______________|  <- removable bottom (4 screws)
   ^ lip       ^ >= 40 mm gap between antenna and nearest stored album
  front                    back
```

## Dimensions (mm)

| Element | Value | Why |
|---------|-------|-----|
| Overall (W x D x H) | 100 x 110 x 95 | Block-like, fits any 3D printer bed / one laser sheet |
| Face panel | 90 wide x 90 tall, 3 mm thick | ~7 mm margin around a 76 mm album; thin so tag stays within ~5 mm of antenna |
| Face angle | 22 deg back from vertical | Album stays put, art visible from seated height |
| Lip at bottom of face | 6 mm tall, 3 mm proud | Catches the album edge without hiding art |
| Antenna center | 38 mm above lip, horizontally centered | Matches a tag centered on a 76 mm album |
| Storage slot | 80 wide x 50 deep x 70 tall, open top | Holds ~20 albums at 2-2.5 mm each with finger room |
| Antenna-to-slot gap | >= 40 mm | PN532 range is ~30 mm; stored albums must never read |
| Electronics bay | Wedge under the face + floor, ~45 mm deep at floor | ESP32 lies flat on the floor, PN532 glued to the back of the face |
| Cable exit | Rear-bottom notch, 8 x 5 mm | Hides the USB cable behind the unit |
| Bottom panel | Removable, 4 x M3 screws or friction fit | Access without disassembly |

## Electronics placement

- **PN532** mounted on the *back* of the face panel with the antenna coil
  centered at the 38 mm mark. Hot glue at the corners; keep glue off the
  coil area (it doesn't matter electrically, just cleaner).
- **ESP32** flat on the floor of the bay, USB port facing the rear cable
  notch. Hot glue or a printed cradle.
- Jumper wires are short (< 80 mm) so I2C is happy. Solder to headers once
  the breadboard version works, then hot glue the connectors for strain.
- Nothing metal in the face panel. Wood, acrylic, PLA/PETG all fine.
  No metal feet directly under the antenna either (they're far away, but
  still).

## Album construction

- Two layers of cardstock, tag sandwiched in the middle, glued edge to
  edge. Front layer gets the art.
- Tag centered on the album. Standard 25 mm round NTAG213 stickers work.
- Keep foil / metallic printing >= 5 mm from the tag or it will detune.
- Test read range through the finished album before making 20 of them:
  the sketch should pick it up from ~20 mm away at the face.

## Materials / build path

All three fabrication options are available, so:

1. **Foam-core mockup first** (one evening). Cut the face at 22 deg, tape
   the PN532 behind it, confirm the album rests nicely and reads with the
   breadboard rig. Adjust angle / lip before committing.
2. **Final build, pick one:**
   - **Laser-cut 3 mm Baltic birch**, finger-jointed, glued, sanded, oiled.
     Best looking. Face panel is one piece with the lip glued on.
   - **3D print** the whole body in one piece (PLA or PETG), face-down on
     the bed so the display surface is smooth. Fastest iteration.
   - **Hybrid**: printed internal cradle for the boards, laser-cut wood
     shell. Nicest if you want wood outside and clean mounting inside.
3. Rubber feet on the bottom.

## Firmware implications

- The reader should fire **once when an album arrives**, not repeatedly
  while it sits on the stand. Current sketch debounces at 1.5 s, which
  still re-fires; the WiFi version should trigger only on UID change or
  on re-arrival after the tag has been absent for a few hundred ms.
- Removing the album could optionally pause playback. Decide later.

## Open decisions

- Wood species / finish (birch ply oiled vs. painted MDF vs. walnut veneer).
- Whether the storage slot gets a divider so albums fan slightly for
  easier flipping.
- Whether to add a small engraved logo/label on the face below the lip.
