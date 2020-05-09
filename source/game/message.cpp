#include "game/message.h"

#include "common/utils.h"

namespace game {

MessageMgr& MessageMgr::Instance() {
  return rst::util::GetPointer<MessageMgr&()>(0x1C51D0)();
}

bool MessageData::Get(u16 id, Message* msg) const {
  int a = 0;
  int b = res_header->num_msgs - 1;
  const auto get_entry = [this](size_t idx) {
    return reinterpret_cast<const MessageResEntry*>((const u8*)res_entries + res_entry_size * idx);
  };
  const MessageResEntry* entry = nullptr;
  while (!entry && a <= b) {
    const int m = (a + b) / 2;
    const auto* candidate = get_entry(m);
    if (candidate->id < id)
      a = m + 1;
    else if (candidate->id > id)
      b = m - 1;
    else
      entry = candidate;
  }
  if (!entry || !msg)
    return false;

  msg->msgid = entry->id;
  msg->is_flag1 = entry->flags & 1;
  msg->is_flag1000 = entry->flags & 0x1000;
  msg->is_flag2000 = entry->flags & 0x2000;
  msg->flag10 = u32(u8(entry->flags)) << 0x18 >> 0x1C;
  msg->flag100 = u32(u16(entry->flags)) << 0x16 >> 0x1E;
  msg->field_10 = entry->field_2;
  auto x14 = entry->flags << 8 >> 0x18;
  msg->field_14 = x14 == 0xFF ? -1 : x14;
  auto x18 = entry->field_4 & 0x3FF;
  msg->field_18 = x18 == 0x3FF ? -1 : x18;
  auto x1C = u32(entry->field_4 << 0xC) >> 0x16;
  msg->field_1C = x1C == 0x3FF ? -1 : x1C;
  auto x20 = u32(entry->field_4 >> 0x14) & 0xFFFFF3FF;
  msg->field_20 = x20 == 0x3FF ? -1 : x20;

  for (size_t i = 0, res_idx = 0; i < NumLanguages; ++i) {
    auto& text = msg->texts[i];
    text.reader = rst::util::GetPointer<MessageReader*(Language)>(0x1C519C)(Language(i));
    if (res_header->languages.IsSet(Language(i))) {
      text.ptr = (u8*)res_header + entry->texts[res_idx].offset;
      text.size = entry->texts[res_idx].length;
      ++res_idx;
    } else {
      text.ptr = nullptr;
      text.size = 0;
    }
  }

  return true;
}

void PlayMessagePassSound() {
  rst::util::GetPointer<void()>(0x1AF034)();
}

void PlayMessageCancelSound() {
  rst::util::GetPointer<void()>(0x1AE498)();
}

}  // namespace game
