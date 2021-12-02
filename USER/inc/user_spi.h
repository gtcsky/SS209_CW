/*
 * user_spi.h
 *
 *  Created on: 2019-11-25
 *      Author: Bough_004
 */

#ifndef SOURCE_INC_USER_SPI_H_
#define SOURCE_INC_USER_SPI_H_

#include "cx32l003_rcc.h"
#include "cx32l003_spi.h"
#include "cx32l003_gpio.h"
#include "stdint.h"
#include "const.h"
//#include "spi.h"
#include	"io_define.h"


extern	void WriteData(uint8_t *sendstr,uint8_t sendlen);
extern	void ReadData(uint8_t *recvstr,uint8_t recvlen);
extern	void userSpiInit(void);
extern	void disableSpi(void);
extern	void enableSpi(void);
#endif /* SOURCE_INC_USER_SPI_H_ */
