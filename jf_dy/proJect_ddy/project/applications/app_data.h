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

/*BAT Val*/
typedef struct
{
  uint32_t Bat_ZRL;
  uint32_t Bat_NOW_ZRL;
  uint16_t Bat_After_DL;
  uint16_t Bat_Now_DL;
  uint8_t  Bat_Run_flag;
  uint8_t  Bat_DL_MIN_Init_flag;
	uint8_t  Bat_DL_MAX_Init_flag;
  uint8_t  Bat_DL_Save_flag;
  uint32_t Bat_Dat_Save_Times;
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
 /*CKP报文*/
typedef struct
{
	uint8_t     head[2];
	uint8_t     lenth[1];
	uint8_t     cmd[1];
	uint8_t     add[2];
	uint8_t     txzt[8];             //通信状态   正常或断线
	uint8_t     gdzt[8];             //供电状态   交流或直流
	uint8_t     dcdy[8];             //电池电压
	uint8_t     dcdl[8];             //电池电量
	uint8_t     cfddl[8];            //充放电电流
	uint8_t     dcwd[8];             //电池温度
	//uint8_t     jhzt[8];             //均衡状态
	uint8_t     ver[4];              //版本号
	uint8_t     value_COM[4];        //通信状态值
	uint8_t     value_EP[4];         //供电状态
	//uint8_t     jh_value[8];         //均衡状态值
	uint8_t     Voltage[2];          //电池电压
	uint8_t     electricity[2];      //电池电量
	uint8_t     cfd_current[2];      //充电电流
	uint8_t     temp_Battery[2];     //电池温度
	uint8_t     end[2];
}CKP_msg;

/**
 @brief function
 */
uint16_t crc_check(uint8_t* buf,uint16_t len);
void Open_Adc(void);
void Again_Open_Adc(void);
uint16_t Get_Bat_Voltage(void);
uint8_t Get_Bat_Temp(void);
void Get_Bat_Current_Val(void);
void Bat_DL_MIN_Loading_Parm(void);
void Bat_DL_MAX_Loading_Parm(void);
void at24c02_SDJC_BAT_STU(void);
void at24c02_JC_BAT_STU(void);
void Get_Bat_DL_Val(void);
uint8_t R_Bat_Stu(void);
uint8_t R_BAT_CD_STU(void);
uint8_t R_BAT_FD_STU(void);
uint8_t R_BAT_FLOW_EQUALIZE(void);
uint8_t serial_msg_parsing(const uint8_t* msg_buf);
uint8_t serial_send_msg(void);
void at24c02_test(void);
#endif

