#include "u_init.h"

/**
 @brief parm
 */
rt_device_t Serial_1,Serial_2_485,drv_iwdg;
rt_sem_t Serial_1_rx_sem,Serial_1_tx_sem,Serial_2_485_rx_sem,Serial_2_485_tx_sem,TimeOut_1_sem;
rt_event_t event;
rt_timer_t timer_1;
flag _init_flag;
IWDG_HandleTypeDef hiwdg;
ADC_HandleTypeDef hadc1;

/**
 @brief function
 */
static rt_err_t Serial_1_Rx_Dma_CallBack(rt_device_t dev,rt_size_t size);
static rt_err_t Serial_2_Rx_Dma_CallBack(rt_device_t dev,rt_size_t size);
static void Timer_1_CallBack(void *parameter);

//inline uint8_t _init_flag_judgement(uint8_t init_flag)

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
	
		/*485控制引脚 PA5*/
		_IO_485_RTC_H;

		GPIO_InitStruct.Pin = _IO_485_RTC;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(_IO_485_RTC_PORT,&GPIO_InitStruct);
		
	 /*供电状态  PB10*/
    GPIO_InitStruct.Pin = _IO_POWER_STU;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_IO_POWER_STU_PORT,&GPIO_InitStruct);
		
		/*电源控制脚   PC13*/
    _IO_DYKZ_L_AC;

    GPIO_InitStruct.Pin = _IO_DYKZ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_IO_DYKZ_PORT, &GPIO_InitStruct);
		
		/*充电控制脚  PA1*/
		
    GPIO_InitStruct.Pin = _IO_CDKZ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_IO_CDKZ_PORT, &GPIO_InitStruct);
		
		/*电池放电控制脚  PB11*/

    GPIO_InitStruct.Pin = _IO_DCFDKZ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_IO_DCFDKZ_PORT, &GPIO_InitStruct);
		
	 /*电池状态采集脚  PA15 PB3 PB4 PB5 PB6*/
    GPIO_InitStruct.Pin = _IO_BAT_STU_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_IO_BAT_STU_PORT_1, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = _IO_BAT_STU_2|_IO_BAT_STU_3|_IO_BAT_STU_4|_IO_BAT_STU_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_IO_BAT_STU_PORT_2_5, &GPIO_InitStruct);
}

/**
 @brief Serial init
 */
rt_uint8_t Serial_Init(void)
{
	_init_flag.serial_init_flag=0;
#define SERIAL_2_DEV "uart2"
#ifdef  SERIAL_2_DEV
	const char serial_2_dev_name[]=SERIAL_2_DEV;
	struct serial_configure serial_2_config=RT_SERIAL_CONFIG_DEFAULT;
	Serial_2_485=rt_device_find(serial_2_dev_name);
	if(Serial_2_485)
	{
		serial_2_config.baud_rate=BAUD_RATE_4800;
		serial_2_config.data_bits=DATA_BITS_8;
		serial_2_config.stop_bits=STOP_BITS_1;
		serial_2_config.bufsz=128;
		serial_2_config.flowcontrol=PARITY_NONE;
		
		rt_device_control(Serial_2_485, RT_DEVICE_CTRL_CONFIG,&serial_2_config);
		rt_device_open(Serial_2_485,RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_DMA_RX);
		rt_device_set_rx_indicate(Serial_2_485,Serial_2_Rx_Dma_CallBack);
	}
	else
	{
		_init_flag.serial_init_flag=1;
	}
#endif
	
#define SERIAL_1_DEV "uart1"
#ifdef  SERIAL_1_DEV
	const char serial_1_dev_name[]=SERIAL_1_DEV;
	struct serial_configure serial_1_config=RT_SERIAL_CONFIG_DEFAULT;
	Serial_1=rt_device_find(serial_1_dev_name);
	if(Serial_1)
	{
		serial_1_config.baud_rate=BAUD_RATE_4800;
		serial_1_config.data_bits=DATA_BITS_8;
		serial_1_config.stop_bits=STOP_BITS_1;
		serial_1_config.bufsz=128;
		serial_1_config.flowcontrol=PARITY_NONE;
		
		rt_device_control(Serial_1, RT_DEVICE_CTRL_CONFIG,&serial_1_config);
		rt_device_open(Serial_1,RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_DMA_RX);
		rt_device_set_rx_indicate(Serial_1,Serial_1_Rx_Dma_CallBack);
	}
	else
	{
		_init_flag.serial_init_flag=1;
	}
#endif
	return ((_init_flag.serial_init_flag==1)?Error:True);
}

