/******************************************************************************/
/*更新日志
2024.8.9 解决当电池未充满时,此时的新容量大于保存的总容量,无法保存。且掉地掉电丢失数据的Bug.
*/
/******************************************************************************/
#include "app_u.h"
/******************************************************************************/
Serial_Value Serial_ValueStruct={0};
static void idle_hook(void);
static void _main_th_entrance(void *parameter);
static void _serial_rx_th_entrance(void *parameter);
static void _serial_tx_th_entrance(void *parameter);
static void _TimeOut_th_entrance(void *parameter);
/******************************************************************************/
/*init*/
uint8_t u_init(void)
{
	_IO_init();
	if(Serial_Init()==Error){
		return Error;
	}
	if(ADC_DMA_INIT()==True){
		Open_Adc();
//		rt_thread_mdelay(100);
//		Get_BAT_Zero_Val();
	}
	else{
		return Error;
	}
	if(Timer_Init()==Error){
		return Error;
	}
  if(Iwdg_Init(IWDG_PRESCALER_256,4095)==Error)
	{
		return Error;
	}
	else
	{
		Flag_Struct.Iwdg_Init_flag=flag_OK;
	}
	if(event_init()==Error){
		return Error;
	}
	if(RT_Sem_Init()==Error){
		return Error;
	}
	AT24C02_INIT();
	SD_READ_DL_M24c02();
	SDJC_CDKZ();
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
	/*TimeOut thread create*/
  RT_Thread_InitStruct._TimeOut_thread=rt_thread_create(_TimeOut_th_name,_TimeOut_th_entrance,RT_NULL,_TimeOut_th_stack_size
					,_TimeOut_th_priority,_TimeOut_th_stick);
	if(RT_Thread_InitStruct._TimeOut_thread)
	{
			rt_thread_startup(RT_Thread_InitStruct._TimeOut_thread);
	}
	else
	{
		Flag_Struct.Rt_Thread_Init_flag=flag_Error;
	}
	return ((Flag_Struct.Rt_Thread_Init_flag==flag_Error)?Error:True);
}
/******************************************************************************/
/**
 @brief thread entrance
 */
