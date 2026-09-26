# zVideoSubtitles

A plugin which allows the playback of `.srt` files from the `_Work/Data/Video/` directory. Originally developed for Archolos 2.0, open-sourced for public use.

- Places a dialogue box underneath the text.
- Works with GD3D11 as well as without it.
- Works correctly when video is paused.

If the `Subtitles` game setting is turned off, the video subtitles will also be turned off.

## Showcase

<p align="center">
  <img alt="The Chronicles of Myrtana: Archolos" src="https://github.com/user-attachments/assets/2737d8a1-a666-4590-a407-3aaaccf765de" width="49%" />
  <img alt="Gothic II: Gold" src="https://github.com/user-attachments/assets/63d45459-826c-44a0-9602-833567ecde0c" width="49%" />
  <br/>
  <img alt="Gothic Sequel" src="https://github.com/user-attachments/assets/1f0da659-5ad3-4325-9a6a-a0cf848bf407" width="49%" />
  <img alt="Gothic" src="https://github.com/user-attachments/assets/fb1c2892-e0ac-4082-bd37-0e9801dd7b82" width="49%" />
</p>

## Usage
The plugin automatically loads and displays the `.srt` file corresponding to the currently playing BIK file. 

File structure example:
```text
_Work/
    Data/
        Video/
            intro.bik
            intro.srt
```

Since the plugin uses the ingame font for text rendering, `.srt` files must be encoded according to your game's localization:
- `cp1250` for Polish, Czech
- `cp1251` for Russian
- `cp1252` for English, German, Spanish, Italian, French

`.srt` files can be both packed (in `.vdf` volumes) or unpacked.
