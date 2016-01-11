//
// Created by Peijie You on 16/1/11.
//

#include <config.h>

#include <sys/types.h>
#include <asm/io.h>
#include <sleep.h>

#include <drivers/sd/sd-zynq7000.h>

#include <kern/mm/pte.h>
#include <settings.h>
//todo for debug
#include <kern/init/init.h>

#ifdef SD_ZYNQ7000

//------my design
static u32 walk_v2p(u32 va, u32* page_table);

// todo check
int sd_dma_spin_load(u32 va, u16 count, u32 offset, u32* page_table)
{
/*------my sd_dma_spin_load, use va instead of pa------*/
    int ret;
    u16 state16;
    u32 state32;
    u32 pa = walk_v2p(va, page_table);
    uart_spin_printf("va: %x\tpa: %d\r\n\0", va, pa);

    /* check card */
    state32 = in32(SD_BASE + SD_PRES_STATE_OFFSET);
    if (!(state32 & SD_PSR_CARD_INSRT)) return -1;
    /* block size set to 512 during controller init, skipping check */
    /* write address */
    out32(SD_BASE + SD_SDMA_SYS_ADDR_OFFSET, pa);
    /* CMD18 with auto_cmd12 */
    ret = sd_spin_send_cmd(SD_CMD18, count, offset, 1);
    if (ret) return -2;
    /* wait for transfer complete */
    do {
        state16 = in16(SD_BASE + SD_NORM_INTR_STS_OFFSET);
        if (state16 & SD_INTR_ERR) {
            out16(SD_BASE + SD_ERR_INTR_STS_OFFSET, \
				SD_ERR_INTR_ALL);
            return -3;
        }
        if (state16 & SD_INTR_DMA) {
            unsigned int next_paddr = in32(SD_BASE + SD_SDMA_SYS_ADDR_OFFSET);

            uart_spin_printf("next: %x\r\n\0", next_paddr);
            if (next_paddr % FRAME_SIZE == 0)
            {
                va += 0x1000;
                next_paddr = walk_v2p(va, page_table);
                uart_spin_printf("----: %x\r\n\0", next_paddr);
            }
            out16(SD_BASE + SD_NORM_INTR_STS_OFFSET, SD_INTR_DMA);
            out32(SD_BASE + SD_SDMA_SYS_ADDR_OFFSET, next_paddr);
        }
    } while (!(state16 & SD_INTR_TC));
    /* clean up */
    out16(SD_BASE + SD_NORM_INTR_STS_OFFSET, SD_INTR_TC);
    return 0;
}

static u32 walk_v2p(u32 va, u32* page_table)
{
//  l2tp here not l2pt
    uart_spin_printf("start\r\n\0");
    u32* l2tp = (u32*)P2V((page_table[va / PAGE_L1SIZE] >> 10) << 10);

    uart_spin_printf("l2tp%x\r\n\0", (u32)l2tp);
    va = va % PAGE_L1SIZE;

    uart_spin_printf("va%x\r\n\0", va);

    uart_spin_printf("2\r\n\0");
    u32 result = l2tp[va / PAGE_L2SIZE] & 0xFFFFF000;

    uart_spin_printf("3\r\n\0");
    return result;
}

#endif /* SD_ZYNQ7000 */
