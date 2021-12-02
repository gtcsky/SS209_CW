/**
  ******************************************************************************
  * @date    2021-xx-xx
  * @brief   i2c EEPROM(AT24C02)应用函数bsp
  ******************************************************************************
  */

#include "user_i2c.h"
#include "user_menu.h"
#include "io_define.h"
#include "misc.h"
#include "user_exti.h"
#include "user_lcd.h"
#include "user_ddl.h"
#include <string.h>

u8 I2C_buffer[20];
u8 I2C_SEND_START;
u8 I2C_REVCIVE_START;
u8 I2C_SEND_END;
u8 I2C_REVCIVE_END;
u8 Driver_Version[DRIVER_VERSION_LEN + 1] = { 0 };

#define REC_BUFF_LENGTH	10
UINT8 data_received[10] = { 0 };
UINT8 data_num = 0;
UINT8 send_index;
UINT8 * pMasterReadResp;
//UINT8 vTestIndex;
UINT8	aEepromBuff[20]={0};
uint8	aPreWriteBuff[5]={0};
I2C_InitTypeDef I2C_InitStruct;
I2C_EveTypeDef I2C_EveStruct;
I2C_TypeDef* I2Cx;

i2cParamStruct	i2cParams;

//u8	vI2cCommErrorTimes=0;
//u8	vtReadTest=0;

/**
  * @brief  Delay.c
  * @param  nCount: delay time
	* @param  None
  * @retval None
  */
//static void Delay(__IO uint32_t nCount)
//{
//	for(; nCount != 0; nCount--);
//}


void setI2cSdaInput(void) {
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;								//input 模式
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;							//
//	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;									//
//	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStruct);

	I2C_SDA_PORT->AFR &= ~(I2C_SDA_PIN);								//normal IO
	I2C_SDA_PORT->DIR &= ~(I2C_SDA_PIN);								//input mode
	I2C_SDA_PORT->PUPD &=~(GPIO_PUPDR_PUPDR0 << ((uint16_t)I2C_SDA_PIN_SOURCE * 2));		//GPIO_PuPd_NOPULL
}
void setI2cSdaOutput(void) {
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;												//output 模式
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;												//GPIOx_OTYPER   0/1 推挽输出/开漏输出
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;												//不上拉
//	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;							//
//	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStruct);

	I2C_SDA_PORT->AFR &= ~(I2C_SDA_PIN);													//normal IO
	I2C_SDA_PORT->DIR &= ~(I2C_SDA_PIN);													//input mode
	I2C_SDA_PORT->DIR |= I2C_SDA_PIN;														//output mode
	I2C_SDA_PORT->OTYP |=I2C_SDA_PIN;														//open drain
	I2C_SDA_PORT->PUPD &=~(GPIO_PUPDR_PUPDR0 << ((uint16_t)I2C_SDA_PIN_SOURCE * 2));		//GPIO_PuPd_NOPULL
}


void IIC_SDA(u8 a) {
	if (a) {
		setI2cSdaOutput();
	} else {
		setI2cSdaInput();
	}
}


/**
  * @brief  GPIO IIC Start
  * @param  None
	* @param  None
  * @retval None
  */
void IIC_Start(void){
	if((I2C_SDA_PORT->DIR | I2C_SDA_PIN)==0)
		setI2cSdaOutput();
//	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(1);
	CLK_OUT(1);
	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
//	__nop();__nop();__nop();__nop();__nop();
}

/**
  * @brief  GPIO IIC Stop
  * @param  None
	* @param  None
  * @retval None
  */
void IIC_Stop(void)
 {
//	IIC_SDA(1);
	setI2cSdaOutput();
//	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(1);
	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(1);
//	__nop();__nop();__nop();__nop();__nop();

}

/**
  * @brief  GPIO IIC ACK
  * @param  None
	* @param  None
  * @retval None
  */
void IIC_Ack(void)
{
	IIC_SDA(1);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(1);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
//	__nop();__nop();__nop();__nop();__nop();
}

