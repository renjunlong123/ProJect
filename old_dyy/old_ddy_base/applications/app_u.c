#include "app_u.h"
/******************************************************************************/
/*更新日志
2024.8.10 解决底板485与分站通信问题
2024.8.11 解决底板数据影响分站数据采集，导致上位机显示报错的bug
*/
/******************************************************************************/
Serial_Value Serial_ValueStruct={0};
static void idle_hook(void);
static void _main_th_entrance(void *parameter);
static void _serial_rx_th_entrance(void *parameter);
static void _serial_tx_th_entrance(void *parameter);
/******************************************************************************/
/*init*/
uint8_t u_init(void)
{
	_Oled_Init();
	oled_SD();
	io_init();
	if(Serial_Init(115200)==Error){
		return Error;
	}
	if(Timer_Init()==Error){
		return Error;
	}
  if(Iwdg_Init(IWDG_PRESCALER_256,4095)==Error)
	{
		return Error;
	}
	if(RT_Sem_Init()==Error){
		return Error;
	}
	rt_thread_mdelay(1000);
	DHT11_Init();
	return True;
}
/******************************************************************************/
/*thread init*/
uint8_t rt_create_thread_init(void)
{
	Flag_Struct.Rt_Thread_Init_flag=0;
	/*idle thread create*/
	if(Flag_Struct.Iwdg_Init_flag==flag_OK)
	{
		rt_thread_idle_sethook(idle_hook);
	}
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
	return ((Flag_Struct.Rt_Thread_Init_flag==flag_Error)?Error:True);
}
/******************************************************************************/
/*thread entrance*/
static void idle_hook(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
static void _main_th_entrance(void *parameter)
{
	_IO_485_RTC_L;
	for(;;)
	{
		oled_show_Bat_parm();
		HAL_IWDG_Refresh(&hiwdg);
		if(_IO_R_KZS_1|_IO_R_KZS_2)
		{
			_IO_KZ_1_H;
			_IO_KZ_2_H;
		}
		else
		{
			_IO_KZ_1_L;
			_IO_KZ_2_L;
		}
	}
}
/*serial_rx entrance*/
static void _serial_rx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_rx_sem,10)==RT_EOK)
		{
			Serial_ValueStruct.serial_1_rx_length=rt_device_read(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_1_rx_buf
							,serial_1_rx_buf_size);
			rt_sem_release(RT_Thread_InitStruct.Serial_1_tx_sem);
		}
		if(rt_sem_take(RT_Thread_InitStruct.Serial_2_rx_sem,10)==RT_EOK)
		{
			Serial_ValueStruct.serial_2_rx_length=rt_device_read(Dev_InitStruct.dev_serial_2,0,Serial_ValueStruct.serial_2_rx_buf
							,serial_2_rx_buf_size);
			if(Proccess_Serial_2_RxData(Serial_ValueStruct.serial_2_rx_buf)==True)
			{
				rt_sem_release(RT_Thread_InitStruct.Serial_2_tx_sem);	
				_IO_485_RTC_H;
			}			
		}
	}
}
/*serial_tx entrance*/
static void _serial_tx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_tx_sem,10)==RT_EOK)
		{
			Proccess_Serial_1_RxData(Serial_ValueStruct.serial_1_rx_buf);
			DHT11_Read_Data();
			memset(Serial_ValueStruct.serial_1_rx_buf,0,Serial_ValueStruct.serial_1_rx_length);
		}
		if(rt_sem_take(RT_Thread_InitStruct.Serial_2_tx_sem,10)==RT_EOK)
		{
			rt_enter_critical();
			rt_device_write(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_2_rx_buf
							,Serial_ValueStruct.serial_2_rx_length);
			memset(Serial_ValueStruct.serial_2_rx_buf,0,Serial_ValueStruct.serial_2_rx_length);
			Serial_ValueStruct.serial_2_tx_length=Serial_2_485_Send_Data(Serial_ValueStruct.serial_2_tx_buf);
			rt_device_write(Dev_InitStruct.dev_serial_2,0,Serial_ValueStruct.serial_2_tx_buf
							,Serial_ValueStruct.serial_2_tx_length);
			memset(Serial_ValueStruct.serial_2_tx_buf,0,Serial_ValueStruct.serial_2_tx_length);
			rt_exit_critical();
			_IO_485_RTC_L;	
		}		
	}
}
/******************************************************************************/
