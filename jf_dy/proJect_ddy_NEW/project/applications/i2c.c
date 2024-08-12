#include "i2c.h"

/**
 @brief i2c写scl
 */
static void i2c_scl_w(uint8_t BitValue){
    if(BitValue==0)
    {
        _IO_24C02_SCL_L;
    }
    else
    {
        _IO_24C02_SCL_H;
    }
    rt_hw_us_delay(10);
}

/**
 @brief i2c写sda
 */
static void i2c_sda_w(uint8_t BitValue){
    if(BitValue==0)
    {
        _IO_24C02_SDA_L;
    }
    else
    {
        _IO_24C02_SDA_H;
    }
    rt_hw_us_delay(10);
}

/**
 @brief i2c读sda
 */
static uint8_t SDA_R(void){
    uint8_t BitValue;
    BitValue = HAL_GPIO_ReadPin(_IO_24C02_PORT,_IO_24C02_SDA) ;
    rt_hw_us_delay(10);
    return BitValue;
}

/**
 @brief i2c初始化
 */
void iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /*24C02 IIC引脚*/
    _IO_24C02_SCL_H;
    _IO_24C02_SDA_H;
    GPIO_InitStruct.Pin = _IO_24C02_SCL|_IO_24C02_SDA;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_IO_24C02_PORT,&GPIO_InitStruct);
}


uint8_t i2c_CheckDevice(uint8_t address)
{
    uint8_t ucAck;
    iic_start();
    iic_sendbyte(address|iic_WR);
    ucAck = iic_receive_ack();
    iic_stop();
    return ucAck;
}


/*
*i2c
*同步时序半双工,对时间要求不严格,对硬件电路不怎么依赖.一主多从,多主多从,通信线高电平的驱动能力比较弱,通信速度慢.
*标准模式,只有100KHZ的时钟频率,I2C最高速度,400KHZ.外围器件的时钟线和数据线都是挂载在IIC总线(由主控芯片提供).并且
*在空闲状态下所有器件的时钟线scl和数据线sda都被总线的上拉电阻拉高.sda引脚和scl引脚设置为开漏模式,好处是防止短路。
*每个挂载在i2c总线上的外围器件都有独立的器件地址.主机发送开始信号后,只需要发送想要通信的设备的地址,如果设备收到地
*址并且匹配正确,则开始进行单独通信。
*
*从机设备地址在i2c协议标准里面分为7位地址和10位地址
*指定地址写:开始+选择从机地址(+写)+接收应答+指定寄存器地址+接收应答+写入数据+接收应答+停止条件
*指定地址读:开始+选择从机地址(+写)+接收应答+指定寄存器地址+接收应答+开始+选择从机地址(+读)+接收应答+写入数据+发送应答+停止条件
*/

/**
	@brief 起始:scl高电平,sda高电平切换到低电平
 */
void iic_start(void)
{
    i2c_sda_w(1);
    i2c_scl_w(1);
    i2c_sda_w(0);
    i2c_scl_w(0);
}

/**
	@brief 中止:scl高电平,sda从低电平切换到高电平
 */
void iic_stop(void)
{
    i2c_sda_w(0);
    i2c_scl_w(1);
    i2c_sda_w(1);
}

/**
	@brief 发送一个字节:scl低电平主机将数据位依次放到sda线上(高位先行),高电平从机读数据,然后释放scl,从机将在scl高电平期间读取数据位,
 *所以scl高电平期间sda不允许有数据变化,依次循环上述过程8次,即可发送一个字节
 */
void iic_sendbyte(uint8_t byte)
{
    uint8_t i;
		i2c_scl_w(0);
    for (i = 0; i < 8; i ++)
    {
        i2c_sda_w(byte & (0x80 >> i));
        i2c_scl_w(1);
        i2c_scl_w(0);
    }
}

/**
	@brief 接收一个字节:主机在接收数据前，先释放sda(将sda拉高),从机取得sda控制权,scl低电平从机放数据,scl高电平主机读数据
 */
uint8_t iic_receive_byte(void)
{
    uint8_t i, byte = 0x00;
    i2c_sda_w(1);
		i2c_scl_w(0);
    for (i = 0; i < 8; i ++)
    {
        i2c_scl_w(1);
        if (SDA_R() == 1){byte |= (0x80 >> i);}
        i2c_scl_w(0);
    }
    return byte;
}

/**
	@brief 发送应答:主机在接收完一个字节后,在下一个时序发送应答(scl低电平时),数据0表示主机应答
数据1表示非应答,scl高电平从机读取应答,scl低电平进入下一个时序单元
 */
void iic_sendack(uint8_t AckBit)
{
    i2c_sda_w(AckBit);
    i2c_scl_w(1);
    i2c_scl_w(0);
}

/**
 @brief 接收应答:主机在发送完一个字节之后,判断从机是否应答,数据0表示应答,数据1表示非应答(主机在接收之前,释放SDA)
scl低电平,主机释放sda,防止干扰从机,从机把应答位放在SDA上,scl高电平主机读取应答,scl低电平进入下一个时序单元
 */
uint8_t iic_receive_ack(void)
{
    uint8_t AckBit=3;
    i2c_sda_w(1);
    i2c_scl_w(1);
    AckBit = SDA_R();
    i2c_scl_w(0);
    return AckBit;
}



