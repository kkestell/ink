#include <string.h>
#include <strings.h>
#include "kalloc.h"
#include "kprintf.h"

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

#define MAX_ORDER     24  // 2 ** 24 == 16,777,216 bytes
#define MIN_ORDER     4   // 2 ** 4  == 16 bytes
#define POOLSIZE      (1 << MAX_ORDER)
#define BLOCKSIZE(i)  (1 << (i))
#define MEMBASE       ((uintptr_t)_buddy->pool)
#define OFFSET(b)     ((uintptr_t)b - MEMBASE)
#define BUDDYOF(b, i) ((void *)((OFFSET(b) ^ (1 << (i))) + MEMBASE))

typedef struct buddy
{
    void *freelist[MAX_ORDER + 2];  // one more slot for first block in pool
    uint8_t pool[POOLSIZE];
} buddy_t;

buddy_t *_buddy = NULL;

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
    kprintf("freelist[%d]: \n", i);

    void **p = &_buddy->freelist[i];
  
    while (*p != NULL)
    {
        kprintf("    0x%08lx, 0x%08lx\n", (uintptr_t)*p, (uintptr_t)*p - (uintptr_t)_buddy->pool);
        p = (void**) *p;
    }
}

void kalloc_debug()
{
    kprintf("========================================\n");
    kprintf("MEMPOOL size: %d\n", POOLSIZE);
    kprintf("MEMPOOL start @ 0x%08x\n", (unsigned int) (uintptr_t)_buddy->pool);
    kprintf("total free: %d\n", total_free());

    for (int i = 0; i <= MAX_ORDER; i++)
    {
        print_list(i);
    }

    kprintf("========================================\n");
}

void *kmalloc(size_t size)
{
    // calculate minimal order for this size
    int i = 0;
    while (BLOCKSIZE(i) < size + 1) // one more byte for storing order
        i++;

    int order = i = (i < MIN_ORDER) ? MIN_ORDER : i;

    // level up until non-null list found
    for (;; i++)
    {
        if (i > MAX_ORDER)
            return NULL;
        if (_buddy->freelist[i])
            break;
    }

    // remove the block out of list
    void *block = _buddy->freelist[i];
    _buddy->freelist[i] = *(void**) _buddy->freelist[i];

    // split until i == order
    while (i-- > order)
    {
        void *buddy = BUDDYOF(block, i);
        _buddy->freelist[i] = buddy;
    }

    // store order in previous byte
    *(((uint8_t *)block) - 1) = order;
    
    return block;
}

void kfree(void *block)
{
    // fetch order in previous byte
    int i = *(((uint8_t*)block) - 1);

    for (;; i++)
    {
        // calculate buddy
        void *buddy = BUDDYOF(block, i);
        void **p = &(_buddy->freelist[i]);

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

void *kcalloc(size_t number, size_t size)
{
    void *p;

    p = kmalloc(number * size);

    if (p == NULL)
        return (p);

    bzero(p, number * size);

    return (p);
}

void *krealloc(void *ptr, size_t size)
{
    kfree(ptr);
    void *new = kmalloc(size);
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

void kalloc_init(kernel_memory_map_t *memory_map) 
{
    kernel_memory_map_descriptor_t *d = memory_map->memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < memory_map->memory_map_size / memory_map->memory_map_descriptor_size; i++)
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
        ptr += memory_map->memory_map_descriptor_size;
        d = (kernel_memory_map_descriptor_t *)ptr;
    }

    kprintf("kalloc_init: mapped %lu pages at 0x%lx\n", page_count, start);

    _buddy = (buddy_t *)start;
    memset(_buddy, 0, sizeof(buddy_t));
    _buddy->freelist[MAX_ORDER] = _buddy->pool;
}
