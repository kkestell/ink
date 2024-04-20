#include "kprintf.h"
#include "efi.h"

void *efi_image_handle;
EFI_SYSTEM_TABLE *efi_system_table;

#define STATUS_CASE(symbol) \
    case symbol:            \
        return L#symbol;    \
        break;

void puts(uint16_t c)
{
    uint16_t chars[2] = { c, '\0' };
    efi_system_table->ConOut->OutputString(efi_system_table->ConOut, (uint16_t *)chars);
}

const wchar_t *efi_error_message(EFI_STATUS const status)
{
    switch (status)
    {
        STATUS_CASE(EFI_SUCCESS)
        STATUS_CASE(EFI_LOAD_ERROR)
        STATUS_CASE(EFI_INVALID_PARAMETER)
        STATUS_CASE(EFI_UNSUPPORTED)
        STATUS_CASE(EFI_BAD_BUFFER_SIZE)
        STATUS_CASE(EFI_BUFFER_TOO_SMALL)
        STATUS_CASE(EFI_NOT_READY)
        STATUS_CASE(EFI_DEVICE_ERROR)
        STATUS_CASE(EFI_WRITE_PROTECTED)
        STATUS_CASE(EFI_OUT_OF_RESOURCES)
        STATUS_CASE(EFI_VOLUME_CORRUPTED)
        STATUS_CASE(EFI_VOLUME_FULL)
        STATUS_CASE(EFI_NO_MEDIA)
        STATUS_CASE(EFI_MEDIA_CHANGED)
        STATUS_CASE(EFI_NOT_FOUND)
        STATUS_CASE(EFI_ACCESS_DENIED)
        STATUS_CASE(EFI_NO_RESPONSE)
        STATUS_CASE(EFI_NO_MAPPING)
        STATUS_CASE(EFI_TIMEOUT)
        STATUS_CASE(EFI_NOT_STARTED)
        STATUS_CASE(EFI_ALREADY_STARTED)
        STATUS_CASE(EFI_ABORTED)
        STATUS_CASE(EFI_ICMP_ERROR)
        STATUS_CASE(EFI_TFTP_ERROR)
        STATUS_CASE(EFI_PROTOCOL_ERROR)
        STATUS_CASE(EFI_INCOMPATIBLE_VERSION)
        STATUS_CASE(EFI_SECURITY_VIOLATION)
        STATUS_CASE(EFI_CRC_ERROR)
        STATUS_CASE(EFI_END_OF_MEDIA)
        STATUS_CASE(EFI_END_OF_FILE)
    default:
        return L"EFI_UNKNOWN_STATUS";
    }
}

void efi_init(void *image_handle, EFI_SYSTEM_TABLE *system_table)
{
    efi_image_handle = image_handle;
    efi_system_table = system_table;

    kprintf_init(puts);
}
