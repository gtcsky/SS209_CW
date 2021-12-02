/*
 * user_lightEffect.c
 *
 *  Created on: 2021年07月23日
 *      Author: Sky Zhang
 */

#include <string.h>
#include <stdlib.h>
#include "user_lightEffect.h"
#include "io_define.h"
#include "user_color.h"
#include "user_lptimer.h"
#include "user_menu.h"
#include "user_key.h"
#include "user_exti.h"
#include "user_adc.h"
#include "user_advtime.h"
#include "user_color.h"
#include "user_ddl.h"
#include "user_utils.h"
//#include "string.h"
u8 	fIsLightEffectOn=0;										//是否开启光效模式
#if(LIGHTEFFECT_INSIDE==1)
uint16	vLEDStepCtrlTimeCnt=0;
LEDParametersStruct	LedStruct;
LEDParametersStruct	*pLedStruct=&LedStruct;
//PcaDataStruct		oriCustomizePwmDataStruct;				//自定义渐变模式起调时,原始值
u8  fIsRisingNow=0;
//SttsArray 参数
//0x00		所有灯灭
//0x01		R亮
//0x02		G亮
//0x04		B亮
//0x08		C亮
//0x10		W亮
#define	MAX_FLASH_MODE1_STYLE_A_INDEX	21
const u8 flashMode1StyleATimerArray[MAX_FLASH_MODE1_STYLE_A_INDEX+1]={ 4, 20, 3, 	20,200,6,		100,150,4,40,2,    	60,100,	4,10,4,8,	4,6,2,200,20};
const u8 flashMode1StyleASttsArray[MAX_FLASH_MODE1_STYLE_A_INDEX+1]=	{  8, 0,   8,    	0,  0,  8,    	0,    0,   8,  0,  8,  	 0,0,		8,0,8,0,	8,0,8,0,0};

#define	MAX_FLASH_MODE1_STYLE_B_INDEX	14
const u8 flashMode1StyleBTimerArray[MAX_FLASH_MODE1_STYLE_B_INDEX+1]={20,20,15,10,   10,6,10,20,   40,20,15,20,    15,200,200};
const u8 flashMode1StyleBSttsArray[MAX_FLASH_MODE1_STYLE_B_INDEX+1]={0x08,0x00,0x08,0x00, 0x08,0x00,0x08,0x00,  0x08,0x00,0x08,0x00,  0x08,0x00,0x00};

#define	MAX_FLASH_MODE1_STYLE_D_INDEX	19
const u8 flashMode1StyleDTimerArray[MAX_FLASH_MODE1_STYLE_D_INDEX+1]={10,39,39,39,39,39,  	99,79,39,79,39,79,		99,39,39,39,39, 39,	 	199,49};
const u8 flashMode1StyleDSttsArray[MAX_FLASH_MODE1_STYLE_D_INDEX+1]={0,0x08,0,0x08,0,0x08,	0,0x08,0,0x08,0,0x08,		0,0x08,0,0x08,0,0x08,	  	0,0};

#define	MAX_GRADUAL_EFFECT2_STYLE_A_INDEX  8
#define	MAX_GRADUAL_EFFECT2_STYLE_D_INDEX  8
const u16 GradualEffectHuesArray[MAX_GRADUAL_EFFECT2_STYLE_D_INDEX+1]={0,240,30,270,60,300,90,345,120};
const u8	GradualEffect1StyleCTimeArray[1]={5};
const u8	GradualEffect2StyleATimeArray[1]={10};
const u8	GradualEffect1StyleETimeArray[1]={1};
const u8	GradualEffect2StyleBTimeArray[1]={2};
const u8	GradualEffect1StyleFTimeArray[1]={5};
const u8	GradualEffect2StyleCTimeArray[1]={5};
const u8	GradualEffect2StyleDTimeArray[1]={2};
u8	customizeFlashTimeArray[1]={0};
u8	customizeGradualTimeArray[1]={1};

//Disco
#define	MAX_GRADUAL_EFFECT2_STYLE_E_INDEX 	12
const uint8 Effect2StyleETimeArray[1] = { 100 };
const uint16 Effect2StyleEHuesArray[MAX_GRADUAL_EFFECT2_STYLE_E_INDEX + 1] = 		{0,0,30,60,90,		120,150,180,210,	240,270,300,330};
const float Effect2StyleESaturationArray[MAX_GRADUAL_EFFECT2_STYLE_E_INDEX + 1]=	{0,1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0};
//Sundown
#define	MAX_GRADUAL_EFFECT2_STYLE_F_INDEX	3
const uint8 Effect2StyleFTimeArray[2] = { 4,5 };
const uint16 Effect2StyleFHuesArray[MAX_GRADUAL_EFFECT2_STYLE_F_INDEX + 1] = 		{0,0,60,60};
const float Effect2StyleFSaturationArray[MAX_GRADUAL_EFFECT2_STYLE_F_INDEX + 1]=	{0,1.0,1.0,0};

#define	MAX_SIMULATION_MODE_STYLE_A_INDEX	19
const u8 simulationModeStyleATimerArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]={20,20,20,20,   20,20,20,20,   20,20,20,20,    20,20,20,20,  20,20,20,20};
const u8 simulationModeStyleASttsArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]={0x01,0x00,0x01,0x00,   0x01,0x00,0x01,0x00,   0x01,0x00,0x04,0x00,    0x04,0x00,0x04,0x00, 0x04,0x00,0x04,0x00};

#define	MAX_SIMULATION_MODE_STYLE_B_INDEX	11
const u8 simulationModeStyleBTimerArray[MAX_SIMULATION_MODE_STYLE_B_INDEX+1]={40,40,40,40,   40,40,40,40,   40,40,40,40};
const u8 simulationModeStyleBSttsArray[MAX_SIMULATION_MODE_STYLE_B_INDEX+1]={0x04,0x00,0x04,0x00,   0x04,0x00,0x08,0x00,   0x08,0x00,0x08,0x00};

#define	MAX_SIMULATION_MODE_STYLE_C_INDEX	11
const u8 simulationModeStyleCTimerArray[MAX_SIMULATION_MODE_STYLE_C_INDEX+1]={40,50,40,50,   40,50,40,50,   40,50,40,50};
const u8 simulationModeStyleCSttsArray[MAX_SIMULATION_MODE_STYLE_C_INDEX+1]={0x01,0x00,0x01,0x00,   0x01,0x00,0x08,0x00,   0x08,0x00,0x08,0x00};

//fireworks    //
#define	MAX_GRADUAL_EFFECT3_STYLE_D_INDEX 	12
const uint8 Effect3StyleDTimeArray[1] = {2 };
const uint16 Effect3StyleDHuesArray[MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1] = 		{0,0,30,60,90,		120,150,180,210,	240,270,300,330};
const float Effect3StyleDSaturationArray[MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1]=	{0,1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0};
const uint8 Effect3StyleDRandomArray[4]={4,5,7,8};

//fireworks 2    //
#define	MAX_GRADUAL_EFFECT3_STYLE_E_INDEX 	12
const uint8 Effect3StyleETimeArray[1] = {1};
const uint16 Effect3StyleEHuesArray[MAX_GRADUAL_EFFECT3_STYLE_E_INDEX + 1] = 		{0,0,30,60,90,		120,150,180,210,	240,270,300,330};
const float Effect3StyleESaturationArray[MAX_GRADUAL_EFFECT3_STYLE_E_INDEX + 1]=	{0,1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0,	1.0,1.0,1.0,1.0};
const uint8 Effect3StyleERandomArray[4] = { 4, 5, 7, 8 };
const uint8 GradualEffect3StyleFTimeArray[1] = { 10 };
uint8 vRandomStepsInc=0;
uint8 vRandomStepsDec=0;
uint8 vRandomIncTimer=0;
uint8 vRandomDecTimer=0;
uint8 vColorCircle=0;
uint8 vColorCircleIndex=0;
uint8 vSpecialColorIndex=0;



