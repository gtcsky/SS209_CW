/*
 * const.h
 *
 *  Created on: 2021年7月12日
 *      Author: Achilles Zhang
 */

#ifndef USER_INC_CONST_H_
#define USER_INC_CONST_H_

typedef signed char 			i8;
typedef unsigned char 		u8;
typedef unsigned char 		UINT8;
typedef unsigned char 		uint8;
typedef short	 			i16;
typedef unsigned short 		u16;
typedef unsigned short 		UINT16;
typedef unsigned short 		uint16;
typedef int 					i32;
typedef unsigned 			u32;
typedef unsigned 			UINT32;
typedef unsigned 			uint32;
typedef long long 			i64;
typedef unsigned long long 	u64;
typedef unsigned long long 	UINT64;
typedef unsigned long long 	uint64;
#define TRUE        ((uint8_t) 1u)
#define FALSE       ((uint8_t) 0u)
typedef void         (*func_ptr_t)(void);

typedef enum{
	TaoBao433Mhz=0,
	SS205Customize=1,
}RemoteType;

#define	REMOTE_TYPE_DEFINE					0			//遥控器类型
#define	POWER_ON_BY_REMOTE_CTRL				0			//遥控开机功能
#define	DEBUG_VOLT_INFO						0			//
#define	RGB_INSIDE								0			//是否包含RGB模式
#define	LCD_INSIDE								0			//是否配置LCD模块
#define	LIGHTEFFECT_INSIDE						0			//是否配置特效模式
#define	EEPROM_INSIDE							1			//是否配置EEPROM
#define	COMMUNICATION_PROTOCAL_TYPE		1			//  	0/1:UART/I2C

#define 	HIRC_24M_ADDR							0X180000C0
#define	HIRC_22M_ADDR							0X180000C2
#define	HIRC_16M_ADDR							0X180000C4
#define	HIRC_8M_ADDR							0X180000C6
#define	HIRC_4M_ADDR							0X180000C8
#define	LIRC_32K768_ADDR						0X180000D0
#define	LIRC_38K4_ADDR							0X180000D4

#define 	SYSTEM_VDD							(3.4)
#define	ADC_CONST								(4096)
#define 	vA2D_Coe								(ADC_CONST/SYSTEM_VDD)			//模数转换系数	2.0v*vA2D_Coe=2.0v对应的AD值
#define 	vD2A_Coe								(SYSTEM_VDD/ADC_CONST)			//数模转换系数	0x0136*vD2A_Coe=0x0136对应的电压
#define	SYSTEM_VCAP_VOLT						(2.50)

#define	VOLT_BATT_FULL							4.20
#define	SOFTWARE_VERSION						"SW:2021-11-13"
#define	SOFTWARE_VERSION_LENGTH				13
#define	DRIVER_VERSION_LEN					10					//有效长度
#define	HARDWARE_VERSION					"HW:VER06"
#define	BATT_ADC_TOLERANCE					0.06


#define	AUTO_SAVE_PARAMS_TIMER				10					//10*50=500MS

#define	BATT_DET_PULL_UP_RES					270
#define	BATT_DET_PULL_DOWN_RES				51

#define	UNLOCK_RAM_CONST					0x55AA6699
#define	LOCK_RAM_CONST						0x55AA6698

#define	USER_DOG_CONST						1600				//1600*5ms=8s
#define	MAX_CHARGE_CURRENT					1.9					//2.0A
#define	MAX_DISCHARGE_CURRENT				2.0					//2.0A
#define	KEEP_VOLT_THRESHOLD					4.15					//4.15V
#define	CHRAGE_FULL_VOLT						4.20

#define	PWM_PERIOD_CONST					1200				//PWM frequency	24M/1200=20Khz
#define	PWM_FRQ_CONST						1200				//PWM frequency	24M/1200=20Khz
#define	PWM_MAX_CW							1080
#define	MAX_ColorTemp							75//56
#define	MIN_ColorTemp							32
#define	LED_REAL_START_DUTY					4		// LED 实际亮度起步4%

#define	MAX_HUES								360
#define	MAX_SATURATION						100
#define	MAX_BRIGHTNESS						100
#define	MIN_HUES								0
#define	MIN_SATURATION						1
#define	MIN_BRIGHTNESS						1
#define	DEFAULT_HUES							0
#define	DEFAULT_BRIGHTNESS					5
#define	DEFAULT_SATURATION					100
#if(RGB_INSIDE==1)
	#define	DEFAULT_ARROR_INDEX				0	//3
