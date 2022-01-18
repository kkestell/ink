#include "uefi.h"
#include "printf.h"
#include "elf.h"
#include "loader.h"

void print_elf_file_info(void* const header_ptr,
                         void* const program_headers_ptr)
{
    Elf64_Ehdr* header = (Elf64_Ehdr*)header_ptr;

    kprintf(L"Debug: ELF Header Info:\r\n");

    kprintf(L"  Magic:                    ");
    UINTN i = 0;
    for(i = 0; i < 4; i++) {
        kprintf(L"0x%x ", header->e_ident[i]);
    }
    kprintf(L"\r\n");

    kprintf(L"  Class:                    ");
    kprintf(L"64bit");
    kprintf(L"\r\n");

    kprintf(L"  Endianness:               ");
    if(header->e_ident[EI_DATA] == 1) {
        kprintf(L"Little-Endian");
    } else if(header->e_ident[EI_DATA] == 2) {
        kprintf(L"Big-Endian");
    }
    kprintf(L"\r\n");

    kprintf(L"  Version:                  0x%x\r\n",
        header->e_ident[EI_VERSION]);

    kprintf(L"  OS ABI:                   ");
    if(header->e_ident[EI_OSABI] == 0x00) {
        kprintf(L"System V");
    } else if(header->e_ident[EI_OSABI] == 0x01) {
        kprintf(L"HP-UX");
    } else if(header->e_ident[EI_OSABI] == 0x02) {
        kprintf(L"NetBSD");
    } else if(header->e_ident[EI_OSABI] == 0x03) {
        kprintf(L"Linux");
    } else if(header->e_ident[EI_OSABI] == 0x04) {
        kprintf(L"GNU Hurd");
    } else if(header->e_ident[EI_OSABI] == 0x06) {
        kprintf(L"Solaris");
    } else if(header->e_ident[EI_OSABI] == 0x07) {
        kprintf(L"AIX");
    } else if(header->e_ident[EI_OSABI] == 0x08) {
        kprintf(L"IRIX");
    } else if(header->e_ident[EI_OSABI] == 0x09) {
        kprintf(L"FreeBSD");
    } else if(header->e_ident[EI_OSABI] == 0x0A) {
        kprintf(L"Tru64");
    } else if(header->e_ident[EI_OSABI] == 0x0B) {
        kprintf(L"Novell Modesto");
    } else if(header->e_ident[EI_OSABI] == 0x0C) {
        kprintf(L"OpenBSD");
    } else if(header->e_ident[EI_OSABI] == 0x0D) {
        kprintf(L"OpenVMS");
    } else if(header->e_ident[EI_OSABI] == 0x0E) {
        kprintf(L"NonStop Kernel");
    } else if(header->e_ident[EI_OSABI] == 0x0F) {
        kprintf(L"AROS");
    } else if(header->e_ident[EI_OSABI] == 0x10) {
        kprintf(L"Fenix OS");
    } else if(header->e_ident[EI_OSABI] == 0x11) {
        kprintf(L"CloudABI");
    }
    kprintf(L"\r\n");

    kprintf(L"  File Type:                ");
    if(header->e_type == 0x00) {
        kprintf(L"None");
    } else if(header->e_type == 0x01) {
        kprintf(L"Relocatable");
    } else if(header->e_type == 0x02) {
        kprintf(L"Executable");
    } else if(header->e_type == 0x03) {
        kprintf(L"Dynamic");
    } else {
        kprintf(L"Other");
    }
    kprintf(L"\r\n");

    kprintf(L"  Machine Type:             ");
    if(header->e_machine == 0x00) {
        kprintf(L"No specific instruction set");
    } else if(header->e_machine == 0x02) {
        kprintf(L"SPARC");
    } else if(header->e_machine == 0x03) {
        kprintf(L"x86");
    } else if(header->e_machine == 0x08) {
        kprintf(L"MIPS");
    } else if(header->e_machine == 0x14) {
        kprintf(L"PowerPC");
    } else if(header->e_machine == 0x16) {
        kprintf(L"S390");
    } else if(header->e_machine == 0x28) {
        kprintf(L"ARM");
    } else if(header->e_machine == 0x2A) {
        kprintf(L"SuperH");
    } else if(header->e_machine == 0x32) {
        kprintf(L"IA-64");
    } else if(header->e_machine == 0x3E) {
        kprintf(L"x86-64");
    } else if(header->e_machine == 0xB7) {
        kprintf(L"AArch64");
    } else if(header->e_machine == 0xF3) {
        kprintf(L"RISC-V");
    }
    kprintf(L"\r\n");

    kprintf(L"  Entry point:              %lx\r\n", header->e_entry);
    kprintf(L"  Program header offset:    %lx\r\n", header->e_phoff);
    kprintf(L"  Section header offset:    %lx\r\n", header->e_shoff);
    kprintf(L"  Program header count:     %u\r\n", header->e_phnum);
    kprintf(L"  Section header count:     %u\r\n", header->e_shnum);

    Elf64_Phdr* program_headers = program_headers_ptr;

    kprintf(L"\r\nDebug: Program Headers:\r\n");
    UINTN p = 0;
    for(p = 0; p < header->e_phnum; p++) {
        kprintf(L"[%u]:\r\n", p);
        kprintf(L"  p_type:      %lx\r\n", program_headers[p].p_type);
        kprintf(L"  p_flags:     %lx\r\n", program_headers[p].p_flags);
        kprintf(L"  p_offset:    %lx\r\n", program_headers[p].p_offset);
        kprintf(L"  p_vaddr:     %lx\r\n", program_headers[p].p_vaddr);
        kprintf(L"  p_paddr:     %lx\r\n", program_headers[p].p_paddr);
        kprintf(L"  p_filesz:    %lx\r\n", program_headers[p].p_filesz);
        kprintf(L"  p_memsz:     %lx\r\n", program_headers[p].p_memsz);
        kprintf(L"  p_align:     %lx\r\n", program_headers[p].p_align);
        kprintf(L"\r\n");
    }
}

