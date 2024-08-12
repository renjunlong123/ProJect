#ifndef __APP_DATA_H
#define __APP_DATA_H
/******************************************************************************/
/**
 @brief Include
 */
#include "m24c02.h"
/******************************************************************************/
/*ADC Val*/
typedef struct{
  uint16_t BAT_Temp_Val_1;
  uint16_t BAT_Temp_Val_2;
  uint16_t BAT_Voltage_Val;
	uint16_t BAT_CD_Val;
	uint16_t BAT_FD_Val;
	uint16_t BAT_Zero_Val;
  uint16_t BAT_CD_Current;
	uint16_t BAT_FD_Current;
}adc;
/*SERIAL DATA*/
typedef struct{
	uint8_t Serial_1_RxInspection[7]; 
	uint8_t Serial_1_Txbuf[SERIAL_1_TXLEN];
	uint16_t Serial_1_RXCRC;
	uint16_t Serial_1_TXCRC;
	uint16_t Serial_2_TXCRC;
	uint16_t Serial_2_RXCRC;
	uint8_t Serial_2_RxInspection[5]; 
	uint8_t Serial_2_Txbuf[SERIAL_2_TXLEN];
}SerialMsg_1;
/*BAT DATA*/
typedef struct{
	uint32_t Bat_ZRL;
	uint32_t Bat_Now_ZRL;
	uint8_t Bat_After_DL;
	uint8_t Bat_Now_DL;
	uint8_t Bat_DL_Flag;
	uint32_t Bat_save_times;
	float Bat_Vol_BC;
}BatVal;
/*BAT STU FLAG*/
typedef struct{
	uint8_t Bat_V_flag;
	uint8_t Bat_Temp_flag;
	uint8_t Bat_Flow_Equalize_flag;
}BatSTU;
/*24c02 Val*/
typedef struct
{
	uint8_t at24c02_rxbuf_BAT[Save_24c02_Addr_Len];
//	uint8_t at24c02_rxbuf_BAT_DL[Save_24c02_BAT_DL_len];
	uint8_t at24c02_txbuf_BAT_1[Save_24c02_Page_1_len];
	uint8_t at24c02_txbuf_BAT_2[Save_24c02_Page_2_len];
}at24c02;
extern adc adc_val;
extern SerialMsg_1 Serial_Msg_1;
extern BatVal _BatVal;
extern BatSTU BatSTU_flag;
extern uint8_t Serial_2_Txbuf[20];
/******************************************************************************/
/**
 @brief Function
 */
uint8_t CD_LED(void);
void RUN_LED(void);
void SDJC_CDKZ(void);
void Bat_ZRL_MAX_Init(void);
void Bat_ZRL_MIN_Init(void);
uint8_t BAT_Flow_Equalize(void);
void Open_Adc(void);
void Again_Open_Adc(void);
void Get_BAT_Zero_Val(void);
uint16_t Get_Bat_Voltage(void);
uint8_t Get_Bat_Temp(void);
void Get_Bat_CDFD_Electricity(void);
void Get_Bat_DL(void);
uint8_t SD_READ_DL_M24c02(void);
uint8_t Proccess_Serial_1_RxData(const uint8_t* Serial_1_RxData);
uint8_t Proccess_Serial_1_TxData(void);
uint8_t Proccess_Serial_2_RxData(const uint8_t* Serial_2_RxData);
uint8_t Proccess_Serial_2_TxData(void);
/******************************************************************************/
#endif
