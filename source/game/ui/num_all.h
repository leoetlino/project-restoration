#pragma once

#include "game/ui.h"

namespace game::ui {

class NumAll : public Layout {
public:
  int GetValue() const;
  int GetColor() const;

  void SetValue(int value);
  void SetColor(int color);

private:
  bool initialised = false;
  AnimPlayer* main_player = nullptr;
  AnimPlayer* color_player = nullptr;
  Anim* main_player_anim;
  Anim* number01_ani;
  Anim* hint_num;
};
static_assert(sizeof(NumAll) == 0x188);

}  // namespace game::ui
