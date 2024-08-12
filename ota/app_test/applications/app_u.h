#ifndef __APP_U
#define __APP_U

/**
 @brief Include
 */
#include "data.h"

/**
 @brief Parm
 */
typedef struct
{
	rt_uint8_t serial_1_rx_length;
	rt_uint8_t serial_1_tx_length;
	rt_uint8_t serial_1_rx_buf[SERIAL_RX_BUF_LEN];
	rt_uint8_t serial_1_tx_buf[SERIAL_TX_BUF_LEN];
}Serial_Value;

/**
 @brief Function
 */
uint8_t u_init(void);
uint8_t rt_create_thread_init(void);
#endif
