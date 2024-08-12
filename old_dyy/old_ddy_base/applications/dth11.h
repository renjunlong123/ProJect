#ifndef __DTH11_H
#define __DTH11_H
#include "dev_init.h"
typedef struct{
	uint8_t _hum_H;			/*湿度高八位--整数*/
	uint8_t _hum_L;			/*湿度低八位--小数*/
	uint8_t _temp_H;		/*温度高八位--整数*/
	uint8_t _temp_L;		/*温度低八位--小数*/
	uint8_t check_node;	/*校验码*/
}_dth11;
extern _dth11 _dth11_data;
uint8_t DHT11_Init (void);
uint8_t DHT11_Read_Data(void);
#endif