#else
	#define	DEFAULT_ARROR_INDEX				3	//3
#endif
#define	DEFAULT_COLOR_TEMP					44//44
#define	DEFAULT_STYLE1_VALUE					0
#define	DEFAULT_STYLE2_VALUE					0
#define	DEFAULT_STYLE3_VALUE					0
#define	MAX_CD_TIMER							100
#define	Max_Batt_level							4
#if(RGB_INSIDE==1)
#define	Min_Arrow_Index						0
#define	Max_Arrow_Index						6
#else
#define	Min_Arrow_Index						DEFAULT_ARROR_INDEX
#define	Max_Arrow_Index						DEFAULT_ARROR_INDEX
#endif
#define	MAX_LightEffect							5//2
#define	PerStyleSize								6//(MAX_LightEffect+1)

#define	MAX_SLOW_ADJUST_STEPS				30
#define	MAX_STARTUP_STEPS					200

#define	ON_LED_DELAY_CONST					20				//20*5ms=100ms
#define 	DISABLE_BATT_ADC_TIMER_CONST		40;				//40*50ms=2s

#define	NORMAL_MODE_TIMEOUT				600*20			//60s
#define	POWER_DOWN_MODE_TIMEOUT			3				//10*50ms=500ms
#define	POWER_DOWN_CHARGE_TIMEOUT		30
//#define	POWER_DONW_LCD_TIMEOUT			60				//3S
#define	POWER_DONW_LCD_TIMEOUT				3				//150ms

#define	BATTERY_RESISTANCE					0.080		//140mR	for charging with protected board
#define	CHARGE_VOLT_DIVIATION				0.06			//		0.06V
#define 	RED_POWER_RATING						2.74			//2.74=0.66*8.3/2
#define 	GREEN_POWER_RATING					3.07			//3.07=0.74*8.3/2
#define 	BLUE_POWER_RATING					2.91			//2.91=0.74*8.3/2
#define	CW_POWER_RATING						10.0//
#define	MAX_CW_DUTY							(1.0)
#define	MW_POWER_RATING						10.0
#define	MAX_MW_DUTY							(1.0)

#define	RGBLED_REAL_START_DUTY				9		// RGB LED 实际亮度起步4%
#define	LED_REAL_START_DATA					0x18	//冷光和暖光最低有效亮度
#define	RedGreenRate							1.0
#define	BlueGreenRate							1.0
#define	RedGreenRateStep						(1-(1/RedGreenRate))
#define	BlueGreenRateStep						(1-(1/BlueGreenRate))
#define	BrightnessThreshold						0.40
#define	BrightnessGap							(BrightnessThreshold-RGBLED_REAL_START_DUTY*1.0/100)
#define	BrightnessCompersationGap				(RGBLED_REAL_START_DUTY-LED_REAL_START_DUTY)
#define	BrightnessCompersationStep				((RGBLED_REAL_START_DUTY-LED_REAL_START_DUTY)/(3*BrightnessGap*100))

#define	INFINITE_LOOP_TIMES					100
#define	SYSTEM_HOT_COE						0.8;

#define	MW_MAX_TIMER_CONST					(1.0/(PWM_MAX_CW*1.0/PWM_FRQ_CONST))					//65min
#define	CW_MAX_TIMER_CONST					(MW_MAX_TIMER_CONST*(CW_POWER_RATING/MW_POWER_RATING))
#define 	DISCHARGE_RESISTENCE					0.040		//80mR for discharging
#define 	DISCHARGE_RESISTENCE1D10				DISCHARGE_RESISTENCE*1.10
#define 	DISCHARGE_RESISTENCE1D08				DISCHARGE_RESISTENCE*1.08
#define 	DISCHARGE_RESISTENCE1D05				DISCHARGE_RESISTENCE*1.05
#define 	DISCHARGE_RESISTENCE0D95				DISCHARGE_RESISTENCE*0.95
#define 	DISCHARGE_RESISTENCE0D90				DISCHARGE_RESISTENCE*0.90
#define 	DISCHARGE_RESISTENCE0D88				DISCHARGE_RESISTENCE*0.88
#define 	DISCHARGE_RESISTENCE0D85				DISCHARGE_RESISTENCE*0.85
#define 	DISCHARGE_RESISTENCE0D82				DISCHARGE_RESISTENCE*0.82

