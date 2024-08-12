#ifndef __OLED_IIC_H
#define __OLED_IIC_H
/**
 @brief Include
 */
#include "dth11.h"
/**
 @brief Function
 */
void _Oled_Init(void);
void OLED_On(void);
void OLED_Clear(void);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
#endif
