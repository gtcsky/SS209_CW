/*
 * user_color.c
 *
 *  Created on: 2019年11月20日
 *      Author: Bough_004
 */
#include "user_color.h"
#include "io_define.h"
#include "user_advtime.h"
#include "user_menu.h"
////																//32,    33   34  35         			36    37   38               	39  40   41          		42  43   44      		45  46     47      48   49   50   51         52   53   54  55       	56   57  58  59          		60   61   62  63  	     		64   65  66    67         		68   69  70   71          		72  73  74  75
//const u16 colorTempCoolArray[MAX_ColorTemp-MIN_ColorTemp+1]={    0, 150,  240  ,290,       		402,462, 516,            	558,612,630,      		678,708,732,    		762,804,834,    852,882,912,948,      972,984,996,1026,   1032,1044,1056,1062,     	1062,1068,1068,1080,     	1044,1098,1104,1104,     	1104,1110,1116, 1122,      	1128,1134,1140,1199};
//																//32,    33   34  35         			36    37   38               	39  40   41          		42  43   44      		45  46     47      48   49   50   51         52   53   54  55       	56   57  58  59          		60   61   62  63  	     		64   65  66    67         		68   69  70   71          		72  73  74  75
//const u16 colorTempWarmArray[MAX_ColorTemp-MIN_ColorTemp+1]={1199,1158,1140,1120,       	1110,1104,1086,            1080,1064,1050,       	1044, 1032,1008,   	960,936,900,    858,834,810,786,      768,744,726,696,    678,654,630,612,      		588, 564,528, 522,    		516,486,456,434,     		414,402,378,360,       		330,276,264,0};
//																//32,    33   34  35         			36    37   38               	39  40   41          	42  43   44      		45  46     47      48   49   50   51         52   53   54  55       	56   57  58  59          		60   61   62  63  	     		64   65  66    67         		68   69  70   71          		72  73  74  75
const u16 colorTempCoolArray[MAX_ColorTemp-MIN_ColorTemp+1]={    0, 150,  240  ,290,       		350,390, 450,            	480,510,540,      		570,600,630,    		660,690,730,    770,810,840,865,      890,915,940,970,   990,1005,1030,1055,     		1075,1085,1095,1105,     	1110,1115,1123,1130,     	1135,1140,1145, 1150,      	1160,1170,1180,1199};
																//32,    33   34  35         			36    37   38               	39  40   41          	42  43   44      		45  46     47      48   49   50   51         52   53   54  55       	56   57  58  59          		60   61   62  63  	     		64   65  66    67         		68   69  70   71          		72  73  74  75
const u16 colorTempWarmArray[MAX_ColorTemp-MIN_ColorTemp+1]={1199,1158,1140,1120,       	1100,1085,1050,            1020,990,950,       	920, 880,840,   		800,760,730,    700,660,630,600,      570,540,520,500,    470,460,440,420,      		405, 385,365, 340,    		320,300,280,260,     		240,220,200,180,       		160,140,120,0};


const float saturationArray[101]={
				0,0.015,0.030,0.045,0.060,		0.075,0.090,0.105,0.120,0.135,
				0.150,0.165,0.180,0.195,0.210,		0.225,0.240,0.255,0.270,0.285,
				0.300,0.315,0.330,0.345,0.360,		0.375,0.390,0.405,0.420,0.435,
				0.450,0.465,0.480,0.495,0.510,		0.525,0.540,0.555,0.570,0.585,
				0.600,0.610,0.620,0.630,0.640,		0.650,0.650,0.670,0.680,0.690,
				0.700,0.710,0.720,0.730,0.740,		0.750,0.760,0.770,0.780,0.790,
				0.800,0.805,0.810,0.815,0.820,		0.825,0.830,0.835,0.840,0.845,
				0.850,0.855,0.860,0.865,0.870,		0.875,0.880,0.885,0.890,0.895,
				0.900,0.905,0.910,0.915,0.920,		0.925,0.930,0.935,0.940,0.945,
				0.950,0.955,0.960,0.965,0.970,		0.975,0.980,0.985,0.990,0.995,1.000
};

/************************************************************
 *
 * 设置CW_LED口切换为默认IO设置(输入口)
 * 	把CH1切换为Blue模式
 *
 *************************************************************/
void	 IoCoolSetInput(void){
	Blue_LED_PORT->AFR&=Blue_PIN_CONFIG_CLR;
	Blue_LED_PORT->AFR|=Blue_PIN_CONFIG;			//GPIOC.3 复用配置 		PC3 as PWM CH3
	CW_LED_PORT->AFR&=CW_PIN_CONFIG_CLR;
	setCoolOutputLow();
	fIsCh1ForCW=FALSE;
}
/************************************************************
 *
 * 设置MW_LED口切换为默认IO设置(输入口)
 * 	把CH1切换为Blue模式
 *
 *************************************************************/
