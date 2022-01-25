#include "memory.h"
#include "printf.h"

/*
https://stackoverflow.com/questions/33447708/kernel-memory-management-where-do-i-begin

PHYSICAL ADDRESS     PAGES    TYPE
            1000       159    conventional memory
          100000      1792    conventional memory
          808000         8    conventional memory
         1500000     10806    conventional memory
         3f56000      9929    conventional memory
         6631000         2    conventional memory
         6725000         1    conventional memory
         7e00000       158    conventional memory

mapped 93614080 bytes usable memory
*/

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

#define MAX_ORDER       24
#define MIN_ORDER       4   // 2 ** 4 == 16 bytes

// the order ranges 0..MAX_ORDER, the largest memblock is 2**(MAX_ORDER)
#define POOLSIZE        (1 << MAX_ORDER)

// blocks are of size 2**i
#define BLOCKSIZE(i)    (1 << (i))

// the address of the buddy of a block from freelists[i]
#define _MEMBASE        ((uintptr_t)BUDDY->pool)
#define _OFFSET(b)      ((uintptr_t)b - _MEMBASE)
#define _BUDDYOF(b, i)  (_OFFSET(b) ^ (1 << (i)))
#define BUDDYOF(b, i)   ((void *)( _BUDDYOF(b, i) + _MEMBASE))

typedef struct buddy
{
    void *freelist[MAX_ORDER + 2];  // one more slot for first block in pool
    uint8_t pool[POOLSIZE];
} buddy_t;

buddy_t *BUDDY = 0;

void *memset(void *b, int c, int len)
{
    int i;
    unsigned char *p = b;
    i = 0;

    while(len > 0)
    {
        *p = c;
        p++;
        len--;
    }
  
    return(b);
}

static int count_blocks(int i)
{
    int count = 0;
    void **p = &(BUDDY->freelist[i]);

    while (*p != NULL)
    {
        count++;
        p = (void **) *p;
    }

    return count;
}

static int total_free()
{
    int i = 0;
    int bytecount = 0;

    for (i = 0; i <= MAX_ORDER; i++)
    {
        bytecount += count_blocks(i) * BLOCKSIZE(i);
    }

    return bytecount;
}

static void print_list(int i)
{
    printf("freelist[%d]: \n", i);

    void **p = &BUDDY->freelist[i];
  
    while (*p != NULL)
    {
        printf("    0x%08lx, 0x%08lx\n", (uintptr_t)*p, (uintptr_t)*p - (uintptr_t)BUDDY->pool);
        p = (void**) *p;
    }
}

void print_buddy()
{
    printf("========================================\n");
    printf("MEMPOOL size: %d\n", POOLSIZE);
    printf("MEMPOOL start @ 0x%08x\n", (unsigned int) (uintptr_t)BUDDY->pool);
    printf("total free: %d\n", total_free());

    for (int i = 0; i <= MAX_ORDER; i++)
    {
        print_list(i);
    }
}

void *malloc(int size)
{
    int i;
    int order;
    void *block;
    void *buddy;

    // calculate minimal order for this size
    i = 0;
    while (BLOCKSIZE(i) < size + 1) // one more byte for storing order
        i++;

    order = i = (i < MIN_ORDER) ? MIN_ORDER : i;

    // level up until non-null list found
    for (;; i++)
    {
        if (i > MAX_ORDER)
            return NULL;
        if (BUDDY->freelist[i])
            break;
    }

    // remove the block out of list
    block = BUDDY->freelist[i];
    BUDDY->freelist[i] = *(void**) BUDDY->freelist[i];

    // split until i == order
    while (i-- > order)
    {
        buddy = BUDDYOF(block, i);
        BUDDY->freelist[i] = buddy;
    }

    // store order in previous byte
    *(((uint8_t *)block) - 1) = order;
    return block;
}

