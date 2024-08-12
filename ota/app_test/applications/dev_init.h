#ifndef __DEV_INIT_H
#define __DEV_INIT_H

/**
 @brief Include
 */
#include "_macro.h"
#include "main.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_gpio.h>
#include <stdbool.h>
#include <string.h>
#include "drv_spi.h"
/**
 @brief Parm
 */
/*dev init struct*/
typedef struct
{
	rt_device_t dev_serial_1;
	rt_device_t dev_serial_2;
}Dev_InitTypedef;
/*flag struct*/
typedef struct
{
	uint8_t Dev_Init_flag;
	uint8_t Iwdg_Init_flag;
	uint8_t Rt_Thread_Init_flag;
	uint8_t Rt_Sem_Init_flag;		
}Flag_Typedef;
/*RT thread struct*/
typedef struct
{
	rt_thread_t _main_thread;
	rt_thread_t _serial_rx_thread;
	rt_thread_t _serial_tx_thread;
	rt_sem_t Serial_1_rx_sem;
	rt_sem_t Serial_1_tx_sem;
}RT_Thread_InitTypedef;

/**
 @brief Function
 */
extern Dev_InitTypedef Dev_InitStruct;
extern Flag_Typedef Flag_Struct;
extern RT_Thread_InitTypedef RT_Thread_InitStruct;
extern IWDG_HandleTypeDef hiwdg;
extern struct rt_spi_device* spi_dev_w25qxx;

void io_init(void);
uint8_t Serial_Init(const uint32_t BATU_RATE);
uint8_t spi_w25qxx_init(void);
uint8_t RT_Sem_Init(void);
uint8_t Iwdg_Init(const uint8_t prer,const uint16_t reload);
#endif