#define	PHASE_1_MIN_SETP					((uint8)0X00)
#define	PHASE_1_MAX_SETP					((uint8)0X4F)
#define	PHASE_2_MIN_SETP					((uint8)0X50)
#define	PHASE_2_MAX_SETP					((uint8)0X52)
#define	PHASE_3_MIN_SETP					((uint8)0X60)
#define	PHASE_3_MAX_SETP					((uint8)0xC3)		//(0x60+50*2)					(0x30+25*2)=0x61
#define	PHASE_3_TOTAL_SECTIONS_PER_STEP	35
#define	PHASE_3_RISING_SECTIONS			25
#define	PHASE_3_FALLING_SECTIONS			10
#define	RISING_START_PERCENT				40
#define	FALLING_END_PERCENT				40
#define	EFFECT_TOP_PERCENT				98



typedef struct{
	colorStructType	backupColorStruct;
	colorStructType	tarColorStruct;
	uint8	fIsNotFirstOnLed;
	uint8	fisNotFirstAtThisHues;
	uint8	cnt;
	uint8	fIsPauseHuesInc;
	float	deltaRed;
	float	deltaGreen;
	float	deltaBlue;
}adjustGradualDef;
#define  ADJUST_THRESHOLD					0X0C
#define	ADJUST_STEP						0X03
#define	ADJUST_MAX_CNT					0X0A

adjustGradualDef adjustParams;

#endif
u8	vCandleIndex=0;







#if(LIGHTEFFECT_INSIDE==1)
void  initLEDStyleParams(void){
	DDL_ZERO_STRUCT(LedStruct);
	DDL_ZERO_STRUCT(pcaDataStruct);
}

void setLEDFunc(LEDParametersStruct * ledStruct) {
	UINT8 vtCurLED;
	vLEDStepCtrlTimeCnt = *(ledStruct->timeArray + ledStruct->currentIndex);
	vtCurLED = *(ledStruct->sttsArray + ledStruct->currentIndex);
	if (!vtCurLED) {
//		memcpy(pBackupDataStruct,&pcaDataStruct,sizeof(pcaDataStruct));		//backup info
		if (pcaDataStruct.valueOfRed)
			pBackupDataStruct->valueOfRed = pcaDataStruct.valueOfRed;
		if (pcaDataStruct.valueOfGreen)
			pBackupDataStruct->valueOfGreen = pcaDataStruct.valueOfGreen;
		if (pcaDataStruct.valueOfBlue)
			pBackupDataStruct->valueOfBlue = pcaDataStruct.valueOfBlue;
		if (pcaDataStruct.valueOfCw)
			pBackupDataStruct->valueOfCw = pcaDataStruct.valueOfCw;
		if (pcaDataStruct.valueOfMw)
			pBackupDataStruct->valueOfMw = pcaDataStruct.valueOfMw;
		turnOffColorTempLamp();
		turnOffRGBLamp();
	} else {
		pBackupDataStruct->valueOfRed = 0;
		pBackupDataStruct->valueOfGreen = 0;
		pBackupDataStruct->valueOfBlue = 0;
		pBackupDataStruct->valueOfCw = 0;
		pBackupDataStruct->valueOfMw = 0;
		if (vtCurLED & 0x01) {
			if (displayParams.brightness)
				setRedData(PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100));
			else
				setRedData(0);
		}
		if (vtCurLED & 0x02) {
			if (displayParams.brightness)
				setGreenData(PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100));
			else
				setGreenData(0);
		}
		if (vtCurLED & 0x04) {
			if (displayParams.brightness)
				setBlueData(PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100));
			else
				setBlueData(0);
		}
		if (vtCurLED & 0x08) {
			if (displayParams.brightness)
				setCoolData(PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100));		//从4%起步
			else
				setCoolData(0);
		}
		if (vtCurLED & 0x10) {
			if (displayParams.brightness)
				setWarmData(PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100));		//从4%起步
			else
				setWarmData(0);
		}
	}
}

void	loadLedStatus(void){
	setRedData(pBackupDataStruct->valueOfRed);
	setGreenData(pBackupDataStruct->valueOfGreen);
	setBlueData(pBackupDataStruct->valueOfBlue);
	setCoolData(pBackupDataStruct->valueOfCw);
	setWarmData(pBackupDataStruct->valueOfMw);
}


void lightEffectFunc(void) {
	if (fIsLightEffectOn) {
		if (!LedStruct.fIsGradualMode) {
//			if(CustomizeEffect==displayParams.arrowIndex){					//自定义模式
//				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
//					if (LedStruct.currentIndex >= LedStruct.indicatorMaxIndex) {
//						LedStruct.currentIndex = 0;
//					}else {
//						LedStruct.currentIndex++;
//					}
//					LedStruct.pfnLedFuncCallBack();
//				}else {
//					//if (!LedStruct.fIsGradualMode)
//					vLEDStepCtrlTimeCnt--;
//				}
//			} else {										//预设模式
				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
					if (LedStruct.currentIndex >= LedStruct.indicatorMaxIndex) {
						LedStruct.currentIndex = 0;
					} else {
						LedStruct.currentIndex++;
					}
					if (!LedStruct.fIsGradualMode)
						setLEDFunc(&LedStruct);
				} else {
					if (!LedStruct.fIsGradualMode)
						vLEDStepCtrlTimeCnt--;
				}
//			}
		} else {			//渐变模式
//			if (CustomizeEffect == displayParams.arrowIndex) {		//自定义模式
//				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
//					if (LedStruct.currentIndex >= LedStruct.indicatorMaxIndex) {
//						LedStruct.currentIndex = 0;
//						if(LedStruct.customizeLoopTimes<INFINITE_LOOP_TIMES){
//							if(++LedStruct.customizeLoopIndex>=LedStruct.customizeLoopTimes){
//								LedStruct.pfncustomizeEffectOverCallBack();
//								return;											//预设循环次数完成
//							}
//						}
//					}else {
//						LedStruct.currentIndex++;
//					}
//					LedStruct.pfnLedFuncCallBack();
//				}else {
//					vLEDStepCtrlTimeCnt--;
//				}
//			} else {											//预设模式
				if (vLEDStepCtrlTimeCnt == 0) {
					LedStruct.pfnLedFuncCallBack();
				} else {
					vLEDStepCtrlTimeCnt--;
				}
//			}
		}
	}
}
#endif
void turnOffAllLightEffect(void) {
	fIsLightEffectOn = 0;
	vCandleIndex=0;
	turnOffColorTempLamp();
#if(RGB_INSIDE==1)
	displayParams.fIsEffectMode=FALSE;
	turnOffRGBLamp();
#endif
}
#if(LIGHTEFFECT_INSIDE==1)
/*********************************************************************
 *
 * 	闪烁模式
 *
 **********************************************************************/
void setFlashEffectPub(LEDParametersStruct * ledStruct){
	fIsLightEffectOn = 1;
	ledStruct->currentIndex=0;
	ledStruct->fIsGradualMode=0;
	displayParams.fIsEffectMode=TRUE;
	setLEDFunc(ledStruct);
}

/*********************************************************************
 *
 * 	渐变模式
 *
 **********************************************************************/
void setGradualEffectPub(LEDParametersStruct * ledStruct){
	fIsLightEffectOn =1;
	vCandleIndex=0;
	displayParams.fIsEffectMode=TRUE;
	ledStruct->fIsGradualMode=1;
	ledStruct->sttsArray=NULL;		//不使用
	ledStruct->pfnLedFuncCallBack();
}

