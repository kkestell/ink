#include <stdint.h>
#include <stdbool.h>
#include "kalloc.h"
#include "kprintf.h"

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

typedef struct block_header
{
    size_t size;
    bool is_free;
    struct block_header* next;
} block_header;

static block_header* free_list = NULL;
static void* heap_start = NULL;
static void* heap_end = NULL;

// Memory statistics
static size_t total_allocated = 0;
static size_t total_free = 0;
static size_t peak_allocated = 0;
static size_t allocation_count = 0;
static size_t free_count = 0;

void kalloc_init(memory_info_t *memory_info)
{
    memory_map_descriptor_t *d = memory_info->memory_map;
    uint64_t largest_block = 0;
    void* largest_block_start = NULL;

    for (uint64_t i = 0; i < memory_info->size / memory_info->descriptor_size; i++) {
        if (d->type == EfiConventionalMemory) {
            uint64_t block_size = d->page_count * 4096;
            if (block_size > largest_block)
            {
                largest_block = block_size;
                largest_block_start = (void*)d->physical_start;
            }
        }
        d = (memory_map_descriptor_t*)((uint8_t*)d + memory_info->descriptor_size);
    }

    heap_start = (void*)ALIGN((uintptr_t)largest_block_start);
    heap_end = (void*)ALIGN((uintptr_t)heap_start + largest_block);

    // Initialize the free list with a single large block
    free_list = (block_header*)heap_start;
    free_list->size = (uintptr_t)heap_end - (uintptr_t)heap_start - sizeof(block_header);
    free_list->is_free = true;
    free_list->next = NULL;

    total_free = free_list->size;

    // kprintf("kalloc_init: Heap initialized from %p to %p, size: %U bytes\n", 
    //         heap_start, heap_end, (uintptr_t)heap_end - (uintptr_t)heap_start);
}

static block_header* find_free_block(size_t size)
{
    block_header* current = free_list;
    while (current)
    {
        if (current->is_free && current->size >= size)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void split_block(block_header* block, size_t size)
{
    size_t remaining_size = block->size - size - sizeof(block_header);
    if (remaining_size >= MIN_BLOCK_SIZE)
    {
        block_header* new_block = (block_header*)((uint8_t*)block + sizeof(block_header) + size);
        new_block->size = remaining_size;
        new_block->is_free = true;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;

        total_free += remaining_size;
    }
}

void* kmalloc(size_t size)
{
    size = ALIGN(size);
    block_header* block = find_free_block(size);

    if (!block)
    {
        kprintf("kmalloc: Out of memory\n");
        return NULL;
    }

    block->is_free = false;
    split_block(block, size);

    total_allocated += block->size;
    total_free -= block->size;
    allocation_count++;

    if (total_allocated > peak_allocated)
    {
        peak_allocated = total_allocated;
    }

    void* result = (void*)((uint8_t*)block + sizeof(block_header));
    kprintf("kmalloc: Allocated %U bytes at %p\n", block->size, result);
    return result;
}

static void merge_free_blocks(void)
{
    block_header* current = free_list;
    while (current && current->next)
    {
        if (current->is_free && current->next->is_free)
        {
            current->size += sizeof(block_header) + current->next->size;
            current->next = current->next->next;
            total_free += sizeof(block_header);
        }
        else
        {
            current = current->next;
        }
    }
}

void kfree(void* ptr)
{
    if (!ptr) 
    {
        return;
    }

    block_header* block = (block_header*)((uint8_t*)ptr - sizeof(block_header));
    block->is_free = true;

    total_allocated -= block->size;
    total_free += block->size;
    free_count++;

    kprintf("kfree: Freed %U bytes at %p\n", block->size, ptr);

    merge_free_blocks();
}

void* kcalloc(size_t num, size_t size)
{
    size_t total_size = num * size;
    void* ptr = kmalloc(total_size);
    if (ptr)
    {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

void* krealloc(void* ptr, size_t new_size)
{
    if (!ptr) 
    {
        return kmalloc(new_size);
    }

    if (new_size == 0)
    {
        kfree(ptr);
        return NULL;
    }

    block_header* old_block = (block_header*)((uint8_t*)ptr - sizeof(block_header));
    if (new_size <= old_block->size)
    {
        // Shrink the block if necessary
        split_block(old_block, new_size);
        return ptr;
    }

    // Allocate a new block
    void* new_ptr = kmalloc(new_size);
    if (!new_ptr)
    {
        return NULL;
    }

    // Copy old data and free old block
    memcpy(new_ptr, ptr, old_block->size);
    kfree(ptr);

    return new_ptr;
}

void kalloc_debug(void)
{
    block_header* current = free_list;
    size_t block_count = 0;
    size_t free_block_count = 0;
    size_t largest_free_block = 0;

    kprintf("Memory Map:\n");
    while (current)
    {
        kprintf("Block at %p, size: %U, %s\n", current, current->size, current->is_free ? "free" : "used");
        if (current->is_free)
        {
            free_block_count++;
            if (current->size > largest_free_block)
            {
                largest_free_block = current->size;
            }
        }
        block_count++;
        current = current->next;
    }

    kprintf("Total blocks: %U\n", block_count);
    kprintf("Free blocks: %U\n", free_block_count);
    kprintf("Total allocated memory: %U bytes\n", total_allocated);
    kprintf("Total free memory: %U bytes\n", total_free);
    kprintf("Peak allocated memory: %U bytes\n", peak_allocated);
    kprintf("Largest free block: %U bytes\n", largest_free_block);
    kprintf("Total allocations: %U\n", allocation_count);
    kprintf("Total frees: %U\n", free_count);
    kprintf("Current fragmentation: %U%%\n", (block_count > 1) ? (block_count - 1) * 100 / (block_count) : 0);
}

void* memcpy(void* dest, void* src, size_t n)
{
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* p = (unsigned char*)s;
    for (size_t i = 0; i < n; i++)
    {
        p[i] = (unsigned char)c;
    }
    return s;
}
