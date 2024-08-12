#ifndef __DATA_H
#define __DATA_H
/**
 @brief Include
 */
#include "Ymodem.h"

/**
 @brief Function
 */
uint32_t cal_ota_flag(const uint8_t* ota_flag_buf);
uint32_t cal_download_data_size(const uint8_t* download_data_size);
void user_select_menu(void);
void backup_program(void);
void delete_app_program(void);
void update_backup_program(void);
void update_app_program(void);
#endif
