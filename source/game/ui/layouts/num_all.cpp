#include "game/ui/layouts/num_all.h"

namespace game::ui {

int NumAll::GetValue() const {
  return main_player->GetFrame();
}

int NumAll::GetColor() const {
  return color_player->GetFrame();
}

void NumAll::SetValue(int value) {
  main_player->SetAnimAndPause(main_player_anim, value);
}

void NumAll::SetColor(int color) {
  color_player->SetFrame(color);
}

}  // namespace game::ui
