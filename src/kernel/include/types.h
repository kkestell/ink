#pragma once

#include <stdint.h>

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;
typedef int64_t  int64;
typedef uint64_t uint64;

typedef unsigned long uintp;

typedef uintp pde_t;

typedef struct mm_desc
{
    uint32  type;
    void    *physical_start;
    void    *virtual_start;
    uint64  page_count;
    uint64  attributes;
} mm_desc_t;

typedef struct efi_boot_payload
{
    mm_desc_t *mm_desc;
    uint64    mm_size;
    uint64    mm_key;
    uint64    mm_desc_size;
    uint32    mm_desc_ver;
} efi_boot_payload_t;
