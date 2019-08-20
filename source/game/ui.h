#pragma once

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

}  // namespace game::ui