static void event_handler(void)
{
	/*事件4 充电限制电压,停止充电*/
	if(rt_event_recv(event,event_4_BAT_V_MAX,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
	  _IO_CDKZ_H;
	}
	if(rt_event_recv(event,event_5_BAT_V_MID,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
	  _IO_CDKZ_L;
		_IO_FDKZ_L;
	}
	if(rt_event_recv(event,event_6_BAT_V_MIN,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
		_IO_FDKZ_H;
	}
	/*事件7-8 温度*/
	if(rt_event_recv(event,event_7_TEMP_H,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
		_IO_CDKZ_H;
//		_IO_DYKZ_H_BAT;
	}
	if(rt_event_recv(event,event_8_TEMP_NORMAL,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
		_IO_CDKZ_L;
	}
	/*事件9-11 电量*/
	/*事件12-13 均流*/
	if(rt_event_recv(event,event_12_BAT_FLOW_EQUALIZE,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
		_IO_CDKZ_H;
	}
	if(rt_event_recv(event,event_13_BAT_NOFLOW_EQUALIZE,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK){
		_IO_CDKZ_L;
	}
}
static void event_send(void)
{
	/*事件4-6 电压*/
	if(BatSTU_flag.Bat_V_flag==BAT_V_MAX){	
		rt_event_send(event,event_4_BAT_V_MAX);}
	else if(BatSTU_flag.Bat_V_flag==BAT_V_MID){
		rt_event_send(event,event_5_BAT_V_MID);}
	else{
		rt_event_send(event,event_6_BAT_V_MIN);
	}
	/*事件7-8 温度*/ 
	if(BatSTU_flag.Bat_Temp_flag){
		rt_event_send(event,event_8_TEMP_NORMAL);
	}
	else{
		rt_event_send(event,event_7_TEMP_H);
	}
	/*事件12-13 均流*/
	if(BatSTU_flag.Bat_Flow_Equalize_flag){
		rt_event_send(event,event_12_BAT_FLOW_EQUALIZE);
	}
	else{
		rt_event_send(event,event_13_BAT_NOFLOW_EQUALIZE);
	}
	/*事件*/
	event_handler();
}

/******************************************************************************/
/*timeout entrance*/
static void _TimeOut_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.TimeOut_1_sem,100)==RT_EOK)
		{
			/*充放电电流*/
			Get_Bat_CDFD_Electricity();
			/*电压*/
			Get_Bat_Voltage();
			if(adc_val.BAT_Voltage_Val>=0x16){
				BatSTU_flag.Bat_V_flag=BAT_V_MAX;
				Bat_ZRL_MAX_Init();
			}
			else if(adc_val.BAT_Voltage_Val<=0x10){
				BatSTU_flag.Bat_V_flag=BAT_V_MIN;
				Bat_ZRL_MIN_Init();
			}
			else{
				BatSTU_flag.Bat_V_flag=BAT_V_MID;
			}
			/*温度*/
			BatSTU_flag.Bat_Temp_flag=Get_Bat_Temp();
			/*均流*/
			BatSTU_flag.Bat_Flow_Equalize_flag=BAT_Flow_Equalize();
			/*电量*/
			Get_Bat_DL();
			event_send();
			/*打开ADC*/
			Again_Open_Adc();
		}
	}
}
/*thread entrance*/
static void idle_hook(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
/*main entrance*/
static void _main_th_entrance(void *parameter)
{
	for(;;)
	{
		CD_LED();
		RUN_LED();
	}
}
/*serial entrance*/
static void _serial_rx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_rx_sem,100)==RT_EOK)
		{
			Serial_ValueStruct.serial_1_rx_length=rt_device_read(Dev_InitStruct.dev_serial_1,0,Serial_ValueStruct.serial_1_rx_buf
							,serial_1_rx_buf_size);
			if(Proccess_Serial_1_RxData(Serial_ValueStruct.serial_1_rx_buf)==True)
			{
				memset(Serial_ValueStruct.serial_1_rx_buf,0,Serial_ValueStruct.serial_1_rx_length);
				rt_event_send(event,event_bit_1);
				rt_sem_release(RT_Thread_InitStruct.Serial_1_tx_sem);
			}
		}
		if(rt_sem_take(RT_Thread_InitStruct.Serial_2_rx_sem,100)==RT_EOK)
		{
			Serial_ValueStruct.serial_2_rx_length=rt_device_read(Dev_InitStruct.dev_serial_2,0,Serial_ValueStruct.serial_2_rx_buf
							,serial_2_rx_buf_size);
			if(Proccess_Serial_2_RxData(Serial_ValueStruct.serial_2_rx_buf)==True)
			{
					memset(Serial_ValueStruct.serial_2_rx_buf,0,serial_2_rx_buf_size);
					rt_sem_release(RT_Thread_InitStruct.Serial_2_tx_sem);	
			}	
		}
	}
}
static void _serial_tx_th_entrance(void *parameter)
{
	for(;;)
	{
		if(rt_sem_take(RT_Thread_InitStruct.Serial_1_tx_sem,100)==RT_EOK)
		{
			if(rt_event_recv(event, event_bit_1,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,100,RT_NULL)==RT_EOK)
			{
				Serial_ValueStruct.serial_1_tx_length=Proccess_Serial_1_TxData();
				rt_device_write(Dev_InitStruct.dev_serial_1,0,Serial_Msg_1.Serial_1_Txbuf
							,Serial_ValueStruct.serial_1_tx_length);
				memset(Serial_Msg_1.Serial_1_Txbuf,0,Serial_ValueStruct.serial_1_tx_length);
#if 1
				uint8_t buf[Save_24c02_Addr_Len]={0};
				AT24C02_R_nByte(buf,0,Save_24c02_Addr_Len);
				rt_thread_mdelay(10);
				rt_device_write(Dev_InitStruct.dev_serial_1,0,buf,Save_24c02_Addr_Len);
				memset(buf,0,Save_24c02_Addr_Len);
#endif
			}
		}
		if(rt_sem_take(RT_Thread_InitStruct.Serial_2_tx_sem,100)==RT_EOK)
		{
			Serial_ValueStruct.serial_2_tx_length=Proccess_Serial_2_TxData();
			rt_device_write(Dev_InitStruct.dev_serial_2,0,Serial_Msg_1.Serial_2_Txbuf
							,Serial_ValueStruct.serial_2_tx_length);
			memset(Serial_ValueStruct.serial_2_tx_buf,0,SERIAL_2_TXLEN);		
		}		
	}
}
/******************************************************************************/