/**
 @brief adc init
 */
static rt_uint8_t Adc_init(void)
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
	/*电池温度3*/
	sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	/*电池电压*/
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
		return Error;
  }
	return True;
}

/**
 @brief dma init
 */
static void Dma_init(void)
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
rt_uint8_t Adc_Dma_Init(void)
{
	Dma_init();
	if(Adc_init()==True)
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

/**
 @brief iwdg init
 */
rt_uint8_t Iwdg_Init(void)
{
	hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = 4095;
	return ((HAL_IWDG_Init(&hiwdg)!=HAL_OK)?Error:True);
}

/**
 @brief Sem init
 */
rt_uint8_t Sem_Init(void)
{
#define SERIAL_1_RX_SEM "serial_1_rx_sem"
	const char serial_1_rx_sem_name[]=SERIAL_1_RX_SEM;
	Serial_1_rx_sem=rt_sem_create(serial_1_rx_sem_name,0,RT_IPC_FLAG_PRIO);
	if(Serial_1_rx_sem==RT_NULL)
	{
		_init_flag.sem_init_flag=1;
	}
#define SERIAL_1_TX_SEM "serial_1_tx_sem"
	const char serial_1_tx_sem_name[]=SERIAL_1_TX_SEM;
	Serial_1_tx_sem=rt_sem_create(serial_1_tx_sem_name,0,RT_IPC_FLAG_PRIO);
	if(Serial_1_tx_sem==RT_NULL)
	{
		_init_flag.sem_init_flag=1;
	}
#define SERIAL_2_RX_SEM "serial_2_485_rx_sem"
	const char serial_2_485_rx_sem_name[]=SERIAL_2_RX_SEM;
	Serial_2_485_rx_sem=rt_sem_create(serial_2_485_rx_sem_name,0,RT_IPC_FLAG_PRIO);
	if(Serial_2_485_rx_sem==RT_NULL)
	{
		_init_flag.sem_init_flag=1;
	}
#define SERIAL_2_TX_SEM "serial_2_485_tx_sem"
	const char serial_2_485_tx_sem_name[]=SERIAL_2_TX_SEM;
	Serial_2_485_tx_sem=rt_sem_create(serial_2_485_tx_sem_name,0,RT_IPC_FLAG_PRIO);
	if(Serial_2_485_tx_sem==RT_NULL)
	{
		_init_flag.sem_init_flag=1;
	}
#define TIMEOUT_1_sem "timeout_1_sem"
	const char TIMEOUT_1_sem_name[]=TIMEOUT_1_sem;
	TimeOut_1_sem=rt_sem_create(TIMEOUT_1_sem_name,0,RT_IPC_FLAG_PRIO);
	if(TimeOut_1_sem==RT_NULL)
	{
		_init_flag.sem_init_flag=1;
	}
	return ((_init_flag.sem_init_flag==1)?Error:True);
}

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
		_init_flag.timer_init_flag=1;
	}
	return ((_init_flag.timer_init_flag==1)?Error:True);
}
/**
 @brief event init
 */
rt_uint8_t event_init(void)
{
	 event=rt_event_create("event", RT_IPC_FLAG_FIFO);
	 return ((event==RT_NULL)?Error:True);
}
/**
 @brief iwdg init
 */

/******************************************************/
static rt_err_t Serial_1_Rx_Dma_CallBack(rt_device_t dev,rt_size_t size)
{
	rt_interrupt_enter();
	rt_sem_release(Serial_1_rx_sem);
	rt_interrupt_leave();
	return RT_EOK;
}

static rt_err_t Serial_2_Rx_Dma_CallBack(rt_device_t dev,rt_size_t size)
{
	rt_interrupt_enter();
	rt_sem_release(Serial_2_485_rx_sem);
	rt_interrupt_leave();
	return RT_EOK;
}

static void Timer_1_CallBack(void *parameter)
{
	rt_interrupt_enter();
	rt_sem_release(TimeOut_1_sem);
	rt_interrupt_leave();
}
