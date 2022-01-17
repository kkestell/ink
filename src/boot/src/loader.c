#include "uefi.h"
#include "utils.h"
#include "elf.h"
#include "loader.h"

UEFI_STATUS load_kernel()
{
    UEFI_STATUS status;

    UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system_service;

    UEFI_GUID guid = UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    status = uefi_system_table->BootServices->LocateProtocol(&guid, 0, (void**)&file_system_service);
    if (status)
    {
        kprintf(L"Error initializing UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL\r\n");
        return status;
    }

    UEFI_FILE_PROTOCOL *root_file_system;

    status = file_system_service->OpenVolume(file_system_service, &root_file_system);
    if (status)
    {
        kprintf(L"Error opening root volume\r\n");
        return status;
    }

    UEFI_FILE_PROTOCOL *kernel_image_file;

    status = root_file_system->Open(root_file_system, &kernel_image_file, L"kernel.elf",  UEFI_FILE_MODE_READ, UEFI_FILE_READ_ONLY);
    if (status)
    {
        kprintf(L"Error opening kernel\r\n");
        return status;
    }

    UINT8* elf_identity_buffer;

    status = read_elf_identity(kernel_image_file, &elf_identity_buffer);
    if (status)
    {
        kprintf(L"Error reading ELF identity\r\n");
        return status;
    }

    status = validate_elf_identity(elf_identity_buffer);
    if (status)
    {
        kprintf(L"Invalid ELF identity buffer\r\n");
        return status;
    }

    status = uefi_system_table->BootServices->FreePool(elf_identity_buffer);
    if (status)
    {
        kprintf(L"Error freeing memory\r\n");
        return status;
    }

    void* kernel_header;
    void* kernel_program_headers;

    status = read_elf_file(kernel_image_file, &kernel_header, &kernel_program_headers);
    if (status)
    {
        kprintf(L"Error reading ELF file\r\n");
        return status;
    }

    return UEFI_SUCCESS;
}
