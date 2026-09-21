// nfc_tag_reader.ino
//
// Starter sketch: initialize a PN532 over I2C and print the UID of any
// tag that is tapped. Validates the reader wiring before WiFi/Spotify
// logic gets layered on.
//
// Wiring (ESP32 DevKit -> PN532, board set to I2C mode):
//   3V3    -> VCC
//   GND    -> GND
//   GPIO21 -> SDA
//   GPIO22 -> SCL
//
// Library: "Adafruit PN532" via Arduino Library Manager.
// Serial Monitor: 115200 baud.

#include <Wire.h>
#include <Adafruit_PN532.h>

// IRQ and RESET are optional over I2C; pass -1 when unwired.
#define PN532_IRQ   (-1)
#define PN532_RESET (-1)

// Ignore repeat reads of the same tag for this long (ms) so holding an
// album on the reader doesn't spam the serial output.
#define DEBOUNCE_MS 1500

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

String lastUid = "";
unsigned long lastReadMs = 0;

// Convert raw UID bytes into an uppercase hex string with no separators,
// e.g. {0x04, 0xA1, 0x2B, ...} -> "04A12B..."
String uidToString(const uint8_t *uid, uint8_t len) {
  String s;
  for (uint8_t i = 0; i < len; i++) {
    if (uid[i] < 0x10) s += "0";
    s += String(uid[i], HEX);
  }
  s.toUpperCase();
  return s;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("NFC tag reader starting...");

  Wire.begin(21, 22);  // SDA, SCL
  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();
  if (!version) {
    Serial.println("PN532 not found. Check wiring and that the board is in I2C mode.");
    while (1) delay(1000);
  }

  Serial.print("Found PN5");
  Serial.println((version >> 24) & 0xFF, HEX);
  Serial.print("Firmware v");
  Serial.print((version >> 16) & 0xFF, DEC);
  Serial.print(".");
  Serial.println((version >> 8) & 0xFF, DEC);

  nfc.SAMConfig();
  Serial.println("Ready. Tap a tag.");
}

void loop() {
  uint8_t uid[7] = {0};
  uint8_t uidLength = 0;

  // ISO14443A covers NTAG213. Timeout keeps the loop responsive.
  bool found = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 200);
  if (!found) return;

  String uidStr = uidToString(uid, uidLength);
  unsigned long now = millis();

  if (uidStr == lastUid && (now - lastReadMs) < DEBOUNCE_MS) {
    lastReadMs = now;
    return;
  }
  lastUid = uidStr;
  lastReadMs = now;

  Serial.println("--- Tag detected ---");
  Serial.print("UID length: ");
  Serial.println(uidLength);
  Serial.print("UID bytes:  ");
  for (uint8_t i = 0; i < uidLength; i++) {
    Serial.print("0x");
    if (uid[i] < 0x10) Serial.print("0");
    Serial.print(uid[i], HEX);
    if (i < uidLength - 1) Serial.print(" ");
  }
  Serial.println();
  Serial.print("UID string: ");
  Serial.println(uidStr);
}
