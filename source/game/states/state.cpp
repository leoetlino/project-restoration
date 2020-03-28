#include "game/states/state.h"

#include <algorithm>

#include "common/debug.h"
#include "common/utils.h"
#include "game/states/state_sot_save.h"

namespace game {

template <class T>
static constexpr StateInfo GenerateStateInfo() {
  StateInfo info{};
  info.init_fn = [](auto* state) { new (state) T(); };
  info.instance_size = sizeof(T);
  info.type = T::Type;
  return info;
}

static constexpr StateInfo ex_state_info_table[] = {
    GenerateStateInfo<StateSotSave>(),
};

template <typename Predicate>
static const StateInfo* FindStateInfo(Predicate predicate) {
  const auto* table = rst::util::GetPointer<StateInfo>(0x6883FC);
  const auto* table_end = table + 10;
  auto it = std::find_if(table, table_end, predicate);
  if (it != table_end)
    return it;

  // Search for extensions.
  it = std::find_if(std::begin(ex_state_info_table), std::end(ex_state_info_table), predicate);
  if (it != std::end(ex_state_info_table))
    return it;

  rst::util::Print("%s: ERROR - unknown game state. The game will break now.", __func__);
  return nullptr;
}

const StateInfo* FindStateInfoByInit(void (*init_fn)(State*)) {
  return FindStateInfo([init_fn](const StateInfo& info) { return info.init_fn == init_fn; });
}

const StateInfo* FindStateInfoByType(StateType type) {
  return FindStateInfo([type](const StateInfo& info) { return info.type == type; });
}

extern "C" const StateInfo* rst_GameStateGetNextStateInfo(State* state) {
  if (state->next_state_init_fn)
    return FindStateInfoByInit(state->next_state_init_fn);
  // Extension: allow switching to new game states by type, which is a lot more convenient.
  rst::util::Print("%s: looking up new game state for type %u", __func__, u8(state->type));
  return FindStateInfoByType(state->type);
}

void State::ChangeState(StateType new_type) {
  status = State::Status::Changing;
  next_state_init_fn = nullptr;
  type = new_type;
}

void SaveFile::Open() {
  rst::util::GetPointer<void(SaveFile*)>(0x18DA50)(this);
}

bool SaveFile::WriteAndFree() {
  return rst::util::GetPointer<bool(SaveFile*)>(0x18AD44)(this);
}

}  // namespace game
