#include "data.h"

data_package code_data_package;
ota_parm ota_parm_info;

/*写使能*/
void w25q64_enable(void)
{
	spi_w25qxx_write_enable();
}

/*关键参数*/
void w25q64_block_1_init(uint32_t ota_flag_tmp)
{
	spi_w25qxx_write_enable();
	ota_parm_info.ota_flag_buf[0]=ota_flag_tmp>>24;
	ota_parm_info.ota_flag_buf[1]=ota_flag_tmp>>16;
	ota_parm_info.ota_flag_buf[2]=ota_flag_tmp>>8;
	ota_parm_info.ota_flag_buf[3]=ota_flag_tmp;
	spi_w25qxx_erase(W25Qxx_Write_BLOCK_1_Addr,W25Qxx_BLOCK_ERASE_64KB);
	spi_w25qxx_write_page(W25Qxx_Write_BLOCK_1_Addr,ota_parm_info.ota_flag_buf,ota_flag_length);
#if 0
	spi_w25qxx_read(W25Qxx_Write_BLOCK_1_Addr,ota_parm_info.ota_flag_buf,4);
	rt_device_write(Dev_InitStruct.dev_serial_1,0,ota_parm_info.ota_flag_buf,4);	
#endif
}

/*新程序地址*/
void w25q64_block_2_init(void)
{
	spi_w25qxx_write_enable();
	spi_w25qxx_erase(W25Qxx_Write_BLOCK_2_Addr,W25Qxx_BLOCK_ERASE_64KB);
	memset(code_data_package.code_data_package_buf_1,0,code_data_package_buf_max_length);
#if 0
	spi_w25qxx_read(W25Qxx_Write_BLOCK_2_Addr,code_data_package.code_data_package_buf_1,100);
	rt_device_write(Dev_InitStruct.dev_serial_1,0,code_data_package.code_data_package_buf_1,100);	
#endif
}

/*备份当前程序起始地址*/
void w25q64_block_3_init(void)
{
	spi_w25qxx_write_enable();
	spi_w25qxx_erase(W25Qxx_Write_BLOCK_3_Addr,W25Qxx_BLOCK_ERASE_64KB);
	memset(code_data_package.code_data_package_buf_1,0,code_data_package_buf_max_length);
#if 0
	spi_w25qxx_read(W25Qxx_Write_BLOCK_3_Addr,code_data_package.code_data_package_buf_1,100);
	rt_device_write(Dev_InitStruct.dev_serial_1,0,code_data_package.code_data_package_buf_1,100);	
#endif
}

