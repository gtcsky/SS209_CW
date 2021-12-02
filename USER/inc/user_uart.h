#ifndef __CX32LOO3_USER_UART_H
#define __CX32LOO3_USER_UART_H


/* Includes --------------------------*/
#include "cx32l003_uart.h"
#include "cx32l003_gpio.h"
#include "cx32l003_rcc.h"
#include "const.h"


//// 串口0-UART0
#define  UARTx                   		UART0
#define  UART_BAUDRATE           57600

#define  RATEDBAUD_ENABLE    	ENABLE
#define  RATETIMER_ENABLE    	DISABLE
#define  UART_TX_GPIO_PORT       	GPIOD
#define  UART_TX_GPIO_PIN        	GPIO_Pin_4
#define  UART_TX_PIN_SOURCE      	GPIO_PinSource4

#define  UART_RX_GPIO_PORT       	GPIOD
#define  UART_RX_GPIO_PIN        	GPIO_Pin_3
#define  UART_RX_PIN_SOURCE      	GPIO_PinSource3

#define  UART_RX_AF_VALUE         	GPIO_AF_UART0_RX_PD3
#define  UART_TX_AF_VALUE         	GPIO_AF_UART0_TX_PD4

#define  UART_MODE               		UART_MODE1
#define  UART_IRQ                		UART0_IRQn
#define  UART_IRQHandler         	UART0_IRQHandler
#define  TIMx      					TIM10


#define	UART_TIME_OUT_CONST		3	//3ms
#define	UART_CMD_MIN_LEN			3//5
#define	UART_TX_BUFFER_SIZE		25
#define	UART_BUFFER_SIZE			25
#define	CMD_OFFSET					5


#define	UART_START_BYTE					0XAA
#define	UART_READ_START_BYTE				0XA5
#define	COLOR_TEMP_LENGTH				6
#define	RGB_DATA_LENGTH					8
//#define	PRESET_EFFECT_DATA_LENGTH			6
#define	PRESET_EFFECT_DATA_LENGTH			7
#define	ON_OFF_DATA_LENGTH				5
#define	BRIGHTNESS_ONLY_LENGTH			5
#define	READ_TEMPERATURE_LENGTH			3
#define	READ_VERSION_LENGTH				3


extern	u8 	FlagState ;
extern	u8	vReceivedByte;
extern	u8	fIsUartSending;
extern	u8	fIsUartReceiving;
extern	u8 	UART_TX_BUF[UART_TX_BUFFER_SIZE];
#if(COMMUNICATION_PROTOCAL_TYPE==0)
extern	u8 	Driver_Version[DRIVER_VERSION_LEN+1];
#endif
extern	u8	fIsUartStartRecevingValidData;
void 	UART_Config(void);
void 	Uart_SendByte( UART_TypeDef * pUARTx, uint8_t ch);
void 	Uart_SendString( UART_TypeDef * pUSARTx, unsigned char *str,UINT8 len);
void 	Uart_SendHalfWord( UART_TypeDef * pUARTx, uint16_t ch);
void 	Uart_GetString(UART_TypeDef * pUARTx, uint8_t* g_cmd_buf);
void 	Uart_SendArray( UART_TypeDef * pUARTx, uint8_t *array, uint16_t num);
extern	void		uartSendDataProcess(void);
extern	void 	uartReceivedDataProcess(void);
void 	setRgbData(u8 len,displayParamsStruct * disParams);
void 	setColorTempData(u8 len,displayParamsStruct * disParams);
void		setEffectData(u8 len,displayParamsStruct * disParams);
void		setOnOffData(u8 len,displayParamsStruct * disParams,u8 status);
void		setBrightnessOnlyData(u8 len,displayParamsStruct * disParams);
void		readTemperatureData(void);
void		readSwVersion(void);
void		checkUartTxBuffer(void);
#endif

