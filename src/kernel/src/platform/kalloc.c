#include <string.h>
#include <strings.h>
#include "kalloc.h"
#include "kprintf.h"

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

uint8_t *head = NULL;

void kalloc_debug()
{
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

    kprintf(
        "kalloc_init: mapped %lu pages from 0x%lx to 0x%lx\n", 
        page_count,
        start,
        (uint64_t *)(((uint8_t *)start) + page_count * 4096));

    head = start;
}

void *kmalloc(size_t size)
{
    void *ptr = head;
    head += size;
    //kprintf("kmalloc: allocated %lu bytes at 0x%lx\n", size, ptr);
    return ptr;
}

void kfree(void *ptr)
{
    //kprintf("kmalloc: freeing ??? bytes at 0x%lx\n", ptr);   
}

void *kcalloc(size_t number, size_t size)
{
    void *new = head;
    head += number * size;
    bzero(new, number * size);
    //kprintf("kcalloc: allocated %lu bytes at 0x%lx\n", size, new);
    return new;
}

void *krealloc(void *ptr, size_t size)
{
    void *new = head;
    head += size;
    memcpy(new, ptr, size);
    //kprintf("krealloc: allocated %lu bytes at 0x%lx\n", size, new);
    return new;
}
