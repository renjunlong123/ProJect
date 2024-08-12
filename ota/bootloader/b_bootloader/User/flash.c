#include "flash.h"

flash_parm flash_parm_info;

static uint32_t hal_bsp_flash_Readword(uint32_t address)
{
	return *(__IO uint32_t*)(address);  
}

/*
static uint16_t hal_bsp_flash_Readhalfword(uint32_t address)
{
	return *(__IO uint16_t*)(address);  
}
*/

static uint8_t hal_bsp_flash_Readbyte(uint32_t address)
{
	return *(__IO uint8_t*)(address);  
}


uint8_t stm_flash_write(flash_parm* f_parm,const uint32_t start_address,const uint32_t end_address,const uint8_t page_length)
{
	if(f_parm->address >= start_address && f_parm->address < end_address)
	{
		f_parm->end_address=f_parm->address+f_parm->length*4;
		if(f_parm->end_address > end_address)
		{
			return _Error;
		}
		if(f_parm->address%4!=0)
		{
			return _Error;
		}
	}
	else
	{
		return _Error;
	}
	uint32_t i;
	HAL_StatusTypeDef FLASH_Status=HAL_OK;
	uint32_t PageError=0;
	FLASH_EraseInitTypeDef flash_Erase_parm;
	flash_Erase_parm.NbPages=page_length;             
	flash_Erase_parm.PageAddress=start_address;
	flash_Erase_parm.TypeErase=FLASH_TYPEERASE_PAGES;    
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&flash_Erase_parm,&PageError);
	if(PageError!=0xFFFFFFFF)
	{
		HAL_FLASH_Lock();
		return _Error;
	}
	for(i=0;i<f_parm->length;i++)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,f_parm->address+4*i,f_parm->w_data[i])==HAL_OK)
		{
			if(*(uint32_t*)(f_parm->address+4*i) != f_parm->w_data[i])
			{
				FLASH_Status=HAL_ERROR;
				break;
			}
		}
		else
		{
			FLASH_Status=HAL_ERROR;
			break;
		}
	}
	HAL_FLASH_Lock();
	if(FLASH_Status!=HAL_OK)
	{
		return _Error;
	}
	return f_parm->length;
}

uint8_t update_To_flash(flash_parm* f_parm,const uint32_t start_address,const uint32_t end_address)
{
	if(f_parm->address >= start_address && f_parm->address < end_address)
	{
		f_parm->end_address=f_parm->address+f_parm->length*4;
		if(f_parm->end_address > end_address)
		{
			return _Error;
		}
		if(f_parm->address%4!=0)
		{
			return _Error;
		}
	}
	else
	{
		return _Error;
	}
	HAL_FLASH_Unlock();
	uint32_t i;
	HAL_StatusTypeDef FLASH_Status=HAL_OK;
	for(i=0;i<f_parm->length;i++)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,f_parm->address+4*i,f_parm->w_data[i])==HAL_OK)
		{
			if(*(uint32_t*)(f_parm->address+4*i) != f_parm->w_data[i])
			{
				FLASH_Status=HAL_ERROR;
				break;
			}
		}
		else
		{
			FLASH_Status=HAL_ERROR;
			break;
		}
	}
	HAL_FLASH_Lock();
	if(FLASH_Status!=HAL_OK)
	{
		return _Error;
	}
	return f_parm->length;	
}


void stm_flash_read(flash_parm* f_parm,const uint8_t system)
{
	switch(system)
	{
		case FLASH_READ_BYTE:
			for(uint32_t i=0;i<f_parm->length*4;i++)
			{
				f_parm->r_data_byte[i]=hal_bsp_flash_Readbyte(f_parm->address+i);
			}
			break;
		case FLASH_READ_WORD:
			for(uint32_t i=0;i<f_parm->length;i++)
			{
				f_parm->r_data_word[i]=hal_bsp_flash_Readword(f_parm->address+4*i);
			}
			break;
	}
}

uint8_t flash_sector_erase(uint8_t pages,uint32_t address)
{
	uint32_t PageError=0;
	FLASH_EraseInitTypeDef flash_Erase_parm;
	flash_Erase_parm.NbPages=pages;             
	flash_Erase_parm.PageAddress=address;
	flash_Erase_parm.TypeErase=FLASH_TYPEERASE_PAGES;    
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&flash_Erase_parm,&PageError);
	if(PageError!=0xFFFFFFFF)
	{
		HAL_FLASH_Lock();
		return _Error;
	}
	return _OK;
}

void flash_test(void)
{
	flash_parm_info.address=STM32_FLASH_ADDR+0xFC00;
	flash_parm_info.length=4;
	for(uint8_t i=0;i<4;i++)
	{
		flash_parm_info.w_data[i]=0x11223344;
	}
	stm_flash_write(&flash_parm_info,0x0800FC00,0x08010000,1);
}
