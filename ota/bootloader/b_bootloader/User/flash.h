#ifndef __FLASH_H
#define __FLASH_H
/**
 @brief Include
 */
#include "w25qxx.h"

/**
 @brief Function
 */
uint8_t stm_flash_write(flash_parm* f_parm,const uint32_t start_address,const uint32_t end_address,const uint8_t page_length);
uint8_t update_To_flash(flash_parm* f_parm,const uint32_t start_address,const uint32_t end_address);
void stm_flash_read(flash_parm* f_parm,const uint8_t system);
uint8_t flash_sector_erase(uint8_t pages,uint32_t address);
void flash_test(void);
#endif
