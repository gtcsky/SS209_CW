/*
 * user_menu.h
 *
 *  Created on: 2019年8月15日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_MENU_H_
#define USER_INC_USER_MENU_H_
#include "const.h"

extern float vBuzzerDuty;
extern float vMotorDuty;
extern float vDischargeDuty;
extern float vChargeDuty;
extern u16 vPWMPeriod;				//PWM频率=系统频率/(vPWMPeriod+1);
extern UserPwmInitTypeDef userPwmParaStruct;
extern UserADCTypeDef userAdcStruct;
extern uint8 fIsMotorStartup;
extern float vPWMDuty;
extern uint8 fIsUpdateRemainTimer;
extern float vPWMStartupDuty;
extern float vMotorDriverLv;
extern float vPreMotorDriverLv;
extern float vTemperatureCoe;
extern uint8 vLightEfftectVoltDectCnt;
extern float vBattVol;
extern float vAdVoltOfBatt;
extern float vTestCompBatt;
extern uint8 vCurrentBattLv;
extern uint8 fIsNeedUpdateBattLv;
extern float vCompensationVolt;
extern float vChargingCurrent;
extern uint8 fIsSystemTempGot;
extern uint8 fIsHotNow;
extern uint8 fIsHotDisplayOn;			//hot 正在显示
extern uint8 fIsChargeDisplaying;
extern uint8 fIsSystemInitialing;
extern uint8 fIsMotorOn;
extern uint8 fIsCharging;
extern uint8 fIsDischarging;
extern uint8 fIsTurnOffByException;
extern uint8 fIsTurnOnByKnob;
extern uint8 fTimeToSendErrorMsg;
extern uint8 fIsSystemOff;
extern uint8 fIsLightActive;
extern uint8 fIsSystemFirstPowerON;
extern uint8 fTimeToI2cComm;
extern uint8 fNeedSetChargeingIC;
extern uint8 fIsBattFully;
extern uint8 fIsPowerOnFirst10s;
extern u16 vTimeToSleep;
//extern	float vSystemTemperature;
extern uint8 vSystemTemperature;
extern displayParamsStruct displayParams;
extern displayParamsStruct *pDisplayParams;
extern PcaDataStruct pcaDataStruct;
extern PcaDataStruct flashModeBackupDataStruct;
extern PcaDataStruct *pBackupDataStruct;

extern colorStructType userColor;
extern colorStructType *pUserColor;
extern uint8 fIsNewCMDGod;
extern float vVap_Volt;
extern uint8 fIsDriverVersionGot;
extern uint8 fNeedFirstGetChargeModeVolt;
extern uint8 vBattFullTimes;
extern u16 vNearBattFullCnt;
extern uint8 vChgVoltCntL;
extern uint8 vChgVoltCntH;
extern uint8 vChgResultForDisplay;
extern u16 vSystem1s;
extern userSysTimesDef sysTimes;
extern factoryModeTypeDef factoryParams;
extern uint8 fIsCh1ForCW;				//0/1: CH1用于蓝色/冷光
extern uint8 fIsCh2ForWW;				//0/1: CH2用于红色/暖光
extern uint8 fPowerOffByBattLow;
extern uint8 fIsDcMode;
//extern uint8 fIsAdjustSlowMode;
extern	graduallyAdjustDef graduallyAdjust;
extern	PcaDataStruct		oriPwmDataStruct;

void 	factoryModeFunc(void);
void 	factoryModeCoolDownFunc(void);
extern	void MenuFunction(void );
//extern	float getBattVolt(void);
void battInfoProcess(void);
extern	void	 startCharging(void);
extern	void	 stopCharging(void);
extern	void	brakeBattDetProcess(void);
extern	void fucnPer5ms(void );
extern	void resetEntrySleepTime(void);
extern	void entryNormalModeChk(void);
extern	void powerOnIoInit(void);
extern	void motorStartupCheck(void);
extern	returnStatus  motorErrorChk(void);
extern	void	powerDataInit(void);
extern	void powerDownModeChargeDisplay(void);
extern	void powerDownFromChageMode(void);
void 	onChargingIconInPowerOffMode(void);
//extern	void powerDownModeChargePercent(void);
extern	void setBattLevel(float vBattReal);
extern	void systemOffByManual(void);
extern	void checkRealEntryPowerDownMode(void);
extern	u16 calcRemainTime(u8 ignoreCompare);
extern	void setSWD2NormalIO(void);
extern	void set2SWDMode(void);
extern	void resetDriverMcu(void);
void 	turnOffAllLightEffect(void);
void 	hotFuncDeinit(void);
void 	updateLEDStts(displayParamsStruct * disParams);
void 	turnOffAllLightEffect(void);
void 	MaxLevelChargeVoltCompensation(void);
void		deinitMaxLevelChargeData(void);
void		freshRemainTimeCheck(void);
void 	chkLightEffectModeVoltCompensation(float *volt);
void 	getChargingCurrentByVolt(float *vtCurrent,float vtVolt);
extern	void	 setCoolOutputLow(void);
extern	void	 setBlueOutputLow(void);
extern	void	 setWarmOutputLow(void);
extern	void	 setGreenOutputLow(void);



/***********************************************************************************************************
  *  @brief  				计算当前电压
  *
  *  @param [in] :
  *
  *  @param [out] :			0: calc volt error
  *						-1:DC mode
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	float calcBattVolt(void);


/***********************************************************************************************************
  *  @brief			使用等待的方式获取电池的DA值
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	float readBattInfoImmediately(void);


/***********************************************************************************************************
  *  @brief 			store current system status automatic when count down
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
void autoSaveFunc(void);

#endif /* USER_INC_USER_MENU_H_ */
