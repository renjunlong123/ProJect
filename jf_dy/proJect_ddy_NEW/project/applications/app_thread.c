#include "app_thread.h"

/**
 @brief parm
 */
serial serial_val;
rt_thread_t _main_thread,_serial_rx_thread,_serial_tx_thread,_TimeOut_thread;
extern IWDG_HandleTypeDef hiwdg;
uint8_t init_flag=0;
uint8_t at24c02_JC_flag=0;
/**
 @brief function
 */
static void idle_hook(void);
static void _main_th_entrance(void *parameter);
static void _TimeOut_th_entrance(void *parameter);
static void _serial_rx_th_entrance(void *parameter);
static void _serial_tx_th_entrance(void *parameter);

/**
 @brief init
 */
rt_uint8_t init(void)
{
	_IO_init();
	if(Serial_Init())
	{
		return Error;
	}
	if(Sem_Init())
	{
		return Error;
	}
	if(event_init())
	{
		return Error;
	}
	if(Timer_Init())
	{
		return Error;
	}
#if 1
	if(Iwdg_Init())
	{
		return Error;
	}
	else
	{
		_init_flag.iwdg_init_flag=1;
	}
#endif
	if(Adc_Dma_Init()==True)
	{
		Open_Adc();
	}
	AT24C02_INIT();
	SD_READ_DL_M24c02();
	rt_thread_mdelay(1000);
	return True;
}
/**
 @brief thread init
 */
rt_uint8_t thread_init(void)
{
	/*idle create*/
	if(_init_flag.iwdg_init_flag==1)
	{
		_init_flag.iwdg_init_flag=0;
		rt_thread_idle_sethook(idle_hook);
	}
	else
	{
		_init_flag.thread_init_flag=1;
	}
	/*main thread create*/
	_main_thread=rt_thread_create(_main_th_name,_main_th_entrance,RT_NULL,_main_th_stack_size
					,_main_th_priority,_main_th_stick);
	if(_main_thread)
	{
			rt_thread_startup(_main_thread);
	}
	else
	{
		_init_flag.thread_init_flag=1;
	}
	/*serial 2 rx thread create*/
	_serial_rx_thread=rt_thread_create(_serial_rx_th_name,_serial_rx_th_entrance,RT_NULL,_serial_rx_th_stack_size
					,_serial_rx_th_priority,_serial_rx_th_stick);
	if(_serial_rx_thread)
	{
			rt_thread_startup(_serial_rx_thread);
	}
	else
	{
		_init_flag.thread_init_flag=1;
	}
	/*serial 2 tx thread create*/
	_serial_tx_thread=rt_thread_create(_serial_tx_th_name,_serial_tx_th_entrance,RT_NULL,_serial_tx_th_stack_size
					,_serial_tx_th_priority,_serial_tx_th_stick);
	if(_serial_tx_thread)
	{
			rt_thread_startup(_serial_tx_thread);
	}
	else
	{
		_init_flag.thread_init_flag=1;
	}
	/*TimeOut thread create*/
  _TimeOut_thread=rt_thread_create(_TimeOut_th_name,_TimeOut_th_entrance,RT_NULL,_TimeOut_th_stack_size
					,_TimeOut_th_priority,_TimeOut_th_stick);
	if(_TimeOut_thread)
	{
			rt_thread_startup(_TimeOut_thread);
	}
	else
	{
		_init_flag.thread_init_flag=1;
	}
	return ((_init_flag.thread_init_flag==1)?Error:True);
}
/**
 @brief error
 */
void _error(void)
{
	static bool flag=1;
	if(flag)
	{
		flag=0;
		for(rt_uint8_t i=0;i<serial_2_485_tx_error_size;i++)
		{
			serial_val.serial_2_485_tx_error_buf[i]=0xff;
		}
	}
	rt_device_write(Serial_2_485,0,serial_val.serial_2_485_tx_error_buf,serial_2_485_tx_error_size);
}

/******************************************************/
/*idle entrance*/
static void idle_hook(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}

/*main thread entrance*/
static void _main_th_entrance(void *parameter)
{
	_IO_485_RTC_L;
	_IO_CDKZ_L;
	_IO_DCFDKZ_L;
#if 0
	uint8_t buf[2]={0};
#endif
	for(;;)
	{
#if 0
		_IO_485_RTC_H;
		rt_thread_mdelay(10);
		rt_device_write(Serial_2_485,0,buf,2);
		rt_device_write(Serial_1,0,buf,2);
#endif
		_IO_RUN_STU_H;
		rt_thread_mdelay(500);
		_IO_RUN_STU_L;
		rt_thread_mdelay(500);
	}
}