void	 set2FlashMode1StyleA(void){
	LedStruct.indicatorMaxIndex=MAX_FLASH_MODE1_STYLE_A_INDEX;
	LedStruct.indicatorStyle=1;					//flashMode1  styleA
	LedStruct.sttsArray=flashMode1StyleASttsArray;
	LedStruct.timeArray=flashMode1StyleATimerArray;
	setFlashEffectPub(&LedStruct);
}
void	 set2FlashMode1StyleB(void){
	LedStruct.indicatorMaxIndex=MAX_FLASH_MODE1_STYLE_B_INDEX;
	LedStruct.indicatorStyle=2;					//flashMode1  styleB
	LedStruct.sttsArray=flashMode1StyleBSttsArray;
	LedStruct.timeArray=flashMode1StyleBTimerArray;
	setFlashEffectPub(&LedStruct);
}
void	set2FlashMode1StyleC(void){
	LedStruct.indicatorStyle=3;					//flashMode1  styleC
	LedStruct.pfnLedFuncCallBack=flashMode1StyleCfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=90;
	LedStruct.timeArray=GradualEffect1StyleCTimeArray;
	fIsRisingNow=1;
	setGradualEffectPub(&LedStruct);
}

//SOS Mode
void	set2FlashMode1StyleD(void){
	LedStruct.indicatorMaxIndex=MAX_FLASH_MODE1_STYLE_D_INDEX;
	LedStruct.indicatorStyle=4;					//flashMode1  styleD
	LedStruct.sttsArray=flashMode1StyleDSttsArray;
	LedStruct.timeArray=flashMode1StyleDTimerArray;
	setFlashEffectPub(&LedStruct);
}

//Faulty Lamp
void	set2FlashMode1StyleE(void){
	LedStruct.indicatorStyle=5;					//flashMode1  styleE
	LedStruct.pfnLedFuncCallBack=flashMode1StyleEfunc;
	LedStruct.indicatorMaxIndex=90;
	LedStruct.currentIndex=LedStruct.indicatorMaxIndex-10;
	LedStruct.timeArray=GradualEffect1StyleETimeArray;
	fIsRisingNow=1;
	setGradualEffectPub(&LedStruct);
}
//Candle 烛光
void	set2FlashMode1StyleF(void){
	LedStruct.pfnLedFuncCallBack=flashMode1StyleFfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=90;
	LedStruct.timeArray=GradualEffect1StyleFTimeArray;
	setGradualEffectPub(&LedStruct);
}
/****************************************************************************
 *
 * 灯光先降后升效果
 *
 *
 *****************************************************************************/
void FallingAndRising(u8* vtIndex, u8 lowThreshold, u8 hiThreshold, u16 fallingStep5ms, u16 risingStep5ms) {
	if (!fIsRisingNow) {
		if (NULL != fallingStep5ms) {
			vLEDStepCtrlTimeCnt = fallingStep5ms;
		}
		if ((!LedStruct.currentIndex)||(--LedStruct.currentIndex <= lowThreshold)) {
			fIsRisingNow = TRUE;
		}
	} else {
		if (NULL != risingStep5ms) {
			vLEDStepCtrlTimeCnt = risingStep5ms;
		}
		if (++LedStruct.currentIndex >= hiThreshold) {
			fIsRisingNow = FALSE;
			*vtIndex += 1;
		}
	}
}
/****************************************************************************
 *
 * 灯光闪烁效果
 *
 *Note: 确保首次执行函数时fIsRisingNow=FALSE. 否则只有HiThreshold
 *		的结果会被执行.
 *****************************************************************************/

void stepByStep(u8* vtIndex, u8 lowThreshold, u8 hiThreshold, u8 lowTime5ms, u8 hiTimes5ms) {
	if (!fIsRisingNow) {
		LedStruct.currentIndex = lowThreshold;
		vLEDStepCtrlTimeCnt = lowTime5ms;
		fIsRisingNow = 1;
	} else {
		LedStruct.currentIndex = hiThreshold;
		vLEDStepCtrlTimeCnt = hiTimes5ms;
		fIsRisingNow = 0;
		*vtIndex+=1;
	}
}
/****************************************************************************
 *
 * 灯光保持效果
 *
 *****************************************************************************/
void keepStep(u8* vtIndex, u8 vtStep,u16 stepTime5ms){
	LedStruct.currentIndex = vtStep;
	vLEDStepCtrlTimeCnt = stepTime5ms;
	fIsRisingNow = FALSE;
	*vtIndex+=1;
}

