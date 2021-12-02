/*
 * io_define.h
 *
 *  Created on: 2019年8月13日
 *      Author: Bough_004
 */

#ifndef USER_INC_IO_DEFINE_H_
#define USER_INC_IO_DEFINE_H_
#include "const.h"

#define	ADC_BATT_CHL					ADC_Channel4_ENABLE
#define	ADC_BATT_No					4
#define 	BatteryDet_Port					GPIOD
#define 	BatteryDet_Pin					GPIO_Pin_3

#define	ADC_TEMP_CHL					ADC_Channel3_ENABLE
#define	TEMP_CH_No					3
#define 	Temperture_Port					GPIOD
#define 	Temperture_Pin					GPIO_Pin_2

#define 	CW_LED_PORT					GPIOD
#define 	CW_LED_PIN						GPIO_Pin_5
#define 	CW_LED_CHANNEL				PWM_CH1		//(CH1N)
//#define 	CW_LED_DUTY_INDEX				0X01
#define	CW_PIN_CONFIG_CLR				0xFF0FFFFF	//0b00001000
#define	CW_PIN_CONFIG					0x00100000	//0b00001000
#define	CLR_CW_LED_PIN					GPIO_ResetBits(CW_LED_PORT, CW_LED_PIN)

#define 	MW_LED_PORT					GPIOA
#define 	MW_LED_PIN					GPIO_Pin_1		//(CH2N)
#define 	MW_LED_CHANNEL				PWM_CH2
#define	MW_PIN_CONFIG_CLR			0xFFFFFF0F
#define	MW_PIN_CONFIG					0x00000010
//#define 	MW_LED_DUTY_INDEX			0X02
#define	CLR_MW_LED_PIN				GPIO_ResetBits(MW_LED_PORT, MW_LED_PIN)

#define 	Red_LED_PORT					GPIOA
#define 	Red_LED_PIN						GPIO_Pin_2		//(CH3)
#define 	Red_LED_CHANNEL				PWM_CH3
#define	Red_PIN_CONFIG_CLR				0xFFFFF0FF
#define	Red_PIN_CONFIG					0x00000100
//#define 	Red_LED_DUTY_INDEX			0X04
#define	CLR_Red_LED_PIN				GPIO_ResetBits(Red_LED_PORT, Red_LED_PIN)




#define 	Green_LED_PORT							GPIOD
#define 	Green_LED_PIN							GPIO_Pin_6
#define	Green_PIN_CONFIG_CLR					0xF0FFFFFF
#define	Green_PIN_CONFIG						0x01000000
#define 	Green_LED_CHANNEL						PWM_CH2		//(CH2)
#define	CLR_Green_LED_PIN						GPIO_ResetBits(Green_LED_PORT, Green_LED_PIN)

#define 	Blue_LED_PORT							GPIOD
#define 	Blue_LED_PIN							GPIO_Pin_4
#define 	Blue_LED_CHANNEL						PWM_CH1
//#define 	Blue_LED_DUTY_INDEX					0X03
#define	Blue_PIN_CONFIG_CLR					0xFFF0FFFF	//0b00001000
#define	Blue_PIN_CONFIG						0x00010000	//0b00001000
#define	CLR_Blue_LED_PIN						GPIO_ResetBits(Blue_LED_PORT, Blue_LED_PIN)

#define	KeyPower_Port							GPIOA
#define	KeyPower_Pin							GPIO_Pin_3
#define	KeyPower_PinNo							3
#define 	Power_KEY_PRESSED   					0x01
#define	checkPowerKeyPressed					(vIsKeyPressed&Power_KEY_PRESSED)
#define	clrPowerKeyPressed						vIsKeyPressed&=~Power_KEY_PRESSED//0xFD
#define	setPowerKeyPressed						vIsKeyPressed|=Power_KEY_PRESSED
#define	GET_POWER_KEY_PIN_STTS				GPIO_ReadInputDataBit(KeyPower_Port, KeyPower_Pin)

#define	KeyInc_Port								GPIOC
#define	KeyInc_Pin								GPIO_Pin_3
#define	KeyInc_PinNo							3
#define 	Inc_KEY_PRESSED   						0x20
#define	checkIncKeyPressed						(vIsKeyPressed&Inc_KEY_PRESSED)
#define	clrIncKeyPressed							vIsKeyPressed&=~Inc_KEY_PRESSED//0xFB
#define	setIncKeyPressed							vIsKeyPressed|=Inc_KEY_PRESSED
#define	GET_INC_KEY_PIN_STTS					GPIO_ReadInputDataBit(KeyInc_Port, KeyInc_Pin)
#define	Right_Pin								GPIO_ReadInputDataBit(KeyInc_Port, KeyInc_Pin)