#define	PARAMS_DATA_RESET					0
#define	BATT_ARRAY_SIZE						10
#define	BATT_LV0_THESHOLD						3.40		//3.3
#define	BATT_LV1_THESHOLD						3.50		//3.48//3.5//3.5
#define	BATT_LV2_THESHOLD						3.60		//3.62//3.63//3.6
#define	BATT_LV3_THESHOLD						3.75		//3.70//3.71//3.7
#define	BATT_LV4_THESHOLD						3.90		//3.78//3.8//3.8
#define	CHARGE_FINAL_VOLT						4.14		//恒压模式电压阈值

#define	REMAIN_TIME_STEP1_THRESHOLD			BATT_LV0_THESHOLD
#define	REMAIN_TIME_STEP1_START				0//MIN
#define	REMAIN_TIME_STEP1_LAST				12//MIN
#define	REMAIN_TIME_STEP1_VOLT_LAST			(REMAIN_TIME_STEP2_THRESHOLD-REMAIN_TIME_STEP1_THRESHOLD)

#define	REMAIN_TIME_STEP2_THRESHOLD			3.48//3.53
#define	REMAIN_TIME_STEP2_START				REMAIN_TIME_STEP1_LAST//MIN
#define	REMAIN_TIME_STEP2_LAST				29//MIN
#define	REMAIN_TIME_STEP2_VOLT_LAST			(REMAIN_TIME_STEP3_THRESHOLD-REMAIN_TIME_STEP2_THRESHOLD)

#define	REMAIN_TIME_STEP3_THRESHOLD			3.63//3.67
#define	REMAIN_TIME_STEP3_START				(REMAIN_TIME_STEP2_LAST+REMAIN_TIME_STEP2_START)//MIN
#define	REMAIN_TIME_STEP3_LAST				17//MIN
#define	REMAIN_TIME_STEP3_VOLT_LAST			(REMAIN_TIME_STEP4_THRESHOLD-REMAIN_TIME_STEP3_THRESHOLD)

#define	REMAIN_TIME_STEP4_THRESHOLD			3.75
#define	REMAIN_TIME_STEP4_START				(REMAIN_TIME_STEP3_LAST+REMAIN_TIME_STEP3_START)//MIN
#define	REMAIN_TIME_STEP4_LAST				31//25//MIN
#define	REMAIN_TIME_STEP4_VOLT_LAST			(REMAIN_TIME_STEP5_THRESHOLD-REMAIN_TIME_STEP4_THRESHOLD)

#define	REMAIN_TIME_STEP5_START				(REMAIN_TIME_STEP4_LAST+REMAIN_TIME_STEP4_START)//MIN
#define	REMAIN_TIME_STEP5_THRESHOLD			4.05
#define	REMAIN_TIME_STEP5_LAST				7//4//MIN

#define	REMAIN_TIME_STEP5_VOLT_LAST			(REMAIN_TIME_STEP6_THRESHOLD-REMAIN_TIME_STEP5_THRESHOLD)

#define	REMAIN_TIME_STEP6_THRESHOLD			4.16

#define	MAX_LEVEL_CHARGE_VOLT_L0				4.00
#define	CHARGE_TIMER_CONST_STEP0			60	//		1min
#define	CHARGE_TIMER_CONST_STEP				210	//		3.5min

#define	TEMPERATURE_COE_NORMAL				1.0
#define	TEMPERATURE_COE_LOW					0.8
#define 	OVER_TEMPERATURE_VOLT_Lv1			85			//75//70//TEMPERATURE_40C_VOLT//80
#define 	OVER_TEMPERATURE_VOLT_Lv0			75			//70//65//TEMPERATURE_35C_VOLT//70
#define 	NORMAL_TEMPERATURE_VOLT			60			//60//60//TEMPERATURE_30C_VOLT//60


