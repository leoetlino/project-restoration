#include "game/ui/res.h"
#include "common/utils.h"

namespace game::ui {

float ResAnimKeyframe::GetValue(WidgetValueType value_type) const {
  switch (value_type) {
  case WidgetValueType::Visible:
    return static_cast<bool>(raw_value) ? 1.0 : 0.0;
  case WidgetValueType::Unk:
    return raw_value;
  default:
    return rst::util::BitCastPtr<float>(&raw_value);
  }
}

void ResAnimKeyframe::SetFloat(float value) {
  raw_value = rst::util::BitCastPtr<u32>(&value);
}

}  // namespace game::ui
