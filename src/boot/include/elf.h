#pragma once

#include "efi.h"

#define EI_NIDENT        16

#define EI_MAG0          0
#define EI_MAG1          0x1
#define EI_MAG2          0x2
#define EI_MAG3          0x3
#define EI_CLASS         0x4
#define EI_DATA          0x5
#define EI_VERSION       0x6
#define EI_OSABI         0x7
#define EI_ABIVERSION    0x8

#define PT_NULL            0
#define PT_LOAD            1
#define PT_DYNAMIC         2
#define PT_INTERP          3
#define PT_NOTE            4
#define PT_SHLIB           5
#define PT_PHDR            6
#define PT_TLS             7

typedef enum elf_file_class
{
    ELF_FILE_CLASS_NONE = 0,
    ELF_FILE_CLASS_32   = 1,
    ELF_FILE_CLASS_64   = 2,
} elf_file_class_t;

typedef struct elf_header
{
    unsigned char e_ident[EI_NIDENT];
    UINT16 e_type;
    UINT16 e_machine;
    UINT32 e_version;
    UINT64 e_entry;
    UINT64 e_phoff;
    UINT64 e_shoff;
    UINT32 e_flags;
    UINT16 e_ehsize;
    UINT16 e_phentsize;
    UINT16 e_phnum;
    UINT16 e_shentsize;
    UINT16 e_shnum;
    UINT16 e_shstrndx;
} elf_header_t;

typedef struct elf_program_header 
{
    UINT32 p_type;
    UINT32 p_flags;
    UINT64 p_offset;
    UINT64 p_vaddr;
    UINT64 p_paddr;
    UINT64 p_filesz;
    UINT64 p_memsz;
    UINT64 p_align;
} elf_program_header;

EFI_STATUS
elf_read_identity(EFI_FILE_PROTOCOL *const kernel_image_file, UINT8 **elf_identity_buffer);

EFI_STATUS
elf_read_file(EFI_FILE_PROTOCOL *const kernel_image_file, void **kernel_header_buffer, void **kernel_program_headers_buffer);

EFI_STATUS
elf_validate_identity(UINT8 *const elf_identity_buffer);

// void
// elf_print_file_info(void *const header_ptr, void *const program_headers_ptr);
