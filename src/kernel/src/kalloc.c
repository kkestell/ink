#include "kalloc.h"
#include "printf.h"
#include <string.h>
#include <strings.h>

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

#define MAX_ORDER     24
#define MIN_ORDER     4   // 2 ** 4 == 16 bytes
#define POOLSIZE      (1 << MAX_ORDER)
#define BLOCKSIZE(i)  (1 << (i))
#define MEMBASE       ((uintptr_t)_buddy->pool)
#define OFFSET(b)     ((uintptr_t)b - MEMBASE)
#define BUDDYOF(b, i) ((void *)((OFFSET(b) ^ (1 << (i))) + MEMBASE))

typedef struct buddy
{
    void *freelist[MAX_ORDER + 2];  // one more slot for first block in pool
    uint8_t pool[POOLSIZE];
} Buddy;

Buddy *_buddy = NULL;

typedef struct MemoryMapDescriptor
{
    uint32_t type;
    void     *physical_start;
    void     *virtual_start;
    uint64_t page_count;
    uint64_t attributes;
} MemoryMapDescriptor;

static int count_blocks(int i)
{
    int count = 0;
    void **p = &(_buddy->freelist[i]);

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

    void **p = &_buddy->freelist[i];
  
    while (*p != NULL)
    {
        printf("    0x%08lx, 0x%08lx\n", (uintptr_t)*p, (uintptr_t)*p - (uintptr_t)_buddy->pool);
        p = (void**) *p;
    }
}

void kalloc_debug()
{
    printf("========================================\n");
    printf("MEMPOOL size: %d\n", POOLSIZE);
    printf("MEMPOOL start @ 0x%08x\n", (unsigned int) (uintptr_t)_buddy->pool);
    printf("total free: %d\n", total_free());

    for (int i = 0; i <= MAX_ORDER; i++)
    {
        print_list(i);
    }
}

void *malloc(size_t size)
{
    printf("malloc: %d\n", size);

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
        if (_buddy->freelist[i])
            break;
    }

    // remove the block out of list
    block = _buddy->freelist[i];
    _buddy->freelist[i] = *(void**) _buddy->freelist[i];

    // split until i == order
    while (i-- > order)
    {
        buddy = BUDDYOF(block, i);
        _buddy->freelist[i] = buddy;
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
        p = &(_buddy->freelist[i]);

        // find buddy in list
        while ((*p != NULL) && (*p != buddy))
            p = (void **) *p;

        // not found, insert into list
        if (*p != buddy)
        {
            *(void **) block = _buddy->freelist[i];
            _buddy->freelist[i] = block;
            return;
        }

        // found, merged block starts from the lower one
        block = (block < buddy) ? block : buddy;
    
        // remove buddy out of list
        *p = *(void **) *p;
    }
}

void *calloc(size_t number, size_t size)
{
    void *p;

    p = malloc(number * size);

    if (p == NULL)
        return (p);

    bzero(p, number * size);

    return (p);
}

void *realloc(void *ptr, size_t size)
{
    free(ptr);
    void *new = malloc(size);
    memcpy(new, ptr, size);
    return new;

    /*
    void *new;

    if (!ptr)
    {
        new = malloc(size);

        if (new == NULL)
            return new;
    } 
    else
    {
        if (malloc_size(ptr) < size) 
        {
            new = malloc(size);

            if (!new)
                goto error;

            memcpy(new, ptr, malloc_size(ptr));

            free(ptr);
        }
        else
        {
            new = ptr;
        }
    }

    return new;
    */
}

void kalloc_init(void *memory_map, size_t memory_map_size, size_t memory_map_descriptor_size) 
{
    MemoryMapDescriptor *d = (MemoryMapDescriptor *)memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < memory_map_size / memory_map_descriptor_size; i++)
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
        ptr += memory_map_descriptor_size;
        d = (MemoryMapDescriptor *)ptr;
    }

    _buddy = (Buddy *)start;
    memset(_buddy, 0, sizeof(Buddy));
    _buddy->freelist[MAX_ORDER] = _buddy->pool;
}