//NTC=470K(NTCG104QH474HT1) 上拉分压=100K
#define TEMPERATURE_5C_VOLT			2.328	//2.328V
#define TEMPERATURE_10C_VOLT			2.278	//2.278V
#define TEMPERATURE_15C_VOLT			2.218	//2.218V
#define TEMPERATURE_20C_VOLT			2.145	//2.145V
#define TEMPERATURE_23C_VOLT			2.112	//2.112V
#define TEMPERATURE_25C_VOLT			2.061	//2.061V
#define TEMPERATURE_28C_VOLT			2.021	//2.021V
#define TEMPERATURE_30C_VOLT			1.962	//1.962V
#define TEMPERATURE_33C_VOLT			2.012	//2.012V
#define TEMPERATURE_35C_VOLT			1.850	//1.850V
#define TEMPERATURE_38C_VOLT			1.801	//1.801V
#define TEMPERATURE_40C_VOLT			1.727	//1.727V
#define TEMPERATURE_43C_VOLT			1.675	//1.675V
#define TEMPERATURE_45C_VOLT			1.597	//1.597V
#define TEMPERATURE_48C_VOLT			1.542	//1.542V
#define TEMPERATURE_50C_VOLT			1.459	//1.459V
#define TEMPERATURE_53C_VOLT			1.404	//1.404V
#define TEMPERATURE_55C_VOLT			1.321	//1.321V
#define TEMPERATURE_58C_VOLT			1.267	//1.267V
#define TEMPERATURE_60C_VOLT			1.185	//1.185V
#define TEMPERATURE_63C_VOLT			1.132	//1.132V
#define TEMPERATURE_65C_VOLT			1.053	//1.053V
#define TEMPERATURE_68C_VOLT			1.003	//1.003V
#define TEMPERATURE_70C_VOLT			0.929	//0.929V
#define TEMPERATURE_73C_VOLT			0.887	//0.887V
#define TEMPERATURE_75C_VOLT			0.813	//0.813V
#define TEMPERATURE_78C_VOLT			0.771	//0.771V
#define TEMPERATURE_80C_VOLT			0.708	//0.708V
#define TEMPERATURE_83C_VOLT			0.671	//0.671V
#define TEMPERATURE_85C_VOLT			0.614	//0.614V
#define TEMPERATURE_88C_VOLT			0.581	//0.581V
#define TEMPERATURE_90C_VOLT			0.531	//0.531V
#define TEMPERATURE_93C_VOLT			0.502	//0.502V
#define TEMPERATURE_95C_VOLT			0.458	//0.458V
#define TEMPERATURE_98C_VOLT			0.433	//0.433V
#define TEMPERATURE_100C_VOLT		0.395	//0.395V


typedef enum{
	OFF=0,
	ON=1,
//	PREINSTALL_EFFECT=2,
//	CUSTOMIZE_EFFECT=3,
	OffByRemote=4
}lightSttsEnum;

typedef enum{
	IdleIamgeDisplay=0,
	ModeTDisplay=1,
	CountDownDisplay=2,
	ChargingAtPowerDown=3,
	FactoryMode=4
}displayModeEnum;

typedef enum{
	HuesSetting=0,
	SaturationSetting=1,
	BrightnessSetting=2,
	ColorTempSetting=3,
	PreinstallEffect=4,
	Style1Setting=4,
	Style2Setting=5,
	Style3Setting=6
}settingIndexEnum;

typedef enum{									//关机充电状态指示
	DoNothing=0,
	SetWakeupLCD=1,
	WakeupLCDOver=2,
	TimeToPowerOff=3
}chargeSttsAtPowerModeEnum;

typedef struct {

	uint8 fIsAdjustSlowMode;		//调节色温时,采用缓慢变化模式,以降低调节时的闪烁现象
	uint16 vAdjustSlowIndex;
	float vAdjustSlowCwStep;
	float vAdjustSlowMwStep;
	uint16 vMaxIndex;

} graduallyAdjustDef;

//typedef void(* daCallbacksType)(void);
typedef uint8 (* daCallbacksType)(uint8 chl);
typedef struct{
	uint8	vSingleChlAdcConverting;
	uint8	vChlResultGot;
	uint8	vConvertingIndex;
	uint16 	aDaValues[8];
	uint8	vPospendConvertInfo;
	daCallbacksType callbacks[8];
}adcConvertStruct;

#pragma pack(1)
typedef struct {
	UINT16 checkCode;				//
	UINT8 DisplayModeIndex;			//	参考值: displayModeEnum
	UINT8 brightness;					//	亮度值	(0~100)%
	UINT8 colorTemperature;			//	33~65(00K)
	UINT8 arrowIndex;					//	光标序号	(0~6)		参考值:settingIndexEnum
	UINT8 battIndex;					//	电量标志	(0~6)
#if(LIGHTEFFECT_INSIDE==1)
	UINT8 style1Value;				//	0~2对应闪光灯标志A,B,C 三种模式
	UINT8 style2Value;				//	0~2对应循环标志A,B,C 三种模式
	UINT8 style3Value;				//	0~2对应锁标志A,B,C 三种模式
	UINT8 fIsEffectMode;				//	自定义特效模式+预设特效模式=1.普通模式=0
	UINT8 preinstallEffectNo;			//	预设特效编号
#endif
#if(RGB_INSIDE==1)
	UINT8 saturation;					//	饱和值	(0~100)
	UINT16 hues;						//	色调值	(0~360)
#endif
} displayParamsStruct;
#pragma pack()

