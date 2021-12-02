/*
 * user_menu.c
 *
 *  Created on: 2021年07月07日
 *      Author: Bough_Sky
 */
#include "user_advtime.h"
#include "user_basetime.h"
#include "io_define.h"
#include "user_menu.h"
#include "misc.h"
#include "user_key.h"
#include "user_lptimer.h"
#include "user_adc.h"
#include "user_led.h"
#include "user_lcd.h"
#include "user_color.h"
#include "user_lightEffect.h"
#include "user_ddl.h"
#include "user_color.h"
#include "user_spi.h"
#include "user_charge.h"
#include "communication.h"
#include "user_i2c.h"
#include "user_internalFlash.h"

float 	vSystemVddNormalMode=0;						//非充电模式下的电池电压
float	vBattVolGap=0;									//充电和非充电时的压差
u16		vSystem1s;
u8		fIsSystemTempGot;
//u8 		vTest;
//u8 		vtIndex;
u16 		vTimeToSleep;								//vTimeToSleep*50ms  timeout , entry sleep mode
u8 		fIsCharging;
uint8	fIsDcMode=0;
u8 		fIsBattFully;
float	vBattVol=0;
float	vAdVoltOfBatt=0;
float	vTestCompBatt=0;
float	vCompensationVolt=0;
float	vChargingCurrent=0;
u8		vSkipDCUnpluggedCnt;
u8		vCurrentBattLv=0;
u8 		fIsNeedUpdateBattLv=FALSE;
u8 		vDisplayingBattLv=0;
u8		fIsSystemFirstPowerON=FALSE;
u8		vPowerOnCnt;
u8		fIsChargingCntStart=0;
u16		vL6ChargeTimer=0;
u8		vLightEfftectVoltDectCnt=0;
float 	vTemperatureCoe=1.0;							//由温度引起的亮度变化系数			>60时 vTemperatureCoe=0.8  <50度时恢复1.0
u8		fIsHotNow=0;
u8		fIsHotDisplayOn=0;							//hot 正在显示
u8		fIsBattAdcValueGot=0;							//已经成功获取到电池 的ADC值
u8 		fIsSystemInitialing;
u8		fIsChargeDisplaying=0;
u8 		fIsSystemOff=0;								//	0/1  系统开启/关闭
u8 		fIsLightActive;									//	初始画面 灯光是否开启
u8		fPowerOffByBattLow=0;
UserPwmInitTypeDef  	userPwmParaStruct;
UserADCTypeDef		userAdcStruct;
u8		fNeedFirstGetChargeModeVolt=0;				//需要在充电后首次获取电池电压
displayParamsStruct 	displayParams;
displayParamsStruct  	 *pDisplayParams=&displayParams;

PcaDataStruct		pcaDataStruct;
PcaDataStruct		*pPcaDataStruct=&pcaDataStruct;

PcaDataStruct		flashModeBackupDataStruct;
PcaDataStruct		*pBackupDataStruct=&flashModeBackupDataStruct;

PcaDataStruct		targetPwmDataStruct;			//调节目标值
PcaDataStruct		oriPwmDataStruct;				//起调时,原始值

colorStructType  userColor;
colorStructType  *pUserColor=&userColor;

graduallyAdjustDef graduallyAdjust={0};

//uint8	fIsAdjustSlowMode = FALSE;		//调节色温时,采用缓慢变化模式,以降低调节时的闪烁现象
//uint16 	vAdjustSlowIndex = 0;
//float 	vAdjustSlowCwStep = 0;
//float  	vAdjustSlowMwStep = 0;
u8	 vSystemTemperature=0;
u8 	fIsCharging=0;
u8 	fIsDischarging=0;
u8	fTimerToPowerOnLED=0;
u8	vPowerOnLEDCnt=0;
u8	vChargePercent=0;
u8 	vDisableGetBattTimer=0;					//LED关闭一段时间内,不进行DA检测,防止电压还没有恢复
u8 	fIsWakeupByCharge=0;
u8	vTimeToTurnOffLCD=0;					//关机状态,退出充电模式后,延时一段时间关闭LCD,防止初IIC_Int再次唤醒
u8	fIsUpdateRemainTimer=0;
u16	vRemainTimer=0;
u16	vRemainCheckTimes=0;
u8	vCwMaxTimer=0;
u8	battIndex=0;
float battArray[BATT_ARRAY_SIZE]={0};
u8	fIsUpdateChargeingVolt=FALSE;
u8	fTimeToI2cComm=FALSE;
u8	vStartI2cAfterTimes=0;
u8	fIsPowerOnFirst10s=FALSE;
u8	fIsNewCMDGod=FALSE;
uint8	vStoreParamCountDwon=0;
u8	fIsDriverVersionGot=FALSE;
factoryModeTypeDef	factoryParams={
		.factoryIndex=VersionDisplay,
		.factoryTimerCnt=0,
		.keyPresssedIndex=0,
		.factoryBrightIndex=50,
		.factoryIncIndex=50,
		.brightnessIncCnt=0,
		.brightnessDecCnt=0,
		.funcIncCnt=0,
		.funcDecCnt=0,
};
userSysTimesDef	sysTimes={0};

u8	fIsCh1ForCW=FALSE;			//0/1: CH1用于蓝色/冷光
u8	fIsCh2ForWW=FALSE;			//0/1: CH2用于红色/暖光

/***********************************************************************************************************
  *  @brief 			IO初始化
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void powerOnIoInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	/*--------------------------function key配置-----------------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;						//input 模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	GPIO_InitStruct.GPIO_Pin = KeyPower_Pin;						//
	GPIO_Init(KeyPower_Port, &GPIO_InitStruct);						//位置  GPIOD.4 GPIOD.5

	GPIO_InitStruct.GPIO_Pin = KeyInc_Pin ;							//位置 . 3
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	GPIO_Init(KeyInc_Port, &GPIO_InitStruct);							//GIOPC
	GPIO_InitStruct.GPIO_Pin = KeyDec_Pin ;							//位置 . 3
	GPIO_Init(KeyDec_Port, &GPIO_InitStruct);						//GIOPA
	GPIO_InitStruct.GPIO_Pin = KeyBrightInc_Pin ;						//位置 . 3
	GPIO_Init(KeyBrightInc_Port, &GPIO_InitStruct);					//GIOPA
	GPIO_InitStruct.GPIO_Pin = KeyBrightDec_Pin ;						//位置 . 3
	GPIO_Init(KeyBrightDec_Port, &GPIO_InitStruct);					//GIOPA

//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;					//
//	GPIO_InitStruct.GPIO_Pin = BatteryDet_Pin ;
//	GPIO_Init(BatteryDet_Port, &GPIO_InitStruct);
	//-----------------------以上为: 上拉输入口 设置------------------------------
//
//	GPIO_InitStruct.GPIO_Pin = Temperture_Pin|Charge_Current_Pin;		//温度检测位置  .6
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
//	GPIO_Init(Temperture_Port, &GPIO_InitStruct);						//温度检测位置  GPIOC.6
//
	//-----------------------以上为: 不上拉输 入口设置------------------------------
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//output 模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_Pin = LED_Power_Pin ;						//
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
	GPIO_Init(LED_Power_Port, &GPIO_InitStruct);
	LEDPowerOff;

//	clr_FlashLED;
	//-----------------------以上为: 输出口 设置------------------------------
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = I2C_CLK_PIN|I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_CLK_PORT,&GPIO_InitStruct);
	set_I2C_CLK;
	set_I2C_SDA;
	//-----------------------以上为: I2C口设置 设置------------------------------
	//-----------------------以上为: 输出口 设置------------------------------
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStruct.GPIO_Pin = EEPROM_IIC_SCK_PIN|EEPROM_IIC_SDA_PIN;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(EEPROM_IIC_SCK_PORT,&GPIO_InitStruct);

	//-----------------------以上为: I2C口设置 设置------------------------------
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Pin = Left_Pin | Right_Pin;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(Left_Pin_PORT, &GPIO_InitStruct);
//	clrOffByIIC;
//	clrOffByError;
//
//	GPIO_InitStruct.GPIO_Pin = zeroCrossOutPin;
//	GPIO_Init(zeroCrossOutPort, &GPIO_InitStruct);
	//-----------------------???: I2C??? ??------------------------------	
	GPIOA->AFR&=	0xFFFF000F;											//GPIOA 复用配置 		PA1.2.3均为普通IO
//	GPIOA->AFR|=	0x00000110;											//GPIOA 复用配置 		PA1.2.3均为普通IO
	GPIOB->AFR&=	0xFF00FFFF;
	GPIOB->AFR|=	0xFF00FFFF;											//GPIOB 复用配置
	GPIOC->AFR&=	0xF0000FFF;
	GPIOD->AFR&=	0xF000000F;
	GPIOD->AFR|=  	0x0000FF00;											//PD2 PD3为ADC输入
//	GPIOD->AFR|=  	0x0111FF00;											//PD2: VC输入,PD3 :PWM输入  PD4,5 :普通IO, PD6 AD输入

}
/***********************************************************************************************************
  *  @brief		重置系统进入休眠的时间
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void resetEntrySleepTime(void){
		if(fIsSystemOff)
			vTimeToSleep=POWER_DOWN_MODE_TIMEOUT;							//(5+1)*50ms=300ms
		else
			vTimeToSleep=NORMAL_MODE_TIMEOUT;								//60s
			//vTimeToSleep=60;								//60s
}

/***********************************************************************************************************
  *  @brief		系统唤醒后相关的处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void entryNormalModeChk(void){
		LPTIMER_Cmd(DEBUG_LPTIMERx, ENABLE);			//醒来后重启LPT定时器
		resetEntrySleepTime();								//重置进入睡眠时间
}
//float userAbs(float f1,float f2){
//	if(f2>=f1)
//		return f2-f1;
//	else
//		return	f1-f2;
//}
//void TestColorTemp(void){
//	if(displayParams.colorTemperature++>=MAX_ColorTemp)
//		displayParams.colorTemperature=MIN_ColorTemp;
//	updateColorTemp(&displayParams);
//}
/***************************************************************
 *
 * 清除关机充电状态,拔出从充电头后,延时熄灭防止闪烁的计时状态
 *
***************************************************************/
void clrPowerTimerOutStts(void){
	vTimeToTurnOffLCD=0;
	fIsWakeupByCharge=DoNothing;
}
/**********************************************************************************
 *
 * 系统关机
 *
 ***********************************************************************************/
