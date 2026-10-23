# Third-party code and art

| What | Where | Origin | Licence |
|---|---|---|---|
| The static recompiler and its runtime, by Supermedo | `port/upstream/` (patched for the Switch) | [Supermedo/prince-of-persia-2-windows](https://github.com/Supermedo/prince-of-persia-2-windows) `e2c407f` | none stated upstream; credited here and in the README |
| LCD filter (lcd3x, by Gigaherz): its formula, drawn as a mask texture | `port/upstream/src/plat_sdl.c` | [frangarcj/vita-shader-collection](https://github.com/frangarcj/vita-shader-collection) `shaders/lcd3x_f.cg` | public domain |
| SDL2 2.28.5 (devkitPro's switch-sdl2 2.28.5-4), statically linked | the NRO | [libsdl-org/SDL](https://github.com/libsdl-org/SDL) `release-2.28.5` with [devkitPro's patch](https://github.com/devkitPro/pacman-packages/tree/master/switch/SDL2) | Zlib |
| Mesa 20.1 (switch-mesa), EGL and OpenGL ES, statically linked | the NRO | [devkitPro/mesa](https://github.com/devkitPro/mesa) | MIT |
| libdrm_nouveau 1.0.1, statically linked | the NRO | [devkitPro/libdrm_nouveau](https://github.com/devkitPro/libdrm_nouveau) | MIT |
| libnx 4.12.0, statically linked | the NRO | [switchbrew/libnx](https://github.com/switchbrew/libnx) | ISC |
| newlib (libc, libm, libsysbase), statically linked | the NRO | [devkitPro/newlib](https://github.com/devkitPro/newlib) | BSD-style, per file (`COPYING.NEWLIB`, `COPYING.LIBGLOSS`) |
| libgcc and libstdc++ (GCC 15.2), statically linked | the NRO | [GCC](https://gcc.gnu.org/), devkitA64 | GPL-3.0 with the GCC Runtime Library Exception 3.1 |
| The HOME-menu forwarder: nx-hbloader 2.4.5 with a forwarder patch | the forwarder NSP | [switchbrew/nx-hbloader](https://github.com/switchbrew/nx-hbloader) `82b9512`; the patch from [SwitchWakerHD](https://github.com/centollOS/SwitchWakerHD) | ISC (nx-hbloader); MPL-2.0 (the patch) |
| Icon and tile: the prince | `port/icon/` | SteamGridDB hero 66505 by Shiios42 | fan art, no licence stated |
| Tile: the title lettering | `port/icon/tile.jpg` | SteamGridDB logo 111904 by Kutsune (from LaunchBox) | fan art, no licence stated |
| Icon and tile: the sky | `port/icon/` | the game's title screen (LaunchBox's screenshot), upscaled by Real-ESRGAN | the game's art, its rights holder's |
