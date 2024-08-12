#ifndef __YMODEM_H
#define __YMODEM_H
/**
 @brief Include
 */
#include "flash.h"

/**
 @brief Function
 */
void array_inversion(flash_parm* f_parm);
void Ymodem_Send_C(uint8_t* Ymodem_tx_buf);
void Ymodem_Send_ACK(uint8_t* Ymodem_tx_buf);
void Ymodem_Send_NAK(uint8_t* Ymodem_tx_buf);
void Open_Ymodem_File_Transfer(const uint8_t* Ymodem_rx_buf);
uint8_t Ymodem_Start_Frame(const uint8_t* Ymodem_rx_buf);
uint8_t Ymodem_Process_Data(const uint8_t* Ymodem_rx_buf);
void Ymodem_End_Frame(uint8_t* Ymodem_rx_buf);
#endif
