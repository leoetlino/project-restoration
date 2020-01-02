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

* **More Fluid Bomber's Notebook**: Less annoyingly slow popups and transitions
  * The Bomber's Notebook screen now updates instantly when it's opened instead of waiting for 1.8 seconds just before starting the "new" text animation (!), followed by another 1.18s pause and another 1.56s delay until the main text box appears (!!).

* **Song of Time Saving**: The Song of Time now optionally saves the game. Players will be greeted with the iconic *Dawn of the First Day* screen after loading a Song of Time save, just like in the original game.

* **More Effective Inverted Song of Time**: Slow time to 1/3 speed (as in the original)
  * Makes some glitchless challenge runs possible again.
  * Gives the player more time in a three-day cycle.

* **Improved Bosses**:
  * Removes the eyeballs from Odolwa, Goht and Gyorg.
  * Balances Odolwa's AI and attacks to improve gameplay.
  * Fixes a game bug that caused Odolwa to be invulnerable to attacks and block excessively.
  * **Improved Twinmold**: Less tedious, less confusing
    * Reduced the number of cycles to make it less repetitive.
    * Red Twinmold no longer resets its hit counter every time it burrows back into the sand. As a result, the battle is much less confusing for new players.

* **Restored Ice Arrows**: Ice arrows that work in more locations
  * Ice arrows now work everywhere in Great Bay Temple, not just in Gyorg's room. This gives the player the freedom to experiment with ice arrows. Ice arrows also work in several other areas, just like in the original game.
  * Removed the sparkling water effects as they were an insult to the player's intelligence.

* **Fast Arrow Switching**: Press ZL to quickly switch between arrow types (Normal/Fire/Ice/Light).

* **Dedicated Physical Buttons** for more convenient access to items and menus
  * Open the *Items* menu by pressing START.
  * The *Bomber's Notebook* is now mapped to SELECT.
  * *Ocarina of Time*: ZR+A
  * *Pictograph Box*: ZL
  * *I*: ZR+X
  * *II*: ZR+Y
  * It is now possible to assign items to I and II with a single press.
  * The Gears and Map menu can be opened with ZR+Start / ZR+Select respectively.
  * For the ocarina screen, it is possible to switch between the instrument screen and the song list with Start/Select.
  * Those are obviously optional and are primarily meant for Citra users.

* Other minor adjustments to improve the player's experience:
  * Adjusted the speed of Stone Tower blocks for a more pleasant climbing experience.
  * The moving platforms in Deku Palace and Woodfall Temple now actually always move.
  * The Bombers will no longer distract the player from a particular cutscene.
  * The new Sheikah hint stone will also no longer distract the player during an important cutscene in the Clock Tower.
  * Captain Keeta now walks as fast as in the original game, which restores the challenge of the chasing mini-game.
  * Link no longer plays the Elegy of Emptiness a second time after it's been played once. This makes Stone Tower a lot less tedious, considering it needs to be played 10+ times to beat Stone Tower Temple normally.
  * For similar reasons, the Song of Soaring is only repeated once during a play session.
  * Owl statues can now be activated by striking them with a sword.
  * It is possible to control Goron Link rolling just like in the Nintendo 64 version: hold A and move to roll, release A to stop. The classic control scheme co-exists with the new one.
  * Free camera mode can now be used more often, notably when targeting, talking or playing the ocarina.
  * Link can now leave the Swordman's School during training if necessary to avoid wasting time.
  * Fixed a game bug that caused the "heart container get" sound not to play after getting four heart pieces.
  * Fixed a game bug that prevented the player from leaving free camera mode by targeting when Zora swimming or Goron rolling.
  * Fixed a possible softlock during the Twinmold boss intro.

## Setup

*Project Restoration* works on both the 3DS console (both eShop and game card) and on the Citra emulator.

First, **determine which version of the game you have**. Because *Project Restoration* works by patching game code and injecting new code, and because Grezzo has released three different versions of the game executable, it is essential that you figure out which version you have in order to use the appropriate Project Restoration patch for your situation.

* If you have MM3D 1.0 (eShop or game card), you likely have v100 (FYI: v100 is the real, internal version number).
* If you have MM3D 1.1 on a game card and it came with 1.1 preinstalled, you likely have v110.
* If you have MM3D 1.1 (eShop or 1.0 game card), then you likely have v101. However for the rest of the instructions you should first try the v100 patch. Only try v101 if v100 doesn't work.

Using the incorrect patch will cause the console or Citra to abort and crash. If this happens, try the other two patches (after removing any file you copied to your SD/Citra folders while following this guide).

