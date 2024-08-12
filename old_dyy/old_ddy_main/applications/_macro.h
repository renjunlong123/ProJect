#ifndef ___MACRO_H
#define ___MACRO_H

/*Return Value*/
#define True  											0x01
#define Error 											0x00
/*Flag Value*/
#define flag_Error 									0x0A
#define flag_OK		 									0x0B
/*Dev name*/
#define DEV_SERIAL_1_NAME					  "uart1"
#define SERIAL_1_RX_SEM_NAME 				"serial_1_rx_sem"
#define SERIAL_1_TX_SEM_NAME 				"serial_1_tx_sem"
#define DEV_SERIAL_2_NAME				  	"uart2"
#define SERIAL_2_RX_SEM_NAME 				"serial_2_rx_sem"
#define SERIAL_2_TX_SEM_NAME 				"serial_2_tx_sem"
#define TIMEOUT_1_sem 							"timeout_1_sem"
/*serial*/
#define serial_1_rx_buf_size 				10
#define serial_2_rx_buf_size 				10
#define serial_2_tx_buf_size 				20
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
#define _serial_rx_th_stack_size            256
#define _serial_rx_th_priority              10
#define _serial_rx_th_stick                 5
/*serial_tx thread macro*/
#define _serial_tx_th_name                  "serial_tx"
#define _serial_tx_th_stack_size            256
#define _serial_tx_th_priority              10
#define _serial_tx_th_stick                 5
/*TIMEOUT thread create*/
#define _TimeOut_th_name                  	"TimeOut"
#define _TimeOut_th_stack_size          	  512
#define _TimeOut_th_priority              	15
#define _TimeOut_th_stick                   5
/**
 @brief IO macro
 */
/*程序运行指示灯 PB12*/
#define _IO_RUN_STU_PORT                GPIOB
    #define _IO_RUN_STU                 GPIO_PIN_12
    #define _IO_RUN_STU_H               HAL_GPIO_WritePin(_IO_RUN_STU_PORT,_IO_RUN_STU, GPIO_PIN_SET)
    #define _IO_RUN_STU_L               HAL_GPIO_WritePin(_IO_RUN_STU_PORT,_IO_RUN_STU, GPIO_PIN_RESET)
/*充电指示灯 PB13*/
#define _IO_CD_LED_PORT                 GPIOB
    #define _IO_CD_LED                  GPIO_PIN_13
    #define _IO_CD_LED_H                HAL_GPIO_WritePin(_IO_CD_LED_PORT,_IO_CD_LED, GPIO_PIN_SET)
    #define _IO_CD_LED_L                HAL_GPIO_WritePin(_IO_CD_LED_PORT,_IO_CD_LED, GPIO_PIN_RESET)
/*充电控制脚 PA1 低电平--充电*/
#define _IO_CDKZ_PORT                   GPIOA
    #define _IO_CDKZ                    GPIO_PIN_1
    #define _IO_CDKZ_H                  HAL_GPIO_WritePin(_IO_CDKZ_PORT,_IO_CDKZ, GPIO_PIN_SET)
    #define _IO_CDKZ_L                  HAL_GPIO_WritePin(_IO_CDKZ_PORT,_IO_CDKZ, GPIO_PIN_RESET)
		#define _IO_READ_CDKZ								HAL_GPIO_ReadPin(_IO_CDKZ_PORT,_IO_CDKZ)
/*放电控制脚 PB11 低电平--放电*/
#define _IO_FDKZ_PORT                   GPIOB
    #define _IO_FDKZ                    GPIO_PIN_11
    #define _IO_FDKZ_H                  HAL_GPIO_WritePin(_IO_FDKZ_PORT,_IO_FDKZ, GPIO_PIN_SET)
    #define _IO_FDKZ_L                  HAL_GPIO_WritePin(_IO_FDKZ_PORT,_IO_FDKZ, GPIO_PIN_RESET)		
/*供电状态引脚  PB10*/
#define _IO_POWER_STU_PORT              GPIOB
    #define _IO_POWER_STU               GPIO_PIN_10
		#define _IO_READ_POWER_STU 					HAL_GPIO_ReadPin(_IO_POWER_STU_PORT,_IO_POWER_STU)
/*电源控制脚 PC13 低电平--交流 高电平--电池*/
#define _IO_DYKZ_PORT                   GPIOC
    #define _IO_DYKZ                    GPIO_PIN_13
    #define _IO_DYKZ_H_BAT              HAL_GPIO_WritePin(_IO_DYKZ_PORT,_IO_DYKZ,GPIO_PIN_SET)
    #define _IO_DYKZ_L_AC               HAL_GPIO_WritePin(_IO_DYKZ_PORT,_IO_DYKZ,GPIO_PIN_RESET)
		#define _IO_R_DYKZ									HAL_GPIO_ReadPin(_IO_DYKZ_PORT,_IO_DYKZ) 
