#ifndef __XM32LOO3_TEST_I2C_H
#define __XM32LOO3_TEST_I2C_H
/* Includes ------------------------------------------------------------------*/
#include "cx32l003.h"
#include "cx32l003_conf.h"
#include "cx32l003_gpio.h"
#include "cx32l003_i2c.h"
#include "const.h"

/*SCK接口定义-开头****************************/
#define	EEPROM_IICx                        		I2C

#define 	OP_Data							0x40
#define 	OP_Command					0x00
#define	LCD_SLAVE_ADDR				0x78
#define	I2C_READ_MODE					0X01

#define EEPROM_SIZE_2K		2
#define EEPROM_SIZE_4K		4
#define EEPROM_SIZE_8K		8
#define EEPROM_SIZE_16K	16
#define EEPROM_SIZE_32K	32
#define EEPROM_SIZE_64K	64
#define EEPROM_SIZE_128K	128
#define EEPROM_SIZE_256K	256
#define EEPROM_SIZE_512K	512


#define EEPROM_SIZE EEPROM_SIZE_8K

#if(EEPROM_SIZE == EEPROM_SIZE_2K)
	#define	EEPROM_PAGE_SIZE				8	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			32
#elif(EEPROM_SIZE == EEPROM_SIZE_4K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			32
#elif(EEPROM_SIZE == EEPROM_SIZE_8K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			64
#elif(EEPROM_SIZE == EEPROM_SIZE_16K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			128
#elif(EEPROM_SIZE == EEPROM_SIZE_32K)
	#define	EEPROM_PAGE_SIZE				32	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			128
#elif(EEPROM_SIZE == EEPROM_SIZE_64K)
	#define	EEPROM_PAGE_SIZE				32	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			256
#endif

#define	EEPROM_SLAVE_ADDR			0XA4




#define      EEPROM_IIC_IRQ                			I2C0_IRQn
#define      EEPROM_IIC_IRQHandler         		I2C0_IRQHandler


#define      EEPROM_IIC_SCK_AF_VALUE    		GPIO_AF_I2C_SCL_PB4
#define      EEPROM_IIC_SDA_AF_VALUE    		GPIO_AF_I2C_SDA_PB5


#define      EEPROM_IIC_ACK    					I2C_Ack_Enable
#define      EEPROM_IIC_BROADCASTADDRESS_RESPONE    0x01
#define      EEPROM_IIC_CLOSKSPEED             		10000//70000
#define      EEPROM_IIC_MODE          			I2C_Mode_Master
#define      EEPROM_IIC_OWNADDRESS     		0xA4//0x50

typedef enum{
	Normal=0,
	SleepMode=1
}sensorStts;

typedef enum{
	wakeupOver=0,
	sendingLowSda=1,
	waiting25ms=2
}sensorWakeupProcess;

#define	SEND_SDA_35MS		8
#define	WAIT_25MS			6

typedef struct{
	sensorStts  			sensorMode;
	sensorWakeupProcess 	sensorWakeupIndex;
	UINT8 	i2cWaitingCnt;
}i2cParamStruct;
extern	i2cParamStruct	i2cParams;
extern	UINT8	aEepromBuff[20];
#define	I2C_TIME_OUT_CONST			4//		3ms
#define	START_I2C_TIME_CONST			10//		10*5=50ms




void IIC_Init(void);

//void I2C0_IRQHandler_MasterSend(void);
//void I2C0_IRQHandler_MasterRecive(void);
//void I2C0_IRQHandler_SlaveRecive(void);
//void I2C0_IRQHandler_SlaveSend(void);
//void MASTER_EE_ReadBytes(uint8_t *_pReadBuf, uint8_t EE_SEND_START,uint8_t EE_SEND_END, uint8_t EE_REVCIVE_START,uint8_t EE_REVCIVE_END);
//void MASTER_EE_WriteBytes(uint8_t *_pWriteBuf, uint8_t EE_SEND_START,uint8_t EE_SEND_END);
//void SLAVE_IIC_Send(uint8_t *_pWriteBuf, uint8_t SEND_START,uint8_t SEND_END);
void Master_IIC_Mode(uint8_t REVCIVE_START,uint8_t REVCIVE_END);
//extern	void IIC_SDA(u8 a);
//void	setI2cIntArrived(void);
/***********************************************************************************************************
  *  @brief  		写入SSD1106 驱动的Command
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void writeLcdCommand(u8 cmd);

/***********************************************************************************************************
  *  @brief  		写入SSD1106驱动的Data
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void writeLcdData(u8 data);
/***********************************************************************************************************
 *  @brief				EEPROM 读操作
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :
 ************************************************************************************************************/
extern	returnStatus readEepromData(uint8 tarAddr, uint8* data, uint8 len);

/***********************************************************************************************************
 *  @brief  				EEPROM 写操作
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :
 ************************************************************************************************************/
extern	returnStatus writeEepromData(uint8 tarAddr, uint8* data, uint8 len);

#endif

