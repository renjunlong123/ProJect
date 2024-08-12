#include "app_data.h"
Bat Bat_Val;
at24c02 at24c02_val;
uint16_t adc_val_buf[ADC_BUF_SIZE];
extern ADC_HandleTypeDef hadc1;
adc adc_val;
static void DL_MaxInit_M24c02(void);
static void DL_MinInit_M24c02(void);
static void DL_Save_M24c02(void);
static void READ_DL_M24c02(void);
/************************************************************************/
/**
 @brief CRC字节值表高位
 */
const unsigned char crcTableHigh[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/**
 @brief CRC字节值表低位
 */
const unsigned char crcTableLow[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
/**
 @brief crc校验
 */
/*modbus CRC 校检*/
static uint16_t CRC_Modbus(const uint8_t* buf,uint16_t len)
{
	uint8_t crc_h=0xff,crc_l=0xff;
	unsigned int crc_index=0;
	while(len--)
	{
			crc_index=crc_h^ *buf++;
			crc_h=crc_l^crcTableHigh[crc_index];
			crc_l=crcTableLow[crc_index];
	}
	return ((crc_h<<8)|crc_l);
}
/*XModem CRC 校检*/
static uint16_t CRC_XModem(const uint8_t* buf,uint8_t len)
{
	uint16_t crc = 0x00;
	uint16_t polynomial = 0x1021;
	uint16_t index,i;
	uint8_t b;
	uint16_t bit;
	uint16_t c15;
	for(index = 0;index<len;index++)
	{
		b = buf[index];
		for(i = 0; i < 8; i++)
		{
			bit = (((b>>(7-i))&1) == 1);
			c15 = (((crc>>15)&1) == 1);
			crc <<= 1;
			if(c15 ^ bit)
			{
				crc ^= polynomial;
			}
		}
	}
	crc &= 0xffff;
	return crc;
}
/*CCITT CRC 校检*/
static uint16_t CRC_CCITT(const uint8_t* di,uint32_t len)
{
	uint16_t crc_poly = 0x8408; //Bit sequence inversion of 0x1021
	uint16_t data_t = 0; //CRC register

	for (uint32_t i = 0; i < len; i++)
	{
		data_t ^= di[i]; //8-bit data

		for (char j = 0; j < 8; j++)
		{
			if (data_t & 0x0001)
				data_t = (data_t >> 1) ^ crc_poly;
			else
				data_t >>= 1;
		}
	}
	return data_t;
}
/************************************************************************/
/**
	@brief 控制引脚
 */
/************************************************************************/
/*读取供电状态*/
uint8_t R_BAT_STU(void)
{
	return _IO_READ_POWER_STU;
}
/*读取电池是否充电*/
uint8_t R_BAT_CD_STU(void)
{
    return _IO_R_CD_STU;
}
/*读取电池是否放电*/
uint8_t R_BAT_FD_STU(void)
{
    return _IO_R_FD_STU;
}
/*读取电池状态  低电平均流,高电平未均流*/
void Bat_ZRL_MAX_Init(void)
{
	Bat_Val.Bat_ZRL=Bat_Val.Bat_Now_ZRL;
	Bat_Val.Bat_Now_DL=100;
	Bat_Val.Bat_DL_Flag=BAT_DL_MAX_FLAG;
	DL_MaxInit_M24c02();
}
uint8_t Bat_ZRL_MIN_Init(void)
{
	DL_MinInit_M24c02();
	READ_DL_M24c02();
	return 1;
}
uint8_t R_BAT_FLOW_EQUALIZE(void)
{
	if(_IO_R_BAT_STU_1||_IO_R_BAT_STU_2||_IO_R_BAT_STU_3||_IO_R_BAT_STU_4||_IO_R_BAT_STU_5)
	{
		return BAT_NO_FLOW_EQUALIZE;
	}
	Bat_ZRL_MAX_Init();
	return BAT_ALLFLOW_EQUALIZE;
}
/************************************************************************/
/************************************************************************/
/**
 @brief ADC
 */
/*开启ADC1 DMA转换*/
void Open_Adc(void)
{
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_val_buf,ADC_BUF_SIZE);
}
/*再次开启开启ADC1 DMA转换*/
void Again_Open_Adc(void)
{
	/*停止开启ADC1 DMA转换*/
	HAL_ADC_Stop_DMA(&hadc1);								
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_val_buf,ADC_BUF_SIZE);
	memset(adc_val_buf,0,ADC_BUF_SIZE);
}
/**
 @brief 电池电压值
 */
uint16_t Get_Bat_Voltage(void)
{
	uint8_t i=0;
	uint32_t temp_VOL_val=0;
	float Temp_Vol=0;
	for(i=0;i<ADC_BUF_SIZE;i+=5)
	{
		temp_VOL_val+=adc_val_buf[i+4];
	}
	Temp_Vol=(float)temp_VOL_val/(ADC_BUF_SIZE/ADC_RANK);
	/*该脚采集到的电压值为电池电压的0.1倍*/
	Temp_Vol=(Temp_Vol*33)/4096;
	if(_IO_READ_POWER_STU==GPIO_PIN_SET)
	{
		/*每增加1A,电压补偿0.36v*/
		float Temp_Vol_BC=((adc_val.BAT_CDFD_Current)*36)/100000;
		adc_val.BAT_Voltage_Val=(uint8_t)(Temp_Vol+Temp_Vol_BC);
	}
	else
	{
		adc_val.BAT_Voltage_Val=(uint8_t)Temp_Vol;
	}
	return adc_val.BAT_Voltage_Val;
}
/**
 @brief 电池温度值
 */
/*10K NTC 温度与阻值对应表 X轴代表温度  Y轴代表阻值*/
const uint16_t X_T[17]={0,6,12,18,25,31,37,45,50,57,63,70,76,83,89,94,100};
const uint16_t Y_R[17]={31908,23698,17794,13498,9900,7625,5925,4288,3529,2709,2177,1701,1386,1101,909,778,649};
/*计算真实温度（参数:NTC当前阻值,返回值:温度值*/
static float R_To_Temp(const uint16_t R)
{
	uint8_t index=0,START_POINT=0,END_POINT=0;
	int16_t d_x=0,d_y=0;
	float k,b,temp;
	for(index=0;index<16;index++)
	{
		if(R<Y_R[index])
		{
			if(R>Y_R[index+1])
			{
					START_POINT=index;
					END_POINT=index+1;
					break;
			}
		}
	}
	d_x=((int16_t)X_T[END_POINT]-(int16_t)X_T[START_POINT]);
	d_y=((int16_t)Y_R[END_POINT]-(int16_t)Y_R[START_POINT]);
	k=(float)(d_y/d_x);
	b=Y_R[START_POINT]-k*X_T[START_POINT];
	temp=(R-b)/k;
	return temp;
}
/*计算温度过程*/
static uint16_t ADC_To_Temp(uint16_t ADC_val)
{
	uint16_t R=0,mV=0;
	float val=0;
	/*ADC转换为电压*/
	mV=(ADC_val*3300)/4096;    /*mv*/
	/*电压转换为电阻*/
	R=(5100*mV)/(3300-mV);
	/*电阻转换为温度*/
	val=R_To_Temp(R);
	//val=val*10;
	return (uint16_t)val;
}
/*电池温度值*/
uint8_t Get_Bat_Temp(void)
{
	uint8_t i=0;
	uint32_t temp_tmp_val_1=0,temp_tmp_val_2=0,temp_tmp_val_3=0;
	for(i=0;i<ADC_BUF_SIZE;i+=5)
	{
			temp_tmp_val_1+=adc_val_buf[i+1];
			temp_tmp_val_2+=adc_val_buf[i+2];
			temp_tmp_val_3+=adc_val_buf[i+3];
	}
	adc_val.BAT_Temp_Val_1=temp_tmp_val_1/(ADC_BUF_SIZE/ADC_RANK);
	adc_val.BAT_Temp_Val_2=temp_tmp_val_2/(ADC_BUF_SIZE/ADC_RANK);
	adc_val.BAT_Temp_Val_3=temp_tmp_val_3/(ADC_BUF_SIZE/ADC_RANK);
	adc_val.BAT_Temp_Val_1=ADC_To_Temp(adc_val.BAT_Temp_Val_1);
	if(adc_val.BAT_Temp_Val_1>45)
	{
			return Error;
	}
	return True;
}
/**
 @brief 电池充放电电流值
 */
void Get_Bat_Current_Val(void)
{
	uint32_t temp_CDFD_val=0;
	uint8_t i=0;
	for(i=0;i<ADC_BUF_SIZE;i+=5)
	{
			temp_CDFD_val+=adc_val_buf[i];
	}
	temp_CDFD_val=(temp_CDFD_val/(ADC_BUF_SIZE/ADC_RANK));
	adc_val.BAT_CDFD_Val=(temp_CDFD_val*3.3*1000)/4096;  /*单位mv*/

	/*当电流传感器为3.3/2V时，电流输出为0*/
	if(adc_val.BAT_CDFD_Val==1650)
	{
			adc_val.BAT_CDFD_Current=0;
	}
	if(adc_val.BAT_CDFD_Val>1650)
	{
		adc_val.BAT_CDFD_Current=((adc_val.BAT_CDFD_Val-1650)*1000)/132;
	}
	if(adc_val.BAT_CDFD_Val<1650)
	{
		adc_val.BAT_CDFD_Current=((1650-adc_val.BAT_CDFD_Val)*1000)/132;
	}
}
/************************************************************************/
/**
@brief
      地址                  保存值                      含义
      0x00                记录上一次总容量
      0x04                记录当前充放的总容量
      0x08 0x09           记录当前电量百分比
      0x0a 0x0b      		      1       				        第一次上电
                              2                       电量MAX
                              3                       电量中间值
                              4                       电量MIN
*/
static inline void DL_Calibration(void)
{
	if(adc_val.BAT_Voltage_Val<=0x0D)/*13V*/
	{
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.02))
		{
			Bat_Val.Bat_Now_ZRL=Bat_Val.Bat_ZRL*0.02;
		}
	}
	else if(adc_val.BAT_Voltage_Val==0x0E)/*14v*/
	{ 
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.04))
		{
			Bat_Val.Bat_Now_ZRL=(Bat_Val.Bat_ZRL*0.04);
		}
	} 
	else if(adc_val.BAT_Voltage_Val==0x0F)/*15v*/
	{
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.08))
		{
			Bat_Val.Bat_Now_ZRL=(Bat_Val.Bat_ZRL*0.08);
		}
	} 
	else if(adc_val.BAT_Voltage_Val==0x10)/*16v*/
	{
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.40))
		{
			Bat_Val.Bat_Now_ZRL=(Bat_Val.Bat_ZRL*0.40);
		}
	}
	else
	{
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.50))
		{
			Bat_Val.Bat_Now_ZRL=(Bat_Val.Bat_ZRL*0.50);
		}
	}
}