void systemPowerDown(void){
	fIsSystemOff=1;
	lcdEnterSleep();
	fIsLightActive=0;
	disableSpi();
	turnOffAllLightEffect();
	LEDPowerOff;
	clearPowerKeyInfo();
	vTimeToTurnOffLCD=0;
	fIsCharging=0;
	fIsBattFully=0;
	fIsChargeDisplaying=0;
	fIsWakeupByCharge=DoNothing;
	Enter_DeepSleep();
}
/***********************************************************************************************************
  *  @brief 			25ms周期性调用入口,期望每25ms被调用的函数添加到此
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer25ms(void) {
	if (!fIsSystemOff) {
//		if(displayParams.arrowIndex<Style1Setting||displayParams.fIsBrightnessAdj )
//			holdingKeyFunc();
	}
}
/***********************************************************************************************************
  *  @brief			50ms周期性调用入口,期望每50ms被调用的函数添加到此
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer50ms(void) {
	if (!fIsSystemOff) {

		autoSaveFunc();
		brakeBattDetProcess();

	}
	if (!GET_POWER_KEY_PIN_STTS || fIsDcMode) {						//关机模式按键和充放电阻止进入睡眠

		resetEntrySleepTime();

	}
	if (!fIsSystemOff) {

		if (ON == fIsLightActive)									//灯光开启时,阻止休眠
			resetEntrySleepTime();

	}
	if (vTimeToSleep && !--vTimeToSleep) {
		displayParams.DisplayModeIndex = IdleIamgeDisplay;
		if (fIsSystemOff) {
			if (vTimeToTurnOffLCD) {
				clrPowerTimerOutStts();
				lcdEnterSleep();
			}
			clearPowerKeyInfo();
			Enter_DeepSleep();
		} else {
			systemPowerDown();
		}
	}
	if (fIsSystemOff) {
		if (fIsWakeupByCharge == TimeToPowerOff) {
			if (vTimeToTurnOffLCD && !--vTimeToTurnOffLCD) {
				fIsWakeupByCharge = DoNothing;
				fIsChargeDisplaying = FALSE;
				lcdEnterSleep();
			}
		}
	}
}
/***********************************************************************************************************
  *  @brief		100ms周期性调用入口,期望每100ms被调用的函数添加到此
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer100ms(void) {
//	chargeSttsDectEvent();
//	getChargeStts(&vSystemChgStts);
}
/***********************************************************************************************************
  *  @brief		500ms周期性调用入口,期望每500ms被调用的函数添加到此
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer500ms(void) {
//	if (fIsCharging) {
//		if (fIsBattFully == 1) {
//			if(!fIsHotNow)
//				batterDisplay(Max_Batt_level);
//		} else {
//			if (vDisplayingBattLv > Max_Batt_level) {
//				if (vCurrentBattLv > 1)
//					vDisplayingBattLv = vCurrentBattLv - 1;
//				else
//					vDisplayingBattLv = 0;
//			}
//			if(!fIsHotNow)
//				batterDisplay(vDisplayingBattLv++);
//			if(vDisplayingBattLv>=2){
//				if(fNeedFirstGetChargeModeVolt){						//充电首次电池图标跑2格时,检测电池端电压,用于记录与常规状态的压差
//					fNeedFirstGetChargeModeVolt=0;
//					vBattVol=getBattVolt();
//					getChargingCurrentByVolt(&vChargingCurrent,vBattVol);
//					vBattVolGap=vChargingCurrent*BATTERY_RESISTANCE;
//					setBattLevel(vBattVol-vBattVolGap);					//用于解决快速拔出充电器后,有时会显示在vCurrentBattLv=0的问题
//				}
//			}
//		}
//	}
	if (TRUE == fIsNeedUpdateBattLv) {
		fIsNeedUpdateBattLv = FALSE;
		batterDisplay(vCurrentBattLv);
	}
}
/**************************************************************
 *
 *从高温模式值 获取常温模式PWM的值
 *
 ***************************************************************/
u16 getOriginalPWMData(u16 curData) {
	u16 vtTemp16 = 0;
	vtTemp16 = curData * TEMPERATURE_COE_NORMAL / TEMPERATURE_COE_LOW;
	return ((vtTemp16 > PWM_FRQ_CONST) ? PWM_FRQ_CONST : vtTemp16);
}

//void updateBrightnessByTemperature(void){
//	setCoolData(vTemperatureCoe*(pcaDataStruct.valueOfCw));
//	setWarmData(vTemperatureCoe*(pcaDataStruct.valueOfMw));
//}

//void	chkSystemHot(void){
//		if ((fIsSystemOff == FALSE)||(fIsSystemOff&&fIsCharging)) {
//			if (vSystemTemperature <= OVER_TEMPERATURE_VOLT_Lv1) {//温度大于60度关机
//				systemOffByManual();
//			} else if (vSystemTemperature <= OVER_TEMPERATURE_VOLT_Lv0 && !fIsHotNow) {
//				vTemperatureCoe = TEMPERATURE_COE_LOW;
//				fIsHotNow = TRUE;
//				if (fIsLightActive==ON) {
//					updateBrightnessByTemperature();
//				}
//			} else if (vSystemTemperature >= NORMAL_TEMPERATURE_VOLT && fIsHotNow) {
//				hotFuncDeinit();
//				if (fIsLightActive==ON) {
//					pcaDataStruct.valueOfCw = getOriginalPWMData( pcaDataStruct.valueOfCw);
//					pcaDataStruct.valueOfMw = getOriginalPWMData( pcaDataStruct.valueOfMw);
//					updateBrightnessByTemperature();
//				}
//			}
//		}
//}

//void adcBatteryResultCallbacks(uint16 vtTemp){
//	u16	vDaValue=0;
//	vDaValue=vtTemp;
//	temperatureVolt=(vDaValue*SYSTEM_VDD)/ADC_CONST;		//换算成2.5v 温度对应的电压
//}
/***********************************************************************************************************
  *  @brief 			获取系统温度值
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return : 			系统温度摄氏值
  *
  *  @note :			当未获取到新的温度时,会返回最近一次的系统温度.
  ************************************************************************************************************/