/*TimeOut thread entrance*/
static void event_handler(void)
{
	/*事件2 发送完成*/
	if(rt_event_recv(event,event_bit_2,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
	{
	  _IO_485_RTC_L;           
	}
	/*事件7 电池温度过高,电池不能充电,进行电池保护*/
	if(rt_event_recv(event,event_7_TEMP_H,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
	{
	  _IO_DCFDKZ_L;
	  _IO_CDKZ_H;
	}
	/*事件8 电池温度正常*/
	if(rt_event_recv(event,event_8_TEMP_L,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
	{
			/*事件4 充电限制电压,停止充电*/
		if(rt_event_recv(event,event_4_BAT_V_MAX,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
		{
			_IO_CDKZ_H;
		}
		/*事件5 放电终止电压,停止放电*/
		if(rt_event_recv(event,event_5_BAT_V_MIN,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
		{
			_IO_DCFDKZ_H;
		}
		/*事件6 电压正常,电池进行充电放电*/
		if(rt_event_recv(event,event_6_BAT_V_CDFD,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
		{
			_IO_DCFDKZ_L;
			_IO_CDKZ_L;
		}
		/*事件9 电量MIN,停止放电*/
		/*事件10 电量正常,电池进行充电放电*/
		/*事件12 电池全部均流*/
		if(rt_event_recv(event,event_12_BAT_ALLFLOW_EQUALIZE,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
		{
			_IO_CDKZ_H;
		}
		/*事件13 电池未均流*/
		if(rt_event_recv(event,event_13_BAT_NOFLOW_EQUALIZE,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
		{
			_IO_DCFDKZ_L;
			_IO_CDKZ_L;
		}
	}
}
/*TimeOut thread entrance*/
static void _TimeOut_th_entrance(void *parameter)
{
	uint8_t Bat_Min_Flag=0;
	for(;;)
	{
		if(rt_sem_take(TimeOut_1_sem,100)==RT_EOK)
		{
			/*充放电电流*/
			Get_Bat_Current_Val();
			/*电压*/
			Get_Bat_Voltage();
			if(adc_val.BAT_Voltage_Val<0x0C){
				rt_event_send(event,event_5_BAT_V_MIN);
				Bat_Min_Flag=Bat_ZRL_MIN_Init();
			}
			else if(adc_val.BAT_Voltage_Val>=0x12 && Bat_Min_Flag==0){
				rt_event_send(event,event_4_BAT_V_MAX);
				Bat_ZRL_MAX_Init();	
			}
			else{
				/*电池电压在从最小电压切换到充电状态时有一瞬间电压会升至18v导致容量错误，执行Bat_ZRL_MAX_Init函数
				为防止这样的错误产生，才写下如下代码*/
				if(adc_val.BAT_Voltage_Val==0x0E)
				{
					Bat_Min_Flag=0;
				}
				rt_event_send(event,event_6_BAT_V_CDFD);
			}
			/*温度*/
			if(Get_Bat_Temp()){
				rt_event_send(event,event_7_TEMP_H);
			}
			else
			{
				rt_event_send(event,event_8_TEMP_L);
			}
			/*均流*/
			if(R_BAT_FLOW_EQUALIZE()==BAT_ALLFLOW_EQUALIZE){
				rt_event_send(event,event_12_BAT_ALLFLOW_EQUALIZE);
			}
			else{
				rt_event_send(event,event_13_BAT_NOFLOW_EQUALIZE);
			}
			/*电量*/
			Get_Bat_DL_Val();
			event_handler();
			Again_Open_Adc();
		}
	}
}
/*serial rx thread entrance*/
static void _serial_rx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(Serial_1_rx_sem,100)==RT_EOK)
		{
			serial_val.serial_1_rx_length=rt_device_read(Serial_1,0,serial_val.serial_1_rx_buf
							,sizeof(serial_val.serial_1_rx_buf)/sizeof(serial_val.serial_1_rx_buf[0]));
			switch(serial_msg_parsing(serial_val.serial_1_rx_buf))
			{
			case AC:
				rt_event_send(event,event_bit_1);
				break;
			case BAT:
				rt_event_send(event,event_bit_1);
				break;
			default:
				rt_event_send(event,event_bit_3);
				break;
			}
			rt_sem_release(Serial_1_tx_sem);
			memset(serial_val.serial_1_rx_buf,0,serial_val.serial_1_rx_length);
		}
		if(rt_sem_take(Serial_2_485_rx_sem,100)==RT_EOK)
		{
			serial_val.serial_2_485_rx_length=rt_device_read(Serial_2_485,0,serial_val.serial_2_485_rx_buf
							,sizeof(serial_val.serial_2_485_rx_buf)/sizeof(serial_val.serial_2_485_rx_buf[0]));
			switch(serial_msg_parsing(serial_val.serial_2_485_rx_buf))
			{
			case AC:
				_IO_DYKZ_L_AC;
				rt_event_send(event,event_bit_1);
				break;
			case BAT:
				_IO_DYKZ_H_BAT;
				rt_event_send(event,event_bit_1);
				break;
			default:
				rt_event_send(event,event_bit_3);
				break;
			}
			rt_sem_release(Serial_2_485_tx_sem);
			memset(serial_val.serial_2_485_rx_buf,0,serial_val.serial_2_485_rx_length);
		}
	}
}

/*serial tx thread entrance*/
static void _serial_tx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(Serial_1_tx_sem,100)==RT_EOK)
		{
			serial_val.serial_1_tx_length=serial_send_msg();
			rt_device_write(Serial_1,0,serial_msg.msg_send_buf,serial_val.serial_1_tx_length);
			memset(serial_msg.msg_send_buf,0,serial_val.serial_1_tx_length);
		}
		if(rt_sem_take(Serial_2_485_tx_sem,100)==RT_EOK)
		{
			_IO_485_RTC_H;
			/*事件1 接收完成*/
			if(rt_event_recv(event, event_bit_1,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
			{
				serial_val.serial_2_485_tx_length=serial_send_msg();
				rt_device_write(Serial_2_485,0,serial_msg.msg_send_buf,serial_val.serial_2_485_tx_length);
#if 1
				uint8_t buf[Save_24c02_Addr_Len]={0};
				AT24C02_R_nByte(buf,0,Save_24c02_Addr_Len);
				rt_thread_mdelay(10);
				rt_device_write(Serial_2_485,0,buf,Save_24c02_Addr_Len);
				memset(buf,0,Save_24c02_Addr_Len);
#endif
				memset(serial_msg.msg_send_buf,0,serial_val.serial_2_485_tx_length);
			}
			/*事件3 接收数据错误*/
			if(rt_event_recv(event, event_bit_3,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
			{
				_error();
			}
			rt_event_send(event,event_bit_2);
		}
	}
}

