#pragma once

#include "common/types.h"

namespace game::ui {

class Screen;

enum class ScreenType {
  Black,
  Main,
  Schedule,
  Quest,
  Items,
  Map,
  Masks,
  /// "Play your instrument" screen
  Ocarina,
};
Screen* GetScreen(ScreenType screen);
bool OpenScreen(ScreenType screen);

bool CheckCurrentScreen(ScreenType screen);

class Layout {
  u8 internal[0x170];
};
void UpdateLayout(Layout* layout);

class Font {
  u8 internal[0x14];
};

}  // namespace game::ui
