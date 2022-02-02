#include "types.h"
#include "defs.h"

void main(efi_boot_payload_t *boot)
{
    consoleinit();
    kinit1(boot);
    kvmalloc();

    while (1);
}