/*********************************************************************************
 *
 * 电视灯光效果
 *
*********************************************************************************/
void	 flashMode1StyleCProess(void){
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if(!vCandleIndex){
		if(fIsRisingNow){
			if (++LedStruct.currentIndex >=(LedStruct.indicatorMaxIndex-2)){
						fIsRisingNow=0;
						vCandleIndex=1;
			}
		}else{
			if(LedStruct.currentIndex ==0)
				fIsRisingNow=1;
			else
				LedStruct.currentIndex--;
		}
	}
	else if(vCandleIndex==1){
			FallingAndRising(&vCandleIndex,(LedStruct.indicatorMaxIndex/2),(LedStruct.indicatorMaxIndex-10),4,1);
	}
	else if(vCandleIndex==2){
			FallingAndRising(&vCandleIndex,10,(LedStruct.indicatorMaxIndex-10),5,2);
	}
	else if(vCandleIndex==3){
			FallingAndRising(&vCandleIndex,0,50,2,1);
	}
	else if(vCandleIndex==4){
			stepByStep(&vCandleIndex, 50, 8, 2, 10);
	}
	else if(vCandleIndex==5){
		FallingAndRising(&vCandleIndex,55,(LedStruct.indicatorMaxIndex-5),3,5);
	}
	else if(vCandleIndex==6){
		if(!fIsRisingNow){
			if (--LedStruct.currentIndex <=1){
					fIsRisingNow=1;
					vCandleIndex=0;
			}
		}
	}
}
/*********************************************************************************
 *
 *	坏灯泡效果
 *
*********************************************************************************/
void flashMode1StyleEProess(void) {
	uint8 temp=randomUint8(6);
	uint8 tempR20=randomUint8(20);
	uint8 tempR30=randomUint8(30);
	uint8 tempR40=randomUint8(40);
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (!vCandleIndex) {
		if (fIsRisingNow) {
			if (++LedStruct.currentIndex >= (LedStruct.indicatorMaxIndex - 2)) {
				fIsRisingNow = 0;
				vCandleIndex = 1;
			}
		} else {
			if (LedStruct.currentIndex == 0)
				fIsRisingNow = 1;
			else
				LedStruct.currentIndex--;
		}
	} else if (vCandleIndex == 1) {
		keepStep(&vCandleIndex, LedStruct.indicatorMaxIndex - 2, 500);
	} else if (vCandleIndex == 2) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 170+tempR30, temp);
	} else if (vCandleIndex == 3) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -10, 30+tempR30, temp);
	} else if (vCandleIndex == 4) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -20, 15+tempR20, temp);
	} else if (vCandleIndex == 5) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -40, 15+tempR20, temp);
	} else if (vCandleIndex == 6) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -20, 140+tempR20, temp);
	} else if (vCandleIndex == 7) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 20+tempR20, temp);
	} else if (vCandleIndex == 8) {
		FallingAndRising(&vCandleIndex,LedStruct.indicatorMaxIndex -12,LedStruct.indicatorMaxIndex -2,0,2);
	} else if (vCandleIndex == 9) {
		keepStep(&vCandleIndex, LedStruct.indicatorMaxIndex - 2, 1000);
	} else if (vCandleIndex == 10) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 180+tempR40, temp);
	} else if (vCandleIndex == 11) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -10, 20+tempR30, tempR20);
	} else if (vCandleIndex == 12) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -20, 30+tempR40, tempR20);
	} else if (vCandleIndex == 13) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -40, 70+tempR40, 3);			//
	} else if (vCandleIndex == 14) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -20, 130+tempR30, temp);
	} else if (vCandleIndex == 15) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 10, 10);
	} else if (vCandleIndex == 16) {
		keepStep(&vCandleIndex, LedStruct.indicatorMaxIndex - 2, 1000);
	} else if (vCandleIndex == 17) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -20, 10, 100);
	} else if (vCandleIndex == 18) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, temp, 50);
	} else if (vCandleIndex == 19) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, temp, 50);
	} else if (vCandleIndex == 20) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 250, 4);
	} else if (vCandleIndex == 21) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 50, 6);
	} else if (vCandleIndex == 22) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, temp, 5);
	} else if (vCandleIndex == 23) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, temp, 6);
	} else if (vCandleIndex == 24) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, temp, 250);
	} else if (vCandleIndex == 25) {
		keepStep(&vCandleIndex, LedStruct.indicatorMaxIndex - 2, 400);
	} else if (vCandleIndex == 26) {
		keepStep(&vCandleIndex, 0, 600);
	} else if (vCandleIndex == 27) {
		stepByStep(&vCandleIndex, 0, LedStruct.indicatorMaxIndex -2, 50, 5);
	} else if (vCandleIndex == 28) {
		if(!fIsRisingNow){
//			if (--LedStruct.currentIndex <=1){
			if (--LedStruct.currentIndex <=(LedStruct.indicatorMaxIndex - 5)){
					fIsRisingNow=1;
					vCandleIndex=0;
			}
		}
	}
}
/*********************************************************************************
 *
 * 	烛光效果
 *
*********************************************************************************/
void	 flashMode1StyleFProess(void){
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if(!vCandleIndex){
		if(fIsRisingNow){
			if (++LedStruct.currentIndex >=(LedStruct.indicatorMaxIndex-10)){
				fIsRisingNow=0;
				vCandleIndex=1;
			}
		}else{
			if(LedStruct.currentIndex ==0)
				fIsRisingNow=1;
			else
				LedStruct.currentIndex--;
		}
	}else if(vCandleIndex==1){
		FallingAndRising(&vCandleIndex,5,70,10,10);
		//vCandleIndex++;
	}else if(vCandleIndex==2){
		FallingAndRising(&vCandleIndex,5,40,1,1);
		//vCandleIndex++;
	}else if(vCandleIndex==3){
		keepStep(&vCandleIndex,38,200);
		//vCandleIndex++;
	}else if(vCandleIndex==4){
		FallingAndRising(&vCandleIndex,40,70,5,5);
		//vCandleIndex++;
	}else if(vCandleIndex==5){
		stepByStep(&vCandleIndex, 60,45, 100, 80);
		//vCandleIndex++;
	}else if(vCandleIndex==6){
		FallingAndRising(&vCandleIndex,2,20,5,10);
		//vCandleIndex++;
	}else if(vCandleIndex==7){
		keepStep(&vCandleIndex,0,2);
		//vCandleIndex++;
	}else if(vCandleIndex==8){
		FallingAndRising(&vCandleIndex,2,15,1,9);
		//vCandleIndex++;
	}else if(vCandleIndex==9){
		keepStep(&vCandleIndex,0,2);
		//vCandleIndex++;
	}else if(vCandleIndex==10){
		FallingAndRising(&vCandleIndex,2,20,1,10);
		//vCandleIndex++;
	}else if(vCandleIndex==11){
		FallingAndRising(&vCandleIndex,20,40,1,10);
		//vCandleIndex++;
	}else if(vCandleIndex==12){
		FallingAndRising(&vCandleIndex,40,60,1,5);
		//vCandleIndex++;
	}else if(vCandleIndex==13){
		keepStep(&vCandleIndex,50,600);
		//vCandleIndex++;
	}else if(vCandleIndex==14){
		FallingAndRising(&vCandleIndex,20,21,10,1);
		//vCandleIndex++;
	}else if(vCandleIndex==15){
		keepStep(&vCandleIndex,0,1);
		//vCandleIndex++;
	}else if(vCandleIndex==16){
		FallingAndRising(&vCandleIndex,20,50,1,10);
		//vCandleIndex++;
	}else if(vCandleIndex==17){
		FallingAndRising(&vCandleIndex,35,55,2,10);
		//vCandleIndex++;
	}else if(vCandleIndex==18){
		FallingAndRising(&vCandleIndex,2,85,5,5);
		//vCandleIndex++;
	}else if(vCandleIndex==19){
		FallingAndRising(&vCandleIndex,10,80,2,NULL);
		//vCandleIndex++;
	}else if(vCandleIndex==20){
		stepByStep(&vCandleIndex, 60, 45, 5, 100);
		//vCandleIndex++;
	}else if(vCandleIndex==21){
		FallingAndRising(&vCandleIndex, (LedStruct.indicatorMaxIndex/3), (LedStruct.indicatorMaxIndex-10),10,NULL);
		//vCandleIndex++;
	}else if(vCandleIndex==22){
		stepByStep(&vCandleIndex, 75, 45, 5, 100);
		//vCandleIndex++;
	}else if(vCandleIndex==23){
		FallingAndRising(&vCandleIndex,2,20,5,10);
		//vCandleIndex++;
	}else if(vCandleIndex==24){
		keepStep(&vCandleIndex,0,1);
		//vCandleIndex++;
	}else if(vCandleIndex==25){
		FallingAndRising(&vCandleIndex,2,25,1,10);
		//vCandleIndex++;
	}else if(vCandleIndex==26){
		FallingAndRising(&vCandleIndex,10,20,10,15);
	}else if(vCandleIndex==27){
		keepStep(&vCandleIndex,0,0);
	}else if(vCandleIndex==28){
		keepStep(&vCandleIndex,20,40);
	}else if(vCandleIndex==28){
		keepStep(&vCandleIndex,0,0);
	}else if(vCandleIndex==29){
		keepStep(&vCandleIndex,20,40);
	}else if(vCandleIndex==30){
		vCandleIndex++;
		//keepStep(&vCandleIndex,0,0);
	}else if(vCandleIndex==31){
		vCandleIndex++;
		//keepStep(&vCandleIndex,20,40);
	}else if(vCandleIndex==32){
		FallingAndRising(&vCandleIndex,10,60,1,10);
	}else if(vCandleIndex==33){
		if(!fIsRisingNow){
			if (--LedStruct.currentIndex <=1){
					fIsRisingNow=1;
					vCandleIndex=0;
			}
		}
	}
}


//void	 flashMode1StyleCProess(void){
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
//	if (fIsRisingNow) {
//		if (++LedStruct.currentIndex >=LedStruct.indicatorMaxIndex)
//			fIsRisingNow=0;
//	}else{
//		if(LedStruct.currentIndex ==0)
//			fIsRisingNow=1;
//		else
//			LedStruct.currentIndex--;
//	}
//}
/******************************************************************************
 *
 * 	光效模式1-C  渐变效果实现
 *
 *******************************************************************************/
void flashMode1StyleCfunc(void) {
	flashMode1StyleCProess();
	if( displayParams.brightness)
		setCoolData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
	else
		setCoolData(0);
}

/******************************************************************************
 *
 * 	光效模式1-E  坏灯泡效果实现
 *
 *******************************************************************************/
void flashMode1StyleEfunc(void) {
	flashMode1StyleEProess();
	if(LedStruct.currentIndex&&displayParams.brightness)
		setWarmData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
	else
		setWarmData(0);
}
///******************************************************************************
// *
// * 	光效模式1-F  烟花效果实现
// *
// *******************************************************************************/
//void flashMode1StyleFfunc(void) {
//	if (++pUserColor->hues >= 360){
//		pUserColor->hues = 0;
//	}
//	pUserColor->saturation=1.0;
//	pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
//	hsb2Rgb(pUserColor);
//	updateColor(pUserColor);
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
//}
/******************************************************************************
 *
 * 	光效模式2-A  渐变效果实现
 *
 *******************************************************************************/
void	flashMode2StyleAfunc(void){
	if (++pUserColor->hues >= 360){
		pUserColor->hues = 0;
	}
	pUserColor->saturation=1.0;
	if(displayParams.brightness)
		pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
	else
		pUserColor->brightness=0;
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}
/******************************************************************************
 *
 * 	光效模式2-B  渐变效果实现
 *
 *******************************************************************************/
