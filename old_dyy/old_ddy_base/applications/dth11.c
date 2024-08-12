#include "dth11.h"
/***************************************************************/
_dth11 _dth11_data;
/***************************************************************/
/*主频72MHz,72个__NOP()即1us延时*/
static void delay_1us()
{
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();
}
static void delayUs(uint16_t us)
{
	for(uint16_t i=0;i<us;i++)
	{
		delay_1us();
	}
}
/***************************************************************/
/*温湿度模块输出函数*/
static void DHT11_IO_OUT (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
}
/*温湿度模块输入函数*/
static void DHT11_IO_IN (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
}
/*DHT11端口复位,发出起始信号(IO发送)*/
static void DHT11_RST (void) 					
{	
	DHT11_IO_OUT();							/*端口为输出*/
	_IO_DHT11_L;	 						/*使总线为低电平*/
	rt_thread_mdelay(25); 					/*拉低至少18ms*/						
	_IO_DHT11_H;	 						/*使总线为高电平*/							
	delayUs(30); 							/*主机拉高20~40us*/	
}
/*等待DHT11回应,返回1:未检测到DHT11,返回0:成功(IO接收)*/	   
static uint8_t DHT11_Check(void)
{ 	   
  uint8_t retry=0;							/*定义临时变量*/
  DHT11_IO_IN();							/*IO到输入状态*/	 
	/*GPIO端口输入时，配置为上拉输入或者浮空输入，因为外接上拉电阻，所以默认为高电平*/
	/*如果DHT11的数据线输入为高电平,且retry小于100,则将retry加1,并延时1微秒,重复这个过程直到retry大于等于100或者DHT11的数据线输入变成低电平。
	如果retry大于等于100,表示检测失败,返回1:否则,将retry重置为0.*/
	while ((_IO_R_DHT11 == 1) && retry<100)	//DHT11会拉低40~80us
	{
		retry++;
		delayUs(1);
  }
  if(retry>=100){
		return 1;
	}		
	else{ 
		retry=0;
	}
	/*如果DHT11的数据线输入为低电平,且retry小于100,则将retry 加1,并延时1微秒.重复这个过程直到retry大于等于100或者DHT11的数据线输入变成高电平。
	如果retry大于等于100表示检测失败.返回1:否则,返回0,表示检测成功.*/
	while ((_IO_R_DHT11 == 0) && retry<100)  /*DHT11拉低后会再次拉高40~80us*/
	{  
		 retry++;
		 delayUs(1);
	}
	if(retry>=100){
		return 1;	    
	}
	return 0;
}
/*从DHT11读取一个位*/
static uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0;
	while((_IO_R_DHT11 == 1) && retry < 100) 			/*等待变为低电平*/
	{
		retry++;
		delayUs(1);
	}
	retry = 0;
	while((_IO_R_DHT11 == 0) && retry < 100) 			/*等待变高电平*/
	{
		retry++;
		delayUs(1);
	}
	delayUs(40);										/*延迟40us*/
	if(_IO_R_DHT11 == 1)       							/*用于判断高低电平，即数据1或0*/
	{		
		return 1;
	}
	else
	{
		return 0;
	}
}
/*DHT11读取一个字节*/
static uint8_t DHT11_Read_Byte(void)
{
	uint8_t dat;
	dat = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		dat <<= 1;				
		dat |= DHT11_Read_Bit();	
	}
	return dat;
}
/***************************************************************/
/*DHT11读取数据
* temp:温度值(范围:0~50°)
* humi:湿度值(范围:20%~90%)
* 返回值：0,正常;1,读取失败*/
uint8_t DHT11_Read_Data(void)
{
	uint8_t buf[5];
	DHT11_RST();										/*DHT11端口复位，发出起始信号*/
	if(DHT11_Check()==0)								/*等待DHT11回应，0为成功回应*/
	{
		for(uint8_t i = 0; i < 5; i++) 					/*读取5字节数据*/
		{
			buf[i] = DHT11_Read_Byte();
		}
		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])	/*数据校验*/
		{
			_dth11_data._hum_H = buf[0];				
			_dth11_data._temp_H = buf[2];				
		}
	}
  else{
		return 1;
	} 
  return 0;
}
/*DHT11初始化*/
uint8_t DHT11_Init (void)
{			
	__HAL_RCC_GPIOC_CLK_ENABLE();	/*开始DHT11的时钟*/
	DHT11_RST();					/*DHT11端口复位，发出起始信号*/
	return DHT11_Check(); 			/*等待DHT11回应*/
}
/***************************************************************/