UEFI_STATUS load_segment(UEFI_FILE_PROTOCOL* const kernel_image_file,
                         UEFI_PHYSICAL_ADDRESS const segment_file_offset,
                         UINTN const segment_file_size,
                         UINTN const segment_memory_size,
                         UEFI_PHYSICAL_ADDRESS const segment_virtual_address)
{
    UEFI_STATUS status;
    void* program_data = NULL;
    UINTN buffer_read_size = 0;
    UINTN segment_page_count = UEFI_SIZE_TO_PAGES(segment_memory_size);

    status = kernel_image_file->SetPosition(kernel_image_file, segment_file_offset);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error setting file pointer position to segment offset %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->AllocatePages(AllocateAnyPages, UefiLoaderData, segment_page_count, (UEFI_PHYSICAL_ADDRESS*)segment_virtual_address);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating pages for ELF segment %s\r\n", uefi_error_message(status));
        return status;
    }

    if(segment_file_size > 0)
    {
        buffer_read_size = segment_file_size;

        status = uefi_system_table->BootServices->AllocatePool(UefiLoaderCode, buffer_read_size, (void**)&program_data);
        if (UEFI_ERROR(status))

        {
            kprintf(L"Error kernel segment buffer %s\r\n", uefi_error_message(status));
            return status;
        }

        status = kernel_image_file->Read(kernel_image_file, &buffer_read_size, (void*)program_data);
        if (UEFI_ERROR(status))

        {
            kprintf(L"Error reading segment data %s\r\n", uefi_error_message(status));
            return status;
        }

        status = uefi_system_table->BootServices->CopyMem((void*)segment_virtual_address, (void*)program_data, segment_file_size);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error copying program section into memory %s\r\n", uefi_error_message(status));
            return status;
        }

        status = uefi_system_table->BootServices->FreePool(program_data);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error freeing program section %s\r\n", uefi_error_message(status));
            return status;
        }
    }

    // As per ELF Standard, if the size in memory is larger than the file size
    // the segment is mandated to be zero filled.
    // For more information on Refer to ELF standard page 34.
    UEFI_PHYSICAL_ADDRESS zero_fill_start = segment_virtual_address + segment_file_size;
    UINTN zero_fill_count = segment_memory_size - segment_file_size;

    if (zero_fill_count > 0)
    {
        status = uefi_system_table->BootServices->SetMem((void*)zero_fill_start, zero_fill_count, 0);
        if (UEFI_ERROR(status))

        {
            kprintf(L"Error zero filling segment %s\r\n", uefi_error_message(status));
            return status;
        }
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS load_program_segments(UEFI_FILE_PROTOCOL* const kernel_image_file,
                                 void* const kernel_header_buffer,
                                 void* const kernel_program_headers_buffer)
{
    UEFI_STATUS status;
    UINT16 n_program_headers = 0;
    UINT16 n_segments_loaded = 0;

    n_program_headers = ((Elf64_Ehdr*)kernel_header_buffer)->e_phnum;

    if (n_program_headers == 0)
    {
        kprintf(L"No program segments in kernel image\r\n");
        return UEFI_INVALID_PARAMETER;
    }

    Elf64_Phdr* program_headers = (Elf64_Phdr*)kernel_program_headers_buffer;

    for (UINTN p = 0; p < n_program_headers; p++)
    {
        if (program_headers[p].p_type == PT_LOAD)
        {
            status = load_segment(
                kernel_image_file,
                program_headers[p].p_offset,
                program_headers[p].p_filesz,
                program_headers[p].p_memsz,
                program_headers[p].p_vaddr);

            if (UEFI_ERROR(status))
            {
                kprintf(L"Error loading segment %s\r\n", uefi_error_message(status));
                return status;
            }

            n_segments_loaded++;
        }
    }

    if (n_segments_loaded == 0)
    {
        kprintf(L"No loadable program in kernel image\r\n");
        return UEFI_NOT_FOUND;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS load_kernel(UEFI_PHYSICAL_ADDRESS *kernel_entry_point)
{
    UEFI_STATUS status;

    UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system_service;

    UEFI_GUID guid = UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    status = uefi_system_table->BootServices->LocateProtocol(&guid, 0, (void**)&file_system_service);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL %s\r\n", uefi_error_message(status));
        return status;
    }

    UEFI_FILE_PROTOCOL *root_file_system;

    status = file_system_service->OpenVolume(file_system_service, &root_file_system);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error opening root volume %s\r\n", uefi_error_message(status));
        return status;
    }

    UEFI_FILE_PROTOCOL *kernel_image_file;

    status = root_file_system->Open(root_file_system, &kernel_image_file, L"kernel.elf",  UEFI_FILE_MODE_READ, UEFI_FILE_READ_ONLY);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error opening kernel %s\r\n", uefi_error_message(status));
        return status;
    }

    UINT8* elf_identity_buffer;

    status = read_elf_identity(kernel_image_file, &elf_identity_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading ELF identity %s\r\n", uefi_error_message(status));
        return status;
    }

    status = validate_elf_identity(elf_identity_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Invalid ELF identity buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->FreePool(elf_identity_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing ELF identity buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    void* kernel_header;
    void* kernel_program_headers;

    status = read_elf_file(kernel_image_file, &kernel_header, &kernel_program_headers);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading ELF file %s\r\n", uefi_error_message(status));
        return status;
    }

#ifdef DEBUG
    print_elf_file_info(kernel_header, kernel_program_headers);
#endif

    *kernel_entry_point = ((Elf64_Ehdr*)kernel_header)->e_entry;

    status = load_program_segments(kernel_image_file, kernel_header, kernel_program_headers);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error loading ELF program segments %s\r\n", uefi_error_message(status));
        return status;
    }

    status = kernel_image_file->Close(kernel_image_file);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error closing kernel image file %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->FreePool((void*)kernel_header);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing kernel header buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->FreePool((void*)kernel_program_headers);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing kernel program headers buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    return UEFI_SUCCESS;
}