uint8 getSystemperature(void) {
	if (adcConvertParams.vChlResultGot & ADC_TEMP_CHL) {
		adcConvertParams.vChlResultGot &= ~ADC_TEMP_CHL;
		float temperatureVolt = 0;
//		temperatureVolt = ((adcConvertParams.aDaValues[TEMP_CH_No] * SYSTEM_VDD) / ADC_CONST);
//		temperatureVolt = (temperatureVolt / SYSTEM_VDD) * SYSTEM_VCAP_VOLT;						//换算成2.5v 温度对应的电压

		temperatureVolt = (adcConvertParams.aDaValues[TEMP_CH_No] * SYSTEM_VCAP_VOLT) / ADC_CONST;
		if (temperatureVolt >= TEMPERATURE_5C_VOLT) 					//温度小于5度)
			return 4;
		else if (temperatureVolt >= TEMPERATURE_10C_VOLT)
			return 9;
		else if (temperatureVolt >= TEMPERATURE_15C_VOLT)
			return 14;
		else if (temperatureVolt >= TEMPERATURE_20C_VOLT)
			return 19;
		else if (temperatureVolt >= TEMPERATURE_23C_VOLT)
			return 22;
		else if (temperatureVolt >= TEMPERATURE_25C_VOLT)
			return 24;
		else if (temperatureVolt >= TEMPERATURE_28C_VOLT)
			return 27;
		else if (temperatureVolt >= TEMPERATURE_30C_VOLT)
			return 29;
		else if (temperatureVolt >= TEMPERATURE_33C_VOLT)
			return 32;
		else if (temperatureVolt >= TEMPERATURE_35C_VOLT)
			return 34;
		else if (temperatureVolt >= TEMPERATURE_38C_VOLT)
			return 37;
		else if (temperatureVolt >= TEMPERATURE_40C_VOLT)
			return 39;
		else if (temperatureVolt >= TEMPERATURE_43C_VOLT)
			return 42;
		else if (temperatureVolt >= TEMPERATURE_45C_VOLT)
			return 44;
		else if (temperatureVolt >= TEMPERATURE_48C_VOLT)
			return 47;
		else if (temperatureVolt >= TEMPERATURE_50C_VOLT)
			return 49;
		else if (temperatureVolt >= TEMPERATURE_53C_VOLT)
			return 52;
		else if (temperatureVolt >= TEMPERATURE_55C_VOLT)
			return 54;
		else if (temperatureVolt >= TEMPERATURE_58C_VOLT)
			return 57;
		else if (temperatureVolt >= TEMPERATURE_60C_VOLT)
			return 59;
		else if (temperatureVolt >= TEMPERATURE_63C_VOLT)
			return 62;
		else if (temperatureVolt >= TEMPERATURE_65C_VOLT)
			return 64;
		else if (temperatureVolt >= TEMPERATURE_68C_VOLT)
			return 67;
		else if (temperatureVolt >= TEMPERATURE_70C_VOLT)
			return 69;
		else if (temperatureVolt >= TEMPERATURE_73C_VOLT)
			return 72;
		else if (temperatureVolt >= TEMPERATURE_75C_VOLT)
			return 74;
		else if (temperatureVolt >= TEMPERATURE_78C_VOLT)
			return 77;
		else if (temperatureVolt >= TEMPERATURE_80C_VOLT)
			return 79;
		else if (temperatureVolt >= TEMPERATURE_83C_VOLT)
			return 82;
		else if (temperatureVolt >= TEMPERATURE_85C_VOLT)
			return 84;
		else if (temperatureVolt >= TEMPERATURE_88C_VOLT)
			return 87;
		else if (temperatureVolt >= TEMPERATURE_90C_VOLT)
			return 89;
		else if (temperatureVolt >= TEMPERATURE_93C_VOLT)
			return 92;
		else if (temperatureVolt >= TEMPERATURE_95C_VOLT)
			return 94;
		else if (temperatureVolt >= TEMPERATURE_98C_VOLT)
			return 97;
		else if (temperatureVolt >= TEMPERATURE_100C_VOLT)
			return 99;
		else
			return 104;
	}
	return vSystemTemperature;		//no change
}

/***********************************************************************************************************
  *  @brief  		尝试获取温度相关ADC数据
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :		调用间隔1s.以产生每2s一次的温度检测.
  ************************************************************************************************************/
void checkTemperature(void) {
	if (vSystem1s % 2 == 0) {
//		if(!getEncoderStatus()&&fIsLightActive){
//			vSystemTemperature=0;
//		vSystemTemperature = getSystemperature();
//		ADC_singleContinuousAccMode_config(ADC_TEMP_CHL);					//开始获取温度的DA值
//		}
		tryTemperatureConvert();
	}
}
/***********************************************************************************************************
  *  @brief 		清除高温相关信息
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void hotFuncDeinit(void) {
	vTemperatureCoe = TEMPERATURE_COE_NORMAL;							//由温度引起的亮度变化系数			>60时 vTemperatureCoe=0.8  <50度时恢复1.0
	fIsHotNow = FALSE;
	vSystemTemperature = 0;
	batterDisplay(vCurrentBattLv);
}
/***********************************************************************************************************
  *  @brief 			系统温度相关动作处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			建议调用周期小于50ms,以便能在ADC转换结果出来后,尽快处理.
  ************************************************************************************************************/
void checkSystemHot(void) {
	u8 vtBrightnessTemp = 0;
	vSystemTemperature = getSystemperature();
	if (fIsLightActive == ON && vSystemTemperature) {
		if (vSystemTemperature >= OVER_TEMPERATURE_VOLT_Lv1) {				//温度大于预设值则关机
			fIsHotNow = FALSE;
			vTemperatureCoe = TEMPERATURE_COE_NORMAL;
//			vKeyValue = POWER_KEY_LONG_PRESSED;
			fPowerOffByBattLow=1;
			systemOffByManual();					//关机,原因等同于低电

		} else if (vSystemTemperature >= OVER_TEMPERATURE_VOLT_Lv0 && !fIsHotNow) {
			fIsHotNow = TRUE;
			vtBrightnessTemp = displayParams.brightness;
			displayParams.brightness *= TEMPERATURE_COE_LOW;
			if (!displayParams.brightness)
				displayParams.brightness = 1;
//			setBrightnessOnlyData(BRIGHTNESS_ONLY_LENGTH, &displayParams);
			displayParams.brightness = vtBrightnessTemp;
		} else if (vSystemTemperature <= NORMAL_TEMPERATURE_VOLT && fIsHotNow) {
			hotFuncDeinit();
//			setBrightnessOnlyData(BRIGHTNESS_ONLY_LENGTH, &displayParams);
		}
	}
}
/***********************************************************************************************************
  *  @brief		1s周期性调用入口,期望每1s被调用的函数添加到此
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer1s(void) {

//	checkBattFunc();
	startBattVoltDect();
#if(DEBUG_VOLT_INFO==1)
	temperatureDisplay();
#endif
	checkTemperature();
	updateHotDisplay();
	if (fIsCharging) {
		fIsUpdateChargeingVolt = TRUE;
	}

}

//float getBattVolt(void) {
//
//	u16 vDaValue = 0;
//	vDaValue = getSignalChlADC(ADC_BATT_CHL);
//	return ((vDaValue * SYSTEM_VDD) / ADC_CONST + BATT_ADC_TOLERANCE);
//
//}
// Immediately
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
float readBattInfoImmediately(void) {
	float tempVolt = 0;
	startBattVoltDect();
	do {
		tempVolt = calcBattVolt();
		if (tempVolt != 0)
			break;
	} while (sysTimes.vSystem5ms < 20);
	sysTimes.vSystem5ms = 0;
	if (tempVolt < 0) {
		fIsDcMode = 1;
		fPowerOffByBattLow = 0;
	}
	return tempVolt;
}

/***********************************************************************************************************
  *  @brief  					计算电池电压
  *
  *  @param [in] :
  *
  *  @param [out] :			0: calc volt error
  *						-1:DC mode
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
float calcBattVolt(void) {
	if (adcConvertParams.vChlResultGot & ADC_BATT_CHL) {
		adcConvertParams.vChlResultGot &= ~ADC_BATT_CHL;
		float volt = 0;
		volt = ((adcConvertParams.aDaValues[ADC_BATT_No] * SYSTEM_VDD) / ADC_CONST);
		volt = volt * (BATT_DET_PULL_UP_RES + BATT_DET_PULL_DOWN_RES) / BATT_DET_PULL_DOWN_RES;
//		if (volt == 0)
		if (volt <= 3.0)
			return -1;
		else
			return volt/4;				//,换算单节
	} else
		return 0;
}

/***********************************************************************************************************
  *  @brief			电池电量相关信息处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void battInfoProcess(void) {
	u8 temp;
	float tempVolt = 0;
	if (!fIsCharging) {
		tempVolt = calcBattVolt();
		if (tempVolt < 0) {
			fIsDcMode = 1;
			fPowerOffByBattLow = 0;
		} else if (tempVolt == 0) {
			return;
		} else {
			vAdVoltOfBatt = tempVolt;
			fIsDcMode = 0;
		}
		//-------------------------------------------
		fIsBattAdcValueGot = 1;
		chkLightEffectModeVoltCompensation(&vAdVoltOfBatt);
		battArray[battIndex] = vAdVoltOfBatt;
		if (++battIndex >= BATT_ARRAY_SIZE)
			battIndex = 0;
		//-------------------------------------------
		if (fIsBattAdcValueGot) {
			vBattVol = vAdVoltOfBatt;
			vAdVoltOfBatt = vBattVol;
			vTestCompBatt = vBattVol;
			vCompensationVolt = vBattVol;
			vSystemVddNormalMode = vBattVol;
			if (vCurrentBattLv > Max_Batt_level)
				vCurrentBattLv = Max_Batt_level;
			temp = vCurrentBattLv;
			setBattLevel(vBattVol);
			if (vCurrentBattLv != temp) {
				fIsNeedUpdateBattLv = TRUE;
			}
		}
	}
}
void resetBrakeBattDet(void){
	vDisableGetBattTimer=DISABLE_BATT_ADC_TIMER_CONST;
}
void	brakeBattDetProcess(void){
	if(vDisableGetBattTimer)
		vDisableGetBattTimer--;
}
//void	 startCharging(void){
//	   fIsCharging=1;
//	   fIsChargeDisplaying=0;
//	   fIsBattFully=0;
//	   vBattFullTimes=0;
//	   fNeedFirstGetChargeModeVolt=1;
//	   vChargingCurrent=MAX_CHARGE_CURRENT;					//初始设高,防止插上充电器一瞬间补偿后电压偏高的
//	   fIsDischarging=FALSE;
//	   vDisplayingBattLv = 0;										//插上充电器时,第一次从第0格开始显示充电符号
//	   vNearBattFullCnt=0;
//	   if(fIsSystemOff){
//		   enableSpi();
//	   }
//}
//void stopCharging(void) {
//	fIsVapCalibrated=FALSE;
//	fIsCharging = 0;
//	fIsBattFully = 0;
//	vBattFullTimes=0;
//	   vNearBattFullCnt=0;
////	else if(fIsChargeDisplaying){									//fIsCharging==0&&fIsChargeDisplaying==1
//	if(fNeedFirstGetChargeModeVolt){								//插入充电器,在还未完成首个充电图标显示循环时拔出充电器
//		fNeedFirstGetChargeModeVolt=0;
//		vBattVol=vSystemVddNormalMode;
//		vCurrentBattLv = Max_Batt_level;							//用于setBattLevel在非充电模式显示回真正的档位
//		setBattLevel(vBattVol);
//	}
//	fIsChargeDisplaying=1;
//	updateChargingIcon();
//	fIsNeedUpdateBattLv = TRUE;				//关掉充电显示
//	fIsChargeDisplaying=FALSE;
//	powerDownFromChageMode();
////		}
//}

//void	 chargeIcStatusProcess(u8 *vtStatus){
//	if(*vtStatus&0xC0){
//		if(*vtStatus&0x40){
//			vChargingCurrent=0.01;
//			fIsBattFully=TRUE;
//			fIsCharging = 1;
//		}else if(*vtStatus&0x80){
//			if(!fIsCharging){
//				startCharging();
//			}
//			if(fIsPowerOnFirst10s&&fIsBattFully){
//				startCharging();						//首次上电前10s,允许从满电变为充电状态
//			}
//		}
//	}else{
//		if(fIsCharging){
//			stopCharging();
//		}
//	}
//	if(*vtStatus&0x10){
//		fIsDischarging=TRUE;
//	}else if(fIsDischarging){
//		fIsDischarging=FALSE;
//	}
//}


//void	  calibrateVcap(void){
//	u16	vDaValue=0;
//		vDaValue=getSignalChlADC(ADC_Channel7_ENABLE);
//		vVap_Volt=VOLT_BATT_FULL*vDaValue/ADC_CONST;
//}
/************************************************************************
 *
 * 处理充电显示相关内容
 *
 *************************************************************************/
