#include "game/ui/screens/main_screen.h"
#include "common/utils.h"

namespace game::ui {

// TODO: figure out how to link to the actual vtable properly...

void MainScreen::Init(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x599D20)(this, ctx);
}

void MainScreen::Open(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B4D4)(this, ctx);
}

void MainScreen::Calc(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B6A0)(this, ctx);
}

void MainScreen::Draw(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B27C)(this, ctx);
}

void MainScreen::PrepareClose(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59BC10)(this, ctx);
}

bool MainScreen::ShouldClose(ScreenContext& ctx) {
  return rst::util::GetPointer<bool(MainScreen*, ScreenContext&)>(0x59B3CC)(this, ctx);
}

void MainScreen::Close(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B388)(this, ctx);
}

void MainScreen::UpdateButtons(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59A874)(this, ctx);
}

void MainScreen::UpdateRupee(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59AB38)(this, ctx);
}

void MainScreen::UpdateHeart(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59ACBC)(this, ctx);
}

void MainScreen::UpdateMagic(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59AED0)(this, ctx);
}

void MainScreen::UpdateKey(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B040)(this, ctx);
}

}  // namespace game::ui
