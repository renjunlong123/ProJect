#ifndef __DEFINITION_H
#define __DEFINITION_H

/**
 @brief Include
 */
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include <string.h>
#include "dma.h"
/**
 @brief Parm
 */
/*FLASH*/
#define FLASH_DATA_MAX_LENGTH              	64
#define FLASH_READ_BYTE											0x08
#define FLASH_READ_HALFWORD									0x16
#define FLASH_READ_WORD                    	0x32
typedef struct
{
	uint32_t address;
	uint32_t end_address;
	uint16_t length;																		/*字的长度*/
	uint32_t w_data[FLASH_DATA_MAX_LENGTH];
	uint32_t r_data_word[FLASH_DATA_MAX_LENGTH];
	uint8_t  r_data_byte[FLASH_DATA_MAX_LENGTH*4];
}flash_parm;

typedef struct
{
	uint8_t ota_flag_buf[4];
	uint8_t download_data_size[4];
}ota_parm;

typedef struct
{
#define STM32_PAGE_SIZE                   1024
	uint8_t updata_buf[STM32_PAGE_SIZE];
	uint8_t w25qxx_block_offset;
}updata_parm;

extern ota_parm ota_parm_info;
#define OTA_PARM_INFO_SIZE sizeof(ota_parm_info)
extern updata_parm updata_parm_info;
extern uint32_t BootState_Flag;
extern UART_HandleTypeDef huart1;
extern flash_parm flash_parm_info;
#define serial_rx_cmd_buf_length 					10
extern uint8_t serial_rx_cmd_buf[serial_rx_cmd_buf_length];
#define serial_rx_cmd_buf_init memset(serial_rx_cmd_buf,0,serial_rx_cmd_buf_length)
extern uint8_t rx_flag;
extern uint8_t jump_to_flag;
#define YMODEM_RECEIVE_START_FRAME_NO_FINISH     1
#define YMODEM_RECEIVE_START_FRAME_FINISH        0
extern uint8_t Receive_Start_Frame_Flag;
#define YMODEM_RECEIVE_DATA_FRAME_NO_FINISH      1
#define YMODEM_RECEIVE_DATA_FRAME_FINISH         0
extern uint8_t Receive_Data_Frame_Flag;
extern uint8_t Data_Package_Flag;
/**
 @brief Macro
 */
/*FLASH*/
#define STM32_FLASH_ADDR								   0x08000000
#define STM32_PAGE_NUM  								   64
#define STM32_B_PAGE_NUM								   20
#define STM32_A_PAGE_NUM								   (STM32_PAGE_NUM - STM32_B_PAGE_NUM)
#define STM32_B_PAGE_ADDR								   0x08000000
#define STM32_A_PAGE_ADDR								   STM32_B_PAGE_ADDR+(STM32_B_PAGE_NUM * STM32_PAGE_SIZE)
#define STM32_FLASH_END_ADDR							 (STM32_FLASH_ADDR+(STM32_PAGE_NUM * STM32_PAGE_SIZE))
/*BOOTLOADER*/
#define BOOTLOART_LOG
/*OTA*/
#define OTA_FLAG_Update_indicate 					  0xaabb1122
#define OTA_FLAG_ADDR			                  0x000000	
/*w25q64 parm macro*/
#define W25Qxx_Write_BLOCK_1_Addr					  0x000000	/*关键参数*/	
#define W25Qxx_Write_BLOCK_2_Addr					  0x010000	/*新程序地址*/	
#define W25Qxx_Write_BLOCK_3_Addr					  0x020000 /*备份当前程序起始地址*/

#define APP_FLAG_INIT												0x00000000
#define APP_FLAG_UPDATE											0x00000001

/*Ymodem*/
#define YMODEM_RX_BUF_LEN 				          140
#define YMODEM_TX_BUF_LEN 				          1
#define YMODEM_DATA_PACKAGE_LEN	  					128
#define YMODEM_DATA_PACKAGE_LEN_WORD	  		(YMODEM_DATA_PACKAGE_LEN / 4)
typedef struct
{
	uint8_t Ymodem_rx_buf[YMODEM_RX_BUF_LEN];
	uint8_t Ymodem_tx_buf[YMODEM_TX_BUF_LEN];
}Ymodem_buf_parm;

#define YMODEM_START												 '1'


#define YMODEM_C														 0x43															
#define YMODEM_SOH													 0x01		 /*128字节数据包帧头*/
#define YMODEM_EOT													 0x04		 /*结束传输*/			
#define YMODEM_ACK													 0x06		 /*应答回应*/
#define YMODEM_NAK													 0x15		 /*错误应该,重传数据*/
#define YMODEM_CAN													 0x18		 /*取消传输*/

/*Return Value*/
#define _Error 	                             0x00
#define _OK			                             0xff

/*IO*/
#define spi_cs_h HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define spi_cs_l HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
/*W25QXX*/
#define W25Qxx_Write_Page_Addr_1											 0x000000											 
#define W25Qxx_Write_Page_Addr_2											 0x000000		
#define W25Qxx_WRITE_ENABLE                            0x06
#define W25Qxx_WRITE_DISABLE                           0x04
#define W25Qxx_READ_STATUS_REGISTER_1                  0x05
#define W25Qxx_READ_STATUS_REGISTER_2                  0x35
#define W25Qxx_WRITE_STATUS_REGISTER                   0x01
#define W25Qxx_PAGE_PROGRAM                            0x02
#define W25Qxx_QUAD_PAGE_PROGRAM                       0x32
#define W25Qxx_BLOCK_ERASE_64KB                        0xD8
#define W25Qxx_BLOCK_ERASE_32KB                        0x52
#define W25Qxx_SECTOR_ERASE_4KB                        0x20
#define W25Qxx_CHIP_ERASE                              0xC7
#define W25Qxx_ERASE_SUSPEND                           0x75
#define W25Qxx_ERASE_RESUME                            0x7A
#define W25Qxx_POWER_DOWN                              0xB9
#define W25Qxx_HIGH_PERFORMANCE_MODE                   0xA3
#define W25Qxx_CONTINUOUS_READ_MODE_RESET              0xFF
#define W25Qxx_RELEASE_POWER_DOWN_HPM_DEVICE_ID        0xAB
#define W25Qxx_MANUFACTURER_DEVICE_ID                  0x90
#define W25Qxx_READ_UNIQUE_ID                          0x4B
#define W25Qxx_JEDEC_ID                                0x9F
#define W25Qxx_READ_DATA                               0x03
#define W25Qxx_FAST_READ                               0x0B
#define W25Qxx_FAST_READ_DUAL_OUTPUT                   0x3B
#define W25Qxx_FAST_READ_DUAL_IO                       0xBB
#define W25Qxx_FAST_READ_QUAD_OUTPUT                   0x6B
#define W25Qxx_FAST_READ_QUAD_IO                       0xEB
#define W25Qxx_OCTAL_WORD_READ_QUAD_IO                 0xE3

#endif
