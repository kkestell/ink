#pragma once

#include <stddef.h>
#include "boot_info.h"

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define MIN_BLOCK_SIZE 32

void
bzero(void *ptr, size_t num);

void *
memcpy(void* dest, void* src, size_t n);

void *
memset(void* s, int c, size_t n);

void
kalloc_init(memory_info_t *memory_info);

void *
kmalloc(size_t size);

void
kfree(void *ptr);

void *
kcalloc(size_t number, size_t size);

void *
krealloc(void *ptr, size_t size);

void
kalloc_debug(void);
