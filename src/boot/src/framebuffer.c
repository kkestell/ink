#include "efi.h"
#include "framebuffer.h"
#include "kprintf.h"

EFI_STATUS framebuffer_init(kernel_framebuffer_info_t *framebuffer_info)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    
    EFI_GUID guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_BS_CALL(LocateProtocol(&guid, 0, (void **)&gop));

    if (gop == NULL)
    {
        kprintf(L"Unable to locate GOP\r\n");
        return EFI_UNSUPPORTED;
    }

    status = gop->QueryMode(gop, gop->Mode == 0 ? 0 : gop->Mode->Mode, NULL, NULL);
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
        UINTN info_size;
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;

        status = gop->QueryMode(gop, i, &info_size, &info);
        if (EFI_ERROR(status))
        {
            kprintf(L"Error querying mode %d\r\n", i);
            continue;
        }

        kprintf(L"Mode %d: %dx%d, PixelFormat=%d\r\n", i, info->HorizontalResolution, info->VerticalResolution, info->PixelFormat);
    }

    for (uint32_t i = 0; i < gop->Mode->MaxMode; i++)
    {
        UINTN info_size;
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;

        status = gop->QueryMode(gop, i, &info_size, &info);
        if (EFI_ERROR(status))
        {
            kprintf(L"Error querying mode %d\r\n", i);
            continue;
        }

        if (info->HorizontalResolution == 1024 && info->VerticalResolution == 768)
        {
            status = gop->SetMode(gop, i);
            if (EFI_ERROR(status))
            {
                kprintf(L"Error setting display resolution %s\r\n", efi_error_message(status));
                return status;
            }

            framebuffer_info->framebuffer_width                = info->HorizontalResolution;
            framebuffer_info->framebuffer_height               = info->VerticalResolution;
            framebuffer_info->framebuffer_pixel_format         = info->PixelFormat;
            framebuffer_info->framebuffer_pixel_information    = info->PixelInformation;
            framebuffer_info->framebuffer_pixels_per_scan_line = info->PixelsPerScanLine;
            framebuffer_info->framebuffer_base_address         = gop->Mode->FrameBufferBase;
            framebuffer_info->framebuffer_size                 = gop->Mode->FrameBufferSize;

            return EFI_SUCCESS;
        }
    }

    kprintf(L"Requested display resolution not available\r\n");
    return EFI_UNSUPPORTED;
}