//void funcCharging(void) {
//	if (fIsCharging) {
//		if(!fNeedFirstGetChargeModeVolt&&fIsUpdateChargeingVolt){
//			vBattVol=getBattVolt();
//			getChargingCurrentByVolt(&vChargingCurrent,vBattVol);
//			vAdVoltOfBatt=vBattVol;
//			if(fIsBattFully){
//				if(fIsSystemOff){
//					batteryPercentDisplay(100);
////					if(!fIsVapCalibrated){
////						fIsVapCalibrated=TRUE;
////						calibrateVcap();
////					}
//				}
//			}
//		}
//		if (!fIsChargeDisplaying) {
//			fIsCharging = 1;
//			fIsBattFully = 0;
//			updateChargingIcon();
//			if (!fIsSystemInitialing && !vDisableGetBattTimer) {
//				vCurrentBattLv = 0;
//			}
//			clrPowerTimerOutStts();
//			onChargingIconInPowerOffMode();
//		}
//		if(!fNeedFirstGetChargeModeVolt&&fIsUpdateChargeingVolt){
//			fIsUpdateChargeingVolt=0;
//			vBattVolGap=vChargingCurrent*BATTERY_RESISTANCE;
//			vTestCompBatt=vBattVol-vBattVolGap;
//			vCompensationVolt=vTestCompBatt;
//			setBattLevel(vCompensationVolt);
//		}
//	}
//}
/*********************************************************************
 *
 * 色温模式调节色温时,缓慢变化,防止过快产生的闪烁感
 *
 **********************************************************************/
void colorTemperatureAdjustSlowFunc(void) {
	uint16 totalSteps=0;
	totalSteps=(fIsSystemFirstPowerON==0)?MAX_SLOW_ADJUST_STEPS:MAX_STARTUP_STEPS;

	if (graduallyAdjust.fIsAdjustSlowMode) {
		if (++graduallyAdjust.vAdjustSlowIndex < (totalSteps + 1)) {
//		if (++graduallyAdjust.vAdjustSlowIndex < (graduallyAdjust.vMaxIndex + 1)) {
			setWarmData(oriPwmDataStruct.valueOfMw + graduallyAdjust.vAdjustSlowMwStep * graduallyAdjust.vAdjustSlowIndex);
			setCoolData(oriPwmDataStruct.valueOfCw + graduallyAdjust.vAdjustSlowCwStep * graduallyAdjust.vAdjustSlowIndex);
		} else {
			graduallyAdjust.fIsAdjustSlowMode = 0;
			graduallyAdjust.vAdjustSlowIndex = 0;
			setWarmData(targetPwmDataStruct.valueOfMw);
			setCoolData(targetPwmDataStruct.valueOfCw);
			fIsSystemFirstPowerON=0;
		}
	}
}
/***********************************************************************************************************
  *  @brief			5ms周期性调用入口
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer5ms(void) {
	battInfoProcess();
	if (!fIsSystemOff) {
#if(LIGHTEFFECT_INSIDE==1)
		lightEffectFunc();
#endif
		colorTemperatureAdjustSlowFunc();
		if (fPowerOffByBattLow) {
			if (!fIsSystemOff) {
				if (!fIsDcMode)
					systemOffByManual();
			}
		}

		checkSystemHot();

		if (sysTimes.vSystemLIRC5ms % 2 == 0) {
			factoryModeCoolDownFunc();
		}
		if (sysTimes.vSystemLIRC5ms % 5 == 0) {
			fucnPer25ms();
		}
	}
	checkAdcConvertRetry();
	if (sysTimes.fTime50ms == 1) {
		sysTimes.fTime50ms = 0;
		fucnPer50ms();
	}
	if (!fIsSystemOff) {
		if (sysTimes.fTime100ms == 1) {
			sysTimes.fTime100ms = 0;
			fucnPer100ms();
		}
		if (sysTimes.fTime500ms == 1) {
			sysTimes.fTime500ms = 0;
			fucnPer500ms();
		}
		if (sysTimes.fTime1s == 1) {
			sysTimes.fTime1s = 0;
			vSystem1s += 1;
			fucnPer1s();
			if (fIsPowerOnFirst10s && vSystem1s > 10) {
				fIsPowerOnFirst10s = FALSE;
			}
		}
	}
}
/***********************************************************************************************************
  *  @brief			测试模式轮转入口,
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			调用间隔10ms
  ************************************************************************************************************/
