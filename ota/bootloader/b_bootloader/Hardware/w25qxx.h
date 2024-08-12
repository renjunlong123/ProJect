#ifndef __W25QXX_H
#define __W25QXX_H

/**
 @brief Include
 */
#include "definition.h"

/**
 @brief Function
 */
void w25qxx_readid(uint8_t* mid,uint16_t* did);
void w25qxx_dma_readid(uint8_t* mid,uint16_t* did);
void w25qxx_write_enable(void);
uint8_t w25qxx_waitbusy(void);
void w25qxx_sectorerase(uint32_t address);
void w25qxx_blockerase(uint32_t address);
void w25qxx_chiperase(void);
void w25qxx_PageProgram(uint32_t address, uint8_t *DataArray, uint32_t Count);
void w25qxx_ReadData(uint32_t address, uint8_t *DataArray, uint32_t Count);
void w25qxx_test(void);
#endif
