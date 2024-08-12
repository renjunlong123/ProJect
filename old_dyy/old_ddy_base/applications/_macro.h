#ifndef ___MACRO_H
#define ___MACRO_H
/**************************************************************/
/*Return Value*/
#define Error 				0x00
#define True					0xff
/*Flag Value*/
#define flag_Error 		0x0a
#define flag_OK		 		0x0b
/*Dev name*/
#define DEV_SERIAL_1_NAME						"uart1"
#define DEV_SERIAL_2_NAME				  	"uart2"
#define SERIAL_1_RX_SEM_NAME 				"serial_1_rx_sem"
#define SERIAL_1_TX_SEM_NAME 				"serial_1_tx_sem"
#define SERIAL_2_RX_SEM_NAME 				"serial_2_rx_sem"
#define SERIAL_2_TX_SEM_NAME 				"serial_2_tx_sem"
/*serial*/
#define serial_1_rx_buf_size 						20
#define serial_1_tx_buf_size 						10
#define serial_2_rx_buf_size 						50
#define serial_2_tx_buf_size 						20
/**************************************************************/
/**
 @brief thread macro
 */
 /*main thread macro*/
#define _main_th_name                       "main"
#define _main_th_stack_size                 512
#define _main_th_priority                   20
#define _main_th_stick                      5
/*serial_rx thread macro*/
#define _serial_rx_th_name                  "serial_rx"
#define _serial_rx_th_stack_size            512
#define _serial_rx_th_priority              10
#define _serial_rx_th_stick                 5
/*serial_tx thread macro*/
#define _serial_tx_th_name                  "serial_tx"
#define _serial_tx_th_stack_size            512
#define _serial_tx_th_priority              10
#define _serial_tx_th_stick                 3
/**
 @brief OLED macro
 */
#define OLED_MODE 											0
#define SIZE 														8
#define XLevelL													0x00
#define XLevelH													0x10
#define Max_Column											128
#define Max_Row													64
#define	Brightness											0xFF 
#define X_WIDTH 												128
#define Y_WIDTH 												64	   
#define OLED_CMD  											0	/*写命令*/
#define OLED_DATA 											1	/*写数据*/
/**
 @brief BAT macro
 */
#define BAT_GDZT_AC											0
#define BAT_GDZT_DC											1
#define DYKZ_AC													0x00
#define DYKZ_BAT                        0x01
/**************************************************************/
/**
 @brief IO macro
 */
/*程序运行指示灯 PA8*/
#define _IO_RUN_STU_PORT                GPIOA
    #define _IO_RUN_STU                 GPIO_PIN_8
    #define _IO_RUN_STU_H               HAL_GPIO_WritePin(_IO_RUN_STU_PORT,_IO_RUN_STU, GPIO_PIN_SET)
    #define _IO_RUN_STU_L               HAL_GPIO_WritePin(_IO_RUN_STU_PORT,_IO_RUN_STU, GPIO_PIN_RESET)
/*485RTC PA5*/
#define _IO_485_RTC_PORT                GPIOA
    #define _IO_485_RTC                 GPIO_PIN_5
    #define _IO_485_RTC_H               HAL_GPIO_WritePin(_IO_485_RTC_PORT,_IO_485_RTC, GPIO_PIN_SET)
    #define _IO_485_RTC_L               HAL_GPIO_WritePin(_IO_485_RTC_PORT,_IO_485_RTC, GPIO_PIN_RESET)
/*AC_LED PB11*/
#define _IO_AC_LED_PORT                 GPIOB
    #define _IO_AC_LED                  GPIO_PIN_11
    #define _IO_AC_LED_H                HAL_GPIO_WritePin(_IO_AC_LED_PORT,_IO_AC_LED, GPIO_PIN_SET)
    #define _IO_AC_LED_L                HAL_GPIO_WritePin(_IO_AC_LED_PORT,_IO_AC_LED, GPIO_PIN_RESET)
/*控制信号输入信号 KZS1--PB10\KZS2--PB1*/
#define _IO_KZS_PORT                 		GPIOB
		#define _IO_KZS_1                   GPIO_PIN_10
		#define _IO_KZS_2                   GPIO_PIN_1
		#define	_IO_R_KZS_1									HAL_GPIO_ReadPin(_IO_KZS_PORT,_IO_KZS_1)
		#define	_IO_R_KZS_2									HAL_GPIO_ReadPin(_IO_KZS_PORT,_IO_KZS_2)
/*控制信号输出信号 Kz1--PA12\Kz2--PA15*/
#define _IO_KZ_PORT                			GPIOA
    #define _IO_KZ_1               		  GPIO_PIN_12
		#define _IO_KZ_2               		  GPIO_PIN_15
    #define _IO_KZ_1_H               		HAL_GPIO_WritePin(_IO_KZ_PORT,_IO_KZ_1, GPIO_PIN_SET)
    #define _IO_KZ_1_L               		HAL_GPIO_WritePin(_IO_KZ_PORT,_IO_KZ_1, GPIO_PIN_RESET)
		#define _IO_KZ_2_H               		HAL_GPIO_WritePin(_IO_KZ_PORT,_IO_KZ_2, GPIO_PIN_SET)
    #define _IO_KZ_2_L               		HAL_GPIO_WritePin(_IO_KZ_PORT,_IO_KZ_2, GPIO_PIN_RESET)
/*DHT11 温湿度传感器*/
#define _IO_DHT11_PORT                	GPIOC
    #define _IO_DHT11		               	GPIO_PIN_15
		#define _IO_DHT11_H               	HAL_GPIO_WritePin(_IO_DHT11_PORT,_IO_DHT11, GPIO_PIN_SET)
    #define _IO_DHT11_L               	HAL_GPIO_WritePin(_IO_DHT11_PORT,_IO_DHT11, GPIO_PIN_RESET)
		#define	_IO_R_DHT11									HAL_GPIO_ReadPin(_IO_DHT11_PORT,_IO_DHT11)
/*OLED PB3、PB4*/
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)//SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)//SDA
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET)
/**************************************************************/
#endif

