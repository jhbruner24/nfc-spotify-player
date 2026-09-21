# Backend

Not yet built. Planned responsibility:

1. Receive a tag UID from the ESP32 (`POST /play` with `{ "uid": "04A12B..." }`).
2. Look up the matching Spotify URI (album / playlist / track).
3. Refresh the Spotify access token using the stored refresh token.
4. Call `PUT https://api.spotify.com/v1/me/player/play` with `context_uri`
   (or `uris` for a single track) targeting the currently active device.

Open decisions (see `docs/PROJECT_CONTEXT.md`):
- Runtime: Node vs. Flask; serverless vs. small always-on host.
- Whether the UID -> URI map lives here or is written to the tag as NDEF.

Secrets go in `.env` (see `.env.example`). Spotify playback control
requires a Premium account.