void	flashMode2StyleBfunc(void){
	u8 vtTemp1=0;
	float vtBrightnessCoe=0;
	if(fIsRisingNow){
		if (++pUserColor->hues >= 360){
		//pUserColor->hues = 0;
			fIsRisingNow=FALSE;
		}
	}
	if(!fIsRisingNow){
		if(--pUserColor->hues<=1){
			fIsRisingNow=TRUE;
		}
	}
	pUserColor->saturation=1.0;
	if(displayParams.brightness)
		pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
	else
		pUserColor->brightness=0;
	vtTemp1=pUserColor->hues%60;
	if(vtTemp1>=30){
		vtBrightnessCoe=(float)(10+(vtTemp1-30)*3)/100;
	}
	else{
		vtBrightnessCoe=(float)(100-vtTemp1*3)/100;
	}
	pUserColor->brightness*=vtBrightnessCoe;
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}
/******************************************************************************
 *
 * 	光效模式1-F  烛光效果实现
 *
 *******************************************************************************/
void	flashMode1StyleFfunc(void){
	flashMode1StyleFProess();
	if(displayParams.brightness)
		setWarmData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
	else
		setWarmData(0);
}
/******************************************************************************
 *
 * 	光效模式2-C  篝火效果实现
 *
 *******************************************************************************/
void	flashMode2StyleCfunc(void){
	flashMode1StyleFProess();
	if(displayParams.brightness)
		setRedData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
	else
		setRedData(0);
}
/******************************************************************************
 *
 * 	光效模式2-D  Party效果实现
 *
 *******************************************************************************/
void flashMode2StyleDfunc(void) {
		u8 vtTemp1=0;
		float vtBrightnessCoe=0;
		if (++LedStruct.currentIndex > LedStruct.indicatorMaxIndex){
			LedStruct.currentIndex=0;
		}
		if(LedStruct.currentIndex==LedStruct.indicatorMaxIndex/2){
			if(++LedStruct.IndexOfHues>LedStruct.MaxHuesIndex)
				LedStruct.IndexOfHues=0;
		}
		pUserColor->hues=LedStruct.HuesArray[LedStruct.IndexOfHues];
		pUserColor->saturation=1.0;
		if(displayParams.brightness)
			pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
		else
			pUserColor->brightness=0;
		vtTemp1=LedStruct.currentIndex;
		if(LedStruct.currentIndex<=(LedStruct.indicatorMaxIndex/2)){
			vtBrightnessCoe=(float)(100-vtTemp1*3)/100;
		}else{
			vtBrightnessCoe=(float)(10+(vtTemp1-30)*3)/100;
		}
		pUserColor->brightness*=vtBrightnessCoe;
		hsb2Rgb(pUserColor);
		updateColor(pUserColor);
		vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}
/******************************************************************************
 *
 * 	光效模式2-E  Disco效果实现
 *
 *******************************************************************************/
void flashMode2StyleEfunc(void) {
	uint8 temp=0;
	if (++LedStruct.currentIndex > LedStruct.indicatorMaxIndex) {
		LedStruct.currentIndex = 0;
	}
	if (LedStruct.currentIndex % 2 == 1) {
		do{
			temp=randomUint8(MAX_GRADUAL_EFFECT3_STYLE_D_INDEX+1);
		}while(temp==LedStruct.LastRandomData);
		LedStruct.LastRandomData=temp;
		temp-=1;
		pUserColor->hues = LedStruct.HuesArray[temp];
		pUserColor->saturation = LedStruct.SaturationArray[temp];
//		pUserColor->hues = LedStruct.HuesArray[LedStruct.IndexOfHues];
//		pUserColor->saturation = LedStruct.SaturationArray[LedStruct.IndexOfHues];
//		if (++LedStruct.IndexOfHues > LedStruct.MaxHuesIndex){
//			LedStruct.IndexOfHues = 0;
//		}
		if(displayParams.brightness)
			pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
		else
			pUserColor->brightness=0;
		hsb2Rgb(pUserColor);
		updateColor(pUserColor);
	}else{
		pUserColor->brightness=0;
		turnOffRGBLamp();
	}
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}
/******************************************************************************
 *
 * 	光效模式2-F  日落效果实现
 *
 *******************************************************************************/
void	flashMode2StyleFfunc(void){
//	u8 vtTemp1=0;
	switch (LedStruct.currentIndex) {
	case 0:
		pUserColor->hues=LedStruct.HuesArray[0];
		pUserColor->saturation+=0.01;
		if(pUserColor->saturation>=1.0){
			pUserColor->saturation=1.0;
			LedStruct.currentIndex++;
		}
		vLEDStepCtrlTimeCnt = LedStruct.timeArray[0];
		break;
	case 1:
		pUserColor->saturation=LedStruct.SaturationArray[1];
		if(++pUserColor->hues>=LedStruct.HuesArray[2]){
			pUserColor->hues=LedStruct.HuesArray[2];
			LedStruct.currentIndex++;
		}
		vLEDStepCtrlTimeCnt = LedStruct.timeArray[1];
		break;
	case 2:
		pUserColor->hues=LedStruct.HuesArray[2];
		pUserColor->saturation-=0.01;
		if(pUserColor->saturation<=0.01){
			pUserColor->saturation=0.01;
			LedStruct.currentIndex++;
		}
		vLEDStepCtrlTimeCnt = LedStruct.timeArray[1];
		break;
	case 3:
		pUserColor->saturation=LedStruct.SaturationArray[0];
		if(pUserColor->hues&&!--pUserColor->hues){
			pUserColor->hues=0;
			LedStruct.currentIndex=0;
		}
		vLEDStepCtrlTimeCnt = LedStruct.timeArray[0];
		break;
	}
	if(displayParams.brightness)
		pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
	else
		pUserColor->brightness=0;
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}


/******************************************************************************
 *
 * 	光效模式3-D  fireworks 烟花效果实现
 *
 *    Phase 1:PHASE_1_MIN_SETP~PHASE_1_MAX_SETP
 *    Phase 2:PHASE_2_MIN_SETP~PHASE_2_MAX_SETP
 *    Phase 3:PHASE_3_MIN_SETP~PHASE_3_MAX_SETP
 *******************************************************************************/
