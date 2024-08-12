#include "dev_init.h"

Dev_InitTypedef Dev_InitStruct={0};
Flag_Typedef Flag_Struct={0};
RT_Thread_InitTypedef RT_Thread_InitStruct={0};
IWDG_HandleTypeDef hiwdg;
struct rt_spi_device* spi_dev_w25qxx;
rt_err_t Serial_1_rx_CallBack(rt_device_t dev,rt_size_t size);

/*IO init*/
void io_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  SPI_1_CS_IO_H; 

  GPIO_InitStruct.Pin = SPI_1_CS_IO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI_1_CS_IO_PORT, &GPIO_InitStruct);
}

/*w25qxx init*/
uint8_t spi_w25qxx_init(void)
{
	struct rt_spi_configuration spi_w25qxx_cfg;
	rt_err_t result=rt_hw_spi_device_attach("spi1", "spi_w25qxx" ,rt_pin_get("PA.4"));
	if(result!=RT_EOK)
	{
		return _Error;
	}
	spi_dev_w25qxx=(struct rt_spi_device*)rt_device_find("spi_w25qxx");
	if(spi_dev_w25qxx==NULL)
	{
		return _Error;
	}
	spi_w25qxx_cfg.data_width=8;
	spi_w25qxx_cfg.mode=RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
	spi_w25qxx_cfg.max_hz=5*1000*1000;
	result=rt_spi_configure(spi_dev_w25qxx, &spi_w25qxx_cfg);	
	return ((result!=RT_EOK)?_Error:_OK);
}

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
	return ((Flag_Struct.Dev_Init_flag==flag_Error)?_Error:_OK);
}

/*Iwdg init*/
/*Tout=(预分频值*重装载值)/(40khz) (ms)*/
uint8_t Iwdg_Init(const uint8_t prer,const uint16_t reload)
{
	hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = prer;
  hiwdg.Init.Reload = reload;
	return ((HAL_IWDG_Init(&hiwdg)!=HAL_OK)?_Error:_OK);
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
	return ((Flag_Struct.Rt_Sem_Init_flag==flag_Error)?_Error:_OK);
}

/*Callback function*/
rt_err_t Serial_1_rx_CallBack(rt_device_t dev,rt_size_t size)
{
	rt_interrupt_enter();
	rt_sem_release(RT_Thread_InitStruct.Serial_1_rx_sem);
	rt_interrupt_leave();
	return _OK;
}
