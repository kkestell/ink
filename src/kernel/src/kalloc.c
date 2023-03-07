#include <string.h>
#include "kalloc.h"
#include "kprintf.h"

#define EfiMaxMemoryType      0x0000000e
#define EfiConventionalMemory 0x00000007

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
    int i;
    uint8_t *p = ptr;
    i = 0;
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

    // FIXME: "um"? in output?
    kprintf("kalloc_init: mapped %U pages from %p to %p\n",
        page_count,
        start,
        (uint64_t *)(((uint8_t *)start) + page_count * 4096));

    head = (uint8_t *)start;
}

void *kmalloc(size_t size)
{
    void *ptr = head;
    head += size;
    kprintf("kmalloc: allocated %U bytes at %p\n", size, ptr);
    return ptr;
}

void kfree(void *ptr)
{
    kprintf("kmalloc: freeing ??? bytes at %p\n", ptr);   
}

void *kcalloc(size_t number, size_t size)
{
    void *new = head;
    head += number * size;
    bzero(new, number * size);
    kprintf("kcalloc: allocated %U bytes at %p\n", size, new);
    return new;
}

void *krealloc(void *ptr, size_t size)
{
    void *new = head;
    head += size;
    memcpy(new, ptr, size);
    kprintf("krealloc: allocated %U bytes at %p\n", size, new);
    return new;
}
