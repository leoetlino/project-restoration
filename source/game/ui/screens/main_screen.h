#pragma once

#include "common/types.h"
#include "game/ui/screens/screen.h"

namespace game::ui {

class Anim;
class AnimPlayer;
class Layout;
class Widget;

/// Used for normal gameplay most of the time; shows the touch panel (hearts, magic, etc.) on the
/// bottom screen.
class MainScreen : public Screen {
private:
  /// Button press handler?
  void* vtable_4;
  Widget* field_8;
  Widget* iconKeyPane;
  Widget* heartStatus;
  Widget* magicGauge_g;
  Widget* gaugeYellow_g;
  Widget* carrotStatus;
  Widget* camChangePane;
  Widget* camPicturePane;
  Widget* camReturn;
  Widget* dispNoMapPane;
  Widget* rankTen;
  Widget* rankHundred;
  Layout* playTouchPanel;
  Layout* CommonBottomBg;
  Layout* playBg;
  Layout* btn_y_g;
  Layout* btn_x_g;
  Layout* btn_corner01_g;
  Layout* btn_corner03_g;
  Layout* btn_item_g;
  Layout* btn_map_g;
  Layout* btn_mask_g;
  Layout* btn_corner00_g;
  Layout* btn_collect_g;
  Layout* btn_corner02_g;
  Layout* icon_y_btn;
  Layout* icon_x_btn;
  Layout* icon_c_btn01;
  Layout* icon_c_btn03;
  Layout* icon_c_btn02;
  Layout* rankA_l;
  Layout* rankTen_l;
  Layout* rankHundred_l;
  Layout* numKey_l;
  Layout* numKeyRankTen_l;
  AnimPlayer* transition;
  AnimPlayer* gauge_value;
  AnimPlayer* gauge_scale;
  AnimPlayer* gauge_color;
  AnimPlayer* magic_gauge_position;
  AnimPlayer* magic_gauge_value;
  AnimPlayer* lupy_color;
  AnimPlayer* key_change;
  AnimPlayer* magic_use;
  AnimPlayer* camera_icon_state;
  Anim* magic_use_ani;
  Layout* heartLayouts[20];
  AnimPlayer* heartBreakPlayers[20];
  AnimPlayer* heartBeatPlayers[20];
  int field_1B0[12];
  u16 field_1E0;
  u16 field_1E2;
  s16 shownRupeeCount;
  u8 numSmallKeys;
  s8 magic_max;
  s8 magic;
  s8 magic_stuff;
  s8 magic_anonymous_l;
  s8 magic_stuff_2;
  u8 field_1EC;
  u8 field_1ED;
  u8 field_1EE;
  u8 field_1EF;
};
static_assert(sizeof(MainScreen) == 0x1F0);



}  // namespace game::ui
