#include <string.h>
#include "kalloc.h"
#include "kprintf.h"

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

typedef struct {
    size_t size;
} alloc_header;

uint8_t *head = NULL;

void bzero(void *ptr, size_t num)
{
  uint8_t *p = (uint8_t *)ptr;
  size_t i;
  for (i = 0; i < num; i++)
    p[i] = '\0';
}

void memcpy(void *dest, void *src, size_t num)
{
    uint8_t *cs = (uint8_t *)src;
    uint8_t *cd = (uint8_t *)dest;
 
    for (size_t i = 0; i < num; i++)
        cd[i] = cs[i];
}

void *memset(void *ptr, int value, int num)
{
    uint8_t *p = ptr;
    while(num > 0)
    {
        *p = value;
        p++;
        num--;
    }
    return(ptr);
}

void kalloc_debug()
{
}

void kalloc_init(memory_info_t *memory_info)
{
    memory_map_descriptor_t *d = memory_info->memory_map;

    uint64_t *start = NULL;
    uint64_t page_count = 0;

    for (uint64_t i = 0; i < memory_info->size / memory_info->descriptor_size; i++)
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
        ptr += memory_info->descriptor_size;
        d = (memory_map_descriptor_t *)ptr;
    }

    kprintf("kalloc_init: mapped %U pages from %p to %p\n",
        page_count,
        start,
        (uint64_t *)(((uint8_t *)start) + page_count * 4096));

    head = (uint8_t *)start;
}

void *kmalloc(size_t size) {
    // Allocate memory for the header plus the requested size
    size_t totalSize = size + sizeof(alloc_header);
    alloc_header *header = (alloc_header *)head;
    head += totalSize;

    // Store the size in the header
    header->size = size;

    // Return the memory address after the header
    void *ptr = (void *)(header + 1);
    kprintf("kmalloc: allocated %U bytes at %p\n", size, ptr);
    return ptr;
}

void kfree(void *ptr) {
    // Retrieve the header
    alloc_header *header = (alloc_header *)ptr - 1;
    size_t size = header->size;

    kprintf("kfree: freeing %U bytes at %p\n", size, ptr);

    // Reset the head if this block is the most recently allocated
    if ((uint8_t *)ptr + size == head) {
        head = (uint8_t *)header;
    }
    // Note: This simplistic approach doesn't handle freeing of blocks not at the end or memory fragmentation
}

void *kcalloc(size_t number, size_t size) {
    size_t totalSize = number * size;
    void *ptr = kmalloc(totalSize);
    bzero(ptr, totalSize);
    return ptr;
}

void *krealloc(void *ptr, size_t newSize) {
    if (!ptr) {
        return kmalloc(newSize);
    }

    // Retrieve the old header and size
    alloc_header *header = (alloc_header *)ptr - 1;
    size_t oldSize = header->size;

    // Allocate new memory and copy the old content
    void *newPtr = kmalloc(newSize);
    memcpy(newPtr, ptr, oldSize < newSize ? oldSize : newSize);

    // Free the old block
    kfree(ptr);

    return newPtr;
}