typedef enum{
	VersionDisplay=0,
	WaitFuncKey,
	RedLedTesting,
	GreenLedTesting,
	BlueLedTesting,
	CoolLedTesting,
	WarmLedTesting,
	WaitBrightnesssDecKey,
	IdleModeTesting,
	SystemVoltTesting,
	WaitIncKey,
	SleepModeTesting

}factoryModeItem;

#define	MAX_FACTORY_ITEM	SleepModeTesting
#define	FACTORY_TIME_1S	100
#define	FACTORY_TIME_2S	200
#define	FACTORY_TIME_2S9	290
#define	FACTORY_TIME_3S	300


#define	FACTORY_KEY_COST	10
typedef struct{
	factoryModeItem	factoryIndex;
	u16	factoryTimerCnt;			//step=10ms
	u8	factoryBrightIndex;
	u8	factoryIncIndex;
	u8	keyPresssedIndex;
	u8    brightnessIncCnt;
	u8    brightnessDecCnt;
	u8	funcIncCnt;
	u8	funcDecCnt;
	u8	factoryTemp;
}factoryModeTypeDef;

typedef enum{
	ReadStatus=0,
	ReadHSB=1,
	ReadBrightness=2,
	ReadColorTemperature=3,
	ReadPreinstallEffectNo=4,
	ReadCustomizeEffectInfo=5,
	ReadTemperature=6,
	ReadSwVersion=16,
	ReadHwVersion=17,
	ReadProtocolVersion=18
}uartReadingType;

typedef enum{
	MasterRead=0,
	MasterCtrl=1,
	UserCtrl=2
}sendCMDType;

typedef struct{
	UINT8	fIsUartBuffNotEmpty;
	UINT8  	backupBuffer[20];
	UINT8	backupCMDLen;
	sendCMDType	uartCmdType;
}uartBufferType;

typedef struct{

	UINT8 	length;
	UINT16 	flag;
	UINT8 	cmd;
	UINT8	preinstallEffectNo;
	UINT8	brightness;
	UINT8	colorTemperature;
	UINT16	hues;
	UINT8	saturation;
	UINT8	customizeEffectMode;
	UINT8	customizeEffectTimes;
	UINT8	customizeEffectFreq;
//	UINT8	fIsMasterWrite;							//	0/1主控读取/写入命令
	UINT8	vSystemTemperature;						//	系统温度
	sendCMDType	uartCmdType;					//	0/1/2 主控读取/写入命令/按键命令
}UARTCommandStruct;


typedef struct{
	UINT16	valueOfRed;
	UINT16	valueOfGreen;
	UINT16	valueOfBlue;
	UINT16	valueOfCw;
	UINT16	valueOfMw;
}PcaDataStruct;

//typedef struct{
//
//	UINT16	red;
//	UINT16	green;
//	UINT16	blue;
//	UINT16	hues;
//	float	saturation;
//	float	brightness;
//}colorStructType;
typedef struct{
	UINT16	red;
	UINT16	green;
	UINT16	blue;
	UINT16	hues;
	float	saturation;
	float	brightness;
	UINT16	cWhite;
	UINT16 	wWhite;
}colorStructType;

typedef struct{
	UINT16	indicatorMaxIndex;
	UINT16	currentIndex;
	UINT8	indicatorStyle;
	UINT16	customizeLoopTimes;				//自定义模式循环次数
	UINT8	customizeLoopIndex;				//自定义模式循环已执行次数
	const 	UINT8 *	timeArray;
	const	UINT8 *	sttsArray;
	const	UINT16 * HuesArray;
	const	float * SaturationArray;
	func_ptr_t  pfnLedFuncCallBack;
	func_ptr_t  pfncustomizeEffectOverCallBack;	//算定循环结束时回调
	UINT8	fIsGradualMode;					//0/1 闪烁模式/渐变模式
	UINT8	IndexOfHues;
	UINT8	MaxHuesIndex;
	UINT8 	LastRandomData;
	UINT8 	vSeed;
	UINT8	fIsFirstCycle;
//	UINT8 	fTimeToOff;
}LEDParametersStruct;

