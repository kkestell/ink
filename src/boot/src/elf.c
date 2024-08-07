#include "elf.h"
#include "efi.h"
#include "kprintf.h"

/*
void elf_print_file_info(void *const header_ptr, void *const program_headers_ptr)
{
    elf_header_t* header = (elf_header_t*)header_ptr;

    kprintf(L"ELF Header Info:\r\n");
    kprintf(L"  Magic:                    ");
    for(int i = 0; i < 4; i++) 
        kprintf(L"0x%x ", header->e_ident[i]);
    kprintf(L"\r\n");
    kprintf(L"  Class:                    64bit\r\n");
    kprintf(L"  Endianness:               ");
    switch (header->e_ident[EI_DATA])
    {
        case 1:
            kprintf(L"Little-Endian\r\n");
            break;
        default:
            kprintf(L"Unknown\r\n");
    }
    kprintf(L"  Version:                  0x%x\r\n", header->e_ident[EI_VERSION]);
    kprintf(L"  OS ABI:                   ");
    switch(header->e_ident[EI_OSABI])
    {
        case 0x00:
            kprintf(L"System V\r\n");
            break;
        default:
            kprintf(L"Unknown\r\n");
    }
    kprintf(L"  File Type:                ");
    switch (header->e_type)
    {
        case 0x02:
            kprintf(L"Executable\r\n");
            break;
        default:
            kprintf(L"Unknown\r\n");
    }
    kprintf(L"  Machine Type:             ");
    switch (header->e_machine)
    {
        case 0x3E:
            kprintf(L"x86-64\r\n");
            break;
        default:
            kprintf(L"Unknown\r\n");
    }

    kprintf(L"  Entry point:              0x%lx\r\n", header->e_entry);
    kprintf(L"  Program header offset:    0x%lx\r\n", header->e_phoff);
    kprintf(L"  Section header offset:    0x%lx\r\n", header->e_shoff);
    kprintf(L"  Program header count:     %u\r\n",    header->e_phnum);
    kprintf(L"  Section header count:     %u\r\n",    header->e_shnum);

    elf_program_header* program_headers = program_headers_ptr;

    kprintf(L"\r\nProgram Headers:\r\n");
    UINTN p = 0;
    for(p = 0; p < header->e_phnum; p++) {
        kprintf(L"[%u]:\r\n", p);
        kprintf(L"  p_type:      0x%lx\r\n", program_headers[p].p_type);
        kprintf(L"  p_flags:     0x%lx\r\n", program_headers[p].p_flags);
        kprintf(L"  p_offset:    0x%lx\r\n", program_headers[p].p_offset);
        kprintf(L"  p_vaddr:     0x%lx\r\n", program_headers[p].p_vaddr);
        kprintf(L"  p_paddr:     0x%lx\r\n", program_headers[p].p_paddr);
        kprintf(L"  p_filesz:    0x%lx\r\n", program_headers[p].p_filesz);
        kprintf(L"  p_memsz:     0x%lx\r\n", program_headers[p].p_memsz);
        kprintf(L"  p_align:     0x%lx\r\n", program_headers[p].p_align);
        kprintf(L"\r\n");
    }
}
*/

EFI_STATUS elf_read_identity(EFI_FILE_PROTOCOL * const file, UINT8 **buffer)
{
    EFI_STATUS status;

    status = file->SetPosition(file, 0);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error setting file position %s\r\n", efi_error_message(status));
        return status;
    }

    UINTN read_size = EI_NIDENT;
    EFI_BS_CALL(AllocatePool(EfiLoaderData, read_size, (void**)buffer));

    status = file->Read(file, &read_size, (void *)*buffer);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading kernel identity %s\r\n", efi_error_message(status));
        return status;
    }

    return EFI_SUCCESS;
}

EFI_STATUS elf_validate_identity(UINT8 *const buffer)
{
    if ((buffer[EI_MAG0] != 0x7F) || (buffer[EI_MAG1] != 0x45) || (buffer[EI_MAG2] != 0x4C) || (buffer[EI_MAG3] != 0x46))
    {
        kprintf(L"Fatal Error: Invalid ELF header\r\n");
        return EFI_INVALID_PARAMETER;
    }

    if (buffer[EI_CLASS] != ELF_FILE_CLASS_64)
    {
        kprintf(L"Unsupported ELF file class %d\r\n", buffer[EI_CLASS]);
        return EFI_UNSUPPORTED;
    }

    if (buffer[EI_DATA] != 1)
    {
        kprintf(L"Only LSB ELF executables are currently supported\r\n");
        return EFI_INCOMPATIBLE_VERSION;
    }

    return EFI_SUCCESS;
}

EFI_STATUS elf_read_file(EFI_FILE_PROTOCOL *const file, void **header, void **program_headers)
{
    EFI_STATUS status;

    status = file->SetPosition(file, 0);
    if (EFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", efi_error_message(status));
        return status;
    }

    UINTN read_size = sizeof(elf_header_t);

    EFI_BS_CALL(AllocatePool(EfiLoaderData, read_size, header));

    status = file->Read(file, &read_size, *header);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading ELF header %s\r\n", efi_error_message(status));
        return status;
    }

    UINTN program_headers_offset = ((elf_header_t *)*header)->e_phoff;
    read_size = sizeof(elf_program_header) * ((elf_header_t *)*header)->e_phnum;

    status = file->SetPosition(file, program_headers_offset);
    if (EFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", efi_error_message(status));
        return status;
    }

    EFI_BS_CALL(AllocatePool(EfiLoaderData, read_size, program_headers));

    status = file->Read(file, &read_size, *program_headers);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading ELF program headers %s\r\n", efi_error_message(status));
        return status;
    }

#ifdef DEBUG
    print_elf_file_info(*header, *program_headers);
#endif

    return EFI_SUCCESS;
}
