#ifndef __U_INIT_H
#define __U_INIT_H

/**
 @brief include
 */
#include "u_macro.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "main.h"
#include <drv_gpio.h>
#include <stdbool.h>
#include <string.h>

/**
 @brief parm
 */
extern rt_device_t Serial_1,Serial_2_485,drv_iwdg;
extern rt_sem_t Serial_1_rx_sem,Serial_1_tx_sem,Serial_2_485_rx_sem,Serial_2_485_tx_sem,TimeOut_1_sem;
extern rt_event_t event;

typedef struct
{
	bool serial_init_flag;
	bool sem_init_flag;
	bool thread_init_flag;
	bool timer_init_flag;
	bool iwdg_init_flag;
}flag;

extern flag _init_flag;

/**
 @brief function
 */
void _IO_init(void);
rt_uint8_t Serial_Init(void);
rt_uint8_t Adc_Dma_Init(void);
rt_uint8_t Iwdg_Init(void);
rt_uint8_t Sem_Init(void);
rt_uint8_t event_init(void);
rt_uint8_t Timer_Init(void);
#endif
