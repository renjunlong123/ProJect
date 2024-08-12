#include "w25qxx.h"


static uint8_t spi_sendbyte(uint8_t tx_data)
{
    uint8_t rx_data=0;
    HAL_SPI_TransmitReceive(&hspi1, &tx_data,&rx_data, 1, 100);
    return rx_data;
}
static uint8_t spi_dma_sendbyte(uint8_t tx_data)
{
    uint8_t rx_data=0;
    HAL_SPI_TransmitReceive_DMA(&hspi1, &tx_data,&rx_data, 1);
    return rx_data;
}

/*******************************************************************************
* Function Name  : W25Qxx_ReadID
* Description    : 获取W25Qxx设备号
* Input          : *MID 厂家ID号
*                  *DID 设备ID号
* Output         : None
* Return         : None
*******************************************************************************/

void w25qxx_readid(uint8_t* mid,uint16_t* did)
{
    spi_cs_l;
    spi_sendbyte(W25Qxx_JEDEC_ID);
    *mid=spi_sendbyte(0xFF);
    *did=spi_sendbyte(0xFF);
    *did<<=8;
    *did|=spi_sendbyte(0xFF);
    spi_cs_h;
}
void w25qxx_dma_readid(uint8_t* mid,uint16_t* did)
{
    spi_cs_l;
    spi_dma_sendbyte(W25Qxx_JEDEC_ID);
    *mid=spi_dma_sendbyte(0xFF);
    *did=spi_dma_sendbyte(0xFF);
    *did<<=8;
    *did|=spi_dma_sendbyte(0xFF);
    spi_cs_h;
}

/*******************************************************************************
* Function Name  : W25Qxx_WriteEnable
* Description    : 写使能
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void w25qxx_write_enable(void)
{
    spi_cs_l;
    spi_sendbyte(W25Qxx_WRITE_ENABLE);
    spi_cs_h;
}

/*******************************************************************************
* Function Name  : W25Qxx_WaitBusy
* Description    : 等待忙碌  写入操作结束后，芯片进入忙状态，不响应新的读写操作
*                  读到的最低为为0x01时代表忙碌0x00代表不忙
* Input          : hNone
* Output         : None
* Return         : None
*******************************************************************************/

uint8_t w25qxx_waitbusy(void)
{
    uint32_t timeout=0xffff;
    spi_cs_l;
    spi_sendbyte(W25Qxx_READ_STATUS_REGISTER_1);
    while((spi_sendbyte(0xFF)&0x01)==0x01){
        timeout--;
        if(timeout==0)
            break;
    }
    spi_cs_h;
    return 0;
}

/*******************************************************************************
* Function Name  : w25qxx_SectorErase
* Description    : 扇区擦除  256个字节
* Input          : Address：擦除地址所在的扇区。
* Output         : None
* Return         : None
*******************************************************************************/

void w25qxx_sectorerase(uint32_t address){
    w25qxx_write_enable();
    spi_cs_l;
    spi_sendbyte(W25Qxx_SECTOR_ERASE_4KB);
    spi_sendbyte(address>>16);
    spi_sendbyte(address>>8);
    spi_sendbyte(address);
    spi_cs_h;
    w25qxx_waitbusy();
}

void w25qxx_blockerase(uint32_t address)
{
    w25qxx_write_enable();
    spi_cs_l;
    spi_sendbyte(W25Qxx_BLOCK_ERASE_64KB);
    spi_sendbyte(address>>16);
    spi_sendbyte(address>>8);
    spi_sendbyte(address);
    spi_cs_h;
    w25qxx_waitbusy();	
}
/*******************************************************************************
* Function Name  : w25qxx_ChipErase
* Description    : 芯片擦除
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void w25qxx_chiperase(void){
    w25qxx_write_enable();
    spi_cs_l;
    spi_sendbyte(W25Qxx_CHIP_ERASE);
    spi_cs_h;
    w25qxx_waitbusy();
}

/*******************************************************************************
* Function Name  : w25qxx_PageProgram
* Description    : 页写入，只能在当前地址页写入，超过会从当前页起始位置开始写。
写入前擦除当前页，芯片规则：只能1改0，不能0改1，相当于前一个数据&之后数据。
例：之前为0x01改为0x02，如果不拆除就变为：0x01&0x02=0x00。
* Input          : Address：写入地址
*                  *DataArray：写入数据数组
*                  Count：数组长度
* Output         : None
* Return         : None
*******************************************************************************/
void w25qxx_PageProgram(uint32_t address, uint8_t *DataArray, uint32_t Count){
    w25qxx_write_enable();
    spi_cs_l;
    spi_sendbyte(W25Qxx_PAGE_PROGRAM);
    spi_sendbyte(address>>16);
    spi_sendbyte(address>>8);
    spi_sendbyte(address);
    for(int i=0;i<Count;i++)
    {
        spi_sendbyte(DataArray[i]);
    }
    spi_cs_h;
    w25qxx_waitbusy();
}

/*******************************************************************************
* Function Name  : w25qxx_ReadData
* Description    : 读数据，可以读任意地址，任意个数，可跨页读
* Input          : Address：写入地址
*                 *DataArray：写入数据数组
*                  Count：数组长度
* Output         : None
* Return         : None
*******************************************************************************/
void w25qxx_ReadData(uint32_t address, uint8_t *DataArray, uint32_t Count){
    spi_cs_l;
    spi_sendbyte(W25Qxx_READ_DATA);
    spi_sendbyte(address>>16);
    spi_sendbyte(address>>8);
    spi_sendbyte(address);
    for(int i=0;i<Count;i++)
    {
        DataArray[i]=spi_sendbyte(0xFF);
    }
    spi_cs_h;
}

/*******************************************************************************
* Function Name  : dev_flash_write_bytes_nocheck
* Description    : 写数据 自动查询当前地址 自动越扇区写入 注意这里只能写擦除过的
* Input          : pdata 数据
*                  write_addr  写入地址
*                  write_length 写入长度
* Output         : None
* Return         : None
*******************************************************************************/

void dev_flash_write_bytes_nocheck(uint8_t *pdata, uint32_t write_addr, uint16_t write_length)   // 0x01 0x02 0x03  , 0x0000FE ,3
{
    // 计算当前页剩余的字节数
    uint16_t PageByte = 256 - write_addr % 256;                                                  // PageByte= 2;

    // 如果写入长度小于等于剩余字节数，则实际写入的字节数等于写入长度
    if(write_length <= PageByte)
    {
        PageByte = write_length;
    }

    // 进入循环
    while(1)
    {
        // 调用 W25Q128_PageProgram 函数将数据写入一页闪存
        w25qxx_PageProgram(write_addr,pdata,PageByte);                                           // 1:0x0000FE=0x01,0x0000FF=0x02
        // 2:write_addr=0x000100，pdata=0x03，PageByte=1.0x000100=0x003
        // 如果写入长度等于实际写入的字节数，则退出循环
        if(write_length == PageByte)     //1=1，break
            break;
        else
        {
            // 更新数据指针、写入地址和写入长度
            pdata += PageByte;                                                                   //pdata 直接指向0x03，相当于指针右移了PageByte位
            write_addr += PageByte;                                                              //write_addr  = 0x000100
            write_length -= PageByte;                                                            //write_length=3-2=1
            // 根据剩余的写入长度确定下一次循环中实际写入的字节数
            if(write_length > 256)
            {
                PageByte = 256;
            }
            else
            {
                PageByte = write_length;                                                         //PageByte =1
            }
        }
    }
}

void w25qxx_test(void)
{
	#define write_length 128
	#define read_length  256
	uint8_t w25q64_write_buf[write_length]={0};
	uint8_t w25q64_read_buf[read_length]={0};
	uint16_t i=0,j=0;
	for(i=0;i<write_length;i++)
	{
		w25q64_write_buf[i]=0xab;
	}
	w25qxx_write_enable();
	w25qxx_sectorerase(W25Qxx_Write_Page_Addr_1);
	w25qxx_PageProgram(W25Qxx_Write_Page_Addr_1,w25q64_write_buf,write_length);
	for(i=0;i<write_length;i++)
	{
		w25q64_write_buf[i]=j;
		j++;
	}	
	w25qxx_PageProgram(W25Qxx_Write_Page_Addr_1+128,w25q64_write_buf,write_length);
	w25qxx_ReadData(W25Qxx_Write_Page_Addr_1,w25q64_read_buf,read_length);
	HAL_UART_Transmit(&huart1,w25q64_read_buf,sizeof(w25q64_read_buf),100);
}
