/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-07     NU-LL        first version
 */
#include <rtthread.h>
#include <board.h>
#include <rthw.h>
#include <finsh.h>

#ifdef BSP_USING_QSPI_FLASH

#include <fal.h>
#include "drv_ospi_flash.h"

//#define DRV_DEBUG
#define LOG_TAG             "drv.qspiflash"
#include <./drivers/drv_log.h>

#define FS_PARTITION_NAME               "fs_qspi"
#define FAL_USING_NOR_FLASH_DEV_NAME    "W25Q64JV"

static int init(void);
static int read(long offset, uint8_t *buf, size_t size);
static int write(long offset, const uint8_t *buf, size_t size);
static int erase(long offset, size_t size);

struct fal_flash_dev nor_flash1 =
{
    .name       = FAL_USING_NOR_FLASH_DEV_NAME,
    .addr       = 0,
    .len        = 8 * 1024 * 1024,
    .blk_size   = 4096,
    .ops        = {init, read, write, erase},
    .write_gran = 4
};

static int init(void)
{
    if( OSPI_W25Qxx_Init() != OSPI_W25Qxx_OK){
        return  -1;
    }
    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    if (OSPI_W25Qxx_ReadBuffer(buf, nor_flash1.addr + offset, size) != OSPI_W25Qxx_OK){
        return -1;
    }

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    if (OSPI_W25Qxx_WriteBuffer(buf, nor_flash1.addr + offset, size) != OSPI_W25Qxx_OK)
    {
        return -1;
    }

    return size;
}

static int erase(long offset, size_t size)
{
    size_t cur_erase_size = nor_flash1.blk_size;;
    uint32_t addr =nor_flash1.addr + offset;

    if (addr + size > nor_flash1.len) {
        LOG_E("Error: Flash address is out of bound.");
        return -1;
    }
    if (addr == 0 && size == nor_flash1.len) {
        return OSPI_W25Qxx_ChipErase ();
    }

    while (size) {

        if (OSPI_W25Qxx_SectorErase(addr) != OSPI_W25Qxx_OK) {
            LOG_E("Error: Flash erase SPI communicate error.");
            goto __exit;
        }

        /* make erase align and calculate next erase address */
        if (addr % cur_erase_size != 0) {
            if (size > cur_erase_size - (addr % cur_erase_size)) {
                size -= cur_erase_size - (addr % cur_erase_size);
                addr += cur_erase_size - (addr % cur_erase_size);
            } else {
                goto __exit;
            }
        } else {
            if (size > cur_erase_size) {
                size -= cur_erase_size;
                addr += cur_erase_size;
            } else {
                goto __exit;
            }
        }
    }
    __exit:
    return size;

}

#endif/* BSP_USING_QSPI_FLASH */
