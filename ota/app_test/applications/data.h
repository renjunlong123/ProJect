#ifndef __DATA_H
#define __DATA_H

/**
 @brief Include
 */
#include "w25qxx.h"
/**
 @brief parm
 */
#define ota_flag_length 4
typedef struct
{
	uint8_t ota_flag_buf[ota_flag_length];
}ota_parm;


#define code_data_package_length                128
#define code_data_package_buf_max_length        100
typedef struct
{
	uint8_t code_data_package_buf[code_data_package_length];
	uint8_t code_data_package_buf_1[code_data_package_buf_max_length];
}data_package;

#define code_data_package_buf_1_init memset(code_data_package_buf_1,0,code_data_package_buf_max_length)

/**
 @brief function
 */
uint8_t ymode_process_data(uint8_t* serial_rx_buf);
void w25q64_enable(void);
void w25q64_block_1_init(uint32_t ota_flag_tmp);
void w25q64_block_2_init(void);
void w25q64_block_3_init(void);
#endif
