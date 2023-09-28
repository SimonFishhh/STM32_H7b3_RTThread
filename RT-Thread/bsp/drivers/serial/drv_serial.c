//
// Created by Xenon on 2023/4/4.
//

#include "drv_serial.h"

#ifdef RT_USING_CONSOLE

static UART_HandleTypeDef UartHandle;
rt_err_t uart_init(rt_device_t dev)
{
    /* TODO: Please modify the UART port number according to your needs */
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK){
        while (1);
    }
    return 0;
}

rt_err_t uart_open(rt_device_t dev, rt_uint16_t oflag){
    return  uart_init(dev);
}

rt_size_t uart_read(rt_device_t dev,
                    rt_off_t    pos,
                    void       *buffer,
                    rt_size_t   size){
    while (1){
        if (HAL_UART_Receive(&UartHandle, buffer, size, 20) == 0){
            return size;
        }
    }
}

rt_size_t uart_write(rt_device_t dev,
                     rt_off_t    pos,
                     const void *buffer,
                     rt_size_t   size){
    rt_size_t i = 0;
    char a = '\r';
    const char *val = 0;

    val = (const char *)(buffer);

    __HAL_UNLOCK(&UartHandle);

    for (i = 0; i < size; i++){
        if (*(val + i) == '\n'){
            HAL_UART_Transmit(&UartHandle, (uint8_t *)&a, 1, 20);
        }
        HAL_UART_Transmit(&UartHandle, (uint8_t *)(val + i), 1, 20);
    }
    return 1;
}

struct rt_device uart_dev;
int rt_hw_usart_init(void){
    uart_dev.init  = uart_init;
    uart_dev.open  = uart_open;
    uart_dev.read  = uart_read;
    uart_dev.write = uart_write;
    uart_dev.user_data = RT_NULL;
    rt_device_register(&uart_dev, "uart1", 0);
    return 0;
}

#endif