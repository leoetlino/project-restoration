#include "game/memory.h"
#include "common/debug.h"

namespace game {

Allocator& Allocator::Instance() {
  return *rst::util::GetPointer<Allocator>(0x72F7F0);
}

void* Allocator::Allocate(size_t size, const char* src) {
  return rst::util::GetPointer<void*(Allocator*, size_t, const char*)>(0x1DFB88)(this, size, src);
}

void Allocator::Free(void* ptr) {
  rst::util::GetPointer<void(Allocator*, void*)>(0x1DAF18)(this, ptr);
}

void Allocator::PrintDebugInfo() const {
  rst::util::Print("===== ALLOCATOR INFO =====");

  rst::util::Print("Root block: %p → %p (%zu bytes)", root_block, root_block_end, size);
  rst::util::Print("Small block threshold: 0x%zx bytes", small_block_threshold);
  rst::util::Print("Number of blocks: %lu", block_count);
  rst::util::Print("Average allocation size: %zu bytes", average_alloc_size);
  rst::util::Print(" ");

  const auto* block = root_block;
  do {
    if (block->size <= 0) {
      if (block->flags.IsSet(AllocatorBlock::Flag::IsRefCounted)) {
        rst::util::Print("%p USED %s size=%d ref_count=%u", block, "(reference counted)",
                         -block->size, block->ref_count);
      } else {
        rst::util::Print("%p USED %s size=%d", block, block->name ? block->name : "(no name)",
                         -block->size);
      }
    } else {
      rst::util::Print("%p FREE %s size=%d", block, "(free)", block->size);
    }
    block = block->next;
  } while (block != root_block);

  rst::util::Print(" ");

  rst::util::Print("----- FREE BLOCKS -----");
  block = root_block;
  size_t total_free_size = 0;
  do {
    if (block->size > 0) {
      rst::util::Print("%p FREE size=%d nextS=%p nextL=%p", block, block->size, block->next_free_s,
                       block->next_free_l);
      total_free_size += block->size;
    }
    block = block->next;
  } while (block != root_block);
  rst::util::Print("----- FREE BLOCKS END -----");

  rst::util::Print("Total free size: %zu bytes / %zu bytes (%f%%)", total_free_size, size,
                   100.0f * total_free_size / size);
  rst::util::Print("Root: nextS=%p nextL=%p", dummy_block.next_free_s, dummy_block.next_free_l);

  rst::util::Print("===== ALLOCATOR INFO END =====");
}

}  // namespace game