void flashMode3StyleDfunc(void) {
	uint8 temp = 0;
	uint8 temp2 = 0;
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (vCandleIndex <= PHASE_1_MAX_SETP) {
		temp = randomUint8(4) + 2;
		temp2 = randomUint8(6) + 3;
		stepByStep(&vCandleIndex, 0, 98, temp, temp2);
		if (vCandleIndex >= PHASE_1_MAX_SETP)
			vCandleIndex = PHASE_2_MIN_SETP;
		pUserColor->hues = 0;
		pUserColor->saturation = 0.5;
		if (LedStruct.currentIndex&&displayParams.brightness) {
//			pUserColor->brightness = ((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100*5) + (float) LED_REAL_START_DUTY / 100;
			pUserColor->brightness = (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100 * 5) + (float) LED_REAL_START_DUTY / 100)
					* ((float) (LedStruct.currentIndex + 10) / 100);
		} else {
			pUserColor->brightness = 0;
		}
	} else if (vCandleIndex <= PHASE_2_MAX_SETP) {
		vLEDStepCtrlTimeCnt = 3;
		switch (vCandleIndex) {
		case PHASE_2_MIN_SETP:
			pUserColor->hues = 0;
			pUserColor->saturation -= 0.01;
			if (pUserColor->saturation <= 0.01) {
				pUserColor->saturation = 0.01;
				vCandleIndex++;
			}
			break;
		case (PHASE_2_MIN_SETP + 1):
			if (++pUserColor->hues >= 240) {
				pUserColor->hues = 240;
				vCandleIndex = PHASE_3_MIN_SETP;
			} else if (pUserColor->hues % 5 == 0) {
				pUserColor->saturation += 0.01;
				if (pUserColor->saturation >= 0.5)
					pUserColor->saturation = 0.5;
			}
			break;
		}
		if(displayParams.brightness)
			pUserColor->brightness = ((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100;
		else
			pUserColor->brightness=0;
	} else if (vCandleIndex <= PHASE_3_MAX_SETP) {

		if(vCandleIndex==PHASE_3_MIN_SETP){
			do {
				temp = randomUint8(MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1);
			} while (temp == LedStruct.vSeed);
			LedStruct.vSeed = temp;
			temp -= 1;
			pUserColor->hues = LedStruct.HuesArray[temp];
			pUserColor->saturation = LedStruct.SaturationArray[temp];
			temp2 = randomUint8(4) - 1;
			LedStruct.LastRandomData = Effect3StyleDRandomArray[temp2];
			vCandleIndex++;
			vLEDStepCtrlTimeCnt = 600;
			if(displayParams.brightness)
				pUserColor->brightness = ((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100;
			else
				pUserColor->brightness=0;
		}else if(vCandleIndex%2==0){
			temp = (LedStruct.vSeed + LedStruct.LastRandomData * ((vCandleIndex - PHASE_3_MIN_SETP) / 2 - 1)) % (MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1);
			pUserColor->hues = LedStruct.HuesArray[temp];
			pUserColor->saturation = LedStruct.SaturationArray[temp];
			if(displayParams.brightness)
				pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
			else
				pUserColor->brightness=0;
			vLEDStepCtrlTimeCnt = 1;
			if (LedStruct.IndexOfHues++ >= PHASE_3_TOTAL_SECTIONS_PER_STEP) {								//LedStruct.IndexOfHues 在Phase3用于记录亮度渐变
				vCandleIndex++;
			} else {
				if (LedStruct.IndexOfHues <= PHASE_3_RISING_SECTIONS) {
					pUserColor->brightness *= (LedStruct.IndexOfHues * 0.02 + (90-2*PHASE_3_RISING_SECTIONS)*0.01);
				} else {
					pUserColor->brightness *= (0.9 - (LedStruct.IndexOfHues-PHASE_3_RISING_SECTIONS) * 0.03);
				}
			}
		}else{
			LedStruct.IndexOfHues=0;
			if (vCandleIndex > (PHASE_3_MIN_SETP + 1)) {
				vLEDStepCtrlTimeCnt = randomUint8(60) - 1;
				vLEDStepCtrlTimeCnt += 20;
			} else
				vLEDStepCtrlTimeCnt = 400;
			pUserColor->brightness = 0;
			vCandleIndex++;
			if (vCandleIndex >= PHASE_3_MAX_SETP) {
				hsb2Rgb(pUserColor);
				updateColor(pUserColor);
				vCandleIndex = 0;
				return;
			}
		}
	}
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
}
/******************************************************************************
 *
 * 	光效模式3-E  fireworks2  烟花2效果实现
 *
 *******************************************************************************/
void flashMode3StyleEfunc(void) {
	uint8 temp = 0;
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (!LedStruct.IndexOfHues) {
		if (!vColorCircleIndex) {															//更换大周期颜色
			temp = randomUint8(MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1);
			LedStruct.LastRandomData = temp;
			vColorCircle = 3 + randomUint8(7);
			vSpecialColorIndex = 3 + randomUint8(vColorCircle - 4);
			pUserColor->hues = LedStruct.HuesArray[temp];
			pUserColor->saturation = LedStruct.SaturationArray[temp];
			vColorCircleIndex++;
		} else {
			if (++vColorCircleIndex == vSpecialColorIndex) {								//插入一次不同的颜色
				do {
					temp = randomUint8(MAX_GRADUAL_EFFECT3_STYLE_D_INDEX + 1);
				} while (temp == LedStruct.LastRandomData);
				pUserColor->hues = LedStruct.HuesArray[temp];
				pUserColor->saturation = LedStruct.SaturationArray[temp];
			}else if(vColorCircleIndex>vColorCircle){										//大周期结束,下次更换大周期颜色
				vColorCircleIndex=0;
			}else{
				pUserColor->hues = LedStruct.HuesArray[LedStruct.LastRandomData];		//特殊的一次异色插入后,下一次返回原来的大周期颜色
				pUserColor->saturation = LedStruct.SaturationArray[LedStruct.LastRandomData];
			}
		}
		vRandomStepsInc = 30 + randomUint8(100);			//设定单周期中,上升的总步数
		vRandomStepsDec = 10 + randomUint8(30);
		vRandomIncTimer = randomUint8(5);				//设定上升步进时间
		vRandomDecTimer = randomUint8(3);
	}
	if (displayParams.brightness)
		pUserColor->brightness = ((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100;
	else
		pUserColor->brightness = 0;
	if (LedStruct.IndexOfHues++ >= (vRandomStepsInc + vRandomStepsDec)) {					//LedStruct.IndexOfHues 在Phase3用于记录亮度渐变.
		LedStruct.IndexOfHues = 0;
		return;
	} else {
		if (LedStruct.IndexOfHues <= vRandomStepsInc) {
			vLEDStepCtrlTimeCnt = vRandomIncTimer;
			pUserColor->brightness *= (RISING_START_PERCENT * 0.01 + LedStruct.IndexOfHues * ((EFFECT_TOP_PERCENT - RISING_START_PERCENT) * 0.01 / vRandomStepsInc));
		} else {
			vLEDStepCtrlTimeCnt = vRandomDecTimer;
			pUserColor->brightness *= (EFFECT_TOP_PERCENT * 0.01 - (LedStruct.IndexOfHues - vRandomStepsInc) * (90 - FALLING_END_PERCENT) * 0.01 / vRandomStepsDec);		//后10递减.
		}
	}
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
}
/***********************************************************************************************************
  *  @brief 					特定颜色缓慢渐变
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void colorAdjustGradual(colorStructType * colorStruct) {
	if (adjustParams.fIsPauseHuesInc) {
		if (++adjustParams.cnt >= ADJUST_MAX_CNT) {
			adjustParams.fIsPauseHuesInc = 0;
			adjustParams.fisNotFirstAtThisHues = 0;
			memcpy(colorStruct, &adjustParams.tarColorStruct, sizeof(colorStructType));
			adjustParams.cnt = 0;
		} else {
			colorStruct->red=adjustParams.backupColorStruct.red+adjustParams.deltaRed*adjustParams.cnt+0.5;		//结果四舍五入
			colorStruct->green=adjustParams.backupColorStruct.green+adjustParams.deltaGreen*adjustParams.cnt+0.5;
			colorStruct->blue=adjustParams.backupColorStruct.blue+adjustParams.deltaBlue*adjustParams.cnt+0.5;

			vLEDStepCtrlTimeCnt /= ADJUST_MAX_CNT;
			if (!vLEDStepCtrlTimeCnt)
				vLEDStepCtrlTimeCnt = 1;
		}
	} else {
		if ((colorStruct->hues <= 15) || (colorStruct->hues >= 345) || ((colorStruct->hues >= 105) && (colorStruct->hues <= 135))
				|| ((colorStruct->hues >= 225) && (colorStruct->hues <= 255))) {
				adjustParams.fIsPauseHuesInc = TRUE;
				adjustParams.fisNotFirstAtThisHues = 1;
				adjustParams.deltaRed = (colorStruct->red - adjustParams.backupColorStruct.red) * 1.0 / ADJUST_MAX_CNT;
				adjustParams.deltaGreen = (colorStruct->green - adjustParams.backupColorStruct.green) * 1.0 / ADJUST_MAX_CNT;
				adjustParams.deltaBlue = (colorStruct->blue - adjustParams.backupColorStruct.blue) * 1.0 / ADJUST_MAX_CNT;
				memcpy(&adjustParams.tarColorStruct, colorStruct, sizeof(colorStructType));
				memcpy(colorStruct, &adjustParams.backupColorStruct, sizeof(colorStructType));
				adjustParams.cnt = 0;
//			}
		}
	}
}


/******************************************************************************
 *
 * 	光效模式3-F  渐变效果实现
 *
 *******************************************************************************/
void	flashMode3StyleFfunc(void){
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (!adjustParams.fIsPauseHuesInc) {
		if (++pUserColor->hues >= 360) {
			pUserColor->hues = 0;
		}
		pUserColor->saturation = 1.0;
		if(displayParams.brightness)
			pUserColor->brightness = ((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100;
		else
			pUserColor->brightness=0;
		if (!adjustParams.fIsNotFirstOnLed) {
			adjustParams.fIsNotFirstOnLed = 1;
		} else {
			memcpy(&adjustParams.backupColorStruct, pUserColor, sizeof(colorStructType));
		}
		hsb2Rgb(pUserColor);
	}
	colorAdjustGradual(pUserColor);
	updateColor(pUserColor);
}

#endif
///******************************************************************************
// *
// * 	自定义渐变灯效果
// *
// *******************************************************************************/
//void customizeGrdualCbfunc(void) {
//	float vtStep = 0, vtMutiRate = 0;
//	//vLEDStepCtrlTimeCnt = 1000 / (displayParams.customizeEffectFreq * 2 * 5);//step=5ms
//	vLEDStepCtrlTimeCnt = 1;//step=5ms
//	vtStep = CUSTOMIZE_GRADUAL_TOTAL_RATE * 2 / LedStruct.indicatorMaxIndex;	//在半个周期内进行80%的亮度变化.
//	if (LedStruct.currentIndex <= LedStruct.indicatorMaxIndex / 2) {
//		vtMutiRate = (1 - LedStruct.currentIndex * vtStep);
//	} else {
//		vtMutiRate = (1-CUSTOMIZE_GRADUAL_TOTAL_RATE)+vtStep*(LedStruct.currentIndex-LedStruct.indicatorMaxIndex / 2);
//		if(vtMutiRate>1)
//			vtMutiRate=1;
//	}
//	if ( displayParams.fIsFromRGBMode) {
//		setRedData(oriCustomizePwmDataStruct.valueOfRed * vtMutiRate);//
//		setGreenData( oriCustomizePwmDataStruct.valueOfGreen * vtMutiRate);
//		setBlueData(oriCustomizePwmDataStruct.valueOfBlue * vtMutiRate);
//	} else {
//		setCoolData(oriCustomizePwmDataStruct.valueOfCw * vtMutiRate);//
//		setWarmData(oriCustomizePwmDataStruct.valueOfMw * vtMutiRate);
//	}
//}
///******************************************************************************
// *
// * 	自定义呼吸灯效果结束
// *
// *******************************************************************************/
//void customizeGrdualEndCbfunc(void){
//	if ( displayParams.fIsFromRGBMode) {
//		setRedData(0);//
//		setGreenData( 0);
//		setBlueData(0);
//	} else {
//		setCoolData(0);//
//		setWarmData(0);
//	}
//	fIsLightEffectOn = FALSE;
//	LedStruct.customizeLoopIndex=0;
//	LedStruct.currentIndex=0;
//	displayParams.fIsEffectMode=FALSE;
//	fIsLightActive=OFF;
//	LEDPowerOff ;
//}
#if(LIGHTEFFECT_INSIDE==1)
void checkFlashLoopOver(void) {
	if (LedStruct.customizeLoopTimes < INFINITE_LOOP_TIMES) {
		if (++LedStruct.customizeLoopIndex >= LedStruct.customizeLoopTimes) {
		//if (++LedStruct.customizeLoopIndex >=3) {
			LedStruct.pfncustomizeEffectOverCallBack();
			return;					//预设循环次数完成
		}
	}
}
#endif
///******************************************************************************
// *
// * 	自定义闪烁效果
// *
// *******************************************************************************/
//void	customizeFlashCbfunc(void){
//	vLEDStepCtrlTimeCnt = 1000/(displayParams.customizeEffectFreq*2*5);		//step=5ms
//	if(LedStruct.currentIndex%2==0){
//		if(FALSE==displayParams.fIsFromRGBMode){
//			updateColorTemp(&displayParams);
//		}else{
//			updateRGBLamp(&displayParams);
//		}
//	}else{
//		if(FlashMode==displayParams.customizeEffectMode){
//			checkFlashLoopOver();
//		}
//		if(FALSE==displayParams.fIsFromRGBMode){
//			turnOffColorTempLamp();
//		}else{
//			turnOffRGBLamp();
//		}
//	}
//}
///******************************************************************************
// *
// * 	自定义闪烁效果结束
// *
// *******************************************************************************/
//void customizeFlashEndCbfunc(void){
////	setCoolData(0);
////	setWarmData(0);
////	fIsLightEffectOn = FALSE;
////	LedStruct.customizeLoopIndex=0;
////	LedStruct.currentIndex=0;
////	displayParams.fIsEffectMode=FALSE;
//	customizeGrdualEndCbfunc();
//}

#if(LIGHTEFFECT_INSIDE==1)
void	set2FlashMode2StyleA(void){
	LedStruct.indicatorStyle=4;					//flashMode2  styleA
	LedStruct.pfnLedFuncCallBack=flashMode2StyleAfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=190;
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT2_STYLE_A_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=GradualEffect2StyleATimeArray;
	LedStruct.HuesArray=GradualEffectHuesArray;
	pUserColor->hues=0;
	fIsRisingNow=TRUE;
	setGradualEffectPub(&LedStruct);
}
void	set2FlashMode2StyleB(void){
	LedStruct.indicatorStyle=5;					//flashMode2  styleB
	LedStruct.pfnLedFuncCallBack=flashMode2StyleBfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=360;
	LedStruct.timeArray=GradualEffect2StyleBTimeArray;
	pUserColor->hues=0;
	fIsRisingNow=1;
	setGradualEffectPub(&LedStruct);
}

//Campfire 篝火
void	set2FlashMode2StyleC(void){
	LedStruct.pfnLedFuncCallBack=flashMode2StyleCfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=90;
	LedStruct.timeArray=GradualEffect2StyleCTimeArray;
	setGradualEffectPub(&LedStruct);
}

//Party
void	set2FlashMode2StyleD(void){
//	LedStruct.indicatorStyle=5;					//flashMode2  styleA
	LedStruct.pfnLedFuncCallBack=flashMode2StyleDfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=60;
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT2_STYLE_D_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=GradualEffect2StyleDTimeArray;
	LedStruct.HuesArray=GradualEffectHuesArray;
	fIsRisingNow=TRUE;
	setGradualEffectPub(&LedStruct);
}

//Disco
void	set2FlashMode2StyleE(void){
//	LedStruct.indicatorStyle=5;					//flashMode2  styleE
	LedStruct.pfnLedFuncCallBack=flashMode2StyleEfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=25;
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT2_STYLE_E_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=Effect2StyleETimeArray;
	LedStruct.HuesArray=Effect2StyleEHuesArray;
	LedStruct.SaturationArray=Effect2StyleESaturationArray;
	LedStruct.LastRandomData=0;
	fIsRisingNow=TRUE;
	setGradualEffectPub(&LedStruct);
}

//Sundown
void	set2FlashMode2StyleF(void){
//	LedStruct.indicatorStyle=5;					//flashMode2  styleE
	LedStruct.pfnLedFuncCallBack=flashMode2StyleFfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=3;
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT2_STYLE_F_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=Effect2StyleFTimeArray;
	LedStruct.HuesArray=Effect2StyleFHuesArray;
	LedStruct.SaturationArray=Effect2StyleFSaturationArray;
	pUserColor->saturation=0;
	fIsRisingNow=TRUE;
	setGradualEffectPub(&LedStruct);
}

void set2SimulatonModeStyleA(void) {

	LedStruct.indicatorMaxIndex = MAX_SIMULATION_MODE_STYLE_A_INDEX;
	LedStruct.indicatorStyle = 7;					//SimulatonMode  styleA
	LedStruct.sttsArray = simulationModeStyleASttsArray;
	LedStruct.timeArray = simulationModeStyleATimerArray;
	setFlashEffectPub(&LedStruct);

}

void set2SimulatonModeStyleB(void) {

	LedStruct.indicatorMaxIndex = MAX_SIMULATION_MODE_STYLE_B_INDEX;
	LedStruct.sttsArray = simulationModeStyleBSttsArray;
	LedStruct.timeArray = simulationModeStyleBTimerArray;
	setFlashEffectPub(&LedStruct);
}

void set2SimulatonModeStyleC(void) {

	LedStruct.indicatorMaxIndex = MAX_SIMULATION_MODE_STYLE_C_INDEX;
	LedStruct.sttsArray = simulationModeStyleCSttsArray;
	LedStruct.timeArray = simulationModeStyleCTimerArray;
	setFlashEffectPub(&LedStruct);
}

//Fireworks 烟花
void set2SimulatonModeStyleD(void) {

	LedStruct.pfnLedFuncCallBack = flashMode3StyleDfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=0x48;								//0x00~0x1f(Phase 1) ,0x20~0x22(Phase 2),0x30~0x48(Phase 3)
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT3_STYLE_D_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=Effect3StyleDTimeArray;
	LedStruct.HuesArray=Effect3StyleDHuesArray;
	LedStruct.SaturationArray=Effect3StyleDSaturationArray;
	LedStruct.LastRandomData=0;
	fIsRisingNow=FALSE;
	setGradualEffectPub(&LedStruct);

}
//Fireworks 烟花2
void set2SimulatonModeStyleE(void) {

	LedStruct.pfnLedFuncCallBack = flashMode3StyleEfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=0x48;									//0x00~0x1f(Phase 1) ,0x20~0x22(Phase 2),0x30~0x48(Phase 3)
	LedStruct.MaxHuesIndex=MAX_GRADUAL_EFFECT3_STYLE_E_INDEX;
	LedStruct.IndexOfHues=0;
	LedStruct.timeArray=Effect3StyleETimeArray;
	LedStruct.HuesArray=Effect3StyleEHuesArray;
	LedStruct.SaturationArray=Effect3StyleESaturationArray;
	LedStruct.LastRandomData=0;
	fIsRisingNow=FALSE;
	setGradualEffectPub(&LedStruct);

}
//RGB cycle
void set2SimulatonModeStyleF(void) {

	LedStruct.pfnLedFuncCallBack=flashMode3StyleFfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=360;
	LedStruct.timeArray=GradualEffect3StyleFTimeArray;
	LedStruct.fIsFirstCycle=1;
	pUserColor->hues=0;
	setGradualEffectPub(&LedStruct);

}
///********************************************************
// *
// *	自定义渐变效果
// *
// ********************************************************/
//void	 customizeGradualEffect(void){
//	LedStruct.currentIndex=0;
//	customizeFlashCbfunc();					//仅仅用于获取PWM的值
//	LedStruct.indicatorStyle=67;					//flashMode2  styleC
//	LedStruct.pfnLedFuncCallBack=customizeGrdualCbfunc;
//	LedStruct.pfncustomizeEffectOverCallBack=customizeGrdualEndCbfunc;
//	LedStruct.customizeLoopIndex=0;
//	LedStruct.indicatorMaxIndex= 1000/(displayParams.customizeEffectFreq*5)-1;		//step=5ms2*displayParams.customizeEffectFreq-1;
////	LedStruct.customizeLoopTimes=displayParams.customizeEffectTimes*displayParams.customizeEffectFreq;
//	LedStruct.customizeLoopTimes=displayParams.customizeEffectTimes;
//	LedStruct.timeArray=customizeGradualTimeArray;
//	fIsLightEffectOn = 1;
//	displayParams.fIsEffectMode=TRUE;
//	LedStruct.fIsGradualMode=1;
//	DDL_ZERO_STRUCT(oriCustomizePwmDataStruct);
//	oriCustomizePwmDataStruct.valueOfRed=pcaDataStruct.valueOfRed;
//	oriCustomizePwmDataStruct.valueOfGreen=pcaDataStruct.valueOfGreen;
//	oriCustomizePwmDataStruct.valueOfBlue=pcaDataStruct.valueOfBlue;
//	oriCustomizePwmDataStruct.valueOfCw=pcaDataStruct.valueOfCw;
//	oriCustomizePwmDataStruct.valueOfMw=pcaDataStruct.valueOfMw;
//	customizeGrdualCbfunc();
//}
///********************************************************
// *
// *	自定义闪烁效果
// *
// ********************************************************/
//void	customizeFlashEffect(void){
//	LedStruct.indicatorStyle=66;
//	LedStruct.pfnLedFuncCallBack=customizeFlashCbfunc;
//	LedStruct.pfncustomizeEffectOverCallBack=customizeFlashEndCbfunc;
//	LedStruct.currentIndex=0;
//	LedStruct.indicatorMaxIndex=2*displayParams.customizeEffectFreq-1;
//	LedStruct.customizeLoopTimes=displayParams.customizeEffectTimes;
//	LedStruct.customizeLoopIndex=0;
//	LedStruct.timeArray=customizeFlashTimeArray;
//	fIsLightEffectOn = 1;
//	displayParams.fIsEffectMode=TRUE;
//	LedStruct.fIsGradualMode=0;
//	customizeFlashCbfunc();
//}

void transferEffectNo(displayParamsStruct * disParams) {
//	MAX_LightEffect
//	disParams->preinstallEffectNo=1;
	if (disParams->arrowIndex == Style1Setting) {
		disParams->preinstallEffectNo = disParams->style1Value + 1;
	} else if (disParams->arrowIndex == Style2Setting) {
		disParams->preinstallEffectNo = disParams->style2Value + 1 + PerStyleSize;
	} else if (disParams->arrowIndex == Style3Setting) {
		disParams->preinstallEffectNo = disParams->style3Value + 1 + PerStyleSize * 2;
	}
}
#endif

void startLightEffect(displayParamsStruct * disParams) {
#if(LIGHTEFFECT_INSIDE==1)
	turnOffAllLightEffect();
//	if(!displayParams.brightness)
//		return;
	transferEffectNo(disParams);
	if (disParams->arrowIndex >= PreinstallEffect) {
		if (disParams->preinstallEffectNo == 1)
			set2FlashMode1StyleA();
		else if (disParams->preinstallEffectNo == 2)
			set2FlashMode1StyleB();
		else if (disParams->preinstallEffectNo == 3)
			set2FlashMode1StyleC();
		else if (disParams->preinstallEffectNo == 4)
			set2FlashMode1StyleD();				//SOS
		else if (disParams->preinstallEffectNo == 5)
			set2FlashMode1StyleE();				//Faulty Lamp
		else if (disParams->preinstallEffectNo == 6)
			set2FlashMode1StyleF();				//Candle
		else if (disParams->preinstallEffectNo ==1+PerStyleSize)
			set2FlashMode2StyleA();
		else if (disParams->preinstallEffectNo == 2+PerStyleSize)
			set2FlashMode2StyleB();
		else if (disParams->preinstallEffectNo == 3+PerStyleSize)
			set2FlashMode2StyleC();				//Campfire
		else if (disParams->preinstallEffectNo == 4+PerStyleSize)
			set2FlashMode2StyleD();				//Party
		else if (disParams->preinstallEffectNo == 5+PerStyleSize)
			set2FlashMode2StyleE();				//Disco
		else if (disParams->preinstallEffectNo == 6+PerStyleSize)
			set2FlashMode2StyleF();				//Sundown
		else if (disParams->preinstallEffectNo == 1+2*PerStyleSize)
			set2SimulatonModeStyleA();
		else if (disParams->preinstallEffectNo == 2+2*PerStyleSize)
			set2SimulatonModeStyleB();
		else if (disParams->preinstallEffectNo == 3+2*PerStyleSize)
			set2SimulatonModeStyleC();
		else if (disParams->preinstallEffectNo == 4+2*PerStyleSize)
			set2SimulatonModeStyleD();			//Fireworks
		else if (disParams->preinstallEffectNo == 5+2*PerStyleSize)
			set2SimulatonModeStyleE();			//Fireworks2
		else if (disParams->preinstallEffectNo == 6+2*PerStyleSize)
			set2SimulatonModeStyleF();			//RGB cycle
	}
//	else if(CustomizeEffect==disParams->arrowIndex){
//		if(GradualMode==displayParams.customizeEffectMode){
//			//setGradualEffectPub();
//			customizeGradualEffect();
//		}else{
//			customizeFlashEffect();
//			//setFlashEffectPub();
//		}
//	}
#endif
}