void factoryModeCoolDownFunc(void) {
	if (FactoryMode == displayParams.DisplayModeIndex) {
		if (factoryParams.factoryTimerCnt && --factoryParams.factoryTimerCnt == 0) {
			switch (factoryParams.factoryIndex) {
			case WaitFuncKey:
				break;
#if(RGB_INSIDE==1)      
			case RedLedTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
				displayParams.hues = 120;
				updateLEDStts(&displayParams);
				displayFactoryMode(Green_Current);
				break;
			case GreenLedTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
				displayParams.hues = 240;
				updateLEDStts(&displayParams);
				displayFactoryMode(Blue_Current);
				break;
			case BlueLedTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
				displayFactoryMode(Cw_Current);
				factoryModeTemperatureDisplay();
				displayParams.colorTemperature = MAX_ColorTemp;
				displayParams.arrowIndex = ColorTempSetting;
				updateLEDStts(&displayParams);
				break;
#endif        
			case CoolLedTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
				displayParams.colorTemperature = MIN_ColorTemp;
				updateLEDStts(&displayParams);
				displayFactoryMode(Mw_Current);
				factoryModeTemperatureDisplay();
				factoryParams.factoryTemp = vSystemTemperature;
				break;
			case WarmLedTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = 0;
//				setOnOffData(ON_OFF_DATA_LENGTH, &displayParams, OFF);
//				displayFactoryMode(Press_Bdec_key);
				break;
			case WaitBrightnesssDecKey:
				break;
			case IdleModeTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = 2;
//				displayFactoryMode(System_Volt_Display);
				break;
			case SystemVoltTesting:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = 2;
//				displayFactoryMode(Press_Inc_key);
				break;
			case WaitIncKey:
				factoryParams.factoryTimerCnt = 30;
				break;
			case SleepModeTesting:
				break;
			case VersionDisplay:
//				vTestCompBatt = getBattVolt();
				factoryVoltDisplay(vTestCompBatt);
				if (vTestCompBatt <= 3.30 || vTestCompBatt > 4.30) {
#if(LCD_INSIDE==1)
					displayFactoryMode(VoltageErrorDisplay);
					factoryVoltDisplay(vTestCompBatt);
					while (1)
						;
#else
					sysTimes.vSystem5ms=0;
					uint8 temp=0;
					temp=(vTestCompBatt > 4.30)?95:2;
					while(1){
						if((sysTimes.vSystem5ms%100)<temp){
							setCoolData(300);
						}else{
							setCoolData(0);
						}
					}
#endif
				}
			default:
				factoryParams.factoryIndex++;
				displayFactoryMode(Press_Func_key);
				factoryParams.factoryTimerCnt = 0;
				fIsCharging = FALSE;
				break;
			}
		} else {
			switch (factoryParams.factoryIndex) {
			case VersionDisplay:
				break;
			case WaitFuncKey:
				if (!GET_POWER_KEY_PIN_STTS && (factoryParams.keyPresssedIndex == 0x0F)) {
#if(RGB_INSIDE==1)
					factoryParams.factoryIndex++;
					factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
					displayFactoryMode(Red_Current);
					displayParams.saturation = 100;
					displayParams.brightness = 100;
					displayParams.hues = 0;
					displayParams.arrowIndex = HuesSetting;
#else
					factoryParams.factoryIndex=CoolLedTesting;
					factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
					clear();
					displayFactoryMode(Cw_Current);
#endif           
					factoryParams.brightnessIncCnt = 0;
					factoryParams.brightnessDecCnt = 0;
					factoryParams.funcIncCnt = 0;
					factoryParams.funcDecCnt = 0;
					factoryParams.keyPresssedIndex = 0;
					updateLEDStts(&displayParams);
					vIsKeyPressed = 0;
					vKeyValue = 0;
				} else if (!GET_POWER_KEY_PIN_STTS) {
#if(LCD_INSIDE==1)
					if ((factoryParams.keyPresssedIndex & 0x03) != 0x03) {
						OLED_ShowString(FACTORY_MODE_RIGHT_X, FACTORY_MODE_PROMPT_Y, "NG");
					}
					if ((factoryParams.keyPresssedIndex & 0x0C) != 0x0C) {
						OLED_ShowString(FACTORY_MODE_LEFT_X, FACTORY_MODE_PROMPT_Y, "NG");
					}
#else
					if((factoryParams.keyPresssedIndex&0x03)!=0x03) {			//
						LEDPowerOn;
						setWarmData(100);
					}
					if((factoryParams.keyPresssedIndex&0x0C)!=0x0C) {
						LEDPowerOn;
						setCoolData(100);
					}
					sysTimes.vSystem5ms = 0;
					while (sysTimes.vSystem5ms < 60);
					sysTimes.vSystem5ms = 0;
					setWarmData(0);
					setCoolData(0);
					LEDPowerOff;
#endif
				}
				break;
			case BlueLedTesting:
				if (FACTORY_TIME_2S9 == factoryParams.factoryTimerCnt) {
//					readTemperatureData();
				}
				if (FACTORY_TIME_2S == factoryParams.factoryTimerCnt) {
					factoryModeTemperatureDisplay();
				}
				break;
			case WaitBrightnesssDecKey:
				factoryParams.factoryIndex++;
				factoryParams.factoryTimerCnt = FACTORY_TIME_2S;
				displayFactoryMode(Standby_Current);
				//------------------
				vIsKeyPressed = 0;
				vKeyValue = 0;
				break;
			case SystemVoltTesting:
				break;
			case WaitIncKey:
				//if (factoryParams.factoryTemp < 19 || factoryParams.factoryTemp > 39) {
				if (factoryParams.factoryTemp < 19 || factoryParams.factoryTemp > 39) {
					clear();
					vSystemTemperature = factoryParams.factoryTemp;
#if(LCD_INSIDE==1)
					displayFactoryMode(TempErrorDisplay);
					factoryModeTemperatureDisplay();
					while (1)
						;
#else
					sysTimes.vSystem5ms=0;
					uint8 temp=0;
					temp=(factoryParams.factoryTemp > 39)?95:2;
					while(1){
						if((sysTimes.vSystem5ms%100)<temp){
							setWarmData(300);
						}else{
							setWarmData(0);
						}
					}
#endif
					//OLED_ShowString(20,4,"Temp  Error");

				}
				sysTimes.vSystem5ms = 0;
				displayParams.DisplayModeIndex = IdleIamgeDisplay;
				factoryParams.factoryTimerCnt = 0;
				factoryParams.factoryBrightIndex = 0;
				factoryParams.factoryIncIndex = 0;
				factoryParams.factoryIndex = VersionDisplay;
				factoryParams.keyPresssedIndex = 0;
//				sysTimes.vSystem5ms = 1800;
//				while(1);
				vIsKeyPressed = 0;
				vKeyValue = 0;
				powerDataInit();
				systemOffByManual();
				break;
			}

			if (vKeyValue) {

				if ((factoryParams.factoryIndex >= WaitFuncKey) && (factoryParams.factoryIndex <= WarmLedTesting)) {
					//OLED_ShowNum
					if ((vKeyValue & FAST_ADJUST_FALG_MASK) == INC_KEY_SHORT_PRESSED) {
						if (++factoryParams.factoryIncIndex > 99)
							factoryParams.factoryIncIndex = 99;
						if (factoryParams.factoryIncIndex < 10)
							factoryParams.factoryIncIndex = 10;
#if(LCD_INSIDE==1)
						OLED_ShowNum(FACTORY_MODE_RIGHT_X, FACTORY_MODE_PROMPT_Y, factoryParams.factoryIncIndex);
#endif
						if (factoryParams.funcIncCnt < FACTORY_KEY_COST)
							factoryParams.funcIncCnt++;
						else
							factoryParams.keyPresssedIndex |= 0x01;
					}
					if ((vKeyValue & FAST_ADJUST_FALG_MASK) == DEC_KEY_SHORT_PRESSED) {
						if (factoryParams.factoryIncIndex-- <= 10)
							factoryParams.factoryIncIndex = 10;
#if(LCD_INSIDE==1)
						OLED_ShowNum(FACTORY_MODE_RIGHT_X, FACTORY_MODE_PROMPT_Y, factoryParams.factoryIncIndex);
#endif
						if (factoryParams.funcDecCnt < FACTORY_KEY_COST)
							factoryParams.funcDecCnt++;
						else
							factoryParams.keyPresssedIndex |= 0x02;
					}
					if ((vKeyValue & FAST_ADJUST_FALG_MASK) == BRIGHTNESS_UP_KEY_SHORT_PRESSED) {
						if (++factoryParams.factoryBrightIndex > 99)
							factoryParams.factoryBrightIndex = 99;
						if (factoryParams.factoryBrightIndex < 10)
							factoryParams.factoryBrightIndex = 10;
#if(LCD_INSIDE==1)
						OLED_ShowNum(FACTORY_MODE_LEFT_X, FACTORY_MODE_PROMPT_Y, factoryParams.factoryBrightIndex);
#endif
						if (factoryParams.brightnessIncCnt < FACTORY_KEY_COST)
							factoryParams.brightnessIncCnt++;
						else
							factoryParams.keyPresssedIndex |= 0x04;
					}
					if ((vKeyValue & FAST_ADJUST_FALG_MASK) == BRIGHTNESS_DOWN_KEY_SHORT_PRESSED) {
						if (factoryParams.factoryBrightIndex-- <= 10)
							factoryParams.factoryBrightIndex = 10;
#if(LCD_INSIDE==1)
						OLED_ShowNum(FACTORY_MODE_LEFT_X, FACTORY_MODE_PROMPT_Y, factoryParams.factoryBrightIndex);
#endif
						if (factoryParams.brightnessDecCnt < FACTORY_KEY_COST)
							factoryParams.brightnessDecCnt++;
						else
							factoryParams.keyPresssedIndex |= 0x08;
					}
					vKeyValue = 0;
				}
			}
		}
	}
}

/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :			*param 待调节参数
  *  						step 调节步进
  *  						min 最小值
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 publicDecFunc(uint8 * param,uint8 step,uint8 min){
	if ((*param >= min+step) && (*param != 1)) {
		*param -= step;
		if (*param<=min)
			*param = min;
	} else if (*param <=min+step) {
		*param = min;
	}
}


/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :			*param 待调节参数
  *  						step 调节步进
  *  						max:上限
  *  						min :下限
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void publicIncFunc(uint8 * param, uint8 step, uint8 max, uint8 min) {
	if (*param < (max + 1 - step)) {
		if ((*param <= min + 1) && (step != 1)) {
			*param = min + step;
		} else {
			*param += step;
		}
	} else {
		*param = max;
	}
}


/***********************************************************************************************************
  *  @brief				用户交互入口,即按键与编码器后续处理入口.
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :				主循环调用.
  ************************************************************************************************************/
