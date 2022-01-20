#include "elf.h"
#include "uefi.h"
#include "printf.h"

UEFI_STATUS read_elf_identity(UEFI_FILE_PROTOCOL* const kernel_image_file,
                              UINT8** elf_identity_buffer)
{
    UEFI_STATUS status;

    UINTN buffer_read_size = EI_NIDENT;

    status = kernel_image_file->SetPosition(kernel_image_file, 0);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error setting file position %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->AllocatePool(UefiLoaderData, buffer_read_size, (void**)elf_identity_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating kernel identity buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    status = kernel_image_file->Read(kernel_image_file, &buffer_read_size, (void *)*elf_identity_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel identity %s\r\n", uefi_error_message(status));
        return status;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS validate_elf_identity(UINT8* const elf_identity_buffer)
{
    if ((elf_identity_buffer[EI_MAG0] != 0x7F) ||
        (elf_identity_buffer[EI_MAG1] != 0x45) ||
        (elf_identity_buffer[EI_MAG2] != 0x4C) ||
        (elf_identity_buffer[EI_MAG3] != 0x46))
    {
        kprintf(L"Fatal Error: Invalid ELF header\r\n");
        return UEFI_INVALID_PARAMETER;
    }

    if (elf_identity_buffer[EI_CLASS] != ELF_FILE_CLASS_64)
    {
        kprintf(L"Unsupported ELF file class %d\r\n", elf_identity_buffer[EI_CLASS]);
        return UEFI_UNSUPPORTED;
    }

    if (elf_identity_buffer[EI_DATA] != 1)
    {
        kprintf(L"Only LSB ELF executables are currently supported\r\n");
        return UEFI_INCOMPATIBLE_VERSION;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS read_elf_file(UEFI_FILE_PROTOCOL* const kernel_image_file,
                          void** kernel_header_buffer,
                          void** kernel_program_headers_buffer)
{
    UEFI_STATUS status;

    UINTN buffer_read_size = 0;
    UINTN program_headers_offset = 0;

    status = kernel_image_file->SetPosition(kernel_image_file, 0);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", uefi_error_message(status));
        return status;
    }

    buffer_read_size = sizeof(Elf64_Ehdr);

    status = uefi_system_table->BootServices->AllocatePool(UefiLoaderData, buffer_read_size, kernel_header_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to allocate memory for kernel image %s\r\n", uefi_error_message(status));
        return status;
    }

    status = kernel_image_file->Read(kernel_image_file, &buffer_read_size, *kernel_header_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel image header %s\r\n", uefi_error_message(status));
        return status;
    }

    program_headers_offset = ((Elf64_Ehdr*)*kernel_header_buffer)->e_phoff;
    buffer_read_size = sizeof(Elf64_Phdr) * ((Elf64_Ehdr*)*kernel_header_buffer)->e_phnum;

    status = kernel_image_file->SetPosition(kernel_image_file, program_headers_offset);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->AllocatePool(UefiLoaderData, buffer_read_size, kernel_program_headers_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating memory for kernel program headers %s\r\n", uefi_error_message(status));
        return status;
    }

    status = kernel_image_file->Read(kernel_image_file, &buffer_read_size, *kernel_program_headers_buffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel program headers %s\r\n", uefi_error_message(status));
        return status;
    }

    return UEFI_SUCCESS;
}
