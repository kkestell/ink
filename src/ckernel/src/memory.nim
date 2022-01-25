import
  memory, printf

##
## https://stackoverflow.com/questions/33447708/kernel-memory-management-where-do-i-begin
##
## PHYSICAL ADDRESS     PAGES    TYPE
##             1000       159    conventional memory
##           100000      1792    conventional memory
##           808000         8    conventional memory
##          1500000     10806    conventional memory
##          3f56000      9929    conventional memory
##          6631000         2    conventional memory
##          6725000         1    conventional memory
##          7e00000       158    conventional memory
##
## mapped 93614080 bytes usable memory
##

const
  EfiMaxMemoryType* = 0x0000000e
  EfiConventionalMemory* = 0x00000007
  MAX_ORDER* = 24
  MIN_ORDER* = 4

##  the order ranges 0..MAX_ORDER, the largest memblock is 2**(MAX_ORDER)

const
  POOLSIZE* = (1 shl MAX_ORDER)

##  blocks are of size 2**i

template BLOCKSIZE*(i: untyped): untyped =
  (1 shl (i))

##  the address of the buddy of a block from freelists[i]

const
  _MEMBASE* = (cast[uintptr_t](BUDDY.pool))

template _OFFSET*(b: untyped): untyped =
  (cast[uintptr_t](b) - _MEMBASE)

template _BUDDYOF*(b, i: untyped): untyped =
  (_OFFSET(b) xor (1 shl (i)))

template BUDDYOF*(b, i: untyped): untyped =
  (cast[pointer]((_BUDDYOF(b, i) + _MEMBASE)))

type
  buddy_t* {.bycopy.} = object
    freelist*: array[MAX_ORDER + 2, pointer] ##  one more slot for first block in pool
    pool*: array[POOLSIZE, uint8_t]


var BUDDY*: ptr buddy_t = 0

proc memset*(b: pointer; c: cint; len: cint): pointer =
  var i: cint
  var p: ptr cuchar = b
  i = 0
  while len > 0:
    p[] = c
    inc(p)
    dec(len)
  return b

proc count_blocks*(i: cint): cint =
  var count: cint = 0
  var p: ptr pointer = addr((BUDDY.freelist[i]))
  while p[] != nil:
    inc(count)
    p = cast[ptr pointer](p[])
  return count

proc total_free*(): cint =
  var i: cint = 0
  var bytecount: cint = 0
  i = 0
  while i <= MAX_ORDER:
    inc(bytecount, count_blocks(i) * BLOCKSIZE(i))
    inc(i)
  return bytecount

proc print_list*(i: cint) =
  printf("freelist[%d]: \n", i)
  var p: ptr pointer = addr(BUDDY.freelist[i])
  while p[] != nil:
    printf("    0x%08lx, 0x%08lx\n", (uintptr_t) * p,
           (uintptr_t) * p - cast[uintptr_t](BUDDY.pool))
    p = cast[ptr pointer](p[])

proc print_buddy*() =
  printf("========================================\n")
  printf("MEMPOOL size: %d\n", POOLSIZE)
  printf("MEMPOOL start @ 0x%08x\n", cast[cuint](cast[uintptr_t](BUDDY.pool)))
  printf("total free: %d\n", total_free())
  var i: cint = 0
  while i <= MAX_ORDER:
    print_list(i)
    inc(i)

proc malloc*(size: cint): pointer =
  var i: cint
  var order: cint
  var `block`: pointer
  var buddy: pointer
  ##  calculate minimal order for this size
  i = 0
  while BLOCKSIZE(i) < size + 1:   ##  one more byte for storing order
    inc(i)
  order = i = if (i < MIN_ORDER): MIN_ORDER else: i
  ##  level up until non-null list found
  while true:
    if i > MAX_ORDER:
      return nil
    if BUDDY.freelist[i]:
      break
    inc(i)
  ##  remove the block out of list
  `block` = BUDDY.freelist[i]
  BUDDY.freelist[i] = cast[ptr pointer](BUDDY.freelist[i])[]
  ##  split until i == order
  while dec(i) > order:
    buddy = BUDDYOF(`block`, i)
    BUDDY.freelist[i] = buddy
  ##  store order in previous byte
  ((cast[ptr uint8_t](`block`)) - 1)[] = order
  return `block`