/*计算电池电量并保存电池数据到24c02中*/
void Get_Bat_DL_Val(void)
{
	uint8_t Dl_Max_Flag=0;
	Bat_Val.Bat_After_DL=Bat_Val.Bat_Now_DL;
	if(_IO_READ_POWER_STU==GPIO_PIN_RESET)
	{
		if(Bat_Val.Bat_DL_Flag!=BAT_DL_MAX_FLAG)
		{
			Bat_Val.Bat_Now_ZRL=Bat_Val.Bat_Now_ZRL+adc_val.BAT_CDFD_Current;
		}
		if(Bat_Val.Bat_Now_ZRL>=Bat_Val.Bat_ZRL){
			uint32_t Tmp=0;
			uint8_t dl_a,dl_b;
			Tmp=Bat_Val.Bat_ZRL;
			Bat_Val.Bat_ZRL=Bat_Val.Bat_Now_ZRL;
			Bat_Val.Bat_Now_DL=100;
			/*防止电源未充满时,容量已经等于最大值却写不进24c02的情况,主要防止掉电丢失数据*/
			dl_a=Bat_Val.Bat_Now_DL;
			dl_b=Bat_Val.Bat_ZRL/(Tmp*0.01);
			if(dl_a!=dl_b)
			{
				Dl_Max_Flag=1;
			}
		}
		else if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_Now_ZRL*0.01)){
			Bat_Val.Bat_Now_DL=1;
			Bat_Val.Bat_DL_Flag=BAT_DL_MIN_FLAG;
		}
		else
		{
			Bat_Val.Bat_Now_DL=(Bat_Val.Bat_Now_ZRL)/(Bat_Val.Bat_ZRL*0.01);
			Bat_Val.Bat_DL_Flag=BAT_DL_MID_FLAG;
		}
	}
	if(_IO_READ_POWER_STU==GPIO_PIN_SET)
	{
		Bat_Val.Bat_Now_ZRL=Bat_Val.Bat_Now_ZRL-adc_val.BAT_CDFD_Current;
		DL_Calibration();
		if(Bat_Val.Bat_Now_ZRL<=(Bat_Val.Bat_ZRL*0.01))
		{
			Bat_Val.Bat_Now_ZRL=(Bat_Val.Bat_ZRL*0.01);
			Bat_Val.Bat_Now_DL=1;
			Bat_Val.Bat_DL_Flag=BAT_DL_MIN_FLAG;
		}
		else
		{
			Bat_Val.Bat_Now_DL=(Bat_Val.Bat_Now_ZRL)/(Bat_Val.Bat_ZRL*0.01);
			Bat_Val.Bat_DL_Flag=BAT_DL_MID_FLAG;
		}
	}
	if(Bat_Val.Bat_After_DL!=Bat_Val.Bat_Now_DL || Dl_Max_Flag==1)
	{
		Bat_Val.Bat_save_times++;
		AT24C02_Erase();
		if(Bat_Val.Bat_DL_Flag==BAT_DL_MAX_FLAG)
		{
			DL_MaxInit_M24c02();
		}
		else if(Bat_Val.Bat_DL_Flag==BAT_DL_MIN_FLAG)
		{	
			DL_MinInit_M24c02();
		}
		else
		{
			DL_Save_M24c02();	
		}
	}
}
/************************************************************************/
/************************************************************************/
/*24c02电池初始化参数*/
void DL_MaxInit_M24c02(void)
{
	/*6Ah BAT_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[0]=(Bat_Val.Bat_ZRL>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[1]=(Bat_Val.Bat_ZRL>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[2]=(Bat_Val.Bat_ZRL>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[3]=Bat_Val.Bat_ZRL&0xff;
	/*BAT_NOW_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[4]=(Bat_Val.Bat_Now_ZRL>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[5]=(Bat_Val.Bat_Now_ZRL>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[6]=(Bat_Val.Bat_Now_ZRL>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[7]=Bat_Val.Bat_Now_ZRL&0xff;
	/*BAT_DL*/
	at24c02_val.at24c02_txbuf_BAT_2[0]=0x00;
	at24c02_val.at24c02_txbuf_BAT_2[1]=0x64;
	/*BAT_DL_FLAG*/
	at24c02_val.at24c02_txbuf_BAT_2[2]=(BAT_DL_MAX_FLAG>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[3]=BAT_DL_MAX_FLAG&0xff;
	/*BAT_DL_save_times*/
	at24c02_val.at24c02_txbuf_BAT_2[4]=(Bat_Val.Bat_save_times>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[5]=(Bat_Val.Bat_save_times>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[6]=(Bat_Val.Bat_save_times>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[7]=(Bat_Val.Bat_save_times)&0xff;
	Bat_Val.Bat_DL_Flag=BAT_DL_MAX_FLAG;
	if(i2c_CheckDevice(AT24C02)==0)
	{
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_1,0,Save_24c02_Page_1_len);
		rt_thread_mdelay(10);
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_2,8,Save_24c02_Page_2_len);
	}
}
void DL_MinInit_M24c02(void)
{
	/*6Ah BAT_ZRL*/
	uint32_t tmp=Bat_Val.Bat_ZRL;
	uint32_t tmp2=Bat_Val.Bat_ZRL*0.01;
	at24c02_val.at24c02_txbuf_BAT_1[0]=(tmp>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[1]=(tmp>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[2]=(tmp>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[3]=tmp&0xff;
	/*BAT_NOW_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[4]=(tmp2>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[5]=(tmp2>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[6]=(tmp2>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[7]=tmp2&0xff;
	/*BAT_DL*/
	at24c02_val.at24c02_txbuf_BAT_2[0]=0x00;
	at24c02_val.at24c02_txbuf_BAT_2[1]=0x01;
	/*BAT_DL_FLAG*/
	at24c02_val.at24c02_txbuf_BAT_2[2]=(BAT_DL_MIN_FLAG>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[3]=BAT_DL_MIN_FLAG&0xff;
	/*BAT_DL_save_times*/
	at24c02_val.at24c02_txbuf_BAT_2[4]=(Bat_Val.Bat_save_times>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[5]=(Bat_Val.Bat_save_times>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[6]=(Bat_Val.Bat_save_times>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[7]=(Bat_Val.Bat_save_times)&0xff;
	Bat_Val.Bat_DL_Flag=BAT_DL_MIN_FLAG;
	if(i2c_CheckDevice(AT24C02)==0)
	{
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_1,0,Save_24c02_Page_1_len);
		rt_thread_mdelay(100);
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_2,8,Save_24c02_Page_2_len);
	}
}
void DL_Save_M24c02(void)
{
	/*6Ah BAT_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[0]=(Bat_Val.Bat_ZRL>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[1]=(Bat_Val.Bat_ZRL>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[2]=(Bat_Val.Bat_ZRL>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[3]=Bat_Val.Bat_ZRL&0xff;
	/*BAT_NOW_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[4]=(Bat_Val.Bat_Now_ZRL>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[5]=(Bat_Val.Bat_Now_ZRL>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[6]=(Bat_Val.Bat_Now_ZRL>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[7]=Bat_Val.Bat_Now_ZRL&0xff;
	/*BAT_DL*/
	at24c02_val.at24c02_txbuf_BAT_2[0]=0x00;
	at24c02_val.at24c02_txbuf_BAT_2[1]=Bat_Val.Bat_Now_DL&0xff;
	/*BAT_DL_FLAG*/
	at24c02_val.at24c02_txbuf_BAT_2[2]=(Bat_Val.Bat_DL_Flag>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[3]=Bat_Val.Bat_DL_Flag&0xff;
	/*BAT_DL_save_times*/
	at24c02_val.at24c02_txbuf_BAT_2[4]=(Bat_Val.Bat_save_times>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[5]=(Bat_Val.Bat_save_times>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[6]=(Bat_Val.Bat_save_times>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[7]=(Bat_Val.Bat_save_times)&0xff;
//	Bat_Val.Bat_DL_Flag=BAT_DL_INIT_FLAG;
	Bat_Val.Bat_DL_Flag=BAT_DL_MID_FLAG;
	if(i2c_CheckDevice(AT24C02)==0)
	{
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_1,0,Save_24c02_Page_1_len);
		rt_thread_mdelay(10);
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_2,8,Save_24c02_Page_2_len);
	}
}
static void DL_FIRST_SD_M24c02(void)
{
	/*10Ah BAT_ZRL*/
	uint32_t tmp=(6*1000*60*60);
	uint32_t tmp2=(6*1000*60*60)*0.01;
	at24c02_val.at24c02_txbuf_BAT_1[0]=(tmp>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[1]=(tmp>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[2]=(tmp>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[3]=tmp&0xff;
	/*BAT_NOW_ZRL*/
	at24c02_val.at24c02_txbuf_BAT_1[4]=(tmp2>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[5]=(tmp2>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[6]=(tmp2>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_1[7]=tmp2&0xff;
	/*BAT_DL*/
	at24c02_val.at24c02_txbuf_BAT_2[0]=0x00;
	at24c02_val.at24c02_txbuf_BAT_2[1]=0x01;
	/*BAT_DL_FLAG*/
	at24c02_val.at24c02_txbuf_BAT_2[2]=(BAT_DL_MIN_FLAG>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[3]=BAT_DL_MIN_FLAG&0xff;
	/*BAT_DL_save_times*/
	at24c02_val.at24c02_txbuf_BAT_2[4]=(Bat_Val.Bat_save_times>>24)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[5]=(Bat_Val.Bat_save_times>>16)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[6]=(Bat_Val.Bat_save_times>>8)&0xff;
	at24c02_val.at24c02_txbuf_BAT_2[7]=(Bat_Val.Bat_save_times)&0xff;
	Bat_Val.Bat_DL_Flag=BAT_DL_MIN_FLAG;
	if(i2c_CheckDevice(AT24C02)==0)
	{
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_1,0,Save_24c02_Page_1_len);
		rt_thread_mdelay(10);
		AT24C02_W_nByte(at24c02_val.at24c02_txbuf_BAT_2,8,Save_24c02_Page_2_len);
	}
}
/*m24c02 read dl*/
static void READ_DL_M24c02(void)
{
	rt_thread_mdelay(10);
	AT24C02_R_nByte(at24c02_val.at24c02_rxbuf_BAT,0,Save_24c02_Addr_Len);
	Bat_Val.Bat_ZRL=(at24c02_val.at24c02_rxbuf_BAT[0]<<24)+(at24c02_val.at24c02_rxbuf_BAT[1]<<16)
									+(at24c02_val.at24c02_rxbuf_BAT[2]<<8)+at24c02_val.at24c02_rxbuf_BAT[3];
	Bat_Val.Bat_Now_ZRL=(at24c02_val.at24c02_rxbuf_BAT[4]<<24)+(at24c02_val.at24c02_rxbuf_BAT[5]<<16)
									+(at24c02_val.at24c02_rxbuf_BAT[6]<<8)+at24c02_val.at24c02_rxbuf_BAT[7];
	Bat_Val.Bat_Now_DL=(at24c02_val.at24c02_rxbuf_BAT[8]<<8)+at24c02_val.at24c02_rxbuf_BAT[9];	
	Bat_Val.Bat_DL_Flag=(at24c02_val.at24c02_rxbuf_BAT[10]<<8)+at24c02_val.at24c02_rxbuf_BAT[11];
	Bat_Val.Bat_save_times=(at24c02_val.at24c02_rxbuf_BAT[12]<<24)+(at24c02_val.at24c02_rxbuf_BAT[13]<<16)
									+(at24c02_val.at24c02_rxbuf_BAT[14]<<8)+at24c02_val.at24c02_rxbuf_BAT[15];
}
/*上电读取DL*/
uint8_t SD_READ_DL_M24c02(void)
{
#if 1
	READ_DL_M24c02();
	if(Bat_Val.Bat_DL_Flag==BAT_DL_MAX_FLAG)
	{
		AT24C02_Erase();
		DL_MaxInit_M24c02();
	}
	else if(Bat_Val.Bat_DL_Flag==BAT_DL_MIN_FLAG)
	{
		AT24C02_Erase();
		DL_MinInit_M24c02();
	}
	else if(Bat_Val.Bat_DL_Flag==BAT_DL_MID_FLAG)
	{
#if 0
		AT24C02_Erase();
		DL_MinInit_M24c02();
#endif
#if 0
		AT24C02_Erase();
		rt_thread_mdelay(10);
		DL_MaxInit_M24c02();
#endif
#if 0
		READ_DL_M24c02();
		rt_device_write(Dev_InitStruct.dev_serial_1,0,at24c02_val.at24c02_rxbuf_BAT,Save_24c02_Addr_Len);
#endif
		return True;
	}
	else if(Bat_Val.Bat_DL_Flag==BAT_DL_INIT_FLAG)
	{
		
	}
	else
	{
		AT24C02_Erase();
		DL_FIRST_SD_M24c02();
	}
#endif
	rt_thread_mdelay(10);
	READ_DL_M24c02();
	return True;
}
/************************************************************************/
/************************************************************************/
/**
	@brief 通上位机通讯
 */
msg serial_msg;

/**
 @brief 上位机报文解析
 */
uint8_t serial_msg_parsing(const uint8_t* msg_buf)
{
	uint8_t i=0;
	serial_msg.Inspection_code[0]=0x00;
	serial_msg.Inspection_code[1]=0x00;
	serial_msg.Inspection_code[2]=0x7E;
	serial_msg.Inspection_code[3]=0x40;
	serial_msg.Inspection_code[4]=0x6A;
	serial_msg.Inspection_code[5]=0x00;
	serial_msg.Inspection_code[6]=0x00;
	for(i=0;i<msg_Inspection_code_len;i++)
	{
			if(serial_msg.Inspection_code[i]!=msg_buf[i])
			{
					return _false;
			}
	}
	for(i=0;i<rev_msg_max_size;i++)
	{
			serial_msg.msg_rec_buf[i]=*msg_buf;
			*msg_buf++;
	}
	serial_msg.rev_crc_dat=CRC_XModem(serial_msg.msg_rec_buf,rev_msg_max_size-2);
	if(serial_msg.rev_crc_dat!=((serial_msg.msg_rec_buf[rev_msg_max_size-2])|(serial_msg.msg_rec_buf[rev_msg_max_size-1]<<8)))
	{
			return _false;
	}
	switch(serial_msg.msg_rec_buf[7])
	{
	case AC:
			return AC;
	case BAT:
			return BAT;
	}
	return _false;
}
/**
 @brief 上位机报文发送
 */
uint8_t serial_send_msg(void)
{
	const uint8_t send_msg_len=send_msg_max_size; 
	/*固定码*/
	serial_msg.msg_send_buf[0]=0x7E;
	serial_msg.msg_send_buf[1]=0x40;
	serial_msg.msg_send_buf[2]=0x00;
	/*供电状态*/
	if(_IO_R_DYKZ)
	{
		serial_msg.msg_send_buf[3]=BAT;
	}
	/*电量*/
	serial_msg.msg_send_buf[4]=Bat_Val.Bat_Now_DL;
	/*固定码*/
	serial_msg.msg_send_buf[5]=0x01;
	/*电池温度*/
	serial_msg.msg_send_buf[6]=adc_val.BAT_Temp_Val_1>>8;
	serial_msg.msg_send_buf[7]=adc_val.BAT_Temp_Val_1;
	if(_IO_R_DYKZ)
	{
		/*放电电流高前低后*/
		serial_msg.msg_send_buf[8]=0x00;
		serial_msg.msg_send_buf[9]=0x00;
		serial_msg.msg_send_buf[10]=adc_val.BAT_CDFD_Current>>8;
		serial_msg.msg_send_buf[11]=adc_val.BAT_CDFD_Current;
	}
	else
	{
		/*充电电流高前低后*/
		serial_msg.msg_send_buf[8]=adc_val.BAT_CDFD_Current>>8;
		serial_msg.msg_send_buf[9]=adc_val.BAT_CDFD_Current;
		serial_msg.msg_send_buf[10]=0x00;
		serial_msg.msg_send_buf[11]=0x00;
	}
	/*电池电压*/
	serial_msg.msg_send_buf[12]=adc_val.BAT_Voltage_Val;
	/*固定码*/
	serial_msg.msg_send_buf[13]=0x0C;
	serial_msg.msg_send_buf[14]=0x0C;
	serial_msg.msg_send_buf[15]=0x0C;
	serial_msg.msg_send_buf[16]=0x0C;
	serial_msg.msg_send_buf[17]=0x0C;
	/*校验码*/
	serial_msg.send_crc_dat=CRC_XModem(serial_msg.msg_send_buf,send_msg_max_size-2);
	serial_msg.msg_send_buf[18]=serial_msg.send_crc_dat;
	serial_msg.msg_send_buf[19]=serial_msg.send_crc_dat>>8;
	return send_msg_len;
}
/************************************************************************/
