/*
 * user_spi.c
 *
 *  Created on: 2019年11月25日
 *      Author: Bough_004
 */

#include	"user_spi.h"
#include	"user_lptimer.h"
#include	"user_menu.h"
#include "misc.h"

uint8_t  SPIFlagState;
uint8_t  SPI_Data;
uint8_t  MASTERFLAG;
//uint8	spiTest=0;

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;

 /* 嵌套向量中断控制器组选择 */
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

 /* 配置USART为中断源 */
 NVIC_InitStructure.NVIC_IRQChannel = SPI0COMB_IRQn;
 /* 抢断优先级*/
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 /* 子优先级 */
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 /* 使能中断 */
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 /* 初始化配置NVIC */
 NVIC_Init(&NVIC_InitStructure);
}



 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_SendByte(uint8_t byte)
 {
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_SendData(SPI, byte);
	while (SPIFlagState == 0&&sysTimes.vSystem5ms<200) {
		;
	}
	SPIFlagState = 0;
	return 0;
}

void WriteData(uint8_t *sendstr,uint8_t sendlen){
	    uint8_t i;
	    /* 通讯开始：CS低 */
	   SPI_SSOutputCmd(SPI,ENABLE);
	 //   Spi_SendData(Spi0,*sendstr);
	    for(i=0;i<sendlen;i++)
	    {
		    SPI_SendByte(*(sendstr + i));
	    }
	    /*通讯结束：CS高 */
	    SPI_SSOutputCmd(SPI,DISABLE);
}
void ReadData(uint8_t *recvstr,uint8_t recvlen){

}

void spiIoInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = SPI0_CS_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(SPI0_CS_PORT, &GPIO_InitStruct);

	/* 配置SPI的 SCK引脚*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = SPI0_SCK_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(SPI0_SCK_PORT, &GPIO_InitStruct);

	/* 配置SPI的 MISO引脚*/		//未使用,用于LCD DC  control
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Pin = Lcd_Dc_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
//	GPIO_Init(Lcd_Dc_Port, &GPIO_InitStruct);

	/* 配置SPI的 MOSI引脚*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = SPI0_MOSI_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(SPI0_MOSI_PORT, &GPIO_InitStruct);

	/*管脚复用*/
	GPIO_PinAFConfig(SPI0_SCK_PORT, SPI0_SCK_PIN, SPI0_SCK_FuncSel); 					//Spiclk
	GPIO_PinAFConfig(SPI0_CS_PORT, SPI0_CS_PIN, SPI0_CS_FuncSel); 						//Spinss
//	GPIO_PinAFConfig(Lcd_Dc_Port, Lcd_Dc_Pin, EEPROM_SPI_MISO_AF_VALUE); 			//Spimiso
	GPIO_PinAFConfig(SPI0_MOSI_PORT, SPI0_MOSI_PIN, SPI0_MOSI_FuncSel); 				//Spimosi
	/* 停止信号 EEPROM: CS引脚高电平*/
	SPI_SSOutputCmd(SPI, DISABLE);
}


void userSpiInit(void) {
	SPI_InitTypeDef SPI_InitStruct;
	spiIoInit();

	/* SPI 模式配置 */
	// EEPROM芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI, &SPI_InitStruct);

	NVIC_Configuration();
	SPI_Cmd(SPI,ENABLE);
	MASTERFLAG = 0x01;
	SPIFlagState = 0x00;
}

void	disableSpi(void){
//	spiTest=0;
	SPI_Cmd(SPI,DISABLE);
}
void	enableSpi(void){
//	spiTest=1;
	SPI_Cmd(SPI,ENABLE);
}

/**
  * @brief  This function handles SPI0COMB_IRQn interrupt request.
  * @param  None
  * @retval None
  */
void SPI0COMB_IRQHandler(void)
 {
	while ((SPI->STAT & 0x80) == 0x80) {
		SPI_Data = SPI->DATA;
	}
	if (MASTERFLAG == 0x00)
		SPI->DATA = 0xff;
	SPIFlagState = 0x01;
}