void free(void *block)
{
    int i;
    void *buddy;
    void **p;

    // fetch order in previous byte
    i = *(((uint8_t*)block) - 1);

    for (;; i++)
    {
        // calculate buddy
        buddy = BUDDYOF(block, i);
        p = &(BUDDY->freelist[i]);

        // find buddy in list
        while ((*p != NULL) && (*p != buddy))
            p = (void **) *p;

        // not found, insert into list
        if (*p != buddy)
        {
            *(void **) block = BUDDY->freelist[i];
            BUDDY->freelist[i] = block;
            return;
        }

        // found, merged block starts from the lower one
        block = (block < buddy) ? block : buddy;
    
        // remove buddy out of list
        *p = *(void **) *p;
    }
}

void initialize_memory(KernelBootInfo* boot_info) 
{
    MemoryMapDescriptor *d = boot_info->memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++) 
    {
        if (d->type == EfiConventionalMemory)
        {
            if (d->page_count > page_count)
            {
                start = (uint64_t *)d->physical_start;
                page_count = d->page_count;
            }
        }

        uint8_t *ptr = ((uint8_t *)d);
        ptr += boot_info->memory_map_descriptor_size;
        d = (MemoryMapDescriptor *)ptr;
    }

    BUDDY = (buddy_t *)start;
    memset(BUDDY, 0, sizeof(buddy_t));
    BUDDY->freelist[MAX_ORDER] = BUDDY->pool;
}



// Free List
/*
#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

static void *free_list = 0;
const int allocation_block_size = 32;

typedef struct _ListEntry {
   struct _ListEntry* next;
   struct _ListEntry* prev;
} ListEntry;

void initialize_memory(KernelBootInfo* boot_info) 
{
    MemoryMapDescriptor *d = boot_info->memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++) 
    {
        if (d->type == EfiConventionalMemory)
        {
            if (d->page_count > page_count)
            {
                start = (uint64_t *)d->physical_start;
                page_count = d->page_count;
            }
        }

        uint8_t *ptr = ((uint8_t *)d);
        ptr += boot_info->memory_map_descriptor_size;
        d = (MemoryMapDescriptor *)ptr;
    }

    free_list = (void *)start;

    uint64_t num_blocks = (page_count * 4096) / allocation_block_size;

    ListEntry *link;
    uint8_t *block = (uint8_t *)free_list;
    for (uint64_t i = 0; i < num_blocks; i++)
    {
        link = (ListEntry *)block;
        link->next = (ListEntry *)(block + allocation_block_size);
        block += allocation_block_size;
    }
    link = (ListEntry *)block;
    link->next = NULL;
}

void *malloc()
{
    void *allocation_unit = free_list;

    ListEntry *link = (ListEntry *)free_list;
    free_list = link->next;

    return allocation_unit;
}

void free(void *memory)
{
   void *allocation_unit = memory;

   ListEntry *link = (ListEntry *)allocation_unit;
   link->next = (ListEntry *)free_list;
   
   free_list = allocation_unit;
}
*/

// Linear allocator
/*
#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

static uint64_t *memory_start  = 0;
static uint64_t *memory_end    = 0;
static uint64_t *memory_offset = 0;

void initialize_memory(KernelBootInfo* boot_info) 
{
    MemoryMapDescriptor *d = boot_info->memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++) 
    {
        if (d->type == EfiConventionalMemory)
        {
            if (d->page_count > page_count)
            {
                start = (uint64_t *)d->physical_start;
                page_count = d->page_count;
            }
        }

        uint8_t *ptr = ((uint8_t *)d);
        ptr += boot_info->memory_map_descriptor_size;
        d = (MemoryMapDescriptor *)ptr;
    }

    memory_start = start;
    memory_end   = start + page_count * 4096;
}

void *malloc(size_t size)
{
    if (memory_offset >= memory_end)
        return NULL;

    void *memory = memory_offset;

    offset += size;
    
    return memory;
}
*/
