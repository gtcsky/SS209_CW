/*
 * user_SysClock.c
 *
 *  Created on: 2019年8月12日
 *      Author: Bough_004
 */
#include "user_SysClock.h"
#include "user_clktrim.h"
#include "user_internalFlash.h"
#include "main.h"

#define 	HIRC_24M_ADDR				0X180000C0
#define	HIRC_22M_ADDR					0X180000C2
#define	HIRC_16M_ADDR					0X180000C4
#define	HIRC_8M_ADDR					0X180000C6
#define	HIRC_4M_ADDR					0X180000C8
#define	LIRC_32K768_ADDR			0X180000D0
#define	LIRC_38K4_ADDR				0X180000D4


#define HIRC24M_FLASHADDR									0x180000A0
#define HIRC22M_FLASHADDR									0x180000A2
#define HIRC16M_FLASHADDR									0x180000A4
#define HIRC8M_FLASHADDR									0x180000A6
#define HIRC4M_FLASHADDR									0x180000A8
#define		LIRC_32K768_FLASHADDR							0X180000B0
#define		LIRC_38K4_FLASHADDR								0X180000B4

#define RCC_HIRCCALIBRATION_24M       					(*(uint16_t*)(HIRC24M_FLASHADDR))
#define RCC_HIRCCALIBRATION_22M						(*(uint16_t*)(HIRC22M_FLASHADDR))
#define RCC_HIRCCALIBRATION_16M						(*(uint16_t*)(HIRC16M_FLASHADDR))
#define RCC_HIRCCALIBRATION_8M						(*(uint16_t*)(HIRC8M_FLASHADDR))
#define RCC_HIRCCALIBRATION_4M						(*(uint16_t*)(HIRC4M_FLASHADDR))


void setSystemHIRC(SystemHIRCType type){
	u32 vtRcc_HIRccr_data=0x5A690000;
	u16 vTemp16=0;
	RCC_SysclkCmd(RCC,RCC_SYSCLKSource_HIRC_EN,ENABLE);			//启用HIRC
	//读取出厂时flash中校准过的HIRC值
	switch (type){
	case HIRC_24M:
		vTemp16=RCC_HIRCCALIBRATION_24M ;
		break;
	case HIRC_22M:
		vTemp16=RCC_HIRCCALIBRATION_22M;
		break;
	case HIRC_16M:
		vTemp16=RCC_HIRCCALIBRATION_16M;
		break;
	case HIRC_8M:
		vTemp16=RCC_HIRCCALIBRATION_8M;
		break;
	case HIRC_4M:
		vTemp16=RCC_HIRCCALIBRATION_4M;
		break;
	}
	vTemp16&=0x0FFF;
	vtRcc_HIRccr_data|=vTemp16;
	RCC->REGLOCK =  RCC_REGLOCKKEY;
	RCC->HIRCCR = 	vtRcc_HIRccr_data;							//使用出厂校准过的HIRC值.
	RCC->REGLOCK =  RCC_RESGLOCKKEY;
}
void setSystemLIRC(SystemLIRCType type){
	u32 vtRcc_LIRccr_data=0x5A690000;
	u16 vTemp16=0;
	RCC_SysclkCmd(RCC,RCC_SYSCLKSource_LIRC_EN,ENABLE);			//启用HIRC
	//读取出厂时flash中校准过的HIRC值
	if(type==LIRC_32K768)
		vTemp16=readInternalFlashWrod(LIRC_32K768_FLASHADDR);
	else
		vTemp16=readInternalFlashWrod(LIRC_38K4_FLASHADDR);
	vTemp16&=0x01FF;
	vtRcc_LIRccr_data|=vTemp16;
	RCC->REGLOCK =  RCC_REGLOCKKEY;
	RCC->LIRCCR = 	vtRcc_LIRccr_data;							//使用出厂校准过的LIRC值.
	RCC->REGLOCK =  RCC_RESGLOCKKEY;
//	while(RCC->LIRCCR&0x00001000);							//等待LIRC稳定
	while(!(RCC->LIRCCR&0x00001000));							//等待LIRC稳定
}
/*
 * 选择时钟
 */
void systemClkSelect(SystemClkSelectType vtData){
	u32	vTemp32=0x5A690000;
	vTemp32|=vtData;
	RCC->REGLOCK =  RCC_REGLOCKKEY;
	RCC->CLKSEL=vTemp32;
	RCC->REGLOCK =  RCC_RESGLOCKKEY;
}

void systemClkSet(void){
	setSystemHIRC(HIRC_24M);									//设定高速时钟
	setSystemLIRC(LIRC_38K4);									//设定低速时钟
	RCC->REGLOCK =  RCC_REGLOCKKEY;
	RCC->CLKCON  |= 0x5a690085;								//(RCC_SYSCLKCR   启用HIRC和LIRC,并且设定从deep sleep醒来后自动启用并切换HIRC
	RCC->REGLOCK =  RCC_RESGLOCKKEY;
	systemClkSelect(HIRC_SEL);									//选择HIRC
}

