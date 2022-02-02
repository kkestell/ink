// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "spinlock.h"

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

void
kinit1(efi_boot_payload_t *boot)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  //freerange(vstart, vend);
}