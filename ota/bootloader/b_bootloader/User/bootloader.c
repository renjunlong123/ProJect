#include "bootloader.h"

ota_parm ota_parm_info;
updata_parm updata_parm_info;
p_app load_app;
void bootloader_brance(void);
void iap_load_app(uint32_t address);
uint8_t jump_to_flag=1;
/*读取ota，判断是否ota升级*/
void read_ota_parm_info(void)
{
	memset(&ota_parm_info,0,OTA_PARM_INFO_SIZE);
	w25qxx_ReadData(OTA_FLAG_ADDR,(uint8_t*)&ota_parm_info,OTA_PARM_INFO_SIZE);
	if(cal_ota_flag(ota_parm_info.ota_flag_buf) == OTA_FLAG_Update_indicate)//OTA_FLAG
	{
#ifdef BOOTLOART_LOG
			user_select_menu();
#endif
		while(1)
		{
			if(rx_flag==1)
			{
				rx_flag=0;
				switch(serial_rx_cmd_buf[0])
				{
					case '1':/*下载应用程序*/
						update_app_program();
						break;
					case '2':/*跳转应用程序*/
						if(jump_to_flag==1)
						{
							printf("Enter 2\r\n");
							iap_load_app(STM32_A_PAGE_ADDR);
						}
						else
						{
							printf("No App Program , Please Update App Program\r\n");
							printf("\r\n");
							break;
						}
					case '3':/*备份应用程序到W25QXX*/
						printf("Enter 3\r\n");
						backup_program();
						break;
					case '4':/*加载备份应用程序至FLASH*/
						jump_to_flag=1;
						printf("Enter 4\r\n");
						update_backup_program();
						break;
					case '5':/*删除应用程序*/
						jump_to_flag=0;
						printf("Enter 5\r\n");
						delete_app_program();
						break;
				}
				serial_rx_cmd_buf_init;
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,serial_rx_cmd_buf, serial_rx_cmd_buf_length);						
				user_select_menu();
			}
		}
	}
	else
	{
#ifdef BOOTLOART_LOG
		printf("jump to app\r\n");
		printf("\r\n");
#endif
		iap_load_app(STM32_A_PAGE_ADDR);	
	}
}

void bootloader_brance(void)
{
	
}

/*设置sp指针*/
__asm void msr_sp(uint32_t address)
{
	MSR MSP,r0/*address保存在r0中*/
	BX r14    /*链接寄存器，保存子函数跳转的返回值*/
}

/*设置pc指针*/
//uint32_t addr;
void iap_load_app(uint32_t address)
{
	/*判断栈指针是否合法*/
	if((*(__IO uint32_t*)address >= 0x20000000) && (*(__IO uint32_t*)address <= 0x20004FFF))
	{
		/*关闭所有中断*/
		__disable_irq();	
		/*关闭滴答定时器,复位到默认值*/
		SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    /*设置所有时钟到默认状态,使用HSI时钟*/  
		HAL_RCC_DeInit();
		/*关闭所有中断，清除所有中断挂起标志 */
    for (uint8_t i = 0; i < 8; i++)
    {
       NVIC->ICER[i] = 0xFFFFFFFF;
       NVIC->ICPR[i] = 0xFFFFFFFF;
    } 
		/*设置中断向量表*/
		//SCB->VTOR = FLASH_BASE|0x5000;
		/*利用函数地址设置pc指针*/
		load_app=(p_app)(*(__IO uint32_t*)(address+4));/*load_app执行复位地址*/
		/*设置栈顶指针*/
		msr_sp(address);
		/*跳转*/
		load_app();
	}
}

