# zVideoSubtitles

A plugin which allows the playback of `.srt` files from the _Work/Data/Video/ directory. Originally developed for Archolos 2.0, opensourced for public use.

Works with GD3D11 as well as without it.

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

The plugin will not load any subtitles if the `subTitles` option is turned off in the game settings.

## Screenshots

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/2737d8a1-a666-4590-a407-3aaaccf765de" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/fc96eb21-07d2-4eac-a9b7-070d9943cf45" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/9be5aab8-b8cd-46ac-9838-93d8c44cc490" />
