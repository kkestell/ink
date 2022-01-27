#include "elf.h"
#include "loader.h"
#include "printf.h"

static EFI_STATUS load_segment(EFI_FILE_PROTOCOL *const file, EFI_PHYSICAL_ADDRESS const offset, UINTN const file_size, UINTN const memory_size, EFI_PHYSICAL_ADDRESS const address)
{
    EFI_STATUS status;
    void *program_data = NULL;
    UINTN read_size = 0;
    UINTN page_count = EFI_SIZE_TO_PAGES(memory_size);

    status = file->SetPosition(file, offset);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error setting file pointer position to segment offset %s\r\n", efi_error_message(status));
        return status;
    }

    EFI_BS_CALL(AllocatePages(AllocateAnyPages, EfiLoaderData, page_count, (EFI_PHYSICAL_ADDRESS *)address));

    if(file_size > 0)
    {
        read_size = file_size;

        EFI_BS_CALL(AllocatePool(EfiLoaderCode, read_size, (void **)&program_data));

        status = file->Read(file, &read_size, (void *)program_data);
        if (EFI_ERROR(status))
        {
            kprintf(L"Error reading segment data %s\r\n", efi_error_message(status));
            return status;
        }

        EFI_BS_CALL(CopyMem((void *)address, (void *)program_data, file_size));

        EFI_BS_CALL(FreePool(program_data));
    }

    // If the size in memory is larger than the file size, the segment must be zero filled
    EFI_PHYSICAL_ADDRESS zero_fill_start = address + file_size;
    UINTN zero_fill_count = memory_size - file_size;

    if (zero_fill_count > 0)
    {
        EFI_BS_CALL(SetMem((void *)zero_fill_start, zero_fill_count, 0));
    }

    return EFI_SUCCESS;
}

static EFI_STATUS load_program_segments(EFI_FILE_PROTOCOL *const file, void *const header_buffer, void *const program_headers_buffer)
{
    EFI_STATUS status;
    UINT16 num_headers = 0;
    UINT16 num_segments = 0;

    num_headers = ((elf_header_t *)header_buffer)->e_phnum;

    if (num_headers == 0)
    {
        kprintf(L"No program segments in kernel image\r\n");
        return EFI_INVALID_PARAMETER;
    }

    elf_program_header *program_header = (elf_program_header *)program_headers_buffer;

    for (UINTN p = 0; p < num_headers; p++)
    {
        if (program_header[p].p_type == PT_LOAD)
        {
            status = load_segment(file, program_header[p].p_offset, program_header[p].p_filesz, program_header[p].p_memsz, program_header[p].p_vaddr);
            if (EFI_ERROR(status))
            {
                kprintf(L"Error loading segment %s\r\n",
                    efi_error_message(status));
                return status;
            }

            num_segments++;
        }
    }

    if (num_segments == 0)
    {
        kprintf(L"No loadable program in kernel image\r\n");
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}

EFI_STATUS load_kernel(EFI_PHYSICAL_ADDRESS *kernel_entry_point)
{
    EFI_STATUS status;

    EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

    EFI_BS_CALL(LocateProtocol(&guid, 0, (void **)&fs));

    EFI_FILE_PROTOCOL *root;

    status = fs->OpenVolume(fs, &root);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error opening root volume %s\r\n", efi_error_message(status));
        return status;
    }

    EFI_FILE_PROTOCOL *file;

    status = root->Open(root, &file, L"kernel.elf", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error opening kernel %s\r\n", efi_error_message(status));
        return status;
    }

    UINT8 *elf_identity_buffer;

    status = elf_read_identity(file, &elf_identity_buffer);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading ELF identity %s\r\n", efi_error_message(status));
        return status;
    }

    status = elf_validate_identity(elf_identity_buffer);
    if (EFI_ERROR(status))
    {
        kprintf(L"Invalid ELF identity buffer %s\r\n", efi_error_message(status));
        return status;
    }

    EFI_BS_CALL(FreePool(elf_identity_buffer));

    void *kernel_header;
    void *kernel_program_headers;

    status = elf_read_file(file, &kernel_header, &kernel_program_headers);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading ELF file %s\r\n", efi_error_message(status));
        return status;
    }

    *kernel_entry_point = ((elf_header_t *)kernel_header)->e_entry;

    status = load_program_segments(file, kernel_header, kernel_program_headers);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error loading ELF program segments %s\r\n", efi_error_message(status));
        return status;
    }

    status = file->Close(file);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error closing kernel image file %s\r\n", efi_error_message(status));
        return status;
    }

    EFI_BS_CALL(FreePool((void *)kernel_header));

    EFI_BS_CALL(FreePool((void *)kernel_program_headers));

    return EFI_SUCCESS;
}
