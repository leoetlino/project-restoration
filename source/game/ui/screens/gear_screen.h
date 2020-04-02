#pragma once

#include "common/types.h"
#include "game/ui/screens/screen.h"

namespace game::ui {

class Anim;
class AnimPlayer;
class Layout;
class Widget;

/// Referred to as the Collect screen internally.
class GearScreen : public Screen {
public:
  void* press_handler;
  u16 field_8;
  u16 field_A;

  Layout* subMenuTitle;
  Layout* subMenuUpCollect;
  Layout* menuCollect;
  Layout* commonBottomBg;

  Layout* btn_return_top_l;
  Layout* btn_return;
  Layout* btn_option_l;

  AnimPlayer* main_player;
  Anim* in_menu_collect_anim;
  Anim* out_menu_collect_anim;
  u8 gap_34[240];
  float field_124;
  float field_128;
  int field_12C;

  Layout* cursor_item_collect;

  Layout* icon_event00_l;
  Layout* icon_event01_l;
  Layout* icon_event02_l;
  Layout* icon_event03_l;
  Layout* icon_shadowgraph_box_l;

  Layout* icon_purse_l;
  Layout* icon_bomb_bag_l;
  Layout* icon_arrow_l;
  Layout* icon_sword_l;
  Layout* icon_shield_l;
  Layout* icon_boss00_l;
  Layout* icon_boss02_l;
  Layout* icon_boss01_l;
  Layout* icon_boss03_l;
  Layout* icon_schedule_l;
  Layout* icon_okarina_l;
  Layout* icon_trumpet_l;
  Layout* icon_drum_l;
  Layout* icon_guitar_l;
  Layout* icon_f_tickets_l;

  Layout* heart_pieces_l;

  AnimPlayer* frame_deformation_player;
  Widget* blink_picture_g;
  Widget* small_picture_g;

  u8 field_194;
  u8 field_195;
  u8 field_196;
  u8 field_197;
  u32 pressed_btn_option;
  u32 field_19C;
  u32 field_1A0;
  u8 has_opened_option_menu;
};
static_assert(sizeof(GearScreen) == 0x1A8);

}  // namespace game::ui
