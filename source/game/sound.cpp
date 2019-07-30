#include "game/sound.h"

#include <tuple>

#include "common/utils.h"

namespace game::sound {

class StreamMgr;

static StreamMgr& GetStreamMgr() {
  return rst::util::GetInstance<StreamMgr>(std::tuple{0x7CB49C}, std::tuple{0x6B0A3C},
                                           std::tuple{0x1E11F8});
}

bool PlayEffect(u32 id) {
  return rst::util::GetPointer<bool(u32)>(0x2006E4)(id);
}

StreamId GetCurrentStreamId(StreamPlayer player) {
  return rst::util::GetPointer<StreamId(StreamMgr&, StreamPlayer)>(0x1E1194)(GetStreamMgr(),
                                                                             player);
}

bool PlayStream(StreamId id, StreamPlayer player) {
  return rst::util::GetPointer<bool(StreamMgr&, StreamId, StreamPlayer, u32)>(0x239228)(
      GetStreamMgr(), id, player, 0xffffffff);
}

}  // namespace game::sound
