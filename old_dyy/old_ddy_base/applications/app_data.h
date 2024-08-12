#ifndef __APP_DATA_H
#define __APP_DATA_H
/**
 @brief Include
 */
#include "oled_chinese.h"
#include "dth11.h"
/*SERIAL DATA*/
typedef struct{
	uint16_t Serial_2_TXCRC;
	uint16_t Serial_2_RXCRC;
	uint8_t Serial_2_RxInspection[5]; 
	uint8_t Serial_2_Txbuf[serial_2_tx_buf_size];
}SerialMsg;
void uart1_send_data_loading(void);
uint8_t Proccess_Serial_1_RxData(const uint8_t* Serial_1_RxData);
uint8_t Proccess_Serial_2_RxData(const uint8_t* Serial_2_RxData);
uint8_t Serial_2_485_Send_Data(uint8_t* Serial_2_TxData);
#endif
