#pragma once

#include "common/types.h"
#include "common/utils.h"
#include "game/ui.h"

namespace game {
struct GlobalContext;
}

namespace game::ui {

#pragma pack(push, 4)
class MessageWindow : public Layout {
public:
  void* vtable_170;
  void* vtable_174;
  u8 gap_178[368];
  Layout* note_layouts[8];
  Widget* plate;
  u8 gap_30C[4];
  Widget* e_waitIcon;
  Widget* itemIcon;
  Widget* branchArrow;
  Widget* num_widgets[5];
  Widget* num_base;
  u8 gap_334[4];
  Widget* notes;
  Widget* note_widgets[8];
  Layout* e_text_layout;
  Layout* mainPlateLayout;
  u32 field_364;
  int field_368;
  u8 gap_36C[56];
  u32 num_choices;
  u8 gap_3A8[16];
  u8 field_3B8;
  u8 gap_3B9[2];
  u8 field_3BB;
  u8 gap_3BC[32];
  int status;
  int field_3E0;
  int field_3E4;
  char field_3E8[4];
  int field_3EC;
  u32 field_3F0;
  int msgid_2;
  int field_3F8;
  int msgid;
  double field_400;
  u32 field_408;
  u8 gap_40C[4];
  u32 field_410;
  u32 field_414;
  u8 gap_418[4];
  int field_41C;
  u32 field_420;
  u32 field_424;
  u32 song;
  u32 state;
  int choice;
  u32 field_434;
  u8 gap_438;
  char field_439;
  char field_43A;
  char field_43B;
  int field_43C;
  int field_440;
  GlobalContext* gctx;
  AnimPlayer* main_player;
  AnimPlayer* sub_player;
  AnimPlayer* switch_plate_player;
  AnimPlayer* ocarina_player;
  AnimPlayer* localize_plat_eplayer;
  AnimPlayer* plate_alpha_player;
  Anim* open;
  Anim* close;
  Anim* mapTelop_display;
  Anim* mapTelop_vanish;
  Anim* timeTelop_display;
  Anim* timeTelop_vanish;
  Anim* keyWait;
  Anim* branchWait;
  Anim* textFadein;
  Anim* textFadeout;
  Anim* failure;
  Anim* num_base_anim;
  Anim* switch_loc_win;
  u32 field_494;
  u8 gap_498[1432];
  u32 field_A30;
};
static_assert(sizeof(MessageWindow) == 0xA34);
#pragma pack(pop)

}  // namespace game::ui
