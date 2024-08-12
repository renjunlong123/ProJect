#ifndef __APP_THREAD_H
#define __APP_THREAD_H

/**
 @brief include
 */
#include "app_data.h"

/**
 @brief parm
 */
typedef struct
{
	rt_uint8_t serial_1_rx_length;
	rt_uint8_t serial_1_tx_length;
	rt_uint8_t serial_1_rx_buf[serial_1_rx_buf_size];

	
	rt_uint8_t serial_2_485_rx_length;
	rt_uint8_t serial_2_485_tx_length;
	rt_uint8_t serial_2_485_rx_buf[serial_2_485_rx_buf_size];
	
	rt_uint8_t serial_2_485_tx_error_buf[serial_2_485_tx_error_size];
}serial;

/**
 @brief function
 */
rt_uint8_t init(void);
rt_uint8_t thread_init(void);
void _error(void);
#endif
