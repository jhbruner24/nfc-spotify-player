# Bill of materials and build plan

Companion to `enclosure.md` (dimensions, layout) and `PROJECT_CONTEXT.md`
(architecture). This is the shopping list and the order of operations.

## Bill of materials

### Electronics

| Item | Qty | Notes | Have? |
|------|-----|-------|-------|
| ESP32 DevKit (30 or 38 pin) | 1 | Any CP2102/CH340 variant | yes |
| PN532 NFC breakout | 1 | Elechouse-style red board or Adafruit; must support I2C | yes |
| NTAG213 stickers, 25 mm round | 20-30 | One per album plus spares; buy the plain white ones | ? |
| Micro-USB **data** cable | 1 | The current two are charge-only. Must say "data" or "sync" | **no** |
| USB wall charger, 5V 1A+ | 1 | Any phone brick | likely |
| Female-female jumper wires | 4 | Breadboard phase only | yes |
| 22-26 AWG hookup wire, 4 colors | ~1 ft | Final soldered wiring; red/black/yellow/green convention for VCC/GND/SDA/SCL | ? |
| 2.54 mm male header strip | 1 | Only if the PN532 shipped without headers soldered | ? |
| Heat-shrink tubing, assorted | small pack | Over solder joints | ? |

### Enclosure (laser-cut wood path)

| Item | Qty | Notes |
|------|-----|-------|
| 3 mm Baltic birch plywood | 1 sheet, 12 x 12 in | All panels fit on one sheet with room to spare |
| Wood glue | 1 | Titebond II or similar |
| M3 x 8 mm screws + heat-set inserts or captive nuts | 4 each | Removable bottom panel |
| Rubber feet, self-adhesive | 4 | ~10 mm |
| Sandpaper, 220 and 320 grit | 1 each | |
| Finish: Danish oil, tung oil, or wipe-on poly | 1 small can | Oil is easiest and forgiving |
| Painter's tape | 1 roll | Clamping and masking |
| Hot glue gun + sticks | 1 | Board mounting and strain relief |

### Enclosure (3D print path, alternative)

| Item | Qty | Notes |
|------|-----|-------|
| PLA or PETG filament | ~150 g | Body prints as one part, face-down |
| M3 x 8 mm screws | 4 | Bottom panel; print bosses for heat-set inserts |
| Heat-set inserts, M3 | 4 | |
| Rubber feet | 4 | |

### Albums

| Item | Qty | Notes |
|------|-----|-------|
| Heavy cardstock, 110 lb / 300 gsm | 1 pack | Two 3 x 3 in squares per album |
| Printed cover art, 3 x 3 in | 1 per album | Matte photo paper or direct print on cardstock; avoid foil |
| Glue stick or double-sided adhesive sheet | 1 | Sandwich the tag between layers |
| Paper trimmer or craft knife + ruler | 1 | Clean 76 mm squares |
| Corner rounder punch (optional) | 1 | Nice touch, keeps corners from fraying |

### Prototyping

| Item | Qty | Notes |
|------|-----|-------|
| Foam core, 5 mm | 1 sheet | Mockup |
| Breadboard | 1 | Have |

## Build plan

Each phase has an exit condition. Don't move on until it's met; it keeps
hardware bugs from hiding behind enclosure bugs.

### Phase 0: Get the ESP32 talking (blocked on cable)

1. Get a micro-USB data cable.
2. Plug in, confirm `/dev/cu.usbserial-*` appears.
3. Upload `firmware/nfc_tag_reader/nfc_tag_reader.ino`, open Serial
   Monitor at 115200.

**Exit:** "Ready. Tap a tag." prints.

### Phase 1: Validate the reader on the breadboard

1. Set the PN532 to I2C (SEL0 = 1, SEL1 = 0).
2. Wire 3V3 / GND / GPIO21-SDA / GPIO22-SCL.
3. Tap a bare NTAG213 sticker. Note the UID.
4. Tap 4-5 more, record each UID in `docs/tags.json` with a placeholder
   label.
5. Measure read range: raise the tag until reads stop. Expect 20-40 mm.

**Exit:** Every tag reads reliably; UIDs recorded; range known.

### Phase 2: Build one album, test through cardstock

1. Cut two 76 mm squares of cardstock.
2. Stick a tag centered on the inside of the back square.
3. Glue the front square over it.
4. Tap it on the bare PN532. Then hold it 3 mm off (simulating the face
   panel) and confirm it still reads.

**Exit:** Album reads at >= 5 mm standoff. If not, use thinner cardstock
or a bigger tag before making more.

### Phase 3: Foam-core mockup

1. Cut a face panel 90 x 90 mm and prop it at 22 deg.
2. Tape the PN532 to the back, antenna centered 38 mm above the bottom edge.
3. Tape a lip along the bottom.
4. Rest the album on it. Does it stay? Does it read? Does the angle look
   right from where the finished piece will sit?
5. Tweak angle / lip height. Update `enclosure.md` with the final numbers.

**Exit:** Angle and lip decided; album reads when resting naturally.

### Phase 4: WiFi + backend (software, parallel with Phase 5)

1. Add WiFi + HTTP POST to the sketch using `secrets.h`.
2. Change trigger logic to fire once on tag arrival, not on repeat reads.
3. Stand up the backend: OAuth flow once, store refresh token, `/play`
   endpoint.
4. Tap album -> Spotify plays on the active device.

**Exit:** End-to-end tap-to-play works on the breadboard.

### Phase 5: Final enclosure

Laser-cut path:

1. Draw panels (front face, two sides, back, top with slot cutout, bottom,
   lip strip, slot inner walls) as a single SVG with finger joints and the
   rear cable notch.
2. Cut a test piece to check finger-joint fit for the sheet's real
   thickness (nominal 3 mm ply is often 2.7-3.2 mm).
3. Cut all panels. Dry-fit.
4. Glue up everything except the bottom. Tape as clamps. Let cure.
5. Sand 220 then 320. Oil finish, two coats, 24 h between.
6. Install heat-set inserts in the bottom-panel bosses (or glue captive
   nuts to blocks inside the corners).

3D-print path: model in the same dimensions, print body face-down, print
bottom panel separately, install inserts.

**Exit:** Empty enclosure assembled, bottom panel removable, looks right.

### Phase 6: Final wiring and install

1. Cut four ~70 mm hookup wires. Solder ESP32 header to PN532 header.
   Heat-shrink each joint.
2. Hot glue the PN532 to the back of the face panel, antenna centered.
3. Hot glue the ESP32 to the floor, USB port facing the cable notch.
4. Hot glue over the header connections for strain relief.
5. Plug in, verify the sketch still reads through the face panel.
6. Screw on the bottom. Add rubber feet.

**Exit:** Enclosed unit reads albums and plays music.

### Phase 7: Make the collection

1. Print cover art for each album.
2. Build albums per Phase 2. Batch of 10 at a time.
3. Tap each on the finished unit, record UID -> Spotify URI in
   `docs/tags.json` (and the backend).

**Exit:** Full crate of working albums.

## Rough cost (excluding what you already own)

| Group | Estimate |
|-------|----------|
| NTAG213 stickers x30 | $10-15 |
| Data cable | $6 |
| Hookup wire, heat-shrink, headers | $10 |
| Baltic birch sheet + hardware + finish | $20-30 |
| Cardstock, adhesive, corner punch | $15 |
| **Total** | **~$60-75** |
