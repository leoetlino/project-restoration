#include "game/state.h"

#include "common/debug.h"
#include "common/utils.h"

namespace game {

const StateInfo* FindStateInfoByInit(void (*init_fn)(State*)) {
  const auto* table = rst::util::GetPointer<StateInfo>(0x6883FC);
  for (size_t i = 0; i < 9; ++i) {
    if (table[i].init_fn == init_fn)
      return &table[i];
  }
  rst::util::Print("%s: ERROR - unknown game state. The game will break now.", __func__);
  return nullptr;
}

extern "C" const StateInfo* rst_GameStateGetNextStateInfo(State* state) {
  return FindStateInfoByInit(state->next_state_init_fn);
}

}  // namespace game
