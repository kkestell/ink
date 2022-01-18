#include "uefi.h"
#include "printf.h"

#define STATUS_CASE(symbol) \
    case symbol: \
        return L#symbol; \
        break;

void* image_handle;

void puts(uint16_t c)
{
    uint16_t chars[2] = { c, '\0' };
    uefi_system_table->ConOut->OutputString(uefi_system_table->ConOut, (uint16_t*)chars);
}

const wchar_t* uefi_error_message(UEFI_STATUS const status)
{
    switch (status)
    {
        STATUS_CASE(UEFI_SUCCESS)
        STATUS_CASE(UEFI_LOAD_ERROR)
        STATUS_CASE(UEFI_INVALID_PARAMETER)
        STATUS_CASE(UEFI_UNSUPPORTED)
        STATUS_CASE(UEFI_BAD_BUFFER_SIZE)
        STATUS_CASE(UEFI_BUFFER_TOO_SMALL)
        STATUS_CASE(UEFI_NOT_READY)
        STATUS_CASE(UEFI_DEVICE_ERROR)
        STATUS_CASE(UEFI_WRITE_PROTECTED)
        STATUS_CASE(UEFI_OUT_OF_RESOURCES)
        STATUS_CASE(UEFI_VOLUME_CORRUPTED)
        STATUS_CASE(UEFI_VOLUME_FULL)
        STATUS_CASE(UEFI_NO_MEDIA)
        STATUS_CASE(UEFI_MEDIA_CHANGED)
        STATUS_CASE(UEFI_NOT_FOUND)
        STATUS_CASE(UEFI_ACCESS_DENIED)
        STATUS_CASE(UEFI_NO_RESPONSE)
        STATUS_CASE(UEFI_NO_MAPPING)
        STATUS_CASE(UEFI_TIMEOUT)
        STATUS_CASE(UEFI_NOT_STARTED)
        STATUS_CASE(UEFI_ALREADY_STARTED)
        STATUS_CASE(UEFI_ABORTED)
        STATUS_CASE(UEFI_ICMP_ERROR)
        STATUS_CASE(UEFI_TFTP_ERROR)
        STATUS_CASE(UEFI_PROTOCOL_ERROR)
        STATUS_CASE(UEFI_INCOMPATIBLE_VERSION)
        STATUS_CASE(UEFI_SECURITY_VIOLATION)
        STATUS_CASE(UEFI_CRC_ERROR)
        STATUS_CASE(UEFI_END_OF_MEDIA)
        STATUS_CASE(UEFI_END_OF_FILE)
    default:
        return L"UEFI_UNKNOWN_STATUS";
    }
}

void uefi_init(void *ih, UEFI_SYSTEM_TABLE *st)
{
    image_handle = ih;
    uefi_system_table = st;

    kprintf_init(puts);
}
