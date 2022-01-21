#include "efi.h"
#include "framebuffer.h"
#include "printf.h"

EFI_STATUS framebuffer_init(KernelBootInfo *boot_info)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    
    EFI_GUID guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    EFI_BS_CALL(LocateProtocol(&guid, 0, (void **)&gop));

    UINTN infosz;
    UINTN modenum = gop->Mode == 0 ? 0 : gop->Mode->Mode;

    status = gop->QueryMode(gop, modenum, NULL, NULL);
    if (status == EFI_NOT_STARTED)
    {        
        status = gop->SetMode(gop, 0);
        if (EFI_ERROR(status))
        {
            kprintf(L"Unable to determine native display resolution %s\r\n", efi_error_message(status));
            return status;
        }
    }

    for (uint32_t i = 0; i < gop->Mode->MaxMode; i++)
    {
        status = gop->QueryMode(gop, i, &infosz, &info);
        if (EFI_ERROR(status))
        {
            kprintf(L"Error querying mode");
            return status;
        }

        if (info->HorizontalResolution == 800 && info->VerticalResolution == 600 && info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor)
        {
            status = gop->SetMode(gop, i);
            if (EFI_ERROR(status))
            {
                kprintf(L"Error setting display resolution %s\r\n", efi_error_message(status));
                return status;
            }

            boot_info->horizontal_resolution    = info->HorizontalResolution;
            boot_info->vertical_resolution      = info->VerticalResolution;
            boot_info->pixel_format             = info->PixelFormat;
            boot_info->pixel_information        = info->PixelInformation;
            boot_info->pixels_per_scan_line     = info->PixelsPerScanLine;
            boot_info->framebuffer_base_address = gop->Mode->FrameBufferBase;
            boot_info->framebuffer_size         = gop->Mode->FrameBufferSize;

            return EFI_SUCCESS;
        }
    }

    kprintf(L"Requested display resolution not available");
    return EFI_UNSUPPORTED;
}
