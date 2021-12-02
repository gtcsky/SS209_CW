/*
 * user_charge.h
 *
 *  Created on: 2020年12月2日
 *      Author: Sky
 */
#include "io_define.h"

typedef enum{
	Non_Vetting=0,
	Charging_Vetting,
	Fully_Vetting,
}fullyCheckEnum;

typedef struct {
	uint32 vChargeTick;
	uint8 vVettingStts;
	uint8 fullyGot;
} chargeDetDef;

typedef enum{
	Non_Charging=0,
	Sys_Charging=1,
	Battery_fully=2
}chargeTypeDef;

//typedef enum{
//	NonVetting=0,
//	HiVetting=1,
//	SwitchHi=1,
//	SelectOff=1,
//	LowVetting=2,
//	SwitchLow=2,
//	SelectOn=2,
//}switchEnum;

//typedef struct {
//	uint32 vSwitchTick;
//	uint8 vVettingStts;
//	uint8 sttsGot;
//	uint8 Stts;
//	uint8 lastStts;
//} switchDef;
extern chargeTypeDef  vSystemChgStts;

extern 	void	chargeSttsDectEvent(void);
extern	void	 getChargeStts(uint8 * result);
#define	 VALID_FULLY_DET_TIMER		2000	//2S


