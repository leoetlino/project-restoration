#include "game/message.h"
#include "common/types.h"

namespace rst {

using namespace game;

RST_HOOK bool MessageDataGet(MessageData* self, u16 id, Message* out) {
  // Extension: replace Tatl's message about Odolwa's eye with a warning about Odolwa's dances.
  if (id == 0x195e) {
    if (!self->Get(0x1962, out))
      return false;
    out->msgid = 0x195e;
    return true;
  }

  return self->Get(id, out);
}

}  // namespace rst