/*电池状态采集脚 低电平表示电池正在均流，高电平表示电池未均流*/
#define _IO_BAT_STU_PORT_1              GPIOA
    #define _IO_BAT_STU_1               GPIO_PIN_15
#define _IO_BAT_STU_PORT_2_6            GPIOB
    #define _IO_BAT_STU_2               GPIO_PIN_3
    #define _IO_BAT_STU_3               GPIO_PIN_4
    #define _IO_BAT_STU_4               GPIO_PIN_5
    #define _IO_BAT_STU_5               GPIO_PIN_6
		#define _IO_BAT_STU_6               GPIO_PIN_7
#define	_IO_R_BAT_STU_1									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_1,_IO_BAT_STU_1)
#define	_IO_R_BAT_STU_2									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_2_6,_IO_BAT_STU_2)
#define	_IO_R_BAT_STU_3									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_2_6,_IO_BAT_STU_3)
#define	_IO_R_BAT_STU_4									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_2_6,_IO_BAT_STU_4)
#define	_IO_R_BAT_STU_5									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_2_6,_IO_BAT_STU_5)
#define	_IO_R_BAT_STU_6									HAL_GPIO_ReadPin(_IO_BAT_STU_PORT_2_6,_IO_BAT_STU_6)
/*24C02 i2c引脚*/
#define _IO_24C02_PORT                  GPIOC
    #define _IO_24C02_SCL               GPIO_PIN_14
    #define _IO_24C02_SDA               GPIO_PIN_15
    #define _IO_24C02_SCL_H             HAL_GPIO_WritePin(_IO_24C02_PORT,_IO_24C02_SCL, GPIO_PIN_SET)
    #define _IO_24C02_SCL_L             HAL_GPIO_WritePin(_IO_24C02_PORT,_IO_24C02_SCL, GPIO_PIN_RESET)
    #define _IO_24C02_SDA_H             HAL_GPIO_WritePin(_IO_24C02_PORT,_IO_24C02_SDA, GPIO_PIN_SET)
    #define _IO_24C02_SDA_L             HAL_GPIO_WritePin(_IO_24C02_PORT,_IO_24C02_SDA, GPIO_PIN_RESET)
    #define _IO_24C02_SDA_R             HAL_GPIO_ReadPin(_IO_24C02_PORT,_IO_24C02_SDA); 	
/**
 @brief Other macro
 */
 /*i2c*/
#define iic_WR      										0
#define iic_RD      										1
/*k24c02 2k*/
#define Save_24c02_Addr_Len  						16
#define Save_24c02_BAT_DL_len      		  2
#define Save_24c02_Page_1_len       		8
#define Save_24c02_Page_2_len       		8
#define AT24C02                     		0xA0						/*地址*/
#define AT24C02_PAGE_SIZE           		8               /*单个周期内最多写入16页*/
#define AT24C02_PAGE_LEN            		8               /*每一页的长度*/
#define AT24C02_SIZE               		  256             /*总容量*/

#define ADC_BUF_SIZE                		20
#define ADC_RANK			              		5
#define SERIAL_1_RXLEN								  10
#define SERIAL_1_TXLEN								  20
#define SERIAL_2_RXLEN								  10
#define SERIAL_2_TXLEN								  20
#define DYKZ_AC													0x00
#define DYKZ_BAT                        0x01
/*BAT FLAG*/
#define BAT_V_MAX												0x01
#define BAT_V_MID												0x02
#define BAT_V_MIN												0x03
#define BAT_TEMP_MAX										0x00
#define BAT_TEMP_NORMAL									0x01
#define BAT_DL_MIN_FLAG									0xA1
#define BAT_DL_MID_FLAG									0xA2
#define BAT_DL_MAX_FLAG									0xA3
#define BAT_DL_INIT_FLAG								0xA4
#define BAT_NO_FLOW_EQUALIZE						0x00
#define BAT_FLOW_EQUALIZE								0x01
/*event*/
#define event_bit_1         						(0x01<<0)	 	/*事件1 接收完成*/
#define event_bit_2         						(0x01<<1)	 	/*事件2 发送完成*/
#define event_bit_3         				  	(0x01<<2)   /*事件3 接收数据错误*/
#define event_4_BAT_V_MAX   						(0x01<<3)   /*事件4 充电限制电压,停止充电*/
#define event_5_BAT_V_MID	  						(0x01<<4)   /*事件5 电压正常,电池进行充电放电*/
#define event_6_BAT_V_MIN	    					(0x01<<5)   /*事件6 放电终止电压,停止放电*/

#define event_7_TEMP_H   	  						(0x01<<6)   /*事件7 电池温度过高，电池不能充电,进行电池保护*/
#define event_8_TEMP_NORMAL   	  			(0x01<<7)   /*事件8 电池温度正常*/

#define event_12_BAT_FLOW_EQUALIZE  		(0x01<<11)  /*事件12 电池均流*/
#define event_13_BAT_NOFLOW_EQUALIZE  	(0x01<<12)  /*事件13 电池未均流*/
#endif
