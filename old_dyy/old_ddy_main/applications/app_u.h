#ifndef __APP_U
#define __APP_U
/**
 @brief Include
 */
#include "app_data.h"
/**
 @brief Parm
 */
typedef struct{
	rt_uint8_t serial_1_rx_length;
	rt_uint8_t serial_1_tx_length;
	rt_uint8_t serial_1_rx_buf[serial_1_rx_buf_size];
	rt_uint8_t serial_2_rx_length;
	rt_uint8_t serial_2_tx_length;
	rt_uint8_t serial_2_rx_buf[serial_2_rx_buf_size];
	rt_uint8_t serial_2_tx_buf[serial_2_tx_buf_size];
}Serial_Value;
/**
 @brief Function
 */
uint8_t u_init(void);
uint8_t rt_create_thread_init(void);
#endif