proc free*(`block`: pointer) =
  var i: cint
  var buddy: pointer
  var p: ptr pointer
  ##  fetch order in previous byte
  i = ((cast[ptr uint8_t](`block`)) - 1)[]
  while true:
    ##  calculate buddy
    buddy = BUDDYOF(`block`, i)
    p = addr((BUDDY.freelist[i]))
    ##  find buddy in list
    while (p[] != nil) and (p[] != buddy):
      p = cast[ptr pointer](p[])
    ##  not found, insert into list
    if p[] != buddy:
      cast[ptr pointer](`block`)[] = BUDDY.freelist[i]
      BUDDY.freelist[i] = `block`
      return
    `block` = if (`block` < buddy): `block` else: buddy
    ##  remove buddy out of list
    p[] = cast[ptr pointer](p[])[]
    inc(i)

proc initialize_memory*(boot_info: ptr KernelBootInfo) =
  var d: ptr MemoryMapDescriptor = boot_info.memory_map
  var start: ptr uint64_t = nil
  var page_count: uint64_t = 0
  var i: uint64_t = 0
  while i < boot_info.memory_map_size div boot_info.memory_map_descriptor_size:
    if d.`type` == EfiConventionalMemory:
      if d.page_count > page_count:
        start = cast[ptr uint64_t](d.physical_start)
        page_count = d.page_count
    var `ptr`: ptr uint8_t = (cast[ptr uint8_t](d))
    inc(`ptr`, boot_info.memory_map_descriptor_size)
    d = cast[ptr MemoryMapDescriptor](`ptr`)
    inc(i)
  BUDDY = cast[ptr buddy_t](start)
  memset(BUDDY, 0, sizeof((constructbuddy_t)))
  BUDDY.freelist[MAX_ORDER] = BUDDY.pool

##  Free List
##
## #define EfiMaxMemoryType      0x0000000e
## #define EfiConventionalMemory 0x00000007
##
## static void *free_list = 0;
## const int allocation_block_size = 32;
##
## typedef struct _ListEntry {
##    struct _ListEntry* next;
##    struct _ListEntry* prev;
## } ListEntry;
##
## void initialize_memory(KernelBootInfo* boot_info)
## {
##     MemoryMapDescriptor *d = boot_info->memory_map;
##
##     uint64_t *start = NULL;
##     uint64_t page_count = 0;
##
##     for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++)
##     {
##         if (d->type == EfiConventionalMemory)
##         {
##             if (d->page_count > page_count)
##             {
##                 start = (uint64_t *)d->physical_start;
##                 page_count = d->page_count;
##             }
##         }
##
##         uint8_t *ptr = ((uint8_t *)d);
##         ptr += boot_info->memory_map_descriptor_size;
##         d = (MemoryMapDescriptor *)ptr;
##     }
##
##     free_list = (void *)start;
##
##     uint64_t num_blocks = (page_count * 4096) / allocation_block_size;
##
##     ListEntry *link;
##     uint8_t *block = (uint8_t *)free_list;
##     for (uint64_t i = 0; i < num_blocks; i++)
##     {
##         link = (ListEntry *)block;
##         link->next = (ListEntry *)(block + allocation_block_size);
##         block += allocation_block_size;
##     }
##     link = (ListEntry *)block;
##     link->next = NULL;
## }
##
## void *malloc()
## {
##     void *allocation_unit = free_list;
##
##     ListEntry *link = (ListEntry *)free_list;
##     free_list = link->next;
##
##     return allocation_unit;
## }
##
## void free(void *memory)
## {
##    void *allocation_unit = memory;
##
##    ListEntry *link = (ListEntry *)allocation_unit;
##    link->next = (ListEntry *)free_list;
##
##    free_list = allocation_unit;
## }
##
##  Linear allocator
##
## #define EfiMaxMemoryType      0x0000000e
## #define EfiConventionalMemory 0x00000007
##
## static uint64_t *memory_start  = 0;
## static uint64_t *memory_end    = 0;
## static uint64_t *memory_offset = 0;
##
## void initialize_memory(KernelBootInfo* boot_info)
## {
##     MemoryMapDescriptor *d = boot_info->memory_map;
##
##     uint64_t *start = NULL;
##     uint64_t page_count = 0;
##
##     for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++)
##     {
##         if (d->type == EfiConventionalMemory)
##         {
##             if (d->page_count > page_count)
##             {
##                 start = (uint64_t *)d->physical_start;
##                 page_count = d->page_count;
##             }
##         }
##
##         uint8_t *ptr = ((uint8_t *)d);
##         ptr += boot_info->memory_map_descriptor_size;
##         d = (MemoryMapDescriptor *)ptr;
##     }
##
##     memory_start = start;
##     memory_end   = start + page_count * 4096;
## }
##
## void *malloc(size_t size)
## {
##     if (memory_offset >= memory_end)
##         return NULL;
##
##     void *memory = memory_offset;
##
##     offset += size;
##
##     return memory;
## }
##
