/*
 * user_charge.c
 *
 *  Created on: 2020年12月2日
 *      Author: Sky
 */

#include "user_charge.h"
#include "user_lptimer.h"
#include "user_menu.h"
#include <string.h>
#include "user_lcd.h"
chargeDetDef chargeParas;
chargeTypeDef  vSystemChgStts=Non_Charging;
extern	userSysTimesDef	sysTimes;
///***********************************************************************************************************
//  *  @brief			 the charge fully pin low process
//  *
//  *  @param [in] :		none
//  *
//  *  @param [out] :		none
//  *
//  *  @return :		none
//  *
//  *  @note :
//  ************************************************************************************************************/
void fullyPinLowCb(void) {
	if (chargeParas.vVettingStts != Fully_Vetting){
		chargeParas.vVettingStts = Fully_Vetting;
		chargeParas.vChargeTick =  0x10000 - sysTimes.vSystemLIRC5ms;
	}
}
///***********************************************************************************************************
//  *  @brief			 the charge fully pin high process
//  *
//  *  @param [in] :		none
//  *
//  *  @param [out] :		none
//  *
//  *  @return :		none
//  *
//  *  @note :
//  ************************************************************************************************************/
void fullyPinHighCb(void) {
	if (chargeParas.vVettingStts != Charging_Vetting) {
		chargeParas.vVettingStts = Charging_Vetting;
		chargeParas.vChargeTick =  0x10000 - sysTimes.vSystemLIRC5ms;
	}
	chargeParas.fullyGot = 0;
}
/***********************************************************************************************************
  *  @brief			 charge status detecting event
  *
  *  @param [in] :		none
  *
  *  @param [out] :		none
  *
  *  @return :		none
  *
  *  @note :
  ************************************************************************************************************/

void	chargeSttsDectEvent(void){
	if(!Get_ChargePin_Stts){
		if(Get_BattFully_Stts){
			fullyPinHighCb();
		}else{
			fullyPinLowCb();
		}
//		osal_start_reload_timer(command_center_TaskID, CHARGE_END_EVENT, 300);
	}else{
		memset(&chargeParas,0,sizeof(chargeParas));
//		osal_stop_timerEx(command_center_TaskID, CHARGE_END_EVENT);
	}
//	if(!osal_get_timeoutEx(command_center_TaskID, SWITCH_EVT))
//		osal_start_timerEx(command_center_TaskID, SWITCH_EVT, 100);
}
/***********************************************************************************************************
  *  @brief				get current charging status
  *
  *
  *  @param [in] :		result point to an uint8 value,which use for output the Charging status
  *
  *  @param [out] :		result point to the Charging status
  *
  *  @return :		none
  *
  *  @note :
  ************************************************************************************************************/
void getChargeStts(uint8 * result) {
	if (!Get_ChargePin_Stts) {
		if (chargeParas.vVettingStts) {
			if (chargeParas.fullyGot && chargeParas.vVettingStts == Fully_Vetting) {
				if(*result==Non_Charging){
					startCharging();
				}
				*result = Battery_fully;
				vChargingCurrent=0.01;
				fIsBattFully=TRUE;
				fIsCharging = 1;
			} else if(((chargeParas.vChargeTick+ sysTimes.vSystemLIRC5ms)&0xffff) > VALID_FULLY_DET_TIMER) {
				if (chargeParas.vVettingStts == Fully_Vetting) {
					if(*result==Non_Charging){
						startCharging();
					}
					*result = Battery_fully;
					chargeParas.fullyGot = 1;
					vChargingCurrent=0.01;
					fIsBattFully=TRUE;
					fIsCharging = 1;
				} else {
					if (*result == Battery_fully && fIsSystemOff) {
						batteryPercentDisplay(0);
					}
					if(*result!=Sys_Charging){
						startCharging();
					}
					*result = Sys_Charging;
					chargeParas.fullyGot = 0;
					fIsBattFully=FALSE;
				}
			} else {
				if (*result == Battery_fully && fIsSystemOff) {
					batteryPercentDisplay(0);
				}
				if(*result!=Sys_Charging){
					startCharging();
				}
				*result = Sys_Charging;
				chargeParas.fullyGot = 0;
				fIsBattFully=FALSE;
			}
		}
	} else {
		chargeParas.fullyGot = 0;
		if(*result!=Non_Charging){
			stopCharging();
		}
		*result = Non_Charging;
	}
}
