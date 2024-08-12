#include "data.h"



inline uint32_t cal_ota_flag(const uint8_t* ota_flag_buf)
{
	return (ota_flag_buf[0]<<24)+(ota_flag_buf[1]<<16)+(ota_flag_buf[2]<<8)+(ota_flag_buf[3]);
}

inline uint32_t cal_download_data_size(const uint8_t* download_data_size)
{
	return (download_data_size[0]<<24)+(download_data_size[1]<<16)+(download_data_size[2]<<8)+(download_data_size[3]);
}

inline void user_select_menu(void)
{
	printf("/**************************************************/\r\n");
	printf("/****************     User Menu    ****************/\r\n");
	printf("*    Enter UpDate Menu                        	 1*\r\n");
	printf("*    Load App Program                         	 2*\r\n");
	printf("*    Backup Flash Program To W25Qxx           	 3*\r\n");
	printf("*    Update Backup Program                    	 4*\r\n");
	printf("*    Delete App Program                       	 5*\r\n");
	printf("/**************************************************/\r\n");
	printf("\r\n");
}

/**
 @brief 备份程序FLASH标志位设置
 */
void backup_program_success_flag_init(void)
{
#define BACK_PROGRAM_FLAG_OFFEST       (0xFC00+0x300)
#define BACK_PROGRAM_FLAG_ADDRESS      (STM32_FLASH_ADDR + BACK_PROGRAM_FLAG_OFFEST)
#define BACK_PROGRAM_FLAG 						 0x44332211
	
	flash_parm_info.address=BACK_PROGRAM_FLAG_ADDRESS;
	flash_parm_info.length=1;
	for(uint8_t i=0;i<flash_parm_info.length;i++)
	{
		flash_parm_info.w_data[i]=BACK_PROGRAM_FLAG;
	}
	stm_flash_write(&flash_parm_info,(STM32_FLASH_ADDR+0xFC00),STM32_FLASH_END_ADDR,1);
}

/**
 @brief 备份程序
 */
void backup_program(void)
{
	uint16_t offest=256;
/*#define BACK_PROGRAM_FLAG_INIT*/
#define BACK_PROGRAM_FLAG_INIT
#ifdef BACK_PROGRAM_FLAG_INIT
	backup_program_success_flag_init();
#endif
	
/*#define W25QXX_INFO_LOG*/
#ifdef W25QXX_INFO_LOG
	uint8_t buf[256]={0};
#endif
	
	w25qxx_blockerase(W25Qxx_Write_BLOCK_3_Addr);
	for(uint8_t i=0 ; i<= ((STM32_A_PAGE_NUM * STM32_PAGE_SIZE)/offest) ; i++)
	{
		flash_parm_info.address=STM32_A_PAGE_ADDR+offest*i;
		flash_parm_info.length=FLASH_DATA_MAX_LENGTH;
		stm_flash_read(&flash_parm_info,FLASH_READ_BYTE);
		w25qxx_PageProgram(W25Qxx_Write_BLOCK_3_Addr+offest*i,flash_parm_info.r_data_byte,(flash_parm_info.length*4));

#ifdef W25QXX_INFO_LOG
		w25qxx_ReadData(W25Qxx_Write_BLOCK_3_Addr+offest*i,buf,(flash_parm_info.length*4));
		HAL_UART_Transmit(&huart1,buf,(flash_parm_info.length*4),100);
		memset(buf,0,sizeof(buf));
#endif		
	}
	
#define W25QXX_BACK_PROGRAM_FLAG_OFFEST   (BACK_PROGRAM_FLAG_OFFEST - 0x5000)
	uint8_t flag_buf[256]={0};
	w25qxx_ReadData( (W25Qxx_Write_BLOCK_3_Addr + W25QXX_BACK_PROGRAM_FLAG_OFFEST) , flag_buf , 256);
	
/*#define W25QXX_BACK_PROGRAM_FLAG_LOG*/
#ifdef W25QXX_BACK_PROGRAM_FLAG_LOG
	HAL_UART_Transmit(&huart1,flag_buf,offest,100);
#endif 
	
	uint32_t flag=0x44332211;
	if(flag == ((flag_buf[0]) + (flag_buf[1]<<8) + (flag_buf[2]<<16) + (flag_buf[3]<<24)))
	{
		printf("Backup Succeeded\r\n");
		printf("\r\n");
	}
	memset((uint8_t*)&flash_parm_info,0,sizeof(flash_parm_info));
}

/**
 @brief 删除应用程序
 */
void delete_app_program(void)
{
	flash_sector_erase((STM32_PAGE_NUM - STM32_B_PAGE_NUM),STM32_A_PAGE_ADDR);
	printf("Delete App Program Success\r\n");
	printf("\r\n");
}

/**
 @brief 下载备份程序
 */
