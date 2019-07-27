# Project Restoration

A *Majora's Mask 3D* patch that restores some mechanics from the original *Majora's Mask*
and fixes some issues to make the game even more enjoyable.

**Note**: Some features make use of the new ZL/ZR buttons,
so playing on a New 3DS/2DS or Citra is recommended for a better experience.

## Changes

* **Zora Fast Swim**: Swim gracefully as a Zora without having to use magic
  * Fast swim is now the default way of swimming and no longer requires magic.
  * Slow swim is still available. Press Up/Down on the D-Pad to *toggle* between fast swim and slow swim. You can also *hold* ZL to slow swim.

* **Fixed Deku Hopping**: Hop on water as fast as in MM
  * Deku Link's walk acceleration value was nerfed in MM3D (2.0 -> 0.6). As a possibly unintended consequence, this made hopping on lilypads very slow compared to the original even if you spin at the optimal time.

* **Fast Transform**: Transform without having to equip items for a more streamlined gameplay
  * Three of the four D-Pad buttons are now used to fast transform (*Left*: Zora, *Up*: Goron, *Down*: Deku)
  * This frees up as many as 3 buttons!

* **Dedicated Physical Buttons** for more convenient access to items and menus
  * Open the *Items* menu by pressing START.
  * The *Bomber's Notebook* is now mapped to SELECT.
  * *Ocarina of Time*: ZR+A
  * *Pictograph Box*: ZL
  * *I*: ZR+X
  * *II*: ZR+Y
  * It is now possible to assign items to I and II with a single press.

* **More Fluid Bomber's Notebook**: Less annoyingly slow popups and transitions
  * The Bomber's Notebook screen now updates instantly when it's opened instead of waiting for 1.8 seconds just before starting the "new" text animation (!), followed by another 1.18s pause and another 1.56s delay until the main text box appears (!!).

* **Fast Arrow Switching**: Press ZL to quickly switch between arrow types (Normal/Fire/Ice/Light).

* **More Effective Inverted Song of Time**: Slow time to 1/3 speed (as in the original)
  * Makes some glitchless challenge runs possible again.
  * Gives the player more time in a three-day cycle.

* **Improved Twinmold Battle**: Less tedious, less confusing
  * Reduced the number of cycles to make it less repetitive.
  * Red Twinmold no longer resets its hit counter every time it burrows back into the sand. As a result, the battle is much less confusing for new players.

* **Restored Ice Arrows**: Ice arrows that work in more locations
  * Ice arrows now work everywhere in Great Bay Temple, not just in Gyorg's room. This gives the player the freedom to experiment with ice arrows. Ice arrows also work in several other areas, just like in the original game.
  * Removed the sparkling water effects as they were an insult to the player's intelligence.

* Other minor adjustments to improve the player's experience:
  * The moving platforms in Deku Palace and Woodfall Temple now actually always move.
  * The Bombers will no longer distract the player from a particular cutscene.
  * The new Sheikah hint stone will also no longer distract the player during an important cutscene in the Clock Tower.
  * Captain Keeta now walks as fast as in the original game, which restores the challenge of the chasing mini-game.
  * Link no longer plays the Elegy of Emptiness a second time after it's been played once. This makes Stone Tower a lot less tedious, considering it needs to be played 10+ times to beat Stone Tower Temple normally.

