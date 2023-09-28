//
// Created by Xenon on 2023/3/29.
//

#include "App.h"

void Task_Blink(void *pVoid){
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        rt_thread_mdelay(1000);
    }
}


void App_Init(){

    static rt_thread_t Blink_thread = RT_NULL;
    static rt_thread_t Lvgl_thread = RT_NULL;

    Blink_thread = rt_thread_create( "Task_Blink",
                                    Task_Blink,
                                    RT_NULL,
                                    1024,
                                    2,
                                    5);
    if (Blink_thread != RT_NULL) rt_thread_startup(Blink_thread);


}