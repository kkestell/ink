#ifndef _KMALLOC_H
#define _KMALLOC_H

#include <stddef.h>
#include "boot_info.h"

void
kalloc_init(kernel_boot_info *boot_info);

void *
kmalloc(size_t size);

void
kfree(void *ptr);

void *
kcalloc(size_t number, size_t size);

void *
krealloc(void *ptr, size_t size);

#endif