For a list of planned changes, see the [project dashboard](https://github.com/leoetlino/project-restoration/projects/1).

## Setup

1. Download the [latest release](https://github.com/leoetlino/project-restoration/releases).
2. Determine which patch to use.
    * If you have MM3D 1.0, use v100 (which targets the MM3D 1.0 code).
    * If you have MM3D 1.1, you need to try all three patches and choose whichever works. If your game card came with MM3D 1.1 preinstalled, use v110. Otherwise, try v100 even if you have the 1.1 update installed. If it still won't work, try v101.
3. Determine what your game's Title ID is:
    * MM3D EU: 0004000000125600
    * MM3D US: 0004000000125500
4. Follow the instructions (for console or Citra) below.

Notes:

* If the Project Restoration patch is applied successfully, a sound effect will be played right before the title screen.
* In order to use the new ZL/ZR buttons, the Circle Pad Pro must be enabled in MM3D's options even on a New 3DS.

### Console

A special version of [Luma3DS](https://github.com/AuroraWright/Luma3DS) that supports IPS and BPS patching is required. Download [it from my repository](https://github.com/leoetlino/Luma3DS/releases) and put the `boot.firm` file at the root of your SD card. (Upstreaming BPS patch support is planned once it has received enough testing.)

You may also need to copy boot.firm to your NAND using a tool like godmode9.

* [Enable game patching](https://github.com/AuroraWright/Luma3DS/wiki/Optional-features) in Luma's menu.
* Create the /luma/titles/*TITLE_ID*/ directory on your SD card (if it doesn't already exist).
* Copy **code.ips** or **code.bps** (whichever file exists) to that directory.
* Copy **exheader_legacy.bin** to that directory and rename the file to **exheader.bin**.

### Citra

A [**canary build**](https://citra-emu.org/download/) of Citra is required, because I had to add the required code patching functionality myself (PR #4812, #4813, #4817), and those changes haven't all been merged to master yet.

**Because Citra does not support BPS patches, only 1.0.0 is supported.** If you have another version, you need to extract the code.bin and apply the patch to code.bin yourself, then put the modified code.bin in exefsdir (see below).

Let *GAME_FILE* be the path to the game file (3ds/cia/app). If you've installed the game, GAME_FILE is "sdmc/Nintendo 3DS/00000000000000000000000000000000/00000000000000000000000000000000/title/00040000/00125x00/content/xxxxxxxx.app" (where xxxxxxxx.app is the largest file in that directory).

* Create the ***GAME_FILE*.exefsdir** directory (if it doesn't already exist).
* Copy **code.ips** to that directory and rename the file to **code.ips**.
* Copy **exheader.bin** to the same folder as the game file, then rename it to ***GAME_FILE*.exheader**.

**Example**: If your game is at /home/leo/games/3ds/zelda_mm.3ds, then:

* create /home/leo/games/3ds/zelda_mm.3ds.exefsdir
* copy **code.ips** to /home/leo/games/3ds/zelda_mm.3ds.exefsdir/**code.ips**
* copy **exheader.bin** and place the file in /home/leo/games/3ds/, calling it **zelda_mm.3ds.exheader**

## Rationale

### Zora swim
In MM3D, swimming is a bit slower. It is possible to fast swim; however it now requires and consumes magic at a fast rate. Chateau Romani isn't a satisfactory workaround: it only becomes available after a bunch of quests and requires wasting most of the First Day, and even then it's still impossible to get rid of the constant buzzing sound that comes from using the barrier.

Besides, why would Zora Link need magic to swim like a Zora?

### Inverted Song of Time potency

The ISoT nerf might have been another unintended change.

The in-game time is updated by adding a speed value and another value I'll call the extra speed to the time variable every frame. In MM, the time speed is usually 3 (units/frame) and the ISoT sets the extra speed to -2, resulting in a +1 effective speed (which means 1/3 speed). Because the time is updated every frame, in MM3D, the developers reduced the speed to 2 to compensate for the increased framerate. The ISoT was updated to set the speed to -1 instead of -2. However, that only gives players 1/2 speed.

I couldn't see any good reason to keep this change, so I reverted it.

### Twinmold

The new Twinmold battle drags on for way too long. Spinning the main stick makes it faster, but that's not an obvious mechanic. Even with that trick, killing Red Twinmold still takes 3 long identical cycles!

Another issue is the addition of a hidden hit counter. 10 hits are required to stun Red or Blue Twinmold. This would have been acceptable if it weren't for the fact that Red Twinmold regularly burrows back into sand during phase 2
and the hit counter is silently reset every time that happens.

This makes for a confusing experience the first time the player fights Twinmold,
as there is nothing in the game that indicates that the hit counter resets every time,
and it's still frustrating on subsequent playthroughs.


## Project information
* `source/` *Project Restoration*'s source code.
  * `addresses.h`: Version-specific memory addresses.
* `v100` and `v110`: Version-specific data.
    * `hooks.hks`: configuration for patches and hooks (for Magikoopa).
    * `Version.cmake`: defines for our own code.
* `loader/`: Code loader (from [Magikoopa](https://github.com/RicBent/Magikoopa)).

To build the project:

* Put the original code.bin and exheader.bin in v100 and v110.
* Run make_release.sh. You need git and Magikoopa in your PATH.
* Generated code patches (code.ips) and patched exheaders can be found in `release/`.

PRs and help are welcome!
