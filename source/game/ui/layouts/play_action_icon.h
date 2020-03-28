#pragma once

#include "common/types.h"
#include "game/ui.h"

namespace game {
struct GlobalContext;
}

namespace game::ui {

class ItemIcon;

class PlayAActionIcon : public Layout {
public:
  virtual void Update(GlobalContext* gctx);

  Widget* a_action_text;
  Widget* a_action_num;
  Widget* a_button_base;
  Anim* field_17C;
  Anim* field_180;
  AnimPlayer* main_player;
  AnimPlayer* number_player;
  AnimPlayer* disp_vanish_player;
  u16 text_num;
  u8 field_192;
};
static_assert(sizeof(PlayAActionIcon) == 0x194);

class PlayBActionIcon : public PlayAActionIcon {
public:
  Widget* a_action_icon;
  ItemIcon* a_action_icon_l;
};
static_assert(sizeof(PlayBActionIcon) == 0x19C);

}  // namespace game::ui
