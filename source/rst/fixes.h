#pragma once

namespace rst {

void FixTwinmold();

void FixIceArrows();

void FixDekuMovingPlatforms();

// Fix some NPCs being too distracting
void FixBombers();
void FixHintStone();

// Fix the free camera not resetting when Z-targeting in some cases.
void FixFreeCameraReset();

// Make it possible to activate owl statues by striking them with a sword.
void FixOwlStatueActivationTrigger();

}  // namespace rst
