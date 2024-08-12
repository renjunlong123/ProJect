#ifndef __OLED_CHINESE_H
#define __OLED_CHINESE_H
/**
 @brief Include
 */
#include "oled_iic.h"
/**
 @brief Parm
 */
 /*BAT DATA*/
typedef struct{
	uint8_t Bat_DL;
	uint8_t Bat_GDZT;
	uint32_t Bat_CD_DL;
	uint32_t Bat_FD_DL;
	uint16_t Bat_TEMP;
	uint8_t Bat_voltage;
	uint16_t XT_TEMP;
}BatVal;
extern BatVal _BatVal;
/**
 @brief Function
 */
void oled_SD(void);
void oled_show_Bat_parm(void);
#endif
