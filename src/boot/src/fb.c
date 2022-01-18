#include "printf.h"
#include "fb.h"
#include "boot_info.h"

UEFI_STATUS fb_init(kernel_boot_info *boot_info)
{
    UEFI_STATUS status;
    UEFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    
    UEFI_GUID guid = UEFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    status = uefi_system_table->BootServices->LocateProtocol(&guid, 0, (void**)&gop);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing UEFI_GRAPHICS_OUTPUT_PROTOCOL %s\r\n", uefi_error_message(status));
        return status;
    }

    uint64_t size_of_info;

    status = gop->QueryMode(gop, gop->Mode == 0 ? 0 : gop->Mode->Mode, &size_of_info, &info);
    if (status == UEFI_NOT_STARTED)
    {        
        status = gop->SetMode(gop, 0);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Unable to determine native display resolution %s\r\n", uefi_error_message(status));
            return status;
        }
    }

    for (uint32_t i = 0; i < gop->Mode->MaxMode; i++)
    {
        status = gop->QueryMode(gop, i, &size_of_info, &info);
        if (UEFI_ERROR(status))
        {
            kprintf(L"Error querying mode");
            return status;
        }

        if (info->HorizontalResolution == 1280 && info->VerticalResolution == 720 && info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor)
        {
            status = gop->SetMode(gop, i);
            if (UEFI_ERROR(status))
            {
                kprintf(L"Error setting display resolution");
            }

            //clear();

            boot_info->horizontal_resolution = info->HorizontalResolution;
            boot_info->vertical_resolution = info->VerticalResolution;
            boot_info->pixel_format = info->PixelFormat;
            boot_info->pixel_information = info->PixelInformation;
            boot_info->pixels_per_scan_line = info->PixelsPerScanLine;
            boot_info->framebuffer_base_address = gop->Mode->FrameBufferBase;
            boot_info->framebuffer_size = gop->Mode->FrameBufferSize;

            return UEFI_SUCCESS;
        }
    }

    kprintf(L"Requested display resolution not available");

    return UEFI_UNSUPPORTED;
}
