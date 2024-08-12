#ifndef __24C02_H
#define __24C02_H

/**
 @brief include
 */
#include "i2c.h"

/**
 @brief function
 */
void AT24C02_INIT(void);
uint8_t AT24C02_W_Byte(uint8_t RegAddress,uint8_t data);
uint8_t AT24C02_W_nByte(uint8_t* pdata_buf,uint8_t RegAddress,uint8_t Data_len);
uint8_t AT24C02_R_nByte(uint8_t* pdata_buf,uint8_t RegAddress,uint16_t Data_len);
void AT24C02_Erase(void);
void AT24C02_Test(void);

#endif