void MenuFunction(void) {
//	if (vKeyValue == POWER_KEY_LONG_PRESSED) {
//		fIsSystemOff ^= 0x01;
//		if (fIsSystemOff) {			//turn off system
//			systemOffByManual();
//		} else {					//turn on system
////			switchRemoteCtrlIRQ(ENABLE);
//			fIsLightActive = ON;
//			LEDPowerOn;
//#if(LCD_INSIDE==1)
//			powerOnLcdInit();
//#endif
//			hotFuncDeinit();
//			if (!GET_DEC_KEY_PIN_STTS) {
//				displayParams.DisplayModeIndex = FactoryMode;
//				fIsLightActive = ON;
//				versionDisplay();
//				factoryParams.factoryIndex = VersionDisplay;
//				factoryParams.factoryTimerCnt = 160;
//				factoryParams.factoryBrightIndex = 50;
//				factoryParams.factoryIncIndex = 50;
//				factoryParams.keyPresssedIndex = 0;
//				return;
//			}
//			vBattVol=readBattInfoImmediately();
//			vCompensationVolt = vBattVol;
//			resetEntrySleepTime();
////			ADVTIM_PWMMODE_Config(&userPwmParaStruct);
//			displayParams.DisplayModeIndex = IdleIamgeDisplay;
//			displaySystemMenu(&displayParams);
//			updateLEDStts(&displayParams);
//			if (FALSE == fIsSystemFirstPowerON) {
//				fIsUpdateRemainTimer = 0;
//			} else {
//				vPowerOnCnt = 20;					//1.0s后更新时间显示
//			}
//		}
//	}
	if (!fIsSystemOff) {
		uint8 step=1;
		fIsSystemFirstPowerON=0;
		if (vKeyValue == FUNC_KEY_SHORT_PRESSED) {
			if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
#if(RGB_INSIDE==1)
				if (displayParams.arrowIndex++ >= Max_Arrow_Index) {
					displayParams.arrowIndex = Min_Arrow_Index;
					turnOffAllLightEffect();
				}
				if (BrightnessSetting == displayParams.arrowIndex)
				displayParams.arrowIndex = ColorTempSetting;
				updateArrowDisplay(&displayParams);
				updateLEDStts(&displayParams);
#else
				if (displayParams.arrowIndex != ColorTempSetting) {
					displayParams.arrowIndex = ColorTempSetting;
					turnOffAllLightEffect();
					updateArrowDisplay(&displayParams);
					updateLEDStts(&displayParams);
				}
#endif
			}
		} else if ((vKeyValue&FAST_ADJUST_FALG_MASK) == INC_KEY_SHORT_PRESSED) {
			if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
				if (ColorTempSetting == displayParams.arrowIndex) {					//色温调节
					if (vKeyValue & FAST_ADJUST_FLAG)
						step = FAST_ADJUST_STEP / 2;
					if (displayParams.colorTemperature != MAX_ColorTemp) {
						publicIncFunc(&displayParams.colorTemperature, step, MAX_ColorTemp, MIN_ColorTemp);
						updateColorTempDisplay(&displayParams);
					}
					graduallyAdjust.fIsAdjustSlowMode = TRUE;
					oriPwmDataStruct.valueOfMw = pcaDataStruct.valueOfMw;
					oriPwmDataStruct.valueOfCw = pcaDataStruct.valueOfCw;
				}
#if(RGB_INSIDE==1)
				else if (HuesSetting == displayParams.arrowIndex) {					//色调调节
					if(vKeyValue&FAST_ADJUST_FLAG)
						step=FAST_ADJUST_STEP*2;
					displayParams.hues+=step;
					displayParams.hues%=MAX_HUES;
					updateHuesDisplay(&displayParams);
				} else if (SaturationSetting == displayParams.arrowIndex) {				//饱和度调节
					if(vKeyValue&FAST_ADJUST_FLAG)
						step=FAST_ADJUST_STEP;
					if (displayParams.saturation < MAX_SATURATION) {
						publicIncFunc(&displayParams.saturation,step,MAX_SATURATION,MIN_SATURATION);
						updateSaturationDisplay(&displayParams);
					}
				} else if (Style1Setting == displayParams.arrowIndex) {					//灯效1 调节
					if (displayParams.style1Value++ >= MAX_LightEffect)
					displayParams.style1Value = 0;
					updateLightEffectDisplay(&displayParams);
				} else if (Style2Setting == displayParams.arrowIndex) {					//灯效2 调节
					if (displayParams.style2Value++ >= MAX_LightEffect)
					displayParams.style2Value = 0;
					updateLightEffectDisplay(&displayParams);
				} else if (Style3Setting == displayParams.arrowIndex) {					//灯效3 调节
					if (displayParams.style3Value++ >= MAX_LightEffect)
					displayParams.style3Value = 0;
					updateLightEffectDisplay(&displayParams);
				}
#endif
				updateLEDStts(&displayParams);
			}
		} else if ((vKeyValue&FAST_ADJUST_FALG_MASK)  == DEC_KEY_SHORT_PRESSED) {
			if (ColorTempSetting == displayParams.arrowIndex) {						//色温调节
				if(vKeyValue&FAST_ADJUST_FLAG)
					step=FAST_ADJUST_STEP/2;
				if (displayParams.colorTemperature != MIN_ColorTemp) {
					publicDecFunc(&displayParams.colorTemperature,step,MIN_ColorTemp);
					updateColorTempDisplay(&displayParams);
				}
				graduallyAdjust.fIsAdjustSlowMode = TRUE;
				oriPwmDataStruct.valueOfMw = pcaDataStruct.valueOfMw;
				oriPwmDataStruct.valueOfCw = pcaDataStruct.valueOfCw;
			}
#if(RGB_INSIDE==1)
			else if (HuesSetting == displayParams.arrowIndex) {						//色调调节
				if(vKeyValue&FAST_ADJUST_FLAG)
					step=FAST_ADJUST_STEP*2;
				if(displayParams.hues<step)
					displayParams.hues=MAX_HUES+displayParams.hues-step;
				else
					displayParams.hues-=step;
				updateHuesDisplay(&displayParams);
			} else if (SaturationSetting == displayParams.arrowIndex) {					//饱和度调节
				if(vKeyValue&FAST_ADJUST_FLAG)
					step=FAST_ADJUST_STEP;
				if (displayParams.saturation > MIN_SATURATION) {
//					displayParams.saturation--;
					publicDecFunc(&displayParams.saturation,step,MIN_BRIGHTNESS);
					updateSaturationDisplay(&displayParams);
				}
			} else if (Style1Setting == displayParams.arrowIndex) {						//灯效1 调节
				if (displayParams.style1Value-- <= 0)
				displayParams.style1Value = MAX_LightEffect;
				updateLightEffectDisplay(&displayParams);
			} else if (Style2Setting == displayParams.arrowIndex) {						//灯效2 调节
				if (displayParams.style2Value-- <= 0)
				displayParams.style2Value = MAX_LightEffect;
				updateLightEffectDisplay(&displayParams);
			} else if (Style3Setting == displayParams.arrowIndex) {						//灯效3 调节
				if (displayParams.style3Value-- <= 0)
				displayParams.style3Value = MAX_LightEffect;
				updateLightEffectDisplay(&displayParams);
			}
#endif
			updateLEDStts(&displayParams);
			//}
		} else if ((vKeyValue&FAST_ADJUST_FALG_MASK)  == BRIGHTNESS_UP_KEY_SHORT_PRESSED) {
//			OLED_ShowOneSegment(0,0,0xff,0xff);
//			OLED_ShowOneSegment(0,2,0xff,0xff);
//			OLED_ShowOneSegment(0,4,0xff,0xff);
//			OLED_ShowOneSegment(0,6,0xff,0xff);
//			OLED_ShowOneSegment(1,0,0xff,0xff);
//			OLED_ShowOneSegment(1,2,0xff,0xff);
//			OLED_ShowOneSegment(1,4,0xff,0xff);
//			OLED_ShowOneSegment(1,6,0xff,0xff);
//			OLED_ShowOneSegment(1,0,0xff,0xff);
//			OLED_ShowOneSegment(1,2,0xff,0xff);
//			OLED_ShowOneSegment(1,4,0xff,0xff);
//			OLED_ShowOneSegment(1,6,0xff,0xff);
//			OLED_ShowOneSegment(1,0,0xff,0xff);
//			OLED_ShowOneSegment(1,6,0xff,0xff);
//			OLED_ShowOneSegment(128,0,0xff,0xff);
//			OLED_ShowOneSegment(128,2,0xff,0xff);
//			OLED_ShowOneSegment(128,4,0xff,0xff);
//			OLED_ShowOneSegment(128,6,0xff,0xff);
//			OLED_ShowOneCom(0,0,1);
			if(vKeyValue&FAST_ADJUST_FLAG)
				step=FAST_ADJUST_STEP;
			if (displayParams.brightness != MAX_BRIGHTNESS) {
				publicIncFunc(&displayParams.brightness,step,MAX_BRIGHTNESS,MIN_BRIGHTNESS);
//				resetLcdSetting();
				updateBrightnessDisplay(&displayParams);
				LEDPowerOn;
				if (ColorTempSetting >= displayParams.arrowIndex) {
					graduallyAdjust.fIsAdjustSlowMode = TRUE;
					oriPwmDataStruct.valueOfMw = pcaDataStruct.valueOfMw;
					oriPwmDataStruct.valueOfCw = pcaDataStruct.valueOfCw;
					updateLEDStts(&displayParams);
				}
			}
		} else if ((vKeyValue & FAST_ADJUST_FALG_MASK) == BRIGHTNESS_DOWN_KEY_SHORT_PRESSED) {
			if (vKeyValue & FAST_ADJUST_FLAG)
				step = FAST_ADJUST_STEP;
			if (displayParams.brightness != MIN_BRIGHTNESS) {
				publicDecFunc(&displayParams.brightness,step,MIN_BRIGHTNESS);
				updateBrightnessDisplay(&displayParams);
				if(!displayParams.brightness){
					LEDPowerOff;
				}else {
					LEDPowerOn;
				}
				if (ColorTempSetting >= displayParams.arrowIndex) {
					graduallyAdjust.fIsAdjustSlowMode = TRUE;
					oriPwmDataStruct.valueOfMw = pcaDataStruct.valueOfMw;
					oriPwmDataStruct.valueOfCw = pcaDataStruct.valueOfCw;
					updateLEDStts(&displayParams);
				}
			}
		} else if (vKeyValue==FACTORY_MODE_KEY) {
			displayParams.DisplayModeIndex = FactoryMode;
			fIsLightActive = ON;
			turnOffAllLightEffect();
#if(LCD_INSIDE==1)
			versionDisplay();
#endif
			factoryParams.factoryIndex = VersionDisplay;
			factoryParams.factoryTimerCnt = 160;
			factoryParams.factoryBrightIndex = 50;
			factoryParams.factoryIncIndex = 50;
			factoryParams.keyPresssedIndex = 0;
			return;
		}
		if (!fIsSystemOff) {
			resetEntrySleepTime();   //
		}
	}
	vStoreParamCountDwon = AUTO_SAVE_PARAMS_TIMER;
	vKeyValue = 0;
}
/***********************************************************************************************************
  *  @brief			上电初始化部分系统变量
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	powerDataInit(void){
//	adcThresholdGenerate();
	userPwmParaStruct.period=ADVICE_TIM_PERIOD;
	displayParams.DisplayModeIndex=IdleIamgeDisplay;
	displayParams.brightness=DEFAULT_BRIGHTNESS;
	displayParams.arrowIndex=DEFAULT_ARROR_INDEX;
	displayParams.colorTemperature=DEFAULT_COLOR_TEMP;
#if(RGB_INSIDE==1)
	displayParams.hues=DEFAULT_HUES;
	displayParams.saturation=DEFAULT_SATURATION;
	displayParams.style1Value=DEFAULT_STYLE1_VALUE;
	displayParams.style2Value=DEFAULT_STYLE2_VALUE;
	displayParams.style3Value=DEFAULT_STYLE3_VALUE;
	displayParams.battIndex=6;
//	displayParams.countDownTimer=MAX_CD_TIMER;
//	displayParams.remainingTime=1;
#endif
	vCurrentBattLv=6;
	fIsCharging=0;
	fIsBattAdcValueGot=0;
	vChargingCurrent=1.0;
//	vVap_Volt=SYSTEM_VCAP;
}

/***************************************************************************
 *
 *  	电压补偿.灯开启时的电压补偿
 *
 ***************************************************************************/