void update_backup_program(void)
{
	uint16_t offest=256;
	flash_sector_erase((STM32_PAGE_NUM - STM32_B_PAGE_NUM),STM32_A_PAGE_ADDR);
	for(uint8_t i=0 ; i< ((STM32_A_PAGE_NUM * STM32_PAGE_SIZE)/offest) ; i++)
	{
		flash_parm_info.address=STM32_A_PAGE_ADDR+offest*i;
		flash_parm_info.length=FLASH_DATA_MAX_LENGTH;
		w25qxx_ReadData(W25Qxx_Write_BLOCK_3_Addr+offest*i,flash_parm_info.r_data_byte,(flash_parm_info.length*4));
		array_inversion(&flash_parm_info);
		update_To_flash(&flash_parm_info,STM32_A_PAGE_ADDR+offest*i,STM32_FLASH_END_ADDR);
		
/*#define FLASH_INFO_LOG*/
#ifdef FLASH_INFO_LOG
		memset(flash_parm_info.r_data_byte,0,sizeof(flash_parm_info.r_data_byte));
		stm_flash_read(&flash_parm_info,FLASH_READ_BYTE);
		HAL_UART_Transmit(&huart1,flash_parm_info.r_data_byte,(flash_parm_info.length*4),100);
#endif
	}
	memset((uint8_t*)&flash_parm_info,0,sizeof(flash_parm_info));
	uint32_t flag=0x44332211;

	flash_parm_info.address=BACK_PROGRAM_FLAG_ADDRESS;
	flash_parm_info.length=1;
	stm_flash_read(&flash_parm_info,FLASH_READ_BYTE);
	
	if(flag == ((flash_parm_info.r_data_byte[0]) + (flash_parm_info.r_data_byte[1]<<8) + (flash_parm_info.r_data_byte[2]<<16)
										+ (flash_parm_info.r_data_byte[3]<<24)))
	{
		printf("Update Backup Succeeded\r\n");
		printf("\r\n");
	}
	memset((uint8_t*)&flash_parm_info,0,sizeof(flash_parm_info));
}

/**
 @brief 下载新的应用程序
 */
void user_update_menu(void)
{
	printf("/****************     UpDate Menu    ****************/\r\n");
	printf("*    Send C And Ready To Download                1*\r\n");
	printf("*    Return User Menu                            2*\r\n");
	printf("/**************************************************/\r\n");
	printf("\r\n");
}

void update_app_program(void)
{
#define YMODEM_SEND_C 1
#define YMODEM_NO_SEND_C 0	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,serial_rx_cmd_buf, serial_rx_cmd_buf_length);	
	user_update_menu();
	while(1)
	{
		if(rx_flag==1)
		{
			rx_flag=0;
			if(serial_rx_cmd_buf[0] == '1')
			{
				Ymodem_buf_parm Ymodem_buf_parm_info={0};
				uint8_t Send_C_Flag = YMODEM_SEND_C;
				uint8_t transfer_end_flag=0;
				Receive_Start_Frame_Flag = YMODEM_RECEIVE_START_FRAME_NO_FINISH;
				Receive_Data_Frame_Flag = YMODEM_RECEIVE_DATA_FRAME_FINISH;
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Ymodem_buf_parm_info.Ymodem_rx_buf, YMODEM_RX_BUF_LEN);
				while(1)
				{
					/*打开Ymodem传输文件*/
					if(Send_C_Flag == YMODEM_SEND_C)
					{
						Ymodem_buf_parm_info.Ymodem_tx_buf[0]=YMODEM_C;
						Open_Ymodem_File_Transfer(Ymodem_buf_parm_info.Ymodem_tx_buf);
					}
					if(rx_flag==1)
					{
						rx_flag=0;
						if(Ymodem_buf_parm_info.Ymodem_rx_buf[0] == '2')
						{
							memset(Ymodem_buf_parm_info.Ymodem_rx_buf,0,sizeof(Ymodem_buf_parm_info.Ymodem_rx_buf));
							HAL_UARTEx_ReceiveToIdle_DMA(&huart1,serial_rx_cmd_buf, serial_rx_cmd_buf_length);
							printf("\r\n");							
							printf("Transmission End\r\n");
							user_update_menu();
							break;
						}
						/*Ymodem数据帧*/
						if(Ymodem_buf_parm_info.Ymodem_rx_buf[0] == YMODEM_SOH && transfer_end_flag == 0)
						{
							Send_C_Flag = YMODEM_NO_SEND_C;
							if(Ymodem_Process_Data(Ymodem_buf_parm_info.Ymodem_rx_buf) == YMODEM_ACK)
							{
								HAL_Delay(100);
								Ymodem_Send_ACK(Ymodem_buf_parm_info.Ymodem_tx_buf);
								if(Receive_Start_Frame_Flag == YMODEM_RECEIVE_START_FRAME_NO_FINISH)
								{
									Receive_Start_Frame_Flag = YMODEM_RECEIVE_START_FRAME_FINISH;
									Receive_Data_Frame_Flag = YMODEM_RECEIVE_DATA_FRAME_NO_FINISH;
									Ymodem_Send_C(Ymodem_buf_parm_info.Ymodem_tx_buf);
								}
							}
							else
							{
								Ymodem_Send_NAK(Ymodem_buf_parm_info.Ymodem_tx_buf);
							}
						}
						/*Ymodem结束帧*/
						if(Ymodem_buf_parm_info.Ymodem_rx_buf[0] == YMODEM_EOT)
						{
							Send_C_Flag = YMODEM_SEND_C;
							Data_Package_Flag=1;
							Receive_Start_Frame_Flag = YMODEM_RECEIVE_START_FRAME_NO_FINISH;
							Receive_Data_Frame_Flag = YMODEM_RECEIVE_DATA_FRAME_FINISH;
							Ymodem_Send_ACK(Ymodem_buf_parm_info.Ymodem_tx_buf);
							jump_to_flag=1;
							transfer_end_flag=1;
						}
						/*Ymodem取消传输*/
						if(Ymodem_buf_parm_info.Ymodem_rx_buf[0] == YMODEM_CAN)
						{
							Send_C_Flag = YMODEM_SEND_C;
							Data_Package_Flag=1;
							Receive_Start_Frame_Flag = YMODEM_RECEIVE_START_FRAME_NO_FINISH;
							Receive_Data_Frame_Flag = YMODEM_RECEIVE_DATA_FRAME_FINISH;
							printf("Cancel Transmission\r\n");
						}
						HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Ymodem_buf_parm_info.Ymodem_rx_buf, YMODEM_RX_BUF_LEN);
					}
				}
			}
			if(serial_rx_cmd_buf[0] == '2')
			{
				break;
			}
		}
	}
}

