#include "elf.h"
#include "uefi.h"
#include "printf.h"

UEFI_STATUS elf_read_identity(UEFI_FILE_PROTOCOL * const kernelImageFile, UINT8 **elfIdentityBuffer)
{
    UEFI_STATUS status;

    UINTN bufferReadSize = EI_NIDENT;

    status = kernelImageFile->SetPosition(kernelImageFile, 0);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error setting file position %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = uefiSystemTable->BootServices->AllocatePool(UefiLoaderData, bufferReadSize, (void**)elfIdentityBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating kernel identity buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = kernelImageFile->Read(kernelImageFile, &bufferReadSize, (void *)*elfIdentityBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel identity %s\r\n", uefiErrorMessage(status));
        return status;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS elf_validate_identity(UINT8 *const elfIdentityBuffer)
{
    if ((elfIdentityBuffer[EI_MAG0] != 0x7F) ||
        (elfIdentityBuffer[EI_MAG1] != 0x45) ||
        (elfIdentityBuffer[EI_MAG2] != 0x4C) ||
        (elfIdentityBuffer[EI_MAG3] != 0x46))
    {
        kprintf(L"Fatal Error: Invalid ELF header\r\n");
        return UEFI_INVALID_PARAMETER;
    }

    if (elfIdentityBuffer[EI_CLASS] != ELF_FILE_CLASS_64)
    {
        kprintf(L"Unsupported ELF file class %d\r\n", elfIdentityBuffer[EI_CLASS]);
        return UEFI_UNSUPPORTED;
    }

    if (elfIdentityBuffer[EI_DATA] != 1)
    {
        kprintf(L"Only LSB ELF executables are currently supported\r\n");
        return UEFI_INCOMPATIBLE_VERSION;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS elf_read_file(UEFI_FILE_PROTOCOL * const kernelImageFile, void **kernelHeaderBuffer, void **kernelProgramHeadersBuffer)
{
    UEFI_STATUS status;

    UINTN bufferReadSize = 0;
    UINTN program_headers_offset = 0;

    status = kernelImageFile->SetPosition(kernelImageFile, 0);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", uefiErrorMessage(status));
        return status;
    }

    bufferReadSize = sizeof(ElfHeader);

    status = uefiSystemTable->BootServices->AllocatePool(UefiLoaderData, bufferReadSize, kernelHeaderBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to allocate memory for kernel image %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = kernelImageFile->Read(kernelImageFile, &bufferReadSize, *kernelHeaderBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel image header %s\r\n", uefiErrorMessage(status));
        return status;
    }

    program_headers_offset = ((ElfHeader *)*kernelHeaderBuffer)->e_phoff;
    bufferReadSize = sizeof(ElfProgramHeader) * ((ElfHeader *)*kernelHeaderBuffer)->e_phnum;

    status = kernelImageFile->SetPosition(kernelImageFile, program_headers_offset);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Unable to set file position %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = uefiSystemTable->BootServices->AllocatePool(UefiLoaderData, bufferReadSize, kernelProgramHeadersBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating memory for kernel program headers %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = kernelImageFile->Read(kernelImageFile, &bufferReadSize, *kernelProgramHeadersBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading kernel program headers %s\r\n", uefiErrorMessage(status));
        return status;
    }

    return UEFI_SUCCESS;
}