void battVoltCompensation(float *volt) {
	float vtCurrent=0;
	if (ON == fIsLightActive) {				//在灯开启的情况下
		if (!fIsLightEffectOn) {
			if(displayParams.arrowIndex<=SaturationSetting){
				vtCurrent+=pUserColor->red*RED_POWER_RATING/(*volt*PWM_PERIOD_CONST);
				vtCurrent+=pUserColor->green*GREEN_POWER_RATING/(*volt*PWM_PERIOD_CONST);
				vtCurrent+=pUserColor->blue*BLUE_POWER_RATING/(*volt*PWM_PERIOD_CONST);
			}else if(displayParams.arrowIndex<=ColorTempSetting){
				vtCurrent+=pUserColor->cWhite*CW_POWER_RATING/(*volt*PWM_PERIOD_CONST);
				vtCurrent+=pUserColor->wWhite*MW_POWER_RATING/(*volt*PWM_PERIOD_CONST);
			}
		}
#if(RGB_INSIDE==1)
		else{
			if ((Style1Setting == displayParams.arrowIndex)
					|| ((Style2Setting == displayParams.arrowIndex) && (displayParams.style2Value != 2))
					|| ((Style3Setting == displayParams.arrowIndex) && (displayParams.style2Value == 2))) {			//有暖光或白光

				vtCurrent=CW_POWER_RATING*MAX_CW_DUTY/(*volt);
			}else{
				vtCurrent=BLUE_POWER_RATING*MAX_MW_DUTY/(*volt);
			}
		}
#endif
	}
	if(fIsHotNow){
		vtCurrent*=TEMPERATURE_COE_LOW;
	}
//	if(fIsDischarging){
//		if(fIsCharging){
//			vtCurrent+=0.35;					//放电模式最大电池设定为1.0
//		} else {
//			if (*volt >= BATT_LV4_THESHOLD)
//				vtCurrent += 1.2;				//放电模式最大电池设定为1.0
//			else if (*volt >= BATT_LV1_THESHOLD)
//				vtCurrent += 0.8;
//			else if (*volt >= BATT_LV0_THESHOLD)
//				vtCurrent += 0.5;
//		}
//	}
	*volt += vtCurrent * DISCHARGE_RESISTENCE;
//	if(fIsDischarging){
//		*volt+=MAX_DISCHARGE_CURRENT*DISCHARGE_RESISTENCE;
//	}
	vTestCompBatt = *volt;
	vCompensationVolt = vTestCompBatt;
}
/***************************************************************************
 *
 *  	ADC电压补偿.在灯开启时的电压补偿
 *
 ***************************************************************************/
void adcVoltCompensation(float *volt) {
//	if (fIsCharging)
//		return;
	battVoltCompensation(volt);
}
/***********************************************************************************************************
  *  @brief			关机模式下,充电标志相关的处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void onChargingIconInPowerOffMode(void) {
	if (fIsSystemOff) {
		fIsWakeupByCharge = WakeupLCDOver;
		lcdExitSleep();
		fIsLightActive = 0;
//		LEDPowerOff;
		displayParams.DisplayModeIndex = ChargingAtPowerDown;
		displaySystemMenu(&displayParams);
		vChargePercent = 0;
//		fIsSystemFirstPowerON = TRUE;			//仅用于下次开机,刷新剩余时间
	}
}
/***********************************************************************************************************
  *  @brief 	充电情况下关机的处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void powerDownFromChageMode(void) {
	if (fIsWakeupByCharge == WakeupLCDOver && fIsSystemOff) {
		fIsWakeupByCharge = TimeToPowerOff;
		vTimeToTurnOffLCD = POWER_DONW_LCD_TIMEOUT;
	}
//	if(fIsSystemOff&&(FactoryMode!=displayParams.DisplayModeIndex)){
//		batteryPercentDisplay(0);
//	}
}
/************************************************************************************
 *
 * RisingEnable	:FALSE     level允许上升
 * 				:TRUE     level不允许上升
 *
 *************************************************************************************/
void processBattLevel(float vtBattReal, u8 RisingEnable) {
	if (vtBattReal >= BATT_LV4_THESHOLD) {
		if (!RisingEnable) {
			if (vCurrentBattLv >= Max_Batt_level)
				vCurrentBattLv = Max_Batt_level;
		} else {
			vCurrentBattLv = Max_Batt_level;
		}
	} else if (vtBattReal >= BATT_LV3_THESHOLD) {
		if (!RisingEnable) {
			if (vCurrentBattLv >= 3)
				vCurrentBattLv = 3;
		} else {
			vCurrentBattLv = 3;
		}
	} else if (vtBattReal >= BATT_LV2_THESHOLD) {
		if (!RisingEnable) {
			if (vCurrentBattLv >= 2)
				vCurrentBattLv = 2;
		} else {
			vCurrentBattLv = 2;
		}
	} else if (vtBattReal >= BATT_LV1_THESHOLD) {
		if (!RisingEnable) {
			if (vCurrentBattLv >= 1)
				vCurrentBattLv = 1;
		} else {
			vCurrentBattLv = 1;
		}
	} else if (vtBattReal < BATT_LV1_THESHOLD) {
		vCurrentBattLv = 0;
	}
}
/***********************************************************************************************************
  *  @brief 		设置电池量电格数
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void setBattLevel(float vBattReal) {
	if (!fIsSystemInitialing && !vDisableGetBattTimer) {
		if (fIsDcMode) {
			vCurrentBattLv = Max_Batt_level;
		} else {
			adcVoltCompensation(&vBattReal);
			processBattLevel(vBattReal, FALSE);
			if (vBattReal < BATT_LV0_THESHOLD) {
				fPowerOffByBattLow = 1;
			}
		}
		displayParams.battIndex = vCurrentBattLv;
	}
}
/***********************************************************************************************************
  *  @brief  		系统关机入口
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void systemOffByManual(void) {
	fIsSystemOff = TRUE;
	hotFuncDeinit();
	clear();
//	if (fIsCharging) {
//		fIsCharging = TRUE;
//		fIsLightActive = 0;
//		turnOffAllLightEffect();
//		clearPowerKeyInfo();
//		vTimeToTurnOffLCD = 0;
//		fIsChargeDisplaying = 0;
//		fIsWakeupByCharge = WakeupLCDOver;
//		displayParams.DisplayModeIndex = ChargingAtPowerDown;
//		displaySystemMenu(&displayParams);
//		if (fIsBattFully) {
//			batteryPercentDisplay(100);
//		}
//		vChargePercent = 0;
//	} else {
		systemPowerDown();
//	}
}

void checkRealEntryPowerDownMode(void){
	systemOffByManual();
}




//void updateLEDStts(displayParamsStruct * disParams) {
//	if((disParams->arrowIndex==HuesSetting)||(disParams->arrowIndex==SaturationSetting)){
//		pUserColor->hues=disParams->hues;
//		pUserColor->saturation=(float)disParams->saturation/100;
//		pUserColor->brightness=((float)disParams->brightness)*(100-RGBLED_REAL_START_DUTY)/(100*100)+(float)RGBLED_REAL_START_DUTY/100;			//从9%起步
//		hsb2Rgb(pUserColor);			//数据仅用于电池电压补偿
////		setRgbData(RGB_DATA_LENGTH,disParams);
//	}else if (ColorTempSetting == disParams->arrowIndex) {
//		updateColorTemp(disParams);
////			setColorTempData(COLOR_TEMP_LENGTH,disParams);
//	} else if (disParams->arrowIndex >= Style1Setting && disParams->arrowIndex <= Style3Setting) {
//		//startLightEffect(disParams);
//			fIsLightEffectOn=TRUE;
////		setEffectData(PRESET_EFFECT_DATA_LENGTH,disParams);
//	}
//}
/***********************************************************************************************************
 *  @brief  		根据PWM调节差,获取渐进总步进数.
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :
 ************************************************************************************************************/
