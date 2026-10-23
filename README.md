# Prince of Persia 2: The Shadow and the Flame for Nintendo Switch

Vibecoded. Based on the recompilation of the IR release by Supermedo. Adjusted for support of v1.1.
The same game code as the PS Vita port (psp2-pop2), on a Switch layer.

## What works

- All levels and cutscenes, played through on the Switch.
- FM music and sound effects (Sound Blaster Pro driver).
- Save and resume.
- Docked, handheld, standby
- 4:3 and widescreen views, four filters (Sharp, Pixel, Soft, LCD).
- All controls remappable.
- A HOME-menu icon (optional forwarder).

## Requirements

- A Switch running custom firmware (Atmosphère) with the Homebrew Menu.
- 20 MB free space on the SD card.
- The DOS game, **version 1.1 only**.
- For the HOME-menu icon: an NSP installer (Sphaira, DBI) and up-to-date sigpatches.

## Setup

1. Extract `nx-pop2.zip` from the [latest release](../../releases/latest) to the root of the SD card.
   It creates `switch/pop2-recomp/` with `pop2.nro` in it.
2. Copy the game files into `switch/pop2-recomp/`, including the `SNDDRVRS` folder.
3. Start Prince of Persia 2 from the Homebrew Menu. The first start checks every game file. Missing or changed files
   are named on-screen.

### HOME-menu icon (optional)

- Install `nx-pop2-forwarder.nsp` with your installer. The icon expects the path `sdmc:/switch/pop2-recomp/pop2.nro`.

### Updating

- Extract the new zip over the old one. Saves and settings are kept.

## Controls

| Button | In play |
|---|---|
| D-pad, left stick | movement |
| B | jump |
| A | grab, hang, step |
| Y | draw sword, strike |
| Plus | pause |
| Minus | timer |
| L | save screen |
| R | resume screen |
| Minus+L | restart the level |
| X+L | view: 4:3, Wide |
| X+R | filter: Sharp, Pixel, Soft, LCD |

In the save and resume screens, Down moves to the next slot, A confirms, B leaves. Any button skips a cutscene.

### Changing the controls

The game writes `sdmc:/switch/pop2-recomp/controls.ini` the first time it starts with its defaults.

```ini
; Prince of Persia 2 - the controls. Read when the game starts: edit, save, start the game again.
; Buttons: Up Down Left Right B A Y X L R Plus Minus
;          LStickUp LStickDown LStickLeft LStickRight RStickUp RStickDown RStickLeft RStickRight
;          ZL ZR LStickPress RStickPress None
; Several buttons: Up, B    A chord, the first held and the second pressed: X+L
; A button held for a chord still does its own action: keep Jump, Crouch, Left, Right, Action and Strike off it.
; A mistake falls back to that action's default and is named on the screen at start.
; Delete a line for its default; delete this file for every default.
; The Screen actions work in the save and resume screens only, the others outside them.
; Fixed: any button skips a cutscene.
[controls]
Jump=Up, LStickUp, B      ; jump, climb, block
Crouch=Down, LStickDown   ; crouch, put the sword away
Left=Left, LStickLeft     ; run left
Right=Right, LStickRight  ; run right
Action=A                  ; grab, hang on, step carefully
Strike=Y                  ; draw the sword, strike
Pause=Plus                ; pause
Time=Minus                ; show the time left
SaveScreen=L              ; the game's save screen
ResumeScreen=R            ; the game's resume screen
Restart=Minus+L           ; restart the level
View=X+L                  ; the next view: 4:3, Wide
Filter=X+R                ; the next filter: Sharp, Pixel, Soft, LCD
ScreenNext=Down, LStickDown ; save and resume screens: the next slot
ScreenConfirm=A           ; save and resume screens: save (name it) or resume
ScreenCancel=B, Plus      ; save and resume screens: leave
```

## Known issues

- None known. Please report new bugs.

## Reporting a bug

1. Extract the **diagnostics** build, `nx-pop2-diagnostics.zip` from the [latest release](../../releases/latest), over
   the normal one.
2. Reproduce the problem.
3. Attach to a [bug report](../../issues/new/choose):
   - `sdmc:/switch/pop2-recomp/debug.log`;
   - `sdmc:/switch/pop2-recomp/watchdog.log`, if present;
   - after a crash, the newest file in `sdmc:/atmosphere/crash_reports/`.

## Building

Building requires unpacking compressed overlays, which the recompiler cannot do. It also exposes the game code directly
which is why there are no building instructions here. This also applies to building your own Switch NRO or forwarder.

## Credits

- The static recompiler and its runtime by Supermedo:
  [prince-of-persia-2-windows](https://github.com/Supermedo/prince-of-persia-2-windows).
- [SDL2](https://github.com/libsdl-org/SDL), [devkitPro](https://devkitpro.org/) and
  [libnx](https://github.com/switchbrew/libnx).
- Switch version built with [Claude Code](https://claude.com/claude-code)
- The LCD filter: lcd3x by Gigaherz.
- The forwarder: [nx-hbloader](https://github.com/switchbrew/nx-hbloader) with the forwarder patch from
  [SwitchWakerHD](https://github.com/centollOS/SwitchWakerHD).
- Icon art: the prince by Shiios42 and the title lettering by Kutsune (SteamGridDB).
- Full list and licences: [THIRD_PARTY.md](THIRD_PARTY.md).

## Legal

- This project is not affiliated with or endorsed by Ubisoft or Nintendo.
- No game data is included.
- No donations are accepted.
- The port's code is MIT licensed ([LICENSE](LICENSE)). The recompiler states no licence and is credited above;
  other libraries keep their own licences ([THIRD_PARTY.md](THIRD_PARTY.md)).
