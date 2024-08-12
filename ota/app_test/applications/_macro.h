#ifndef ___MACRO_H
#define ___MACRO_H

/*Return Value*/
#define _Error 	                           0x00
#define _OK			                           0xff
#define flag_Error 		                     0x0a
#define flag_OK		 	                       0x0b
/*io macro*/
#define SPI_1_CS_IO_PORT									 GPIOA
#define SPI_1_CS_IO												 GPIO_PIN_4
#define SPI_1_CS_IO_H											 HAL_GPIO_WritePin(SPI_1_CS_IO_PORT, SPI_1_CS_IO, GPIO_PIN_SET);
#define SPI_1_CS_IO_L											 HAL_GPIO_WritePin(SPI_1_CS_IO_PORT, SPI_1_CS_IO, REGPIO_PIN_SET);
/*Dev name*/
#define DEV_SERIAL_1_NAME					         "uart1"
#define SERIAL_1_RX_SEM_NAME 				       "serial_1_rx_sem"
#define SERIAL_1_TX_SEM_NAME 				       "serial_1_tx_sem"
/*FLASH*/
#define FLASH_DATA_MAX_LENGTH              0xFF
#define FLASH_READ_BYTE								     0x08
#define FLASH_READ_HALFWORD							   0x16
#define FLASH_READ_WORD                    0x32
/*OTA*/
#define OTA_FLAG_Update_indicate 					 0xaabb1122
#define OTA_FLAG_init			                 0x00000001	
/*SERIAL*/
#define SERIAL_TX_BUF_LEN									 128
#define SERIAL_RX_BUF_LEN									 128
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
#define _serial_rx_th_priority              8
#define _serial_rx_th_stick                 5
/*serial_tx thread macro*/
#define _serial_tx_th_name                  "serial_tx"
#define _serial_tx_th_stack_size            512
#define _serial_tx_th_priority              10
#define _serial_tx_th_stick                 5

/*w25q64 parm macro*/
#define W25Qxx_Write_BLOCK_1_Addr					             0x000000	/*关键参数*/	
#define W25Qxx_Write_BLOCK_2_Addr					             0x010000	/*新程序地址*/	
#define W25Qxx_Write_BLOCK_3_Addr					             0x020000 /*备份当前程序起始地址*/


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