uint16 getMaxIndex(uint16 deltaC, uint16 deltaW) {

	return (deltaC > deltaW) ? (deltaC / 3) : (deltaW / 3);

}
/***********************************************************************************************************
 *  @brief		获取两个uint16数的差的绝对值
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :
 ************************************************************************************************************/
uint16 absSubUint16(uint16 a, uint16 b) {

	return (a > b) ? (a - b) : (b-a);

}
/***********************************************************************************************************
  *  @brief			实时更新灯光参数到LED
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void updateLEDStts(displayParamsStruct * disParams) {
	if (!disParams->brightness)
		LEDPowerOn;
#if(RGB_INSIDE==1)
	if (ColorTempSetting == disParams->arrowIndex) {
		turnOffRGBLamp();
		if (!graduallyAdjust.fIsAdjustSlowMode) {
			updateColorTemp(disParams);
		} else {
			setTargeColorTempData(disParams, &targetPwmDataStruct);
			graduallyAdjust.vAdjustSlowIndex = 0;
			graduallyAdjust.vAdjustSlowMwStep = ((float) (targetPwmDataStruct.valueOfMw - pcaDataStruct.valueOfMw) / MAX_SLOW_ADJUST_STEPS);
			graduallyAdjust.vAdjustSlowCwStep = ((float) (targetPwmDataStruct.valueOfCw - pcaDataStruct.valueOfCw) / MAX_SLOW_ADJUST_STEPS);
//			graduallyAdjust.vMaxIndex=getMaxIndex(absSubUint16(targetPwmDataStruct.valueOfMw, pcaDataStruct.valueOfMw),absSubUint16(targetPwmDataStruct.valueOfCw,pcaDataStruct.valueOfCw));
//			if(graduallyAdjust.vMaxIndex){
//				graduallyAdjust.vAdjustSlowMwStep = ((float) (targetPwmDataStruct.valueOfMw - pcaDataStruct.valueOfMw) / graduallyAdjust.vMaxIndex);
//				graduallyAdjust.vAdjustSlowCwStep = ((float) (targetPwmDataStruct.valueOfCw - pcaDataStruct.valueOfCw) / graduallyAdjust.vMaxIndex);
//			}
		}
	} else if (disParams->arrowIndex >= Style1Setting) {
		startLightEffect(disParams);
	} else if ((disParams->arrowIndex == HuesSetting) || (disParams->arrowIndex == SaturationSetting)) {
		turnOffColorTempLamp();
		updateRGBLamp(disParams);
	} else if (disParams->arrowIndex >= Style1Setting && disParams->arrowIndex <= Style3Setting) {
		startLightEffect(disParams);
	}
#else
	if (!graduallyAdjust.fIsAdjustSlowMode) {
		updateColorTemp(disParams);
	} else {
		uint16 totalSteps=0;
		totalSteps=(fIsSystemFirstPowerON==0)?MAX_SLOW_ADJUST_STEPS:MAX_STARTUP_STEPS;
		setTargeColorTempData(disParams, &targetPwmDataStruct);
		graduallyAdjust.vAdjustSlowIndex=0;
		graduallyAdjust.vAdjustSlowMwStep = ((float) (targetPwmDataStruct.valueOfMw - pcaDataStruct.valueOfMw) / totalSteps);
		graduallyAdjust.vAdjustSlowCwStep = ((float) (targetPwmDataStruct.valueOfCw - pcaDataStruct.valueOfCw) / totalSteps);
	}
#endif
}
/***************************************************************************
 *
 *  	电压补偿.灯开启时的电压补偿
 *
 ***************************************************************************/
void chkLightEffectModeVoltCompensation(float *volt){
	float vtMin=5.0;
	int i=0;
	if(fIsLightEffectOn){
		if(++vLightEfftectVoltDectCnt>BATT_ARRAY_SIZE){
			vLightEfftectVoltDectCnt=PARAMS_DATA_RESET;
			for(;i<BATT_ARRAY_SIZE;i++){
				if(vtMin>battArray[i]){
					vtMin=battArray[i];
				}
			}
			*volt=vtMin;
			fIsBattAdcValueGot=1;
		}else{
			fIsBattAdcValueGot=0;
		}
	}else{
			fIsBattAdcValueGot=1;
	}
}

/*********************************************************************
 *
 * 充电状态,根据当前电池两端电压估算充电电流
 *
 **********************************************************************/
void getChargingCurrentByVolt(float *vtCurrent,float vtVolt){
	if(vtVolt<=KEEP_VOLT_THRESHOLD){
		*vtCurrent=MAX_CHARGE_CURRENT;
	}else{
		*vtCurrent=MAX_CHARGE_CURRENT*(1-(vtVolt-KEEP_VOLT_THRESHOLD)*(1.0/(CHRAGE_FULL_VOLT-KEEP_VOLT_THRESHOLD)));
	}
}
/***********************************************************************************************************
  *  @brief		设置SWD口为普通IO口
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void setSWD2NormalIO(void) {
		RCC->REGLOCK = UNLOCK_RAM_CONST;
		RCC->IOMUX = 0x5A690000;					//set swdio			b0=0/1   普通IO/仿真端口
		RCC->REGLOCK = LOCK_RAM_CONST;
}
/***********************************************************************************************************
  *  @brief		设置SWD口为仿真口,无普通IO功能
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/

void set2SWDMode(void) {
		RCC->REGLOCK = UNLOCK_RAM_CONST;
		RCC->IOMUX = 0x5A690001;					//设置为swdio
		RCC->REGLOCK = LOCK_RAM_CONST;
}

/***********************************************************************************************************
  *  @brief		Cool LED对应的Pin退出PWM模式,设为IO模式并置低
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 setCoolOutputLow(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//output 模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_Pin = CW_LED_PIN ;							//
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
	GPIO_Init(CW_LED_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(CW_LED_PORT, CW_LED_PIN);
}
/***********************************************************************************************************
  *  @brief		WarmLED对应的Pin退出PWM模式,设为IO模式并置低
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 setWarmOutputLow(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//output 模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_Pin = MW_LED_PIN ;							//
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
	GPIO_Init(MW_LED_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(MW_LED_PORT, MW_LED_PIN);
}
/***********************************************************************************************************
  *  @brief		Blue LED对应的Pin退出PWM模式,设为IO模式并置低
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 setBlueOutputLow(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//output 模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_Pin = Blue_LED_PIN ;						//
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
	GPIO_Init(Blue_LED_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(Blue_LED_PORT, Blue_LED_PIN);
}
/***********************************************************************************************************
  *  @brief		Green LED对应的Pin退出PWM模式,设为IO模式并置低
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 setGreenOutputLow(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//output 模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//GPIOx_OTYPER   0/1 推挽输出/开漏输出
	GPIO_InitStruct.GPIO_Pin = Green_LED_PIN ;						//
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不上拉
	GPIO_Init(Green_LED_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(Green_LED_PORT, Green_LED_PIN);

}

//void resetParamStoreTimer(void) {
//	vStoreParamCountDwon = AUTO_SAVE_PARAMS_TIMER;
//}
/***********************************************************************************************************
 *  @brief 			store current system status automatic when count down
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :
 ************************************************************************************************************/
void autoSaveFunc(void) {

	if (vStoreParamCountDwon && !--vStoreParamCountDwon) {

		StoreExceptions2Flash();

	}

}
