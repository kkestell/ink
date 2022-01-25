#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct kernel_memory_map_descriptor
{
    uint32_t type;
    void     *physical_start;
    void     *virtual_start;
    uint64_t page_count;
    uint64_t attributes;
} kernel_memory_map_descriptor_t;

typedef struct kernel_memory_map
{
    kernel_memory_map_descriptor_t *memory_map;
    uint64_t memory_map_size;
    uint64_t key;
    uint64_t memory_map_descriptor_size;
    uint32_t memory_map_descriptor_version;
} kernel_memory_map_t;

void kalloc_init(kernel_memory_map_t *memory_map);
void *kmalloc(size_t size);
void kfree(void *block);
void *kcalloc(size_t number, size_t size);
void *krealloc(void *ptr, size_t size);
