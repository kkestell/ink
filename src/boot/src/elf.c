#include "elf.h"
#include "efi.h"
#include "printf.h"

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

    UINTN read_size = sizeof(ElfHeader);

    EFI_BS_CALL(AllocatePool(EfiLoaderData, read_size, header));

    status = file->Read(file, &read_size, *header);
    if (EFI_ERROR(status))
    {
        kprintf(L"Error reading ELF header %s\r\n", efi_error_message(status));
        return status;
    }

    UINTN program_headers_offset = ((ElfHeader *)*header)->e_phoff;
    read_size = sizeof(ElfProgramHeader) * ((ElfHeader *)*header)->e_phnum;

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

    return EFI_SUCCESS;
}
