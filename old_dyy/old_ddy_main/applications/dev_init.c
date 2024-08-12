#include "dev_init.h"
/******************************************************************************/
Dev_InitTypedef Dev_InitStruct={0};
Flag_Typedef Flag_Struct={0};
RT_Thread_InitTypedef RT_Thread_InitStruct={0};
IWDG_HandleTypeDef hiwdg;
ADC_HandleTypeDef hadc1;
rt_timer_t timer_1;
rt_event_t event;
rt_err_t Serial_1_rx_CallBack(rt_device_t dev,rt_size_t size);
rt_err_t Serial_2_rx_CallBack(rt_device_t dev,rt_size_t size);
static void Timer_1_CallBack(void *parameter);
/******************************************************************************/
/**
 @brief IO init
 */
void _IO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
	/*程序运行指示灯 PB12*/
	_IO_RUN_STU_L;
	GPIO_InitStruct.Pin = _IO_RUN_STU;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_RUN_STU_PORT,&GPIO_InitStruct);
	/*充电指示灯 PB13*/
	GPIO_InitStruct.Pin = _IO_CD_LED;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_CD_LED_PORT,&GPIO_InitStruct);
	/*供电状态  PB10*/
	GPIO_InitStruct.Pin = _IO_POWER_STU;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_IO_POWER_STU_PORT,&GPIO_InitStruct);
	/*充电控制脚  PA1*/
	GPIO_InitStruct.Pin = _IO_CDKZ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_CDKZ_PORT, &GPIO_InitStruct);
	/*放电控制脚  PB11*/
	GPIO_InitStruct.Pin = _IO_FDKZ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_FDKZ_PORT, &GPIO_InitStruct);
	/*电源控制脚  PC13*/
	_IO_DYKZ_L_AC;
	GPIO_InitStruct.Pin = _IO_DYKZ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_IO_DYKZ_PORT, &GPIO_InitStruct);
	/*电池状态采集脚 PA15、PB3-PB7*/
	GPIO_InitStruct.Pin = _IO_BAT_STU_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = _IO_BAT_STU_2|_IO_BAT_STU_3|_IO_BAT_STU_4|_IO_BAT_STU_5|_IO_BAT_STU_6;;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(_IO_BAT_STU_PORT_1,&GPIO_InitStruct);
	HAL_GPIO_Init(_IO_BAT_STU_PORT_2_6,&GPIO_InitStruct);
}
/******************************************************************************/
/*serial init*/
uint8_t Serial_Init(void)
{
#ifdef DEV_SERIAL_1_NAME
	struct serial_configure serial_1_config=RT_SERIAL_CONFIG_DEFAULT;
	Dev_InitStruct.dev_serial_1=rt_device_find(DEV_SERIAL_1_NAME);
	if(Dev_InitStruct.dev_serial_1)
	{
		serial_1_config.baud_rate=BAUD_RATE_115200;
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
		serial_2_config.baud_rate=BAUD_RATE_115200;
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
/******************************************************************************/
/*Iwdg init*/
/*Tout=(预分频值*重装载值)/(40khz) (ms)*/
uint8_t Iwdg_Init(const uint8_t prer,const uint16_t reload)
{
	hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = prer;
  hiwdg.Init.Reload = reload;
	return ((HAL_IWDG_Init(&hiwdg)!=HAL_OK)?Error:True);
}
/******************************************************************************/
/**
 @brief event init
 */
uint8_t event_init(void)
{
	 event=rt_event_create("event", RT_IPC_FLAG_FIFO);
	 return ((event==RT_NULL)?Error:True);
}
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
	RT_Thread_InitStruct.TimeOut_1_sem=rt_sem_create(TIMEOUT_1_sem,0,RT_IPC_FLAG_PRIO);
	if(RT_Thread_InitStruct.TimeOut_1_sem==RT_NULL)
	{
		Flag_Struct.Rt_Sem_Init_flag=flag_Error;
	}
	return ((Flag_Struct.Rt_Sem_Init_flag==flag_Error)?Error:True);
}
/******************************************************************************/
/**
 @brief adc init
 */
static rt_uint8_t ADC_INIT(void)
{
	/**ADC1 GPIO Configuration
	PA4     ------> ADC1_IN4
	PA6     ------> ADC1_IN6
	PA7     ------> ADC1_IN7
	PB0     ------> ADC1_IN8
	PB1     ------> ADC1_IN9
	*/
  ADC_ChannelConfTypeDef sConfig = {0};
	
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
		return Error;
  }
	/*充放电电流*/
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	/*电池温度1*/
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	/*电池温度2*/
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	/*电池电压*/
	sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	/*电池电流*/
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	return True;
}
/******************************************************************************/
/**
 @brief dma init
 */
static void DMA_INIT(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
/**
 @brief open adc
 */
rt_uint8_t ADC_DMA_INIT(void)
{
	DMA_INIT();
	if(ADC_INIT()==True)
	{
		HAL_ADCEx_Calibration_Start(&hadc1);
		HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
	}
	else
	{
		return Error;
	}
	return True;
}
/******************************************************************************/
/**
 @brief Timer init
 */
rt_uint8_t Timer_Init(void)
{
#define TIMER_1 "timer1"
#define TIMER_1_TIMEOUT  1000
	const char Timer_1_name[]=TIMER_1;
	timer_1=rt_timer_create(Timer_1_name,Timer_1_CallBack,RT_NULL,TIMER_1_TIMEOUT
													,RT_TIMER_FLAG_PERIODIC);
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
/******************************************************************************/
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
static void Timer_1_CallBack(void *parameter)
{
	rt_interrupt_enter();
	rt_sem_release(RT_Thread_InitStruct.TimeOut_1_sem);
	rt_interrupt_leave();
}
/******************************************************************************/