typedef struct{
	volatile	uint16 	vSystemLIRC5ms;
	volatile	uint16 	vSystem5ms;
	volatile	uint16	vSystem1s;
	volatile	uint16	vTime1msCnt;
	volatile	uint16	vTimerCnt;
	volatile	uint8	fTime50ms;
	volatile	uint8	fTime100ms;
	volatile	uint8	fTime500ms;
	volatile	uint8	fTime1s;
	volatile	uint8	fTime1ms;
}userSysTimesDef;

typedef enum{
	HIRC_24M=0,
	HIRC_22M=1,
	HIRC_16M=2,
	HIRC_8M=3,
	HIRC_4M=4
}SystemHIRCType;
typedef enum{
	LIRC_32K768=0,
	LIRC_38K4=1
}SystemLIRCType;

typedef enum{
	HIRC_SEL=1,
	HXT_SEL=2,
	LIRC_SEL=4,
	LXT_SEL=8
}SystemClkSelectType;

typedef enum{
	EXT_PORTA=1,
	EXT_PORTB=2,
	EXT_PORTC=4,
	EXT_PORTD=8
}ExtIntPortType;

typedef enum{
	PWM_CH1=1,
	PWM_CH2=2,
	PWM_CH3=4,
	PWM_CH4=8
}PwmChlDefType;

typedef enum{
	ADC_CH0=0,
	ADC_CH1=1,
	ADC_CH2=2,
	ADC_CH3=3,
	ADC_CH4=4,
	ADC_CH5=5,
	ADC_CH6=6,
	ADC_CH7=7
}ADCChlDefType;

//typedef struct
//{
//  u16 period;				//PWM频率=系统频率/(vPWMPeriod+1);
//  u8 channels;				//channel可以是	BUZZER_CHANNEL|MOTOR_CHANNEL|DISCHARGE_CHANNEL|CHARGE_CHANNEL以及他们的子集
//  float buzzerDuty;
//  float motorDuty;
//  float chargeDuty;
//  float dischargeDuty;
//} UserPwmInitTypeDef;
typedef struct{
  u16 period;					//PWM频率=系统频率/(vPWMPeriod+1);
  u8 channels;					//channel可以是	BUZZER_CHANNEL|MOTOR_CHANNEL|DISCHARGE_CHANNEL|CHARGE_CHANNEL以及他们的子集
  float dutyArray[5];			//dutyArray[0] 不存储,1~4存储对应的4个PWM channel的duty
} UserPwmInitTypeDef;

typedef struct {
	u8 channels;				//b0~b7分别对应AIN0~AIN7,对应位置1,则表示需要转换该通道
	u16 DaValues[8];
//	u16 DaValueAIN0;			//AIN0 转换结果的存储位置
//	u16 DaValueAIN1;
//	u16 DaValueAIN2;
//	u16 DaValueAIN3;
//	u16 DaValueAIN4;
//	u16 DaValueAIN5;
//	u16 DaValueAIN6;
//	u16 DaValueAIN7;
} UserADCTypeDef;


typedef enum{
	BrightnessAndColorTemp=0x0c,
	BrightnesssOnly=04,
	BrightnessAndRgb=0x34,
	PreinstallEffectOnly=0x07,
	LedOnOff=0x01,
}uartCMDEnum;

typedef	enum{
	FALLING=0,
	RISING=1
}extIntEdge;

typedef enum{
	I2C_COMM_ERROR=-1,
	I2C_COMM_OK=0,
	I2C_OUT_OF_SIZE=1,
}returnStatus;

typedef enum{
	Red_Current=0,
	Green_Current=1,
	Blue_Current=2,
	Cw_Current=3,
	Mw_Current=4,
	Standby_Current=5,
	Sleep_Current=6,
	Press_Any_key1=7,
	Press_Func_key=7,
	Press_Any_key2=8,
	Press_Binc_key=8,
	Press_Bdec_key=8,
	Press_Any_key3=9,
	Press_Dec_key=9,
	Press_Inc_key=9,
	System_Volt_Display=10,
	TempErrorDisplay=11,
	VoltageErrorDisplay=12,
}TestModeStringEnum;

#endif /* USER_INC_CONST_H_ */
