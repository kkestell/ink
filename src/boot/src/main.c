#include "uefi.h"
#include "utils.h"
#include "fb.h"
#include "mm.h"
#include "loader.h"

uint64_t uefi_main(void *image_handle, UEFI_SYSTEM_TABLE *system_table)
{
    uefi_init(image_handle, system_table);
    fb_init();
    mm_init();
    load_kernel();
 
    while (1)
        ;

    return (UEFI_SUCCESS);
}
