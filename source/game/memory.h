#pragma once

#include "common/flags.h"
#include "common/types.h"
#include "common/utils.h"

namespace nn::os {
class CriticalSection;
}

namespace game {

class Allocator;

/// Allocator memory block.
///
/// * Blocks are always aligned to a 16-byte boundary.
///
/// * Large blocks are allocated from the start of the root block
///   and the heap grows in the direction of high memory addresses.
///
/// * Small blocks are allocated from the end of the root block
///   and the heap grows in the direction of low memory addresses.
///
/// * Blocks can optionally be reference counted and given a user-specified ID. This is typically
///   used for files and some actor resources.
struct AllocatorBlock {
  enum class Flag : u8 {
    /// Is reference counted
    IsRefCounted = 1 << 0,
    /// Prevent this block from being reused even when ref count is 0
    PreventReuse = 1 << 1,
    /// Free this block immediately when ref count reaches 0
    FreeBlockOnRefCountZero = 1 << 3,
  };

  static AllocatorBlock* FromData(void* ptr) {
    return reinterpret_cast<AllocatorBlock*>(uintptr_t(ptr) - sizeof(AllocatorBlock));
  }

  void* Data() { return reinterpret_cast<void*>(this + 1); }

  /// 'S.Moriya'
  u64 magic;
  /// Result of nn::svc::GetSystemTick() when this block was allocated.
  u64 alloc_ticks;

  /// Previous block. Null for root block. This is a circular list starting at root_block.
  AllocatorBlock* prev;
  /// Next block. Null for root block. This is a circular list starting at root_block.
  AllocatorBlock* next;

  /// Next free block to try for small allocations. This is a circular list starting at dummy_block.
  AllocatorBlock* next_free_s;
  /// Next free block to try for large allocations. This is a circular list starting at dummy_block.
  AllocatorBlock* next_free_l;

  /// Next reference counted block. This is a circular list.
  AllocatorBlock* refcounted_next;
  /// Previous reference counted block. This is a circular list.
  AllocatorBlock* refcounted_prev;

  /// Arbitrary, user-specified value to identify reference counted blocks.
  u32 id;
  /// Reference count. Always 0 or 1 for non-reference counted blocks.
  u16 ref_count;
  rst::Flags<Flag> flags;
  /// Size in bytes. If positive: free size. If negative: allocation size (including header size).
  int size;
  /// Usually source code file path + line number, but can also be a file path or nullptr.
  /// This is almost always a dangling pointer for reference counted blocks because FileEntity sets
  /// the name using a member char array even though FileEntities are not reference counted. Whoops!
  const char* name;

  u32 field_38;
  u32 field_3C;
};
static_assert(sizeof(AllocatorBlock) == 0x40);

class Allocator {
public:
  static Allocator& Instance();

  /// Allocate `size` bytes of memory (always aligned to a 16-byte boundary).
  void* Allocate(size_t size, const char* source_location);
  /// Free memory that was allocated with Allocate().
  void Free(void* ptr);

  void PrintDebugInfo() const;

private:
  /// Only next_free_s and next_free_l should be used.
  AllocatorBlock dummy_block;

  AllocatorBlock* root_block;
  void* root_block_end;
  size_t average_alloc_size;
  // Normally 0x800 bytes.
  size_t small_block_threshold;
  size_t size;
  u32 block_count;
  nn::os::CriticalSection* crit_section;
};
static_assert(sizeof(Allocator) == 0x60);

}  // namespace game
