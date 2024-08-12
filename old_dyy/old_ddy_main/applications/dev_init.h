#ifndef __DEV_INIT_H
#define __DEV_INIT_H
/******************************************************************************/
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
/******************************************************************************/
/**
 @brief Parm
 */
/*dev init struct*/
typedef struct{
	rt_device_t dev_serial_1;
	rt_device_t dev_serial_2;
}Dev_InitTypedef;
/*flag struct*/
typedef struct{
	uint8_t Dev_Init_flag;
	uint8_t Iwdg_Init_flag;
	uint8_t Rt_Thread_Init_flag;
	uint8_t Rt_Sem_Init_flag;		
	uint8_t RT_Timer_flag;
}Flag_Typedef;
/*RT thread struct*/
typedef struct{
	rt_thread_t _main_thread;
	rt_thread_t _serial_rx_thread;
	rt_thread_t _serial_tx_thread;
	rt_thread_t _TimeOut_thread;
	rt_sem_t Serial_1_rx_sem;
	rt_sem_t Serial_1_tx_sem;
	rt_sem_t Serial_2_rx_sem;
	rt_sem_t Serial_2_tx_sem;
	rt_sem_t TimeOut_1_sem;
}RT_Thread_InitTypedef;
/******************************************************************************/
/**
 @brief Function
 */
extern Dev_InitTypedef Dev_InitStruct;
extern Flag_Typedef Flag_Struct;
extern RT_Thread_InitTypedef RT_Thread_InitStruct;
extern IWDG_HandleTypeDef hiwdg;
extern rt_event_t event;
void _IO_init(void);
uint8_t Serial_Init(void);
uint8_t event_init(void);
uint8_t RT_Sem_Init(void);
uint8_t Iwdg_Init(const uint8_t prer,const uint16_t reload);
rt_uint8_t ADC_DMA_INIT(void);
rt_uint8_t Timer_Init(void);
/******************************************************************************/
#endif
