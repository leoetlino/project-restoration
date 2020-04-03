#pragma once

#include "game/ui.h"

namespace game::ui {

class PlayAActionIcon;
class PlayBActionIcon;

enum class TatlHudState : u8 {
  Hidden = 4,
};

class PlayHud : public Layout {
public:
  Widget* chat_alert;
  Widget* event_timer;
  Widget* gyro_group;
  Widget* shooting_frame;
  Widget* a_action_btn;
  Widget* b_action_btn;
  Anim* chat_disp_anim;
  Anim* chat_idle_anim;
  Anim* chat_vanish_anim;
  AnimPlayer* main_player;
  AnimPlayer* btn_ab_pos_player;
  bool gyro_group_visible;
  TatlHudState tatl_state;
  Layout* layout_chat_alert;
  Layout* layout_event_timer;
  PlayAActionIcon* a_action_btn_l;
  PlayBActionIcon* b_action_btn_l;
  Layout* countdown_l;
  u16 countdown_package_id;
  u16 field_1B6;
  Layout* perfect_layout;
  u16 perfect_package_id;
  u8 gap_1BE[2];
  float field_1C0;
  AnimPlayer* timer_players[17];
  Widget* last_timer_g;
  Widget* game_timer_g;
  Anim* field_210;
  AnimPlayer* switch_minigame_player;
  Layout* num_layouts[4];
  Widget* minigame_g;
  bool fade_in;
  bool time_frozen;
  bool field_22E;
  bool field_22F;
  float opacity;
  u8 gap_234[8];
};
static_assert(sizeof(PlayHud) == 0x23C);

}  // namespace game::ui
