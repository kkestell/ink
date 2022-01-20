#include "elf.h"
#include "loader.h"
#include "printf.h"

#define UEFI_BS_CALL(call) {                                                  \
    status = uefiSystemTable->BootServices->call;                             \
    if (UEFI_ERROR(status))                                                   \
    {                                                                         \
        kprintf(L"%s\r\n", uefiErrorMessage(status));                         \
        return status;                                                        \
    }                                                                         \
}

static UEFI_STATUS load_segment(UEFI_FILE_PROTOCOL *const kernelImageFile, UEFI_PHYSICAL_ADDRESS const segmentFileOffset, UINTN const segmentFileSize, UINTN const segmentMemorySize, UEFI_PHYSICAL_ADDRESS const segmentVirtualAddress)
{
    UEFI_STATUS status;
    void *programData = NULL;
    UINTN bufferReadSize = 0;
    UINTN segmentPageCount = UEFI_SIZE_TO_PAGES(segmentMemorySize);

    status = kernelImageFile->SetPosition(kernelImageFile, segmentFileOffset);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error setting file pointer position to segment offset %s\r\n", uefiErrorMessage(status));
        return status;
    }

    UEFI_BS_CALL(AllocatePages(AllocateAnyPages, UefiLoaderData, segmentPageCount, (UEFI_PHYSICAL_ADDRESS*)segmentVirtualAddress));

    /*
    status = uefiSystemTable->BootServices->AllocatePages(AllocateAnyPages, UefiLoaderData, segmentPageCount, (UEFI_PHYSICAL_ADDRESS*)segmentVirtualAddress);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating pages for ELF segment %s\r\n", uefiErrorMessage(status));
        return status;
    }
    */

    if(segmentFileSize > 0)
    {
        bufferReadSize = segmentFileSize;

        UEFI_BS_CALL(AllocatePool(UefiLoaderCode, bufferReadSize, (void **)&programData));

        /*
        status = uefiSystemTable->BootServices->AllocatePool(UefiLoaderCode, bufferReadSize, (void **)&programData);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error kernel segment buffer %s\r\n", uefiErrorMessage(status));
            return status;
        }
        */

        status = kernelImageFile->Read(kernelImageFile, &bufferReadSize, (void *)programData);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error reading segment data %s\r\n", uefiErrorMessage(status));
            return status;
        }

        UEFI_BS_CALL(CopyMem((void *)segmentVirtualAddress, (void *)programData, segmentFileSize));

        /*
        status = uefiSystemTable->BootServices->CopyMem((void *)segmentVirtualAddress, (void *)programData, segmentFileSize);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error copying program section into memory %s\r\n", uefiErrorMessage(status));
            return status;
        }
        */

        UEFI_BS_CALL(FreePool(programData));

        /*
        status = uefiSystemTable->BootServices->FreePool(programData);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error freeing program section %s\r\n", uefiErrorMessage(status));
            return status;
        }
        */
    }

    // If the size in memory is larger than the file size, the segment must be zero filled
    UEFI_PHYSICAL_ADDRESS zeroFillStart = segmentVirtualAddress + segmentFileSize;
    UINTN zeroFillCount = segmentMemorySize - segmentFileSize;

    if (zeroFillCount > 0)
    {
        UEFI_BS_CALL(SetMem((void *)zeroFillStart, zeroFillCount, 0));

        /*
        status = uefiSystemTable->BootServices->SetMem((void *)zeroFillStart, zeroFillCount, 0);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error zero filling segment %s\r\n", uefiErrorMessage(status));
            return status;
        }
        */
    }

    return UEFI_SUCCESS;
}

static UEFI_STATUS load_program_segments(UEFI_FILE_PROTOCOL * const kernelImageFile, void * const kernelHeaderBuffer, void * const kernelProgramHeadersBuffer)
{
    UEFI_STATUS status;
    UINT16 numProgramHeaders = 0;
    UINT16 numSegmentsLoaded = 0;

    numProgramHeaders = ((ElfHeader *)kernelHeaderBuffer)->e_phnum;

    if (numProgramHeaders == 0)
    {
        kprintf(L"No program segments in kernel image\r\n");
        return UEFI_INVALID_PARAMETER;
    }

    ElfProgramHeader *program_headers = (ElfProgramHeader *)kernelProgramHeadersBuffer;

    for (UINTN p = 0; p < numProgramHeaders; p++)
    {
        if (program_headers[p].p_type == PT_LOAD)
        {
            status = load_segment(
                kernelImageFile,
                program_headers[p].p_offset,
                program_headers[p].p_filesz,
                program_headers[p].p_memsz,
                program_headers[p].p_vaddr);

            if (UEFI_ERROR(status))
            {
                kprintf(L"Error loading segment %s\r\n", uefiErrorMessage(status));
                return status;
            }

            numSegmentsLoaded++;
        }
    }

    if (numSegmentsLoaded == 0)
    {
        kprintf(L"No loadable program in kernel image\r\n");
        return UEFI_NOT_FOUND;
    }

    return UEFI_SUCCESS;
}

UEFI_STATUS load_kernel(UEFI_PHYSICAL_ADDRESS *kernelEntryPoint)
{
    UEFI_STATUS status;

    UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystemService;

    UEFI_GUID guid = UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    UEFI_BS_CALL(LocateProtocol(&guid, 0, (void **)&fileSystemService));

    /*
    status = uefiSystemTable->BootServices->LocateProtocol(&guid, 0, (void **)&fileSystemService);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL %s\r\n", uefiErrorMessage(status));
        return status;
    }
    */

    UEFI_FILE_PROTOCOL *rootFileSystem;

    status = fileSystemService->OpenVolume(fileSystemService, &rootFileSystem);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error opening root volume %s\r\n", uefiErrorMessage(status));
        return status;
    }

    UEFI_FILE_PROTOCOL *kernelImageFile;

    status = rootFileSystem->Open(rootFileSystem, &kernelImageFile, L"kernel.elf",  UEFI_FILE_MODE_READ, UEFI_FILE_READ_ONLY);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error opening kernel %s\r\n", uefiErrorMessage(status));
        return status;
    }

    UINT8 *elfIdentityBuffer;

    status = elf_read_identity(kernelImageFile, &elfIdentityBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading ELF identity %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = elf_validate_identity(elfIdentityBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Invalid ELF identity buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }

    UEFI_BS_CALL(FreePool(elfIdentityBuffer));

    /*
    status = uefiSystemTable->BootServices->FreePool(elfIdentityBuffer);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing ELF identity buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }
    */

    void *kernelHeader;
    void *kernelProgramHeaders;

    status = elf_read_file(kernelImageFile, &kernelHeader, &kernelProgramHeaders);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error reading ELF file %s\r\n", uefiErrorMessage(status));
        return status;
    }

    *kernelEntryPoint = ((ElfHeader *)kernelHeader)->e_entry;

    status = load_program_segments(kernelImageFile, kernelHeader, kernelProgramHeaders);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error loading ELF program segments %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = kernelImageFile->Close(kernelImageFile);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error closing kernel image file %s\r\n", uefiErrorMessage(status));
        return status;
    }

    UEFI_BS_CALL(FreePool((void *)kernelHeader));

    /*
    status = uefiSystemTable->BootServices->FreePool((void *)kernelHeader);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing kernel header buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }
    */

    UEFI_BS_CALL(FreePool((void *)kernelProgramHeaders));

    /*
    status = uefiSystemTable->BootServices->FreePool((void *)kernelProgramHeaders);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error freeing kernel program headers buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }
    */

    return UEFI_SUCCESS;
}
