#include "app_u.h"

Serial_Value Serial_ValueStruct={0};

static void _main_th_entrance(void *parameter);
static void _serial_rx_th_entrance(void *parameter);
static void _serial_tx_th_entrance(void *parameter);

/*init*/
uint8_t u_init(void)
{
	__enable_irq();
	io_init();
	if(Serial_Init(115200)==_Error)
	{
		return _Error;
	}
	if(spi_w25qxx_init()==_OK)
	{
#if 1
		w25q64_enable();
		w25q64_block_1_init(OTA_FLAG_init);
//		w25q64_block_2_init();
//		w25q64_block_3_init();
#endif
	}
	else
	{
		return _Error;
	}
	if(RT_Sem_Init()==_Error)
	{
		return _Error;
	}
	rt_thread_mdelay(1000);
	return _OK;
}

/*thread init*/
uint8_t rt_create_thread_init(void)
{
	Flag_Struct.Rt_Thread_Init_flag=0;
	/*main thread create*/
	RT_Thread_InitStruct._main_thread=rt_thread_create(_main_th_name,_main_th_entrance,RT_NULL,_main_th_stack_size
					,_main_th_priority,_main_th_stick);
	if(RT_Thread_InitStruct._main_thread)
	{
			rt_thread_startup(RT_Thread_InitStruct._main_thread);
	}
	else
	{
		Flag_Struct.Rt_Thread_Init_flag=flag_Error;
	}
	/*serial rx thread create*/
	RT_Thread_InitStruct._serial_rx_thread=rt_thread_create(_serial_rx_th_name,_serial_rx_th_entrance,RT_NULL,_serial_rx_th_stack_size
					,_serial_rx_th_priority,_serial_rx_th_stick);
	if(RT_Thread_InitStruct._serial_rx_thread)
	{
			rt_thread_startup(RT_Thread_InitStruct._serial_rx_thread);
	}
	else
	{
		Flag_Struct.Rt_Thread_Init_flag=flag_Error;
	}
	/*serial tx thread create*/
	RT_Thread_InitStruct._serial_tx_thread=rt_thread_create(_serial_tx_th_name,_serial_tx_th_entrance,RT_NULL,_serial_tx_th_stack_size
					,_serial_tx_th_priority,_serial_tx_th_stick);
	if(RT_Thread_InitStruct._serial_tx_thread)
	{
			rt_thread_startup(RT_Thread_InitStruct._serial_tx_thread);
	}
	else
	{
		Flag_Struct.Rt_Thread_Init_flag=flag_Error;
	}
	return ((Flag_Struct.Rt_Thread_Init_flag==flag_Error)?_Error:_OK);
}


static void _main_th_entrance(void *parameter)
{
	for(;;)
	{
		
	}
}
static void _serial_rx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_rx_sem,100)==RT_EOK)
		{
			Serial_ValueStruct.serial_1_rx_length=rt_device_read(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_1_rx_buf
							,SERIAL_RX_BUF_LEN);
			rt_sem_release(RT_Thread_InitStruct.Serial_1_tx_sem);
		}
	}
}
#define SERIAL_1_RX_BUF_INIT memset(Serial_ValueStruct.serial_1_rx_buf,0,SERIAL_RX_BUF_LEN)
#define SERIAL_1_TX_BUF_INIT memset(Serial_ValueStruct.serial_1_tx_buf,0,SERIAL_TX_BUF_LEN)
static void _serial_tx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_tx_sem,100)==RT_EOK)
		{
#if 1	
			if(Serial_ValueStruct.serial_1_rx_buf[0] == 0xAB)
			{
				Serial_ValueStruct.serial_1_tx_buf[0]=0x01;
				rt_device_write(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_1_tx_buf
							,Serial_ValueStruct.serial_1_rx_length);
				SERIAL_1_TX_BUF_INIT;
				w25q64_block_1_init(OTA_FLAG_Update_indicate);
			}
			else
			{
				rt_device_write(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_1_rx_buf
							,Serial_ValueStruct.serial_1_rx_length);
			}
			SERIAL_1_RX_BUF_INIT;			
#endif
		}
	}
}
