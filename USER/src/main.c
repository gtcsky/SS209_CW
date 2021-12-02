#include "user_exti.h"
#include "user_led.h"
#include "user_lcd.h"
#include "user_advtime.h" 
#include "user_internalFlash.h"
#include "user_SysClock.h"
#include "user_rtc.h"
#include "user_lptimer.h"
#include "user_key.h"
#include "user_menu.h"
#include "user_adc.h"
#include "io_define.h"
#include "user_basetime.h"
#include "user_awk.h"
#include "user_iwdt.h"
#include "user_color.h"
#include "user_ddl.h"
#include "user_lcd.h"
#include "user_i2c.h"
#include "communication.h"

 /**
  * @brief   main
  * @param   无
  * @retval 无
  */
int main(void) {
//	u32	iCNTVAL=0;
//	GPIO_InitTypeDef GPIO_InitStruct;

	RCC->AHBCLKEN = 0xffffffff; //flash,crc,gpioa/b/c/d
	RCC->APBCLKEN = 0xffffffff; //uart,i2c,spi,pca,tim,btim, pwm? iwdt,dwdt,rtc,awk,trim

	//P47 时钟主频设置
	systemClkSet();
	fIsSystemInitialing = 1;
	LPTIMER_Config();													      	//32.768K/38.4K时钟中断配置
	set2SWDMode();
	powerOnIoInit();
	while (sysTimes.vSystem5ms < 40)
		;																 	//Reset0.5s之内,把SWD io用作SWD口防止无法烧录的问题
	sysTimes.vSystem5ms = 0;
	setSWD2NormalIO();
	powerDataInit();
	/*配置外部中断管脚*/
	EXTI_Key_Config();
	/*配置lptimer模块*/
	//basic定时器开启1ms定时中断
	BASIC_TIM_Mode_Config(TIM10);											//basic定时器开启1ms定时中断
//	BASIC_TIM_Mode_Config(TIM11);

	/*配置PWM 模式*/
//	userPwmParaStruct.period=ADVICE_TIM_PERIOD;
//	ADVTIM_PWMMODE_Config(&userPwmParaStruct);
	userPwmParaStruct.period = ADVICE_TIM_PERIOD;
	ADVTIM_PWMMODE_Config(&userPwmParaStruct, ADVTIM1);
//	ADVTIM_PWMMODE_Config(&userPwmParaStruct,ADVTIM2);
	setLedIoAf();

	setCoolData(0);
	setWarmData(0);
	resetEntrySleepTime();
	sysTimes.vSystem5ms = 0;
	while (sysTimes.vSystem5ms <20)
		;
	sysTimes.vSystem5ms = 0;
	/*Config IWDG Parameters*/
	fIsSystemInitialing = 0;

	vBattVol = readBattInfoImmediately();
	vCompensationVolt = vBattVol;
//	setBattLevel(vBattVol);
	exceptionsCheck();
#if(RGB_INSIDE==1)															//从CW机型烧录成RGB机型时
	if (displayParams.hues > MAX_HUES || displayParams.saturation > MAX_SATURATION) {
		powerDataInit();
	}
#else
	if(displayParams.arrowIndex!=DEFAULT_ARROR_INDEX) {

		powerDataInit();

	}
#endif
//	adcCallbackSetting();
	fIsSystemFirstPowerON = TRUE;
	if (fIsDcMode || vCompensationVolt > BATT_LV0_THESHOLD) {
		fIsSystemOff = 0;
		fIsLightActive = ON;
		if (!displayParams.brightness) {
			LEDPowerOff;
		} else {
			LEDPowerOn;
		}
		if (!GET_POWER_KEY_PIN_STTS) {
			vKeyValue = FACTORY_MODE_KEY;
		}
		pcaDataStruct.valueOfMw=0;
		pcaDataStruct.valueOfCw=0;
		graduallyAdjust.fIsAdjustSlowMode = TRUE;
		oriPwmDataStruct.valueOfMw = pcaDataStruct.valueOfMw;
		oriPwmDataStruct.valueOfCw = pcaDataStruct.valueOfCw;
		updateLEDStts(&displayParams);

		if (fIsDcMode)
			displayParams.battIndex = Max_Batt_level;
	} else {
		fIsSystemOff = TRUE;
		fIsLightActive = OFF;
	}
#if(LCD_INSIDE==1)
	powerOnLcdInit();
#endif
	fIsPowerOnFirst10s = TRUE;
	fIsBattFully = FALSE;

	if (fIsSystemOff)
		checkRealEntryPowerDownMode();
	while (1) {
		if (!fIsSystemOff) {
			funcKeyTask();
			encoderCheck();
			if (vKeyValue) {
				if (FactoryMode != displayParams.DisplayModeIndex) {
					MenuFunction();
				}
			}
		}
		if (sysTimes.vSystem5ms != 0) {
			sysTimes.vSystem5ms--;
			fucnPer5ms();
			//iCNTVAL=IWDG_GetReload();
//			IWDG_GetReload();
//			IWDG_Feed();
//			IWDG_RELOAD();
		}
	}
}//main




