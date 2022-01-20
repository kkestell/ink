#ifndef _ELF_H
#define _ELF_H

#include "uefi.h"

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

typedef enum ElfFileClass 
{
    ELF_FILE_CLASS_NONE,
    ELF_FILE_CLASS_32,
    ELF_FILE_CLASS_64,
} ElfFileClass;

typedef struct ElfHeader 
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
} ElfHeader;

typedef struct ElfProgramHeader 
{
    UINT32 p_type;
    UINT32 p_flags;
    UINT64 p_offset;
    UINT64 p_vaddr;
    UINT64 p_paddr;
    UINT64 p_filesz;
    UINT64 p_memsz;
    UINT64 p_align;
} ElfProgramHeader;

UEFI_STATUS elf_read_identity(UEFI_FILE_PROTOCOL *const kernelImageFile, UINT8 **elfIdentityBuffer);
UEFI_STATUS elf_read_file(UEFI_FILE_PROTOCOL *const kernelImageFile, void **kernelHeaderBuffer, void **kernelProgramHeadersBuffer);
UEFI_STATUS elf_validate_identity(UINT8 *const elfIdentityBuffer);

#endif // _ELF_H