#define	KeyDec_Port								GPIOC				//GPIOA
#define	KeyDec_Pin								GPIO_Pin_4			//GPIO_Pin_3
#define	KeyDec_PinNo							4					//3
#define 	Dec_KEY_PRESSED   						0x10
#define	checkDecKeyPressed						(vIsKeyPressed&Dec_KEY_PRESSED)
#define	clrDecKeyPressed							vIsKeyPressed&=~Dec_KEY_PRESSED//0xF7
#define	setDecKeyPressed						vIsKeyPressed|=Dec_KEY_PRESSED
#define	GET_DEC_KEY_PIN_STTS					GPIO_ReadInputDataBit(KeyDec_Port, KeyDec_Pin)
#define	Left_Pin									GPIO_ReadInputDataBit(KeyDec_Port, KeyDec_Pin)

#define	KeyBrightInc_Port						GPIOC
#define	KeyBrightInc_Pin							GPIO_Pin_5
#define	KeyBrightInc_PinNo						5
//#define 	Bright_Inc_KEY_PRESSED   					0x20
//#define	checkBrightIncKeyPressed					(vIsKeyPressed&Inc_KEY_PRESSED)
//#define	clrBrightIncKeyPressed						vIsKeyPressed&=~Inc_KEY_PRESSED//0xFB
//#define	setBrightIncKeyPressed					vIsKeyPressed|=Inc_KEY_PRESSED

#define	KeyBrightDec_Port						GPIOC
#define	KeyBrightDec_Pin							GPIO_Pin_6
#define	KeyBrightDec_PinNo						6
//#define 	Bright_Dec_KEY_PRESSED   				0x20
//#define	checkBrightDecKeyPressed				(vIsKeyPressed&Inc_KEY_PRESSED)
//#define	clrBrightDecKeyPressed					vIsKeyPressed&=~Inc_KEY_PRESSED//0xFB
//#define	setBrightDecKeyPressed					vIsKeyPressed|=Inc_KEY_PRESSED
//#define	GET_INC_KEY_PIN_STTS					GPIO_ReadInputDataBit(KeyInc_Port, KeyInc_Pin)
//#define	Right_Pin								GPIO_ReadInputDataBit(KeyInc_Port, KeyInc_Pin)

#define	KeyDec_Port								GPIOC				//GPIOA
#define	KeyDec_Pin								GPIO_Pin_4			//GPIO_Pin_3
#define	KeyDec_PinNo							4					//3
#define 	Dec_KEY_PRESSED   						0x10
#define	checkDecKeyPressed						(vIsKeyPressed&Dec_KEY_PRESSED)
#define	clrDecKeyPressed							vIsKeyPressed&=~Dec_KEY_PRESSED//0xF7
#define	setDecKeyPressed						vIsKeyPressed|=Dec_KEY_PRESSED
#define	GET_DEC_KEY_PIN_STTS					GPIO_ReadInputDataBit(KeyDec_Port, KeyDec_Pin)
#define	Left_Pin									GPIO_ReadInputDataBit(KeyDec_Port, KeyDec_Pin)


//#define	KeySwitch_Port							GPIOC
//#define	KeySwitch_Pin							GPIO_Pin_5
//#define	KeySwitch_PinNo							5
//#define	GET_SWITCH_KEY_PIN_STTS					GPIO_ReadInputDataBit(KeySwitch_Port, KeySwitch_Pin)

//#define	ChargePort				GPIOD
//#define	ChargePin				GPIO_Pin_2
//#define	Get_ChargePin_Stts		GPIO_ReadInputDataBit(ChargePort, ChargePin)

//#define	BattFullyPort				GPIOB
//#define	BattFullyPin				GPIO_Pin_5
//#define	Get_BattFully_Stts			GPIO_ReadInputDataBit(BattFullyPort, BattFullyPin)


//#define	DischargePort			GPIOC
//#define	DischargePin				GPIO_Pin_3
//#define	Get_DischargePin_Stts		GPIO_ReadInputDataBit(DischargePort, DischargePin)



#define	SPI0_SCK_PORT			GPIOA
#define	SPI0_SCK_PIN			GPIO_Pin_1
#define	SPI0_SCK_PinSource		GPIO_PinSource1
#define	SPI0_SCK_FuncSel			GPIO_AF_SPI_CLK_PA1

#define	SPI0_CS_PORT			GPIOA
#define	SPI0_CS_PIN				GPIO_Pin_2
#define	SPI0_CS_PinSource		GPIO_PinSource2
#define	SPI0_CS_FuncSel			GPIO_AF_SPI_NSS_PA3

