//
// Created by Xenon on 2023/1/9.
//

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

///C Head files
#include "rtthread.h"
#include "gpio.h"
#include "board.h"
#include "drivers/flash/drv_flash.h"
#include "../RT-Thread/bsp/ST7789/lcd_spi_200.h"
void App_Init();

#ifdef __cplusplus
}
///CPP Head files

#endif

#endif //APP_H
