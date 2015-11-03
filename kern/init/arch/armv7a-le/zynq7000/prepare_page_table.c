#include <config.h>

#define LAKSMI_PT_BASE_ADDRESS 0x100000

void prepage_page_table()
{
    u32 i;

    // try l1 page first, 1M page.
    // TODO, what is NS???
    for (i = 0; i < 4096; i++)
    {
        //  pte format : 31-20(PA), NS, 0, nG, S, AP2, TEX[2-0], AP[1-0], 0, domain, xxx, 10
        u32 pte = (i << 20) | (1 << 16) | (3 << 10) | 2;
    }
}