#include "24c02.h"


/**
@brief AT24C02初始化
*/
void AT24C02_INIT(void)
{
    iic_init();
}
/**
@brief AT24C02写数据
*/
uint8_t AT24C02_W_Byte(uint8_t RegAddress,uint8_t data)
{
    iic_start();
    iic_sendbyte(AT24C02|iic_WR);
    if(iic_receive_ack()!=0)
    {
        iic_stop();
        return 0;
    }
    iic_sendbyte(RegAddress);
    if(iic_receive_ack()!=0)
    {
        iic_stop();
        return 0;
    }
    iic_sendbyte(data);
    iic_stop();
    return 1;
}
/**
@brief AT24C02写页操作
*/
uint8_t AT24C02_W_nByte(uint8_t* pdata_buf,uint8_t RegAddress,uint8_t Data_len)
{
    uint8_t i;
    iic_start();
    iic_sendbyte(AT24C02|iic_WR);
    iic_receive_ack();
    iic_sendbyte(RegAddress);
    iic_receive_ack();
    if(Data_len>AT24C02_PAGE_LEN)
    {
        iic_stop();
        return 0;
    }
    for(i=0;i<Data_len;i++)
    {
        iic_sendbyte(pdata_buf[i]);
        iic_receive_ack();
    }
    iic_stop();
    return 1;
}
/**
@brief AT24C02读数据
*/
uint8_t AT24C02_R_nByte(uint8_t* pdata_buf,uint8_t RegAddress,uint16_t Data_len)
{
    uint8_t i=5;
    iic_start();
    iic_sendbyte(AT24C02|iic_WR);
    if(iic_receive_ack()!=0)
    {
        iic_stop();
        return 0;
    }
    iic_sendbyte(RegAddress);
    if(iic_receive_ack()!=0)
    {
        iic_stop();
        return 0;
    }

    iic_start();
    iic_sendbyte(AT24C02|iic_RD);
    if(iic_receive_ack()!=0)
    {
        iic_stop();
        return 0;
    }
    for(i=0;i<Data_len;i++)
    {
        pdata_buf[i]=iic_receive_byte();
        if(i!=Data_len-1)
        {
            iic_sendack(0);
        }
    }
    //iic_sendack(1);
    iic_stop();
    return 1;
}
/**
@brief AT24C02擦除
*/
void AT24C02_Erase(void)
{
    uint8_t i,buf[AT24C02_PAGE_LEN];
    for(i=0;i<AT24C02_PAGE_LEN;i++)
    {
        buf[i] = 0xff;
    }
		AT24C02_W_nByte(buf,0,AT24C02_PAGE_LEN);
		rt_thread_mdelay(10);
		AT24C02_W_nByte(buf,8,AT24C02_PAGE_LEN);
		rt_thread_mdelay(10);
}

/**
@brief AT24C02测试
*/
void AT24C02_Test(void)
{
	  rt_uint8_t w_buf[5]={0},r_buf[5]={0};
    w_buf[0]=0xaa;
    w_buf[1]=0xaa;
    w_buf[2]=0xfe;
    w_buf[3]=0xfe;
    w_buf[4]=0xfe;
    if(i2c_CheckDevice(AT24C02)==0)
    {
			AT24C02_W_nByte(w_buf,0,5);
			memset(r_buf,0,sizeof(r_buf));
			rt_thread_mdelay(1000);
			AT24C02_R_nByte(r_buf,0,5);
			_IO_485_RTC_H;
			rt_device_write(Serial_2_485,0,r_buf,5);
			_IO_485_RTC_L;
    }
}
