# sa-asi-loader

A simple [ASI Loader](https://gtamods.com/wiki/ASI_Loader) for Grand Theft Auto: San Andreas.

## Requirements

- cmake 3.19 (or higher)
- Visual Studio 17 2022

## Building

1. `mkdir build`
2. `cd build`
3. `cmake .. -G "Visual Studio 17 2022" -A Win32`
4. `cmake --build .` (optionally, suffix with `--config Release`)

A `vorbisFile.dll` will be output to the `Debug`/`Release` folder. Before moving that file into the root folder of GTA: San Andreas, rename the original `vorbisFile.dll` to `vorbisFileHooked.dll` (or whichever name you specify in as the original file in `dllmain.h`).

Your directory should look like this:

```
GTA San Andreas
`-- vorbis.dll
`-- vorbisFile.dll (the build artifact)
`-- vorbisFileHooked.dll (the renamed, original file)
```

You should then be able to load any ASI mod upon starting up the game.