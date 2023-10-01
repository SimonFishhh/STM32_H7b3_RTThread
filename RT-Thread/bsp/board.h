//
// Created by Xenon on 2023/1/8.
//

#ifndef BOARD_H
#define BOARD_H

#include <rtthread.h>
#include <stm32h7xx.h>
#include "main.h"
#include "./drivers/serial/drv_serial.h"

#define STM32_FLASH_START_ADRESS     ((uint32_t)0x08000000)
#define STM32_FLASH_SIZE             (16 * 128 * 1024)
#define STM32_FLASH_END_ADDRESS      ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#define STM32_SRAM_START     (0x24000000)//RAM
#define STM32_SRAM_SIZE      (1024)
#define STM32_SRAM_END       (STM32_SRAM_START + STM32_SRAM_SIZE * 1024)

#if defined(__ARMCC_VERSION)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end__;
#define HEAP_BEGIN      ((void *)&__bss_end__)
#endif

#define HEAP_END        STM32_SRAM_END

#endif
