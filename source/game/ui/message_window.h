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
  u32 field_2E8;
  u8 gap_2EC[28];
  int field_308;
  u8 gap_30C[4];
  int field_310;
  u32 field_314;
  int field_318;
  u8 gap_31C[20];
  int field_330;
  u8 gap_334[4];
  u32 field_338;
  u8 gap_33C[32];
  u32 field_35C;
  u32 field_360;
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
  u32 field_448;
  u32 field_44C;
  int field_450;
  u32 field_454;
  u8 gap_458[4];
  int field_45C;
  int field_460;
  int field_464;
  int field_468;
  int field_46C;
  int field_470;
  int field_474;
  int field_478;
  int field_47C;
  int field_480;
  int field_484;
  int field_488;
  int field_48C;
  int field_490;
  u32 field_494;
  u8 gap_498[1432];
  u32 field_A30;
};
static_assert(rst::util::OffsetOf(&MessageWindow::song) == 0x428);
static_assert(sizeof(MessageWindow) == 0xA34);
#pragma pack(pop)

}  // namespace game::ui
