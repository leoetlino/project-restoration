#pragma once

#include "common/types.h"
#include "game/ui/screens/screen.h"

namespace game {
struct GlobalContext;
}  // namespace game

namespace game::ui {

class Anim;
class AnimPlayer;
class ItemIcon;
class Layout;
class NumAll;
class Widget;

/// Used for normal gameplay most of the time; shows the touch panel (hearts, magic, etc.) on the
/// bottom screen.
class MainScreen : public Screen {
public:
  void Init(ScreenContext&) override;
  void Open(ScreenContext&) override;
  void Calc(ScreenContext&) override;
  void Draw(ScreenContext&) override;
  void PrepareClose(ScreenContext&) override;
  bool CalcClose(ScreenContext&) override;
  void Close(ScreenContext&) override;

  void MagicPlayAndHideYellowGauge();
  void MagicHideYellowGauge();
  void MagicStartUseAnim();
  /// Called from the global context.
  void MagicUpdateCost();

  /// This is supposed to be called from the global context on scene change.
  void UpdateKeyVisibility(game::GlobalContext& gctx);

protected:
  struct AnimPlayerAndAnim {
    AnimPlayer* player;
    Anim* anim;
  };

  void UpdateButtons(ScreenContext&);
  void UpdateRupee();
  void UpdateHeart();
  void UpdateMagic();
  void UpdateKey(Context&);
  void UpdateCarrot(game::GlobalContext& gctx);

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
  Layout* playTouchPanel = nullptr;
  Layout* CommonBottomBg = nullptr;
  Layout* playBg = nullptr;
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

  /// Y button icon.
  ItemIcon* icon_y_btn;
  /// X button icon.
  ItemIcon* icon_x_btn;
  /// I button icon.
  ItemIcon* icon_c_btn01;
  /// II button icon.
  ItemIcon* icon_c_btn03;
  /// Ocarina button icon.
  ItemIcon* icon_c_btn02;

  // Rupee numbers.
  NumAll* rank_a_l;
  NumAll* rank_ten_l;
  NumAll* rank_hundred_l;

  NumAll* num_key_l;
  NumAll* num_key_rank_ten_l;

  AnimPlayer* transition = nullptr;
  AnimPlayer* gauge_value = nullptr;
  AnimPlayer* gauge_scale = nullptr;
  AnimPlayer* gauge_color = nullptr;
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
  AnimPlayerAndAnim carrot_anims[6];
  u16 field_1E0;
  u16 field_1E2;
  s16 shownRupeeCount;
  u8 numSmallKeys;
  s8 magic_max = -1;
  s8 magic = -1;
  s8 has_magic = -1;
  s8 magic_num_upgrades = -1;
  /// Non-zero when magic is unlimited (after drinking Chateau Romani).
  u8 magic_unlimited = 0xFF;
  u8 field_1EC = 0;
  u8 num_carrots;
  u8 field_1EE = 0;

  bool resume_bgm_on_open = false;
};
static_assert(sizeof(MainScreen) == 0x1F0);

}  // namespace game::ui
