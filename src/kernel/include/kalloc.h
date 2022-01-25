#pragma once

#include <stdint.h>
#include <stddef.h>

void kalloc_init(void *memory_map, size_t memory_map_size, size_t memory_map_descriptor_size);
void kalloc_debug();
void *malloc();
void free(void *memory);

