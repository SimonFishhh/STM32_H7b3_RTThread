/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include "board.h"

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
#ifdef BSP_SCB_ENABLE_I_CACHE
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef BSP_SCB_ENABLE_D_CACHE
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    extern void SystemClock_Config(void);
//    extern void PeriphCommonClock_Config(void);
    
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
//    PeriphCommonClock_Config();
    /* 
     * 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */
    SysTick_Config(HAL_RCC_GetHCLKFreq()/RT_TICK_PER_SECOND);

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

    /* USART driver initialization is open by default */
#ifdef RT_USING_SERIAL
    rt_hw_usart_init();
#endif

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

    /* Set the shell console output device */
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}

void SysTick_Handler(void)
{
    rt_interrupt_enter();

    rt_tick_increase();

    rt_interrupt_leave();
}

