#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>
#include "boot_info.h"

void initialize_memory(KernelBootInfo* boot_info);
void *malloc();
void free(void *memory);
void print_buddy();

#endif