#define	SPI0_MOSI_PORT			GPIOD
#define	SPI0_MOSI_PIN			GPIO_Pin_6
#define	SPI0_MOSI_PinSource		GPIO_PinSource6
#define	SPI0_MOSI_FuncSel		GPIO_AF_SPI_M0SI_PD6

#define	Lcd_Res_Port						GPIOD
#define	Lcd_Res_Pin						GPIO_Pin_1										//LCD  reset pin
#define	Lcd_Res_FuncSel					0
#define	set_LcdResPin					GPIO_SetBits(Lcd_Res_Port, Lcd_Res_Pin)
#define	clr_LcdResPin					GPIO_ResetBits(Lcd_Res_Port, Lcd_Res_Pin)
#define	GET_LCD_RES_PIN_STTS			GPIO_ReadOutputDataBit(Lcd_Res_Port, Lcd_Res_Pin)

//#define	Lcd_Dc_Port					GPIOD
//#define	SPI0_MISO_PORT				GPIOD
//#define	Lcd_Dc_Pin					GPIO_Pin_5											//LCD  DC  (data/command) pin
//#define	SPI0_MISO_PIN				GPIO_Pin_5
//#define	Lcd_Dc_FuncSel				0
//#define	SPI0_MISO_FuncSel			0
//#define	set_LcdDcPin					GPIO_SetBits(Lcd_Dc_Port, Lcd_Dc_Pin)
//#define	clr_LcdDcPin					GPIO_ResetBits(Lcd_Dc_Port, Lcd_Dc_Pin)

#define	Debug_Port						GPIOC
#define	Debug_Pin						GPIO_Pin_4
#define	set_Debug_Pin					GPIO_SetBits(Debug_Port, Debug_Pin)
#define	clr_Debug_Pin					GPIO_ResetBits(Debug_Port, Debug_Pin)

//#define	DriverReset_Port				GPIOC
//#define	DriverReset_Pin				GPIO_Pin_7
//#define	set_DriverResetPin				GPIO_SetBits(DriverReset_Port, DriverReset_Pin)
//#define	clr_DriverResetPin				GPIO_ResetBits(DriverReset_Port, DriverReset_Pin)

//#define	CommunPort					GPIOB
//#define	CommunPin					GPIO_Pin_4
//#define	Get_Comm_Int_Stts			GPIO_ReadInputDataBit(CommunPort, CommunPin)

#define	LED_Power_Port					GPIOC
#define	LED_Power_Pin					GPIO_Pin_7
#define	LED_Power_FuncSel				0
//#define	LEDPowerOn						GPIO_SetBits(LED_Power_Port, LED_Power_Pin);fTimerToPowerOnLED=1;vPowerOnLEDCnt=ON_LED_DELAY_CONST
//#define	LEDPowerOff						fTimerToPowerOnLED=0;vPowerOnLEDCnt=0;GPIO_ResetBits(LED_Power_Port, LED_Power_Pin)
#define	LEDPowerOn						GPIO_SetBits(LED_Power_Port, LED_Power_Pin)
#define	LEDPowerOff					GPIO_ResetBits(LED_Power_Port, LED_Power_Pin)

#define      	I2C_CLK_PORT                 		GPIOB
#define     	I2C_CLK_PIN                  		GPIO_Pin_4
#define      	I2C_CLK_PIN_SOURCE           	GPIO_PinSource4
#define      	set_I2C_CLK					GPIO_SetBits(I2C_CLK_PORT, I2C_CLK_PIN)
#define      	clr_I2C_CLK					GPIO_ResetBits(I2C_CLK_PORT, I2C_CLK_PIN)
#define 		CLK_OUT(a)  if(a) 				GPIO_SetBits(I2C_CLK_PORT, I2C_CLK_PIN);\
										else \
										GPIO_ResetBits(I2C_CLK_PORT, I2C_CLK_PIN)
//SDA引脚
#define      	I2C_SDA_PORT                 		GPIOB
#define      	I2C_SDA_PIN                  		GPIO_Pin_5
#define      	I2C_SDA_PIN_SOURCE           	GPIO_PinSource5
#define     	set_I2C_SDA					GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define      	clr_I2C_SDA					GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define		is_I2cSdaHi					GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN)

#define SDA_OUT(a)  if(a) \
	                     	     	     	     	     	     	    	GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);\
	                     	     	     	     	     	     	        else \
									        GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)

#endif /* USER_INC_IO_DEFINE_H_ */