void	 IoWarmSetInput(void){
	Green_LED_PORT->AFR&=Green_PIN_CONFIG_CLR;
	Green_LED_PORT->AFR|=Green_PIN_CONFIG;			//GPIOC.3 复用配置 		PC3 as PWM CH3
	MW_LED_PORT->AFR&=MW_PIN_CONFIG_CLR;
	setWarmOutputLow();
	fIsCh2ForWW=FALSE;
}
/************************************************************
 *
 * 设置Blue_LED口切换为默认IO设置(输入口)
 * 	把CH1切换为CW模式
 *
 *************************************************************/
void	 IoBlueSetInput(void){
	CW_LED_PORT->AFR&=CW_PIN_CONFIG_CLR;
	CW_LED_PORT->AFR|=CW_PIN_CONFIG;			//GPIOD.3 复用配置 		PD3 as PWM CH3N
	Blue_LED_PORT->AFR&=Blue_PIN_CONFIG_CLR;
	setBlueOutputLow();
	fIsCh1ForCW=TRUE;
}
/************************************************************
 *
 * 设置Blue_LED口切换为默认IO设置(输入口)
 * 	把CH2切换为CW模式
 *
 *************************************************************/
void	 IoGreenSetLow(void){
	MW_LED_PORT->AFR&=MW_PIN_CONFIG_CLR;
	MW_LED_PORT->AFR|=MW_PIN_CONFIG;			//GPIOD.3 复用配置 		PD3 as PWM CH3N
	Green_LED_PORT->AFR&=Green_PIN_CONFIG_CLR;
	setGreenOutputLow();
	fIsCh2ForWW=TRUE;
}

void PwmChDefault(void) {
	IoBlueSetInput();
	IoGreenSetLow();
}

