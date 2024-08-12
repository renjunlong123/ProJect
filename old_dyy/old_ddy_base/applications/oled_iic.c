#include "oled_iic.h"
#include "oled_font.h"
static void OLED_WR_Byte(uint8_t data,uint8_t cmd);
/**************************************************************/
static void _Io_Oled_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void _Oled_Init(void)
{
	_Io_Oled_Init();
	rt_thread_mdelay(10);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	
	OLED_WR_Byte(0x80,OLED_CMD);//
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	
	OLED_WR_Byte(0x14,OLED_CMD);//
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	OLED_Clear();
}
/**************************************************************/
/*坐标设置*/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}  
/*oled全亮*/
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte(0xb0+i,OLED_CMD); 
		OLED_WR_Byte(0x00,OLED_CMD);	
		OLED_WR_Byte(0x10,OLED_CMD);         
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(1,OLED_DATA);
		}
	} 
}
/*oled清屏*/
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte(0xb0+i,OLED_CMD);    
		OLED_WR_Byte(0x00,OLED_CMD);      
		OLED_WR_Byte(0x10,OLED_CMD);       
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(0,OLED_DATA);
		}
	} 
}
/*m^n函数*/
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)
	{
		result*=m;   
	} 
	return result;
}		
/*
*在指定位置显示数字
*x,y:起点坐标
*num:数字大小
*len:数字的位数(0~4294967295)
*size:选择字体 16/12
*/
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else
			{ 
				enshow=1; 
			}
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
/*
*在指定位置显示一个字符
*x:0~127,y:0~63
*mode:0,反白显示、1,正常显示
*size:选择字体 16/12
*/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';	
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
	}
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
	}
}
/*显示字符串*/
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120)
		{
			x=0;y+=2;
		}
		j++;
	}
}
/*显示汉字*/
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	 }	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}
/****************************IIC*******************************/
/*iic start*/
static void IIC_Start(void)
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}
/*iic stop*/
static void IIC_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();	
}
/*iic wait ack*/
static void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/****************************oled******************************/
/*iic write byte*/
static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
			OLED_SDIN_Set();
		}
		else
		{
			OLED_SDIN_Clr();
		}			
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/*oled write command*/
static void OLED_WriteCommand(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);   /*地址*/
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);		/*写命令寄存器*/
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/*oled write data*/
static void OLED_WriteData(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);		/*地址*/	
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);		/*写数据寄存器*/
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
/*oled write data*/
static void OLED_WR_Byte(uint8_t data,uint8_t cmd)
{
	if(cmd)
	{
		OLED_WriteData(data);
	}
	else
	{
		OLED_WriteCommand(data);
	}
}
