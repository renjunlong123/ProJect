#ifndef __APP_DATA_H
#define __APP_DATA_H
/**
 @brief include
 */
#include "24c02.h"
/**
 @brief 
 */
 /*serial报文*/
typedef struct
{
	uint8_t Inspection_code[msg_Inspection_code_len];
	uint8_t msg_rec_buf[rev_msg_max_size];
	uint8_t msg_send_buf[send_msg_max_size];
	uint16_t rev_crc_dat;
	uint16_t send_crc_dat;
}msg;
extern msg serial_msg;

 /*ADC Val*/
typedef struct
{
	uint16_t BAT_Temp_Val_1;
	uint16_t BAT_Temp_Val_2;
	uint16_t BAT_Temp_Val_3;
	uint16_t BAT_Voltage_Val;
	uint16_t BAT_CDFD_Val;
	uint32_t BAT_CDFD_Current;
}adc;
typedef struct{
	uint32_t Bat_ZRL;
	uint32_t Bat_Now_ZRL;
	uint8_t Bat_After_DL;
	uint8_t Bat_Now_DL;
	uint8_t Bat_DL_Flag;
	uint32_t Bat_save_times;
	float Bat_Vol_BC;
}Bat;
/*24c02 Val*/
typedef struct
{
	uint8_t at24c02_rxbuf_BAT[20];
//	uint8_t at24c02_rxbuf_BAT[Save_24c02_Addr_Len];
	uint8_t at24c02_rxbuf_BAT_DL[Save_24c02_BAT_DL_len];
	uint8_t at24c02_txbuf_BAT_1[Save_24c02_Page_1_len];
	uint8_t at24c02_txbuf_BAT_2[Save_24c02_Page_2_len];
}at24c02;
/**
 @brief function
 */
extern adc adc_val;
extern Bat Bat_Val;
/*控制引脚*/
uint8_t R_Bat_Stu(void);
uint8_t R_BAT_CD_STU(void);
uint8_t R_BAT_FD_STU(void);
uint8_t R_BAT_FLOW_EQUALIZE(void);
/*adc*/
void Open_Adc(void);
void Again_Open_Adc(void);
uint16_t Get_Bat_Voltage(void);
uint8_t Get_Bat_Temp(void);
void Get_Bat_Current_Val(void);
void Get_Bat_DL_Val(void);
/*BAT PARM*/
void Bat_ZRL_MAX_Init(void);
uint8_t Bat_ZRL_MIN_Init(void);
uint8_t SD_READ_DL_M24c02(void);
/*Serial*/
uint8_t serial_msg_parsing(const uint8_t* msg_buf);
uint8_t serial_send_msg(void);
#endif

