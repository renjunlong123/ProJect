#ifndef __I2C_H
#define __I2C_H

/**
 @brief include
 */
#include "u_init.h"

/**
 @brief function
 */
void iic_init(void);
uint8_t i2c_CheckDevice(uint8_t address);
void iic_start(void);
void iic_stop(void);
void iic_sendbyte(uint8_t byte);
uint8_t iic_receive_byte(void);
void iic_sendack(uint8_t AckBit);
uint8_t iic_receive_ack(void);

#endif
