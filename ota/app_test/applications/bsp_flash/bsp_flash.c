#include "bsp_flash.h"

FlashParm_TypeDef f={0};

static uint32_t hal_bsp_flash_Readword(uint32_t address)
{
	return *(__IO uint32_t*)(address);  
}

static uint16_t hal_bsp_flash_Readhalfword(uint32_t address)
{
	return *(__IO uint16_t*)(address);  
}

static uint8_t hal_bsp_flash_Readbyte(uint32_t address)
{
	return *(__IO uint8_t*)(address);  
}


uint8_t STM_Flash_Write(FlashParm_TypeDef* f_parm,uint32_t start_address,uint32_t end_address,uint8_t page_length)
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

void STM_Flash_Read(FlashParm_TypeDef* f_parm,const uint8_t system)
{
	switch(system)
	{
		case FLASH_READ_BYTE:
			for(uint32_t i=0;i<f_parm->length*4;i++)
			{
				f_parm->r_data_byte[i]=hal_bsp_flash_Readbyte(f_parm->address+i);
			}
			break;
		case FLASH_READ_HALFWORD:
			for(uint32_t i=0;i<f_parm->length*2;i++)
			{
				f_parm->r_data_halfword[i]=hal_bsp_flash_Readhalfword(f_parm->address+2*i);
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
