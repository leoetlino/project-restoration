#pragma once

#include "game/ui.h"

namespace game::ui {

class ItemIcon : public Layout {
public:
  int GetIcon() const { return icon_id; }
  int GetItemCount() const { return item_count; }

  void SetIcon(int icon);
  void SetIconForItem(int item);
  void SetCount(int count);
  void SetCountVisible(bool visible);
  void SetCountColor(int color);

private:
  bool initialised = false;
  int icon_id = 0;
  int item_count = 0;
  Widget* icon_number = nullptr;
  Widget* rank_ten;
  AnimPlayer* item_icon_player;
  AnimPlayer* rank_ten_player;
  AnimPlayer* rank_a_player;
  AnimPlayer* color_player;
  AnimPlayer* main_player = nullptr;
  Anim* idle_anim;
  Anim* pressed_anim;
  Anim* activated_anim;
  Anim* disabled_anim;
  Anim* unusable_anim;
  Anim* selected_anim;
  AnimPlayer* return_player;
  Anim* return_to_start_anim;
  Anim* return_vanish_idle_anim;
  Anim* return_remain_idle_anim;
  Anim* return_empty_idle_anim;
  Anim* equip_anim;
  Pane* item_icon_pane = nullptr;
};
static_assert(sizeof(ItemIcon) == 0x1CC);

}  // namespace game::ui
