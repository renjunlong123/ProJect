#include "dev_init.h"
/***************************************************************/
Dev_InitTypedef Dev_InitStruct={0};
Flag_Typedef Flag_Struct={0};
RT_Thread_InitTypedef RT_Thread_InitStruct={0};
IWDG_HandleTypeDef hiwdg;
rt_timer_t timer_1;
rt_err_t Serial_1_rx_CallBack(rt_device_t dev,rt_size_t size);
rt_err_t Serial_2_rx_CallBack(rt_device_t dev,rt_size_t size);
static void Timer_1_CallBack(void *parameter);
/**************************************************************/
/*io init*/
void io_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
	/*程序运行指示灯 PA8*/
	_IO_RUN_STU_L;
	GPIO_InitStruct.Pin = _IO_RUN_STU;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_RUN_STU_PORT,&GPIO_InitStruct);
	/*485 RTC*/
	_IO_485_RTC_H;
	GPIO_InitStruct.Pin = _IO_485_RTC;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_485_RTC_PORT,&GPIO_InitStruct);
	/*AC_LED*/
	GPIO_InitStruct.Pin = _IO_AC_LED;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_AC_LED_PORT,&GPIO_InitStruct);
	/*控制信号输出信号 Kz1--PA12\Kz2--PA15*/
	_IO_KZ_1_L;
	_IO_KZ_2_L; 
	GPIO_InitStruct.Pin = _IO_KZ_1|_IO_KZ_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_AC_LED_PORT,&GPIO_InitStruct);
	/*控制信号输入信号 KZS1--PB10\KZS2--PB1*/
	GPIO_InitStruct.Pin = _IO_KZS_1|_IO_KZS_2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_IO_KZS_PORT,&GPIO_InitStruct);
}

/**************************************************************/
/*serial init*/
uint8_t Serial_Init(const uint32_t BATU_RATE)
{
#ifdef DEV_SERIAL_1_NAME
	struct serial_configure serial_1_config=RT_SERIAL_CONFIG_DEFAULT;
	Dev_InitStruct.dev_serial_1=rt_device_find(DEV_SERIAL_1_NAME);
	if(Dev_InitStruct.dev_serial_1)
	{
		serial_1_config.baud_rate=BATU_RATE;
		serial_1_config.data_bits=DATA_BITS_8;
		serial_1_config.stop_bits=STOP_BITS_1;
		serial_1_config.bufsz=128;
		serial_1_config.flowcontrol=PARITY_NONE;
		rt_device_control(Dev_InitStruct.dev_serial_1,RT_DEVICE_CTRL_CONFIG,&serial_1_config);
		rt_device_open(Dev_InitStruct.dev_serial_1,RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_DMA_RX);
		rt_device_set_rx_indicate(Dev_InitStruct.dev_serial_1,Serial_1_rx_CallBack);
		Flag_Struct.Dev_Init_flag=flag_OK;
	}
	else
	{
		Flag_Struct.Dev_Init_flag=flag_Error;
	}
#endif
#ifdef DEV_SERIAL_2_NAME
	struct serial_configure serial_2_config=RT_SERIAL_CONFIG_DEFAULT;
	Dev_InitStruct.dev_serial_2=rt_device_find(DEV_SERIAL_2_NAME);
	if(Dev_InitStruct.dev_serial_2)
	{
		serial_2_config.baud_rate=BAUD_RATE_4800;
		serial_2_config.data_bits=DATA_BITS_8;
		serial_2_config.stop_bits=STOP_BITS_1;
		serial_2_config.bufsz=128;
		serial_2_config.flowcontrol=PARITY_NONE;
		rt_device_control(Dev_InitStruct.dev_serial_2,RT_DEVICE_CTRL_CONFIG,&serial_2_config);
		rt_device_open(Dev_InitStruct.dev_serial_2,RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_DMA_RX);
		rt_device_set_rx_indicate(Dev_InitStruct.dev_serial_2,Serial_2_rx_CallBack);
		Flag_Struct.Dev_Init_flag=flag_OK;
	}
	else
	{
		Flag_Struct.Dev_Init_flag=flag_Error;
	}
#endif
	return ((Flag_Struct.Dev_Init_flag==flag_Error)?Error:True);
}
/**************************************************************/
/*Iwdg init*/
/*Tout=(预分频值*重装载值)/(40khz) (ms)*/
uint8_t Iwdg_Init(const uint8_t prer,const uint16_t reload)
{
	hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = prer;
  hiwdg.Init.Reload = reload;
	return ((HAL_IWDG_Init(&hiwdg)!=HAL_OK)?Error:True);
}
/**************************************************************/
/**
 @brief Timer init
 */
rt_uint8_t Timer_Init(void)
{
	timer_1 = rt_timer_create("timer1", Timer_1_CallBack,RT_NULL, 1000,RT_TIMER_FLAG_PERIODIC);
	if(timer_1)
	{
		rt_timer_start(timer_1);
	}
	else
	{
		Flag_Struct.RT_Timer_flag=flag_Error;
	}
	return ((Flag_Struct.RT_Timer_flag==flag_Error)?Error:True);
}
/**************************************************************/
/*Sem init*/
uint8_t RT_Sem_Init(void)
{
	Flag_Struct.Rt_Sem_Init_flag=0;
	RT_Thread_InitStruct.Serial_1_rx_sem=rt_sem_create(SERIAL_1_RX_SEM_NAME,0,RT_IPC_FLAG_PRIO);
	if(RT_Thread_InitStruct.Serial_1_rx_sem==RT_NULL)
	{
		Flag_Struct.Rt_Sem_Init_flag=flag_Error;
	}
	
	RT_Thread_InitStruct.Serial_1_tx_sem=rt_sem_create(SERIAL_1_TX_SEM_NAME,0,RT_IPC_FLAG_PRIO);
	if(RT_Thread_InitStruct.Serial_1_tx_sem==RT_NULL)
	{
		Flag_Struct.Rt_Sem_Init_flag=flag_Error;
	}
	
	RT_Thread_InitStruct.Serial_2_rx_sem=rt_sem_create(SERIAL_2_RX_SEM_NAME,0,RT_IPC_FLAG_PRIO);
	if(RT_Thread_InitStruct.Serial_2_rx_sem==RT_NULL)
	{
		Flag_Struct.Rt_Sem_Init_flag=flag_Error;
	}
	
	RT_Thread_InitStruct.Serial_2_tx_sem=rt_sem_create(SERIAL_2_TX_SEM_NAME,0,RT_IPC_FLAG_PRIO);
	if(RT_Thread_InitStruct.Serial_2_tx_sem==RT_NULL)
	{
		Flag_Struct.Rt_Sem_Init_flag=flag_Error;
	}
	
	return ((Flag_Struct.Rt_Sem_Init_flag==flag_Error)?Error:True);
}
/**************************************************************/
/*Callback function*/
rt_err_t Serial_1_rx_CallBack(rt_device_t dev,rt_size_t size)
{
	rt_interrupt_enter();
	rt_sem_release(RT_Thread_InitStruct.Serial_1_rx_sem);
	rt_interrupt_leave();
	return True;
}
rt_err_t Serial_2_rx_CallBack(rt_device_t dev,rt_size_t size)
{
	rt_interrupt_enter();
	rt_sem_release(RT_Thread_InitStruct.Serial_2_rx_sem);
	rt_interrupt_leave();
	return True;
}
extern void uart1_send_data_loading(void);
static void Timer_1_CallBack(void *parameter)
{
	uart1_send_data_loading();
}
/**************************************************************/
