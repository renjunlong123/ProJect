#include "w25qxx.h"

/*read id*/
void spi_w25qxx_read_id(void)
{
	uint8_t w25qxx_cmd[4]={0};
	uint8_t w25qxx_id_buf[4]={0};
	w25qxx_cmd[0]=W25Qxx_JEDEC_ID;
	struct rt_spi_message msg1;
	msg1.send_buf=w25qxx_cmd;
	msg1.recv_buf=w25qxx_id_buf;
	msg1.length=4;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	rt_device_write(Dev_InitStruct.dev_serial_1,0,w25qxx_id_buf,sizeof(w25qxx_id_buf));
}

/*write enable*/
void spi_w25qxx_write_enable(void)
{
	struct rt_spi_message msg1;
	rt_uint8_t w25qxx_write_enable=W25Qxx_WRITE_ENABLE;
	msg1.send_buf=&w25qxx_write_enable;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
}

/*write disable*/
void spi_w25qxx_write_disable(void)
{
	struct rt_spi_message msg1;
	rt_uint8_t w25qxx_write_disable=W25Qxx_WRITE_DISABLE;
	msg1.send_buf=&w25qxx_write_disable;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
}
/*
读取W25QXX的状态寄存器
BIT7  6   5   4   3   2   1   0
SPR   RV  TB BP2 BP1 BP0 WEL BUSY
SPR:默认0,状态寄存器保护位,配合WP使用
TB,BP2,BP1,BP0:FLASH区域写保护设置
WEL:写使能锁定
BUSY:忙标记位(1,忙;0,空闲)
默认:0x00
*/

/*读取W25QXX的状态寄存器*/
static rt_uint8_t spi_w25qxx_read_sr(void)
{
	rt_uint8_t byte=0;
	struct rt_spi_message msg1,msg2;
	rt_uint8_t w25qxx_read_statusreg=W25Qxx_READ_STATUS_REGISTER_1;
	msg1.send_buf=&w25qxx_read_statusreg;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=0;
	msg1.next=&msg2;

	msg2.send_buf=RT_NULL;
	msg2.recv_buf=&byte;
	msg2.length=1;
	msg2.cs_take=0;
	msg2.cs_release=1;
	msg2.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	return byte;
}

/**
  * @brief  W25Qxx等待BUSY结束
  * @param  无形参
  * @retval 无返回值
  */
void spi_w25qxx_waitbusy(void)
{
    while((spi_w25qxx_read_sr() & 0x01)==0x01);
}

/**
  * @brief  W25Qxx扇区擦除
  * @param  需要擦除的地址,擦除大小（最小擦除为4kb）
  * @retval 无返回值
  */
void spi_w25qxx_erase(uint32_t w25qxx_address,uint8_t sector_size)
{
	struct rt_spi_message msg1;
	rt_uint8_t sector_address[4]={0};
	sector_address[0]=sector_size;
	sector_address[1]=w25qxx_address>>16;
	sector_address[2]=w25qxx_address>>8;
	sector_address[3]=w25qxx_address;
	msg1.send_buf=sector_address;
	msg1.recv_buf=RT_NULL;
	msg1.length=4;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
}

/**
  * @brief  W25Qxx全部擦除
  * @param  无
  * @retval 无返回值
  */
void spi_w25qxx_erase_chip(void)
{
	struct rt_spi_message msg1;
	rt_uint8_t chip_addr=W25Qxx_CHIP_ERASE;
	msg1.send_buf=&chip_addr;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
}

/**
  * @brief  W25Qxx写操作
  * @param  地址，写入数据，数据个数
  * @retval 无返回值
  */
void spi_w25qxx_write_page(uint32_t w25qxx_addr,void *DataArray, uint32_t Count)
{
	struct rt_spi_message msg1,msg2;
	rt_uint8_t addr[4]={0};
	addr[0]=W25Qxx_PAGE_PROGRAM;
	addr[1]=w25qxx_addr>>16;
	addr[2]=w25qxx_addr>>8;
	addr[3]=w25qxx_addr;
	spi_w25qxx_write_enable();
	msg1.send_buf=addr;
	msg1.recv_buf=RT_NULL;
	msg1.length=4;
	msg1.cs_take=1;
	msg1.cs_release=0;
	msg1.next=&msg2;
	
	msg2.send_buf=DataArray;
	msg2.recv_buf=RT_NULL;
	msg2.length=Count;
	msg2.cs_take=0;
	msg2.cs_release=1;
	msg2.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
}

/**
  * @brief  W25Qxx连续写操作
  * @param  地址，写入数据，数据个数
  * @retval 无返回值
  */
uint8_t spi_w25qxx_continuous_write_page(uint32_t w25qxx_addr,uint16_t *DataArray, uint32_t Count,uint8_t page_length)
{
	if(page_length>0xff)
	{
		return _Error;
	}
	for(uint8_t i=0;i<page_length;i++)
	{
		w25qxx_addr=w25qxx_addr+(i*0x0080);
		DataArray=DataArray+(i*0x0080);
		spi_w25qxx_write_page(w25qxx_addr,DataArray,Count);
	}
	return _OK;
}
/**
  * @brief  W25Qxx普通读取
  * @param  地址，读取数据，数据个数
  * @retval 无返回值
  */
void spi_w25qxx_read(uint32_t w25qxx_addr, void *DataArray, uint32_t Count)
{
	struct rt_spi_message msg1,msg2;
	rt_uint8_t addr[4]={0};
	addr[0]=W25Qxx_READ_DATA;
	addr[1]=w25qxx_addr>>16;
	addr[2]=w25qxx_addr>>8;
	addr[3]=w25qxx_addr;
	spi_w25qxx_write_enable();

	msg1.send_buf=addr;
	msg1.recv_buf=RT_NULL;
	msg1.length=4;
	msg1.cs_take=1;
	msg1.cs_release=0;
	msg1.next=&msg2;

	msg2.send_buf=RT_NULL;
	msg2.recv_buf=DataArray;
	msg2.length=Count;
	msg2.cs_take=0;
	msg2.cs_release=1;
	msg2.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
}

/**
  * @brief  W25Qxx断电
  * @param  无
  * @retval 无返回值
  */
void spi_w25qxx_powerdown(void)
{
	struct rt_spi_message msg1;
	rt_uint8_t powerdown_addr=W25Qxx_POWER_DOWN;
	spi_w25qxx_write_enable();
	msg1.send_buf=&powerdown_addr;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
  rt_thread_mdelay(10);
}

/**
  * @brief  W25Qxx唤醒
  * @param  无
  * @retval 无返回值
  */
void wakeup_addr_wakeup(void)
{
	struct rt_spi_message msg1;
	rt_uint8_t wakeup_addr=W25Qxx_RELEASE_POWER_DOWN_HPM_DEVICE_ID;
	spi_w25qxx_write_enable();
	msg1.send_buf=&wakeup_addr;
	msg1.recv_buf=RT_NULL;
	msg1.length=1;
	msg1.cs_take=1;
	msg1.cs_release=1;
	msg1.next=RT_NULL;
	rt_spi_transfer_message(spi_dev_w25qxx,&msg1);
	spi_w25qxx_waitbusy();
	rt_thread_mdelay(10);
}



