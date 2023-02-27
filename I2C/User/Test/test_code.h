#ifndef __TEST_CODE_H_
#define __TEST_CODE_H_

#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_led.h"
#include "bsp_misc.h"

//#define	I2C_SINGLE_READ_WRITE	
//#define	I2C_PAGE_READ_WRITE	
#define	I2C_BUFFER_READ_WRITE	

void cmd_receive(uint8_t tmp);
void I2C_Read_Write_Test(void);

#endif
