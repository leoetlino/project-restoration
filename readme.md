# Project Restoration

**Project Restoration** is a *Majora's Mask 3D* patch that restores some mechanics from the original *Majora's Mask*
and fixes some issues to make the game more enjoyable.

Please refer to the [official website at restoration.zora.re](https://restoration.zora.re) for features and setup instructions.

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
* `hooks/`: configuration for patches and hooks (for Magikoopa).
* `loader/`: Code loader (from [Magikoopa](https://github.com/RicBent/Magikoopa)).

Sometimes you will find *Project Restoration* extensions and new code under `source/game`. Those extensions will always be clearly labelled as such.

### Build instructions

* Put the original code.bin and exheader.bin files in the v100, v101 and v110 folders respectively. You must dump those binaries from the ExeFS yourself; those are **not** provided in this repository for copyright reasons.
* Run make_release.sh. You need git and Magikoopa in your PATH. **Currently, a [fork](https://github.com/leoetlino/Magikoopa) is required**.
* Generated code patches (code.bps) and patched exheaders can be found in `release/`.

PRs and help are welcome!