void	allLedIoAsInput(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStruct.GPIO_Pin = Green_LED_PIN|CW_LED_PIN| MW_LED_PIN;
	GPIO_Init(CW_LED_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = Blue_LED_PIN ;
	GPIO_Init(Blue_LED_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = Red_LED_PIN ;
	GPIO_Init(Red_LED_PORT,&GPIO_InitStruct);

}
void	allLedIoAsInputDown(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStruct.GPIO_Pin = Green_LED_PIN|CW_LED_PIN| MW_LED_PIN;
	GPIO_Init(CW_LED_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = Blue_LED_PIN ;
	GPIO_Init(Blue_LED_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = Red_LED_PIN ;
	GPIO_Init(Red_LED_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LED_Power_Pin ;
	GPIO_Init(LED_Power_Port,&GPIO_InitStruct);

}

void	setLedIoAf(void){
	PwmChDefault();
	Red_LED_PORT->AFR&=Red_PIN_CONFIG_CLR;
	Red_LED_PORT->AFR|=Red_PIN_CONFIG;
}

void	 setCoolDuty(float duty){
	if((duty)||(!duty&&(TRUE==fIsCh1ForCW))){
		if(FALSE==fIsCh1ForCW){
			IoBlueSetInput();
		}
		updatePWMChlDuty(CW_LED_CHANNEL,(1-duty),ADVTIM1);
	}
}
void	 setWarmDuty(float duty){
	if((duty)||(!duty&&(TRUE==fIsCh2ForWW))){
		if(FALSE==fIsCh2ForWW){
			IoGreenSetLow();
		}
		updatePWMChlDuty(MW_LED_CHANNEL,(duty),ADVTIM1);
	}
}

void	 setBlueDuty(float duty){
	if((duty)||(!duty&&(FALSE==fIsCh1ForCW))){
		if(TRUE==fIsCh1ForCW){
			IoCoolSetInput();
		}
		updatePWMChlDuty(Blue_LED_CHANNEL,duty,ADVTIM1);
	}
}
void	 setGreenDuty(float duty){
	if((duty!=1)||((duty==1)&&(FALSE==fIsCh2ForWW))){
		if(TRUE==fIsCh2ForWW){
			IoWarmSetInput();
		}
		updatePWMChlDuty(Green_LED_CHANNEL,(1-duty),ADVTIM1);
	}
}
/************************************************************************
 *
 * 通过设置捕获值设置PWM 占空比
 *
 ************************************************************************/
void setCoolData(u16 pwmData) {
	float duty = 0;
	if (pwmData >= PWM_PERIOD_CONST)
		pwmData = PWM_PERIOD_CONST;
	duty = (float) pwmData / PWM_PERIOD_CONST;
	setCoolDuty(duty);
	pcaDataStruct.valueOfCw = pwmData;
//	userColor.cWhite = duty;
	userColor.cWhite = pwmData;
}
/************************************************************************
 *
 * 通过设置捕获值设置PWM 占空比
 *
 ************************************************************************/
void setWarmData(u16 pwmData){
	float duty=0;
	if (pwmData >= PWM_PERIOD_CONST)
		pwmData = PWM_PERIOD_CONST;
	duty=(float)pwmData/PWM_PERIOD_CONST;
//	updatePWMChlDuty(MW_LED_CHANNEL,duty,ADVTIM1);
	setWarmDuty(duty);
	pcaDataStruct.valueOfMw=pwmData;
//	userColor.wWhite = duty;
	userColor.wWhite = pwmData;
}

void setRedDuty(float duty){
	updatePWMChlDuty(Red_LED_CHANNEL,duty,ADVTIM1);
}
void	 setRedData(u16 pwmData){
	float duty=0;
	if (pwmData >= PWM_PERIOD_CONST)
		pwmData = PWM_PERIOD_CONST;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	updatePWMChlDuty(Red_LED_CHANNEL,duty,ADVTIM1);
	pcaDataStruct.valueOfRed=pwmData;
//	userColor.red=duty;
	userColor.red=pwmData;
}


void	 setGreenData(u16 pwmData){
	float duty=0;
	if (pwmData >= PWM_PERIOD_CONST)
		pwmData = PWM_PERIOD_CONST;
	duty=(float)pwmData/PWM_PERIOD_CONST;
//	updatePWMChlDuty(Green_LED_CHANNEL,(1-duty),ADVTIM1);
	setGreenDuty(1-duty);
	pcaDataStruct.valueOfGreen=pwmData;
//	userColor.green = duty;
	userColor.green = pwmData;
}
void	 setBlueData(u16 pwmData){
	float duty=0;
	if (pwmData >= PWM_PERIOD_CONST)
		pwmData = PWM_PERIOD_CONST;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	setBlueDuty(duty);
	pcaDataStruct.valueOfBlue=pwmData;
//	userColor.blue = duty;
	userColor.blue = pwmData;
}

void updateColor(colorStructType * colorStruct){
	turnOffColorTempLamp();
	setRedData(vTemperatureCoe*(colorStruct->red));			//Red
	setGreenData(vTemperatureCoe*(colorStruct->green));		//Green
	setBlueData(vTemperatureCoe*(colorStruct->blue));		//Blue
}


/************************************************************************
 *
 *	HSV转RGB
 *
 ************************************************************************/
void hsb2Rgb(colorStructType * colorStruct) {
	UINT16 	h=colorStruct->hues;
//	float	 s=colorStruct->saturation;
	float s=0;
//	float vtRgbRate=0;
	float	 v=colorStruct->brightness;
	float r = 0, g = 0, b = 0;
	float f,p,q,t,total;
	int i=0;
	s=saturationArray[(u8)(colorStruct->saturation*100)];
	if(h>=360)
		h=0;
	if(s>=1.0)
		s=1.0;
	if(v>=1.0)
		v=1.0;
	i = ( h / 60) % 6;
	f = ((float)h / 60) - i;
	p = v * (1 - s);
	q = v * (1 - f * s);
	t = v * (1 - (1 - f) * s);
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
		r = v;
		g = p;
		b = q;
		break;
	default:
		break;
	}

	//minChange(&r,&g,&b,s);

	total=r+g+b;
//	r=r*v/total;
//	g=g*v/total;
//	b=b*v/total;
	r/=total;
	g/=total;
	b/=total;
	if(v>=BrightnessThreshold){
		r*=1/RedGreenRate;
		b*=1/BlueGreenRate;

//		total=r+g+b;
//		r=r*v/total;
//		g=g*v/total;
//		b=b*v/total;
	}else{
		r*=0.90/RedGreenRate+(RedGreenRateStep*(BrightnessThreshold-v)/BrightnessGap);
		b*=1.15/BlueGreenRate+(BlueGreenRateStep*(BrightnessThreshold-v)/BrightnessGap);
//		total=r+g+b;
//		r=r*v/total;
//		g=g*v/total;
//		b=b*v/total;
//		if(r){
//			r*=1-RGBLED_REAL_START_DUTY_PERCENT;
//			r+=RGBLED_REAL_START_DUTY_PERCENT;
//		}
//		if(g){
//			g*=1-RGBLED_REAL_START_DUTY_PERCENT;
//			g+=RGBLED_REAL_START_DUTY_PERCENT;
//		}
//		if(b){
//			b*=1-RGBLED_REAL_START_DUTY_PERCENT;
//			b+=RGBLED_REAL_START_DUTY_PERCENT;
//		}

	}
	total=r+g+b;
	r=r*v/total;
	g=g*v/total;
	b=b*v/total;

	r*=0.95/1.0;
	g*=0.95/1.0;
	b*=0.95/1.0;

	colorStruct->red=(UINT16) (r*PWM_FRQ_CONST);
	colorStruct->blue=(UINT16) (b*PWM_FRQ_CONST);
	colorStruct->green=(UINT16) (g*PWM_FRQ_CONST);
}


/*****************************************************************
 *
 * 关闭色温灯通道
 *
 ******************************************************************/
void turnOffColorTempLamp(void){
	graduallyAdjust.fIsAdjustSlowMode=0;
	setWarmData(0);				//暖光
	setCoolData(0);				//冷光
}
void	turnOffRGBLamp(void){
	setRedData(0);
	setGreenData(0);
	setBlueData(0);
}
//void updateColorTemp(displayParamsStruct * disParams) {
//	u16 vtCoolData=0, vtWarmData=0;
//	vtCoolData = colorTempCoolArray[(disParams->colorTemperature - MIN_ColorTemp)];
//	vtWarmData = colorTempWarmArray[(disParams->colorTemperature - MIN_ColorTemp)];
//	if(vtCoolData){
//		setCoolData(vTemperatureCoe*vtCoolData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));				//从4%起步
//	}else{
//		setCoolData(0);
//	}
//	if(vtWarmData){
//		setWarmData(vTemperatureCoe*vtWarmData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));			//从4%起步
//	}else{
//		setWarmData(0);
//	}
//}
//
//void	setTargeColorTempData(displayParamsStruct * disParams,PcaDataStruct *targetData){
//	u16 vtCoolData=0, vtWarmData=0;
//	vtCoolData = colorTempCoolArray[(disParams->colorTemperature - MIN_ColorTemp)];
//	vtWarmData = colorTempWarmArray[(disParams->colorTemperature - MIN_ColorTemp)];
//	if(vtCoolData){
////		vtCoolData-=LED_REAL_START_DATA;
////		targetData->valueOfCw=vTemperatureCoe*vtCoolData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100);
//		vtCoolData-=LED_REAL_START_DATA;
//		targetData->valueOfCw=vTemperatureCoe*(vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
//	}else{
//		targetData->valueOfCw=0;
//	}
//	if(vtWarmData){
////		vtWarmData-=LED_REAL_START_DATA;
////		targetData->valueOfMw=vTemperatureCoe*vtWarmData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100);
//		vtWarmData-=LED_REAL_START_DATA;
//		targetData->valueOfMw= vTemperatureCoe*(vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
//	}else{
//		targetData->valueOfMw=0;
//	}
//}
void updateColorTemp(displayParamsStruct * disParams) {
	u16 vtCoolData=0, vtWarmData=0;
	vtCoolData = colorTempCoolArray[(disParams->colorTemperature - MIN_ColorTemp)];
	vtWarmData = colorTempWarmArray[(disParams->colorTemperature - MIN_ColorTemp)];
	if(vtCoolData&&disParams->brightness){
		vtCoolData-=LED_REAL_START_DATA;
		setCoolData(vTemperatureCoe*(vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));				//从4%起步
	}else{
		setCoolData(0);
	}
	if(vtWarmData&&disParams->brightness){
		vtWarmData-=LED_REAL_START_DATA;
		setWarmData(vTemperatureCoe*(vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));			//从4%起步
	}else{
		setWarmData(0);
	}
}

void	setTargeColorTempData(displayParamsStruct * disParams,PcaDataStruct *targetData){
	u16 vtCoolData=0, vtWarmData=0;
	vtCoolData = colorTempCoolArray[(disParams->colorTemperature - MIN_ColorTemp)];
	vtWarmData = colorTempWarmArray[(disParams->colorTemperature - MIN_ColorTemp)];
	if(vtCoolData&&disParams->brightness){
//		vtCoolData-=LED_REAL_START_DATA;
//		targetData->valueOfCw=vTemperatureCoe*vtCoolData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100);
		vtCoolData-=LED_REAL_START_DATA;
		targetData->valueOfCw=vTemperatureCoe*(vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
	}else{
		targetData->valueOfCw=0;
	}
	if(vtWarmData&&disParams->brightness){
//		vtWarmData-=LED_REAL_START_DATA;
//		targetData->valueOfMw=vTemperatureCoe*vtWarmData * (((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100);
		vtWarmData-=LED_REAL_START_DATA;
		targetData->valueOfMw= vTemperatureCoe*(vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
	}else{
		targetData->valueOfMw=0;
	}
}
#if(RGB_INSIDE==1)
void	updateRGBLamp(displayParamsStruct * disParams){
	pUserColor->hues=disParams->hues;
	pUserColor->saturation=(float)disParams->saturation/100;
	if(disParams->brightness)
		pUserColor->brightness=((float)disParams->brightness)*(100-RGBLED_REAL_START_DUTY)/(100*100)+(float)RGBLED_REAL_START_DUTY/100;			//从9%起步
	else
		pUserColor->brightness=0;
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
}
#endif
