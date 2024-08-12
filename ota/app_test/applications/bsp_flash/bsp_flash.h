#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

/**
 @brief Include
 */
#include "dev_init.h"

typedef struct
{
	uint32_t address;
	uint32_t end_address;
	uint16_t length;
	uint32_t w_data[FLASH_DATA_MAX_LENGTH];
	uint32_t r_data_word[FLASH_DATA_MAX_LENGTH];
	uint16_t r_data_halfword[FLASH_DATA_MAX_LENGTH*2];
	uint8_t r_data_byte[FLASH_DATA_MAX_LENGTH*4];
}FlashParm_TypeDef;

extern FlashParm_TypeDef f;
/**
 @brief function
 */
uint8_t STM_Flash_Write(FlashParm_TypeDef* f_parm,uint32_t start_address,uint32_t end_address,uint8_t page_length);
void STM_Flash_Read(FlashParm_TypeDef* f_parm,const uint8_t system);

#endif