/**
  * @brief  GPIO IIC NACK
  * @param  None
	* @param  None
  * @retval None
  */
void IIC_NAck(void)
{
	IIC_SDA(1);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
	__nop();__nop();__nop();__nop();__nop();
	SDA_OUT(1);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(1);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
//	__nop();__nop();__nop();__nop();__nop();
}

/**
  * @brief  GPIO IIC Wait ACK
  * @param  None
	* @param  None
  * @retval None
  */
u8 IIC_Wait_Ack(void){
	u8 Time=0;
	SDA_OUT(1);
	__nop();__nop();__nop();__nop();__nop();__nop();
	IIC_SDA(0);
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(1);
	while(is_I2cSdaHi)
	{
		Time++;
		if(Time>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	__nop();__nop();__nop();__nop();__nop();
	CLK_OUT(0);
	setI2cSdaOutput();
	return 0;
}

/**
  * @brief  GPIO IIC Send byte.
  * @param  txd：Send byte of data
	* @param  None
  * @retval None
  */
void IIC_Send_Byte(u8 txd){
	u8 t;
//	IIC_SDA(1);
	CLK_OUT(0);
//	__nop();__nop();__nop();__nop();__nop();
	for(t=0;t<8;t++){
		if((txd&0x80)== 0x80)
			SDA_OUT(1);
		else
			SDA_OUT(0);
		txd<<=1;
		__nop();__nop();__nop();__nop();__nop();
//		__nop();__nop();__nop();__nop();__nop();
		CLK_OUT(1);
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		CLK_OUT(0);
//		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

/**
  * @brief  GPIO IIC Read ack.
  * @param  ack : return ack
	* @param  None
  * @retval None
  */
u8 IIC_Read_Byte(u8 ack) {
	u8 i, receive = 0;
	IIC_SDA(0);
//	__nop();__nop();__nop();__nop();__nop();
	for (i = 0; i < 8; i++) {
		receive <<= 1;
		CLK_OUT(0);
		__nop();__nop();__nop();__nop();__nop();
		CLK_OUT(1);
		__nop();__nop();__nop();__nop();__nop();
//		if(GPIO_ReadInputDataBit(DEBUG_IIC_SDA_GPIO_PORT,DEBUG_IIC_SDA_PIN))
		if (is_I2cSdaHi)
			receive++;
		__nop();__nop();__nop();__nop();__nop();
		CLK_OUT(0);
//		__nop();__nop();__nop();__nop();__nop();
	}
	if (!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}

/************************************************************************************
 *
 * 	EE_SEND_START		待发送数据在数组中的起始偏移量
 * 	EE_SEND_END		待发送数据在数组中的结束偏移量(包含)
 *
 *************************************************************************************/

returnStatus	MasterWriteBytes(u8 EE_SEND_START,u8 EE_SEND_END){
	u8 i;
	IIC_Start();
	for(i=0;i<EE_SEND_END;i++)
	{
		IIC_Send_Byte(I2C_buffer[i]);
		if(IIC_Wait_Ack())
		{
			IIC_Stop();
			return I2C_COMM_ERROR;
		}
	}
	IIC_Stop();
	return I2C_COMM_OK;
}
/************************************************************************************
 *
 * 	EE_SEND_START				待发送数据在数组中的起始偏移量
 * 	EE_SEND_END				待发送数据在数组中的结束偏移量(不包含)
 * 	EE_SEND_START~EE_SEND_END写完后,会写入一个 "读取模式" I2C_READ_MODE=LCD_SLAVE_ADDR|0x01
 *	EE_REVCIVE_START			接收数据在数组中的起始存储偏移量
 *	EE_REVCIVE_END				接收数据在数组中的结束存储偏移量(不包含)
 *
 *************************************************************************************/
returnStatus	MasterReadBytes(u8 EE_SEND_START,u8 EE_SEND_END, u8 EE_REVCIVE_START,u8 EE_REVCIVE_END){
	u8 i=0;
	IIC_Start();
	for(i=0;i<(EE_SEND_END);i++)
	{
		IIC_Send_Byte(I2C_buffer[i]);
		if(IIC_Wait_Ack())
		{
			IIC_Stop();
			return I2C_COMM_ERROR;
		}
	}
	IIC_Start();
	IIC_Send_Byte(I2C_buffer[EE_SEND_END]);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return I2C_COMM_ERROR;
	}
	i=EE_REVCIVE_START;
	while(i<EE_REVCIVE_END)
	{
		if (i == (EE_REVCIVE_END-1)){

			I2C_buffer[i]=IIC_Read_Byte(0); //Nack
		}
		else{

			I2C_buffer[i]=IIC_Read_Byte(1); //Nack
		}
		i++;
	}
	IIC_Stop();
	return	I2C_COMM_OK;
}

//u8	vI2cStatus=FALSE;

//extern	u16 vTime1msCnt;
/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = EEPROM_IIC_IRQ;
	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);

	userNVICPrioirySetting(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Init(void){
	I2C_InitTypeDef I2C_InitStruct;

	/*I2C模块初始化*/
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;										//ACK  enable
	I2C_InitStruct.I2C_BroadcastAddress_Response = EEPROM_IIC_BROADCASTADDRESS_RESPONE;
	I2C_InitStruct.I2C_ClockSpeed = EEPROM_IIC_CLOSKSPEED;
	I2C_InitStruct.I2C_Mode = I2C_Mode_Master;
	I2C_InitStruct.I2C_OwnAddress = EEPROM_IIC_OWNADDRESS;

	/*i2c 初始化配置*/
	I2C_Init(EEPROM_IICx, &I2C_InitStruct);

	/*i2c 中断配置*/
	NVIC_Configuration();

	/*I2C发送使能*/
	I2C_Cmd(EEPROM_IICx,ENABLE);
	Master_IIC_Mode(0,10);
	DDL_ZERO_STRUCT(i2cParams);
}
//========================================================================================================
/** @ test_i2c_slavesend 
  * @{  
  */
void SLAVE_IIC_Send(u8 *_pWriteBuf, u8 SEND_START,u8 SEND_END){

	int count,i;
	I2C_SEND_START = SEND_START;
	I2C_SEND_END = SEND_END;
	I2C_EveStruct.I2C_Slave_Recive = 0x00;
	I2C_EveStruct.I2C_Slave_Send =0x01;
	
	count = I2C_SEND_END-I2C_SEND_START;
	for(i=0;i<count;i++)
	I2C_buffer[i] = _pWriteBuf[i];
	
//	I2C_Cmd(I2C,ENABLE);//I2C发送使能
	
	while(I2C_EveStruct.I2C_Slave_Send == 0x01);
}

/** @ test_i2c_slaverecive 
  * @{  
  */
void Master_IIC_Mode(u8 REVCIVE_START,u8 REVCIVE_END){

	I2C_REVCIVE_START = REVCIVE_START;
	I2C_REVCIVE_END = REVCIVE_END;

	I2C_EveStruct.I2C_Master_Send = 0x01;
	I2C_EveStruct.I2C_Master_Recive =0x00;

}

/**
  * @brief  This function handles I2C0 IRQHandler Master send data  with ack
  * @param  None
  * @retval None
  */
void I2C0_IRQHandler_MasterSend(void){
	/*obtain STAT Value*/
	if (I2C->STAT == 0x08 || I2C->STAT == 0x10 ) /* START has been transmitted */
	{
		/*发送7bit的地址数据*/
		I2C_SendData(I2C,I2C_buffer[I2C_SEND_START]);
		/*no Generate start*/
		I2C_GenerateSTART(I2C,DISABLE);
		/*no Generate ack*/
		I2C_AcknowledgeConfig(I2C,DISABLE);
		/*no Generate stop*/
		I2C_GenerateSTOP(I2C,DISABLE);
	}
	else if (I2C->STAT == 0x18 || I2C->STAT == 0x20)/* SLA+W has been transmitted and ACK has been received */
	{
		/*master send data */
		I2C_SEND_START++;
		if(I2C_SEND_START <= I2C_SEND_END)
		{
			I2C_SendData(I2C,I2C_buffer[I2C_SEND_START]);
			/*no Generate start*/
			I2C_GenerateSTART(I2C,DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C,DISABLE);
			/*no Generate stop*/
			I2C_GenerateSTOP(I2C,DISABLE);
		}
		else
		{
			/*no Generate start*/
			I2C_GenerateSTART(I2C,DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C,DISABLE);
			/*Generate stop*/
			I2C_GenerateSTOP(I2C,ENABLE);
			I2C_EveStruct.I2C_Master_Send = 0;
		}
	}
	else if (I2C->STAT == 0x28 ||I2C->STAT == 0x30) /*same to 0x28 can send data */
	{
		I2C_SEND_START++;
		if(I2C_SEND_START < I2C_SEND_END)
		{

			/*master send data */
			I2C_SendData(I2C,I2C_buffer[I2C_SEND_START]);
			/*no Generate start*/
			I2C_GenerateSTART(I2C,DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C,DISABLE);
			/*Generate stop*/
			I2C_GenerateSTOP(I2C,DISABLE);
		}
		else
		{
			/*no Generate start*/
			I2C_GenerateSTART(I2C,DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C,DISABLE);
			/*Generate stop*/
			I2C_GenerateSTOP(I2C,ENABLE);
			I2C_EveStruct.I2C_Master_Send = 0;
		}

	}
	else if (I2C->STAT == 0xF8)										//Own SLA +W has been received ACK has been return
	{
		;/*无任何动作，等待传输*/
	}
	else
	{
		/*no Generate start*/
		I2C_GenerateSTART(I2C,DISABLE);
		/*no Generate ack*/
		I2C_AcknowledgeConfig(I2C,DISABLE);
		/*no Generate stop*/
		I2C_GenerateSTOP(I2C,ENABLE);
		I2C_EveStruct.I2C_Master_Send = 0;
	}
}

/**
  * @brief  This function handles I2C0 IRQHandler Master recive data
  * @param  None
  * @retval None
  */
void I2C0_IRQHandler_MasterRecive(void) {
//	if (vtReadTest) {
//		vtReadTest = 0;
//	}
	if (I2C->STAT == 0x08) {/* START has been transmitted and prepare SLA+R */

		/*发送7bit的地址数据  sla+r*/
		I2C_SendData(I2C, I2C_buffer[I2C_SEND_START]);
		/*no Generate start*/
		I2C_GenerateSTART(I2C, DISABLE);
		/*no Generate ack*/
		I2C_AcknowledgeConfig(I2C, DISABLE);
		/*no Generate stop*/
		I2C_GenerateSTOP(I2C, DISABLE);
	} else if (I2C->STAT == 0x18 || I2C->STAT == 0x20) {/* SLA+W has been transmitted and ACK has been received */

		/*master send data */
		I2C_SEND_START++;
		if (I2C_SEND_START <= (I2C_SEND_END - 1)) {
			I2C_SendData(I2C, I2C_buffer[I2C_SEND_START]);
			/*no Generate start*/
			I2C_GenerateSTART(I2C, DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C, DISABLE);
			/*no Generate stop*/
			I2C_GenerateSTOP(I2C, DISABLE);
		}
	} else if (I2C->STAT == 0x28 || I2C->STAT == 0x30) { /*same to 0x28 can send data */

		I2C_SEND_START++;
		if (I2C_SEND_START <= (I2C_SEND_END - 1)) {
			/*master send data */
			I2C_SendData(I2C, I2C_buffer[I2C_SEND_START]);
			/*no Generate start*/
			I2C_GenerateSTART(I2C, DISABLE);
			/*no Generate ack*/
			I2C_AcknowledgeConfig(I2C, DISABLE);
		} else {
			/* Generate start*/
			I2C_GenerateSTART(I2C, ENABLE);
		}
	} else if (I2C->STAT == 0x10) {
//		vtReadTest = 1;
		/*master send data */
		I2C_SendData(I2C, I2C_buffer[I2C_SEND_END]);
//			I2C_SendData(I2C,LCD_SLAVE_ADDR|I2C_READ_MODE);
		I2C_GenerateSTOP(I2C, DISABLE);
	} else if (I2C->STAT == 0x38) {
		/*Generate start*/
		I2C_GenerateSTART(I2C, ENABLE);
		I2C_GenerateSTOP(I2C, DISABLE);
	} else if (I2C->STAT == 0x40) {/* SLA+R has been transmitted and ACK has been received */

		/*设置第一个字节recive ack*/
		if (I2C_REVCIVE_START < I2C_REVCIVE_END) {
			if (I2C_REVCIVE_START == (I2C_REVCIVE_END - 1))
				I2C_AcknowledgeConfig(I2C, DISABLE);										//goto the status(0x50)
			else
				I2C_AcknowledgeConfig(I2C, ENABLE);										//goto the status(0x50)
			I2C_GenerateSTART(I2C, DISABLE);
			I2C_GenerateSTOP(I2C, DISABLE);
		} else {
			I2C_GenerateSTOP(I2C, ENABLE);
			/*Generate dummy start*/
			I2C_GenerateSTART(I2C, DISABLE);
		}
	} else if (I2C->STAT == 0x48) {/* SLA+R has been transmitted and ACK has been received */

		/*Generate start*/
		I2C_GenerateSTART(I2C, ENABLE);
		I2C_GenerateSTOP(I2C, ENABLE);
	} else if (I2C->STAT == 0x50) {

		I2C_buffer[I2C_REVCIVE_START++] = I2C_ReceiveData(I2C);	//  data recive
		if (I2C_REVCIVE_START < I2C_REVCIVE_END) {
			if (I2C_REVCIVE_START == (I2C_REVCIVE_END - 1)) { //设置倒数第一个数据接收

				/*设置最后一个字节recive ack*/
				I2C_AcknowledgeConfig(I2C, DISABLE); //data recive no over
				I2C_GenerateSTART(I2C, DISABLE);
				I2C_GenerateSTOP(I2C, DISABLE);
			} else {
				/*设置下一个字节recive ack*/
				I2C_AcknowledgeConfig(I2C, ENABLE); //data recive no over
				I2C_GenerateSTART(I2C, DISABLE);
				I2C_GenerateSTOP(I2C, DISABLE);
			}
		}
	} else if (I2C->STAT == 0x58) { /* SLA+R has been transmitted and ACK has been received */

		I2C_buffer[I2C_REVCIVE_START++] = I2C_ReceiveData(I2C);
		I2C_GenerateSTOP(I2C, ENABLE);
		/*Generate dummy start*/
		I2C_GenerateSTART(I2C, DISABLE);
		I2C_EveStruct.I2C_Master_Recive = 0x00;

	} else if (I2C->STAT == 0xF8) { //Own SLA +W has been received ACK has been return

		/*无任何动作，等待传输*/

	} else {
		I2C_EveStruct.I2C_Master_Recive = 0x00;
		/*Generate start*/
		I2C_GenerateSTART(I2C, DISABLE);
		/*no Generate ack*/
		I2C_AcknowledgeConfig(I2C, DISABLE);
		/*接收结束，设置STOP*/
		I2C_GenerateSTOP(I2C, ENABLE);
	}
}

void SlaveSendACK(FunctionalState NewState) {
	I2C_GenerateSTART(I2C, DISABLE);
	/*no Generate stop*/
	I2C_GenerateSTOP(I2C, DISABLE);
	if (NewState == ENABLE) {
		/*Generate ack*/
		I2C_AcknowledgeConfig(I2C, ENABLE);				//回复ACK
	} else {
		/*no Generate ack*/
		I2C_AcknowledgeConfig(I2C, DISABLE);				//回复NACK
	}
}

/**
  * @brief  This function handles I2C0 IRQHandler
  * @param  None
  * @retval None
  */
void EEPROM_IIC_IRQHandler(void) {
	if (I2C_EveStruct.I2C_Master_Send == 0x01) { /*master send mode */

		I2C0_IRQHandler_MasterSend();
	} //end  master send
	if (I2C_EveStruct.I2C_Master_Recive == 0x01) { /*master recive mode*/

		I2C0_IRQHandler_MasterRecive();
	}  //end master recive
	if (I2C->CR & 0x08)   //退出中断清中断标志位；
		I2C->CR &= 0xf7;
}

returnStatus readI2cData(u8 *addr, u8 len) {
	I2C_buffer[0] = LCD_SLAVE_ADDR;
	I2C_buffer[1] = *addr;
	I2C_buffer[2] = LCD_SLAVE_ADDR | I2C_READ_MODE;
	if (I2C_COMM_ERROR == MasterReadBytes(0, 2, 3, 3 + len)) {
		return I2C_COMM_ERROR;
	} else {
		if (I2C_buffer[1] == ReadSwVersion) {
			memcpy(Driver_Version, &I2C_buffer[6], DRIVER_VERSION_LEN);
			fIsDriverVersionGot = TRUE;
		} else if (I2C_buffer[1] == ReadTemperature) {
			if (I2C_buffer[3])
				vSystemTemperature = I2C_buffer[3] - 1;
		}
		return I2C_COMM_OK;
	}
}


returnStatus	 readSigmaCtrlRegister(u8 addr,u8* data){
//		user_I2C_EEPROM_BufferRead(HT_I2C0,SIGMAL_DEVICE_ADDR,data,addr,1);
//	u8 vtTemp=0;
	I2C_buffer[0]=LCD_SLAVE_ADDR;
	I2C_buffer[1]=addr;
	I2C_buffer[2]=LCD_SLAVE_ADDR|I2C_READ_MODE;
	if(I2C_COMM_ERROR==MasterReadBytes(0,2,3,4))
		return	I2C_COMM_ERROR;
	else
		* data=I2C_buffer[3];
		return	I2C_COMM_OK;

}

returnStatus writeI2cData(u8* data, u8 len) {

	I2C_buffer[0] = LCD_SLAVE_ADDR;
	memcpy(&I2C_buffer[1], data, len + 1);
//	if (I2C_COMM_ERROR == MasterWriteBytes(0, 2 + len))
//		return I2C_COMM_ERROR;
//	else
//		return I2C_COMM_OK;
	return MasterWriteBytes(0, 2 + len);
}
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
void writeLcdCommand(u8 cmd) {

	aPreWriteBuff[0] = OP_Command;
	aPreWriteBuff[1] = cmd;
	writeI2cData(aPreWriteBuff, 1);

}
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
void writeLcdData(u8 data) {

	aPreWriteBuff[0] = OP_Data;
	aPreWriteBuff[1] = data;
	writeI2cData(aPreWriteBuff, 1);

}

returnStatus	riteSigmaCtrlRegister(u8 addr,u8* data){

	I2C_buffer[0] = LCD_SLAVE_ADDR;
	I2C_buffer[1] = addr;
	I2C_buffer[2] = * data;
	return MasterWriteBytes(0, 3);

}
#if(EEPROM_INSIDE==1)
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
returnStatus writeEepromData(uint8 tarAddr, uint8* data, uint8 len) {
	uint8 writeSize = 0;
	uint8 retryTimes = 3;
	returnStatus ret = I2C_COMM_ERROR;
	I2C_buffer[0] = EEPROM_SLAVE_ADDR;

	while (len) {
		I2C_buffer[1] = tarAddr;
		memcpy(&I2C_buffer[2], data, len);
		writeSize = (len > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : len;
		len-=writeSize;
		do {
			ret = MasterWriteBytes(0, 2 + writeSize);
			if (ret != I2C_COMM_OK) {			//写入失败
				if (--retryTimes) {
					uint16 tick = 0x10000 - sysTimes.vTime1msCnt;
					while (((tick + sysTimes.vTime1msCnt) & 0xFFFF) < 6)
						;	//5ms后重试
				}
			} else {							//写入成功
				break;
			}
		}while (ret != I2C_COMM_OK&&retryTimes);
		if(ret != I2C_COMM_OK){
			return ret;
		}

		data+=writeSize;
		if(len){
			tarAddr+=writeSize;
			if(tarAddr>=EEPROM_PAGE_SIZE*EEPROM_TOTAL_PAGES)
				return I2C_OUT_OF_SIZE;					//大于最大Eeprom地址
			uint16 tick = 0x10000 - sysTimes.vTime1msCnt;
			while (((tick + sysTimes.vTime1msCnt) & 0xFFFF) < 6);	//5ms后再写第二个Page
		}
	}
	return ret;
}
#endif
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
//returnStatus readEepromData(uint8 tarAddr, uint8* data, uint8 len) {
//	I2C_buffer[0] = EEPROM_SLAVE_ADDR;
//	I2C_buffer[1] = tarAddr;
//	I2C_buffer[2] = EEPROM_SLAVE_ADDR | I2C_READ_MODE;
//	if (I2C_COMM_ERROR == MasterReadBytes(0, 2, 3, 3+len))
//		return I2C_COMM_ERROR;
//	else
//		memcpy(data, &I2C_buffer[3], len);
//	return I2C_COMM_OK;
//}
#if(EEPROM_INSIDE==1)
returnStatus readEepromData(uint8 tarAddr, uint8* data, uint8 len) {
	uint8 readSize = 0;
	uint8 retryTimes = 3;
	returnStatus ret = I2C_COMM_ERROR;
	I2C_buffer[0] = EEPROM_SLAVE_ADDR;

	while (len) {

		I2C_buffer[1] = tarAddr;
		I2C_buffer[2] = EEPROM_SLAVE_ADDR | I2C_READ_MODE;
		readSize= (len > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : len;
		len-=readSize;

		do{
			ret= MasterReadBytes(0, 2, 3, 3 + readSize);
			if (ret != I2C_COMM_OK) {			//读取失败
				if (--retryTimes) {
					uint16 tick = 0x10000 - sysTimes.vTime1msCnt;
					while (((tick + sysTimes.vTime1msCnt) & 0xFFFF) < 6)
						;	//5ms后重试
				}
			} else {							//读取成功
				memcpy(data, &I2C_buffer[3], readSize);
				break;
			}
		}while (ret != I2C_COMM_OK&&retryTimes);
		if(ret != I2C_COMM_OK){
			return ret;
		}

		data+=readSize;
		if(len){
			tarAddr+=readSize;
			if(tarAddr>=EEPROM_PAGE_SIZE*EEPROM_TOTAL_PAGES)
				return I2C_OUT_OF_SIZE;					//大于最大Eeprom地址
		}
	}
	return ret;
}
#endif
/***********************************************************************************************************
  *  @brief				EEPROM读写测试
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
//void eepromTest(void) {
//	for (uint8 i = 0; i < 20; i++) {
//		aEepromBuff[i] = i + 0x40;
//	}
//	writeEepromData(0x00, aEepromBuff, 20);
//	memset(aEepromBuff, 0, 20);
//
//	sysTimes.vSystem5ms = 0;
//	while (sysTimes.vSystem5ms < 1)
//		;
//	readEepromData(0x00, aEepromBuff, 20);
//}

/*********************************************END OF FILE**********************/