Now, download [the latest release of *Project Restoration*](https://github.com/leoetlino/project-restoration/releases) (**not** source code!) and open the 7z archive. Open the v100/v101/v110 folder in the archive and leave it open for the rest of the instructions.

Second, **determine your game's title ID**. Because the 3DS is a region-locked console, several versions of the game were released for different regions. Each of them has its own title ID.

* For the European release: 0004000000125600
* For the American release: 0004000000125500
* For the Japanese release: 00040000000D6E00

Keep both the version number and the title ID in mind, then follow the instructions for your platform (console or Citra) below.

Please note that:

* If the patch is applied successfully, **a sound effect will be played** right before the title screen, even before the spinning mask appears.
* In order to use the new ZL/ZR buttons, **the Circle Pad Pro must be enabled** in MM3D's options, even if you are playing on a New 3DS.
* This project only directly supports v100. All other versions of MM3D will be temporarily and automatically downgraded to 1.0 (v100) to ensure the code patches work.

### Console

* If you are a first time Luma3DS user, follow [this guide](https://3ds.hacks.guide/).
* Download [my version of Luma3DS](https://github.com/leoetlino/Luma3DS/releases)\* and put the `boot.firm` file at the root of your SD card.
* [**Only** if you have ever copied boot.firm to your NAND] Also copy the new boot.firm to your NAND using a tool like godmode9.
* [Enable game patching in Luma's menu](https://github.com/AuroraWright/Luma3DS/wiki/Optional-features).
* Create the /luma/titles/`TITLE_ID`/ directory/folder on your SD card (if it doesn't already exist). Obviously, please replace `TITLE_ID` with the actual title ID.
* Copy **code.bps** to that directory.
  * (If you want faster aiming, copy **code_faster_aim.bps** instead and rename the patch to code.bps.)
* Copy **exheader_legacy.bin** to that directory and rename the file to **exheader.bin**.

The relevant files on your SD card should look like this at the end:

* SD card root
  * boot.firm
  * [Folder] luma
    * [Folder] titles
      * [Folder] (the Title ID you determined earlier)
        * code.bps
        * exheader.bin

\* *Why is a fork necessary?* As of Jan 2020, the latest Luma3DS has a critical bug in the loader code and missing support for BPS patching. Both issues are fixed in the fork, and a fix for the former has been upstreamed. A [pull request](https://github.com/AuroraWright/Luma3DS/pull/1349) for BPS patch support has been submitted but not yet merged.

### Citra

Note: Canary 1529+ is required until my [pull request](https://github.com/citra-emu/citra/pull/5036) for BPS patch support is merged.

* Open the folder containing your game file (3ds/cia/app).
* Assuming your game file is called `GAME_FILE.extension` (full name including the file extension), create a directory/folder with the path **`GAME_FILE.extension`.exefsdir** (if it doesn't already exist).
* Copy **code.bps** to that directory.
  * If you want faster aiming, copy **code_faster_aim.bps** instead and rename the patch to code.bps.
* Copy **exheader.bin** to the folder containing your game file, then rename it to **`GAME_FILE.extension`.exheader**. It is **very important that you keep your game ROM's file extension, and also add '.exheader' at the end**!

**Example**: If your game is at /home/leo/games/3ds/zelda_mm.3ds, then:

* create /home/leo/games/3ds/zelda_mm.3ds.exefsdir
* copy **code.bps** to /home/leo/games/3ds/zelda_mm.3ds.exefsdir/**code.bps**
* copy **exheader.bin** and place the file in /home/leo/games/3ds/, calling it **zelda_mm.3ds.exheader** (note the double extension!)

**IMPORTANT: Read the instructions carefully again. It is all too common for people to make a mistake with file names.**

* Example: If your game file is called _00000000.app_, the exefsdir folder and the exheader must be called _00000000.app.exefsdir_ and _00000000.app.exheader_ respectively.
* Example: If your game file is called _zelda_mm3d.cia_, the exefsdir folder and the exheader must be called _zelda_mm3d.cia.exefsdir_ and _zelda_mm3d.cia.exheader_ respectively.

The directory which contains your game file should look like this at the end:

* [Folder] Containing directory
  * yourgamefilenamehere.3ds
  * yourgamefilenamehere.3ds.exheader
  * [Folder] yourgamefilenamehere.3ds.exefsdir
    * code.bps

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


## About the project

*Project Restoration* was born out of frustration with some of the changes in the remaster (regarding Zora Link in particular), with the realisation that I might be able to have my cake and eat it too, and get the best out of the original and MM3D's massively improved graphics and small quality of life improvements.

I personally consider the project to be pretty much complete. The game is **playable with no game-breaking issues**, has been 100% completed several times by myself and others (now that the project is more than two months old!) and I have fixed everything I have most wanted to fix, so I probably will not be working on it very actively anymore. Future versions of *Project Restoration* will most likely only be bug fix releases, which may occasionally bring small new improvements.

### Project structure

* `source/` *Project Restoration*'s source code.
  * `build/`: build outputs.
  * `common/`: small utilities.
  * `game/`: implementation for the known parts of *Majora's Mask 3D*. Contains headers and some reimplementation of game functions.
  * `rst/`: *Project Restoration* code.
* `v100/`, `v101/` and `v110/`: Version-specific data.
    * `hooks.hks`: configuration for patches and hooks (for Magikoopa).
    * `Version.cmake`: defines for *Project Restoration* code.
* `loader/`: Code loader (from [Magikoopa](https://github.com/RicBent/Magikoopa)).

Sometimes you will find *Project Restoration* extensions and new code under `source/game`. Those extensions will always be clearly labelled as such.

### Build instructions

* Put the original code.bin and exheader.bin in v100, v101 and v110.
* Run make_release.sh. You need git and Magikoopa in your PATH. **Currently, a [fork](https://github.com/leoetlino/Magikoopa) is required**.
* Generated code patches (code.bps) and patched exheaders can be found in `release/`.

PRs and help are welcome!
