#include "uefi.h"
#include "utils.h"
#include "fb.h"

// ???
void *_fltused = 0;

UEFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

static void set_mode(uint32_t width, uint32_t height)
{
    UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
    uint64_t size_of_info;

    if (gop->QueryMode(gop, gop->Mode == 0 ? 0 : gop->Mode->Mode, &size_of_info, &info) == UEFI_NOT_STARTED)
    {        
        if (gop->SetMode(gop, 0))
        {
            kprintf(L"Unable to determine native display resolution");
            return;
        }
    }

    for (uint32_t i = 0; i < gop->Mode->MaxMode; i++)
    {
        if (gop->QueryMode(gop, i, &size_of_info, &info))
        {
            kprintf(L"Error querying mode");
            return;
        }

        if (info->HorizontalResolution == width && info->VerticalResolution == height && info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor)
        {
            if (gop->SetMode(gop, i))
            {
                kprintf(L"Error setting display resolution");
            }

            return;
        }
    }

    kprintf(L"Requested display resolution not available");
}

static void put_pixel(uint32_t x, uint32_t y, uint32_t c)
{
    *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * y + 4 * x)) = c;
}

static rgb_color hsv_to_rgb(hsv_color hsv)
{
    rgb_color rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;

        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

static hsv_color rgb_to_hsv(rgb_color rgb)
{
    hsv_color hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (long)(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

static uint32_t rgb_to_color(rgb_color rgb)
{
    uint32_t color = rgb.r;
    color = (color << 8) + rgb.g;
    color = (color << 8) + rgb.b;

    return color;
}

static void julia()
{
    int32_t w = 1280;
    int32_t h = 720;
    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1, moveX = 0, moveY = 0;
    int32_t maxIterations = 300;

    cRe = -0.7;
    cIm = 0.27015;

    for (int32_t y = 0; y < h; y++)
    {
        for (int32_t x = 0; x < w; x++)
        {
            newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
            newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
            int32_t i;
            for (i = 0; i < maxIterations; i++)
            {
                oldRe = newRe;
                oldIm = newIm;

                newRe = oldRe * oldRe - oldIm * oldIm + cRe;
                newIm = 2 * oldRe * oldIm + cIm;

                if ((newRe * newRe + newIm * newIm) > 4)
                    break;
            }

            hsv_color c;
            c.h = i % 256;
            c.s = 255;
            c.v = 255 * (i < maxIterations);

            put_pixel(x, y, rgb_to_color(hsv_to_rgb(c)));
        }
    }
}

static void clear()
{
    uefi_system_table->ConOut->SetCursorPosition(uefi_system_table->ConOut, 0, 0);

    uint32_t c = 0x3b6ea5;

    for (uint32_t y = 0; y < gop->Mode->Info->VerticalResolution; y++)
    {
        for (uint32_t x = 0; x < gop->Mode->Info->HorizontalResolution; x++)
        {
            put_pixel(x, y, c);
        }
    }
}

void fb_init()
{
    UEFI_GUID guid = UEFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    if (uefi_system_table->BootServices->LocateProtocol(&guid, 0, (void**)&gop))
    {
        kprintf(L"Error initializing UEFI_GRAPHICS_OUTPUT_PROTOCOL\r\n");
        return;
    }

    set_mode(1280, 720);
    clear();
    julia();
}
