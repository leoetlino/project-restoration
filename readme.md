# Project Restoration

A *Majora's Mask 3D* patch that restores some mechanics from the original *Majora's Mask*
and fixes some issues to make the game even more enjoyable.

**Note**: Some features make use of the new ZL/ZR buttons,
so playing on a New 3DS/2DS or Citra is recommended for a better experience.

## Changes

* **Zora Fast Swim**: Swim gracefully as a Zora without having to use magic
  * Fast swim is now the default way of swimming and no longer requires magic.
  * Slow swim is still available. Hold ZL to slow swim.

* **Fixed Deku Hopping**: Hop on water as fast as in MM
  * Deku Link's walk acceleration value was nerfed in MM3D (2.0 -> 0.6). As a possibly unintended consequence, this made hopping on lilypads very slow compared to the original even if you spin at the optimal time.

* **Fast Transform**: Transform without having to equip items for a more streamlined gameplay
  * Three of the four D-Pad buttons are now used to fast transform (*Left*: Zora, *Up*: Goron, *Down*: Deku)
  * This frees up as many as 3 buttons!

* **Fast Ocarina**: Dedicated physical button for the Ocarina of Time
  * Press ZR to play the instrument.

* ~~**More Effective Inverted Song of Time**: The ISoT makes time go at 1/3 speed (rather than 1/2).~~ Temporarily disabled because of issues with some NPCs.
  * Makes some glitchless challenge runs possible again.
  * Gives the player more time in a three-day cycle.

* **Improved Twinmold Battle**: Less tedious, less confusing
  * Reduced the number of cycles to make it less repetitive.
  * Red Twinmold no longer resets its hit counter every time it burrows back into the sand. As a result, the battle is much less confusing for new players.

* **Restored Ice Arrows**: Ice arrows that work everywhere
  * Ice arrows now work in all locations, not just in Gyorg's room. This gives the player the freedom to experiment with ice arrows.
  * Removed the sparkling water effects as they were an insult to the player's intelligence.

### Planned changes

In a roughly increasing order of difficulty.

* **Fixed Deku Palace Platforms**: In the remaster, the moving platforms don't actually... move until you land on them. This should be fixed.

* **Faster Captain Keeta**: In MM3D, Captain Keeta walks a lot slower and the battle ends a lot faster. This was probably another attempt at making the game more accessible, but IMO it wasn't even that difficult in the first place. I don't particularly care about this but someone has suggested reverting this change.

* **More Fluid Bomber's Notebook**: MM3D has annoying long popups and UI animations. It should be possible to decrease the transition durations and reduce pauses.

### Considered changes

* **Optional Saving via Song of Time**: This was suggested to me and I have no strong opinion on this, though adding a brand new option to save the game is probably a bit difficult to implement, especially considering it will likely require text edits, when Citra still doesn't support LayeredFS-style patching functionality.


## Setup

Currently, the patch has been tested with:

* MM3D 1.0.0 on Citra: works
* MM3D 1.1.0 on Citra: works
* MM3D 1.0.0 on console: works
* MM3D 1.1.0 on console: **broken** -- probably because the cartridge 1.1 version is different from the eShop 1.1

First, download the [latest release](https://github.com/leoetlino/project-restoration/releases) that **corresponds to your game version**. You can determine the version by checking whether "v1.1" is shown on the title screen.

**In the following instructions, replace 00125600 with 00125500 if you're playing the US version.**

### Console

On console, [Luma3DS](https://github.com/AuroraWright/Luma3DS) (or a loader that supports IPS patching and exheader replacement)  is required.

**IMPORTANT**: Luma currently crashes because of a critical bug in its loader. I've submitted [a fix](https://github.com/AuroraWright/Luma3DS/pull/1287) to Luma3DS; in the meantime, please copy [my `boot.firm` file](https://github.com/leoetlino/project-restoration/releases) to the root of your SD card.

* Open the release archive.
* Create /luma/titles/0004000000125600/ (SD) on your SD card (if it doesn't already exist).
* Copy **code.ips** to /luma/titles/0004000000125600/ and rename the file to **code.ips** (SD).
* Copy **exheader_legacy.bin** to /luma/titles/0004000000125600/ and rename the file to **exheader.bin** (SD).

### Citra

A [**canary build**](https://citra-emu.org/download/) of Citra is required, because I had to add the required code patching functionality myself (PR #4812, #4813, #4817), and those changes haven't all been merged to master yet.

Let *GAME_FILE* be the path to the game file (3ds/cia/app). If you've installed the game, GAME_FILE is "sdmc/Nintendo 3DS/00000000000000000000000000000000/00000000000000000000000000000000/title/00040000/00125600/content/xxxxxxxx.app" (where xxxxxxxx.app is the largest file in that directory).

* Open the release archive.
* Create ***GAME_FILE*.exefsdir** (if it doesn't already exist).
* Copy **code.ips** to *GAME_FILE*.exefsdir/ and rename the file to **code.ips**.
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
