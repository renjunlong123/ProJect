#ifndef __W25QXX_H
#define __W25QXX_H
/**
 @brief Include
 */
#include "bsp_flash.h"

/**
 @brief function
 */
void spi_w25qxx_read_id(void);
void spi_w25qxx_write_enable(void);
void spi_w25qxx_write_disable(void);
void spi_w25qxx_erase(uint32_t w25qxx_addr,uint8_t sector_size);
void spi_w25qxx_erase_chip(void);
void spi_w25qxx_write_page(uint32_t w25qxx_addr,void *DataArray, uint32_t Count);
uint8_t spi_w25qxx_continuous_write_page(uint32_t w25qxx_addr,uint16_t *DataArray, uint32_t Count,uint8_t page_length);
void spi_w25qxx_read(uint32_t w25qxx_addr, void *DataArray, uint32_t Count);
void spi_w25qxx_powerdown(void);
void wakeup_addr_wakeup(void);
#endif
