/*
 * user_adc.c
 *
 *  Created on: 2021年7月20日
 *      Author: Achilles Zhang
 */
#include "user_adc.h"
#include "const.h"
#include "misc.h"
#include "user_menu.h"
#include "io_define.h"

adcConvertStruct adcConvertParams={0};

/********************************************************************************************************
 *
 * 根据开启的通道把对应的IO复用模式 改为ADC模式
 *
 *********************************************************************************************************/
void ADCPinAFSet(u8 vtADCChls){
	if(vtADCChls&((u8)ADC_Channel0_ENABLE))
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,ADC_AF_VALUE); 		//AIN0 PC6
	if(vtADCChls&((u8)ADC_Channel1_ENABLE))
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,ADC_AF_VALUE); 		//AIN1 PC3
	if(vtADCChls&((u8)ADC_Channel2_ENABLE))
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource4,ADC_AF_VALUE); 		//AIN2 PC4
	if(vtADCChls&((u8)ADC_Channel3_ENABLE))
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,ADC_AF_VALUE); 		//AIN3 PD2
	if(vtADCChls&((u8)ADC_Channel4_ENABLE))
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource3,ADC_AF_VALUE); 		//AIN4 PD3
	if(vtADCChls&((u8)ADC_Channel5_ENABLE))
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,ADC_AF_VALUE); 		//AIN5 PD5
	if(vtADCChls&((u8)ADC_Channel6_ENABLE))
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,ADC_AF_VALUE); 		//AIN6 PD6
}


void ADC_SignalMode_Config(u8 vtADCChl){
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	u16 channelSel;
	assert_param(IS_ADC_CHANNEL_CMD(vtADCChl));
	channelSel=adcChlCMD2adcAdcChl(vtADCChl);
	/*初始化ADC通信结构体*/
//	ADC_CommonInitStruct.ADC_CircleMode = _ADC_CIRCLEMODE;						//ADC 非循环模式    0/1:非循环/循环模式
//	ADC_CommonInitStruct.ADC_CtMode = ADC_Ct_Single;							//ADC_CR1.CT 设置为单次转换    0/1:单次转换/连续转换		//连续转换为依次从AIN0~AIN7
//	ADC_CommonInitStruct.ADC_SetAdcCnt = _ADC_SETADCCNT_SIGNAL;				//连续转换次数 15次 ,结果不累加,结果为最后次的值
//	ADC_CommonInitStruct.ADC_Trigs0Sel = _ADC_TRIGS0SEL;							//TRIGS0[4:0]	禁止自动触发ADC转换0
//	ADC_CommonInitStruct.ADC_Trigs1Sel = _ADC_TRIGS1SEL;							//TRIGS1[4:0]	禁止自动触发ADC转换1
//	ADC_CommonInitStruct.ADC_CircleMode = _ADC_CIRCLEMODE;						//ADC 非循环模式    0/1:非循环/循环模式
	ADC_CommonInitStruct.ADC_CircleMode = ADC_Mode_Independent;					//ADC 非循环模式    0/1:非循环/循环模式
	ADC_CommonInitStruct.ADC_CtMode = ADC_Ct_Single;							//ADC_CR1.CT 设置为单次转换    0/1:单次转换/连续转换		//连续转换为依次从AIN0~AIN7
	ADC_CommonInitStruct.ADC_SetAdcCnt = _ADC_SETADCCNT;						//连续转换次数 15次 ,结果不累加,结果为最后次的值
	ADC_CommonInitStruct.ADC_Trigs0Sel = _ADC_TRIGS0SEL;							//TRIGS0[4:0]	禁止自动触发ADC转换0
	ADC_CommonInitStruct.ADC_Trigs1Sel = _ADC_TRIGS1SEL;							//TRIGS1[4:0]	禁止自动触发ADC转换1

	/*初始化ADC结构体*/
	ADC_InitStruct.ADC_ChannelSel = channelSel;										//设置ADC channel
	if(vtADCChl==ADC_BATT_CHL){
		ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div64;                                      			//设置ADC时钟分频	为16
		ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;							//ADC 采样周期选择
//	}else if(ADC_CURRENT_CHL==vtADCChl){
//		ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div32;                                      			//设置ADC时钟分频	为16
//		ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;							//ADC 采样周期选择
	}else{
		ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div4;                                      			//设置ADC时钟分频	为0
		ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;							//ADC 采样周期选择
	}
	/*初始化ADC*/
	ADC_Init(DEBUG_ADCx, &ADC_InitStruct);
	ADC_CommonInit(DEBUG_ADCx,&ADC_CommonInitStruct);

	/*GPIO 管脚复用配置*/
	ADCPinAFSet(vtADCChl);

	ADC_NVIC_Config();
	ADC_ITCmd(DEBUG_ADCx, vtADCChl, ADC_IT_STATE);								//开启指定通道中断
//	ADC_ITCmd(DEBUG_ADCx, ADC_TICONT_FLAG, ADC_IT_STATE);						//开启连续转换完成中断
	// 开启ADC ，
	ADC_Cmd(DEBUG_ADCx, ENABLE);
//	//开始单次转换							改成函数由外部开启转换
	ADC_SoftwareStartConv(DEBUG_ADCx);
//	ADC_ContinuousConverChannelCmd(ADC,vtADCChls, ENABLE);
//	ADC_ContinuousConverChannelCmd(ADC,vtADCChl, ENABLE);
}

/***********************************************************************************************************
  *  @brief     					单通道连续累加转换模式
  *
  *  @param [in] :			试图开启转换的通道
  *
  *  @param [out] :			true:开启转换成功
  *  						false:开启新的转换失败，新的转换任务会被加入待准换队列
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
uint8 ADC_singleContinuousAccMode_config(u8 vtADCChl) {
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	u16 channelSel;
	assert_param(IS_ADC_CHANNEL_CMD(vtADCChl));
	channelSel = adcChlCMD2adcAdcChl(vtADCChl);
	if (adcConvertParams.vSingleChlAdcConverting){			//如果当前有通道正在转换，则推迟转换
		adcConvertParams.vPospendConvertInfo=vtADCChl;
		adcConvertParams.callbacks[adcChlCMD2adcIndex(vtADCChl)]=ADC_singleContinuousAccMode_config;
		return FALSE;
	}
	adcConvertParams.vPospendConvertInfo&=~vtADCChl;
	adcConvertParams.vSingleChlAdcConverting=vtADCChl;
	//	vtADCChls=0x7f;					// AIN0~6开启
	/*初始化ADC通信结构体*/
	ADC_CommonInitStruct.ADC_CircleMode = ADC_Mode_Independent;			//ADC 非循环模式    0/1:非循环/循环模式
	ADC_CommonInitStruct.ADC_CtMode = ADC_Ct_Contine;					//ADC_CR1.CT 设置为连续转换    0/1:单次转换/连续转换		//连续转换为依次从AIN0~AIN7
	ADC_CommonInitStruct.ADC_SetAdcCnt = _ADC_SETADCCNT;				//连续转换次数16次		(并非累加, 8个通道(如果都激活总计转换16次,每个通道平均2次) 结果中是每个通道最后一次的值)
	ADC_CommonInitStruct.ADC_Trigs0Sel = _ADC_TRIGS0SEL;					//TRIGS0[4:0]	禁止自动触发ADC转换0
	ADC_CommonInitStruct.ADC_Trigs1Sel = _ADC_TRIGS1SEL;					//TRIGS1[4:0]	禁止自动触发ADC转换1

	/*初始化ADC结构体*/
	ADC_InitStruct.ADC_ChannelSel = channelSel;								//设置ADC channel 	0
	//	ADC_InitStruct.ADC_ClkSel = _ADC_CLKSEL;							//设置ADC时钟分频	为0
	//	ADC_InitStruct.ADC_SamSel = _ADC_SAMSEL;							//ADC 采样周期选择
	ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div64;                                      		//设置ADC时钟分频	为16
	ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;						//ADC 采样周期选择
	/*初始化ADC*/
	ADC_Init(DEBUG_ADCx, &ADC_InitStruct);
	ADC_CommonInit(DEBUG_ADCx, &ADC_CommonInitStruct);

	/*GPIO 管脚复用配置*/

	ADCPinAFSet(vtADCChl);
	ADC_ClearFlag(DEBUG_ADCx, vtADCChl);									//ADC_INTCLR ADC 中断清除寄存器
	// 开启ADC ，并开始转换
	ADC_ContinuousConverChannelCmd(ADC, vtADCChl, ENABLE);
	ADC_NVIC_Config();
	ADC_AccCmd(ADC, ENABLE);											//开启转换结果累加
	ADC_ClearFlag(ADC, ADC_TICONT_FLAG);
	ADC_ITCmd(DEBUG_ADCx, ADC_TICONT_FLAG, ADC_IT_STATE);				//开启连续转换完成中断
	ADC_AccResultClear(ADC, ENABLE);										//清除累加器
	ADC_ContinuousConverRstCmd(DEBUG_ADCx, ENABLE);
	ADC_ContinuousConverRstCmd(DEBUG_ADCx, DISABLE);
	ADC_Cmd(DEBUG_ADCx, ENABLE);
	ADC_SoftwareStartConv(DEBUG_ADCx);

	return TRUE;
}
/*****************************************************************************
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  ******************************************************************************/
void ADC_MutiMode_Config(u8 vtADCChls) {
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
//	vtADCChls=0x7f;					// AIN0~6开启
	/*初始化ADC通信结构体*/
	ADC_CommonInitStruct.ADC_CircleMode = ADC_Mode_Independent;			//ADC 非循环模式    0/1:非循环/循环模式
	ADC_CommonInitStruct.ADC_CtMode = ADC_Ct_Contine;					//ADC_CR1.CT 设置为连续转换    0/1:单次转换/连续转换		//连续转换为依次从AIN0~AIN7
	ADC_CommonInitStruct.ADC_SetAdcCnt = _ADC_SETADCCNT;				//连续转换次数16次		(并非累加, 8个通道(如果都激活总计转换16次,每个通道平均2次) 结果中是每个通道最后一次的值)
	ADC_CommonInitStruct.ADC_Trigs0Sel = _ADC_TRIGS0SEL;					//TRIGS0[4:0]	禁止自动触发ADC转换0
	ADC_CommonInitStruct.ADC_Trigs1Sel = _ADC_TRIGS1SEL;					//TRIGS1[4:0]	禁止自动触发ADC转换1

	/*初始化ADC结构体*/
	ADC_InitStruct.ADC_ChannelSel = _ADC_ChannelSel;						//设置ADC channel 	0
//	ADC_InitStruct.ADC_ClkSel = _ADC_CLKSEL;								//设置ADC时钟分频	为0
//	ADC_InitStruct.ADC_SamSel = _ADC_SAMSEL;								//ADC 采样周期选择
	ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div64;                                      		//设置ADC时钟分频	为16
	ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;						//ADC 采样周期选择
	/*初始化ADC*/
	ADC_Init(DEBUG_ADCx, &ADC_InitStruct);					//
	ADC_CommonInit(DEBUG_ADCx, &ADC_CommonInitStruct);

	/*GPIO 管脚复用配置*/

	ADCPinAFSet(vtADCChls);
	ADC_ClearFlag(DEBUG_ADCx, vtADCChls);							//ADC_INTCLR ADC 中断清除寄存器
	// 开启ADC ，并开始转换
	ADC_ContinuousConverChannelCmd(ADC, vtADCChls, ENABLE);
	ADC_NVIC_Config();
	ADC_AccCmd(ADC, ENABLE);										//开启转换结果累加
	ADC_ClearFlag(ADC, ADC_TICONT_FLAG);
	ADC_ITCmd(DEBUG_ADCx, ADC_TICONT_FLAG, ADC_IT_STATE);			//开启连续转换完成中断
	ADC_AccResultClear(ADC, ENABLE);									//清除累加器
	ADC_ContinuousConverRstCmd(DEBUG_ADCx, ENABLE);
	ADC_ContinuousConverRstCmd(DEBUG_ADCx, DISABLE);
	ADC_Cmd(DEBUG_ADCx, ENABLE);
	ADC_SoftwareStartConv(DEBUG_ADCx);
}

void ADC_NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// 配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_ADC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/****************************************************************************************************************
 *  转换各个已在形参结构体中定义的channel的值,
 *
 * 获取结果通过调用readADCResultFromMutiChls(ADCChlDefType ch)实现
 * 	调用范例
 * 	userAdcStruct.channels=(u8)ADC_NTC_CHL|ADC_MOTOR_CHL|ADC_BATTDET_CHL|ADC_5V_CHL;
* 	mutiChannesADC(&userAdcStruct);
* 	vDaValue=readADCResultFromMutiChls(ADC_MOTOR_CHL);
* 	vDaVolt=vDaValue*vD2A_Coe;
 *****************************************************************************************************************/
//void mutiChannesADC(UserADCTypeDef *adcStruct){
//		ADC_MutiMode_Config(adcStruct->channels);
//		ADC_SoftwareStartConv(DEBUG_ADCx);
//		while(ADC_GetRISFlagStatus(DEBUG_ADCx, ADC_TICONT_FLAG) == RESET);
//		adcStruct->DaValues[0]=DEBUG_ADCx->RESULT0;
//		adcStruct->DaValues[1]=DEBUG_ADCx->RESULT1;
//		adcStruct->DaValues[2]=DEBUG_ADCx->RESULT2;
//		adcStruct->DaValues[3]=DEBUG_ADCx->RESULT3;
//		adcStruct->DaValues[4]=DEBUG_ADCx->RESULT4;
//		adcStruct->DaValues[5]=DEBUG_ADCx->RESULT5;
//		adcStruct->DaValues[6]=DEBUG_ADCx->RESULT6;
//		adcStruct->DaValues[7]=DEBUG_ADCx->RESULT7;
//		// 关闭ADC
//		ADC_Cmd(DEBUG_ADCx, DISABLE);
//}
/****************************************************************************************************************
 *  转换指定通道的ADC结果并返回,
 *
 * 实参取值范围见  IS_ADC_CHANNEL_CMD
 *****************************************************************************************************************/
//u16 getSignalChlADC(u8 channel) {
//	u8 i = 8;				//转换9次
//	u32 vtMin = 0xffff;
//	u32 vtMax = 0;
//	u32 vtTemp;
//	u32 vResultACC = 0;
//	assert_param(IS_ADC_CHANNEL_CMD(channel));
//	ADC_SignalMode_Config(channel);
//	//clr_FlashLED;
//		do{
//		//开始单次转换

//		ADC_SoftwareStartConv(DEBUG_ADCx);
//		while(ADC_GetSoftwareStartConvStatus(DEBUG_ADCx) == SET);			//等待单次转换完成

//			if(i--<8){			//丢弃  第1,2次的数据
//				vtTemp=ADC_GetConversionValue(DEBUG_ADCx);
//				if(vtTemp<=vtMin){
//					vtMin=vtTemp;
//				}
//				if(vtTemp>=vtMax){
//					vtMax=vtTemp;
//				}
//				vResultACC+=vtTemp;
//			}
//		}while(i);
//		// 关闭ADC
//		ADC_Cmd(DEBUG_ADCx, DISABLE);					//关闭ADC模块
//	vResultACC -= vtMin;		//去除一次最小值
//	vResultACC -= vtMax;		//去除一次最大值
//	//set_FlashLED;
//	return vResultACC / 5;
//}
/****************************************************************************************************************
 *
 *  IS_ADC_CHANNEL_CMD  格式转换成  ADC通道编号
 *
 *****************************************************************************************************************/
u8 adcChlCMD2adcIndex(u32 chlCMD){
	u8 i=0;
	assert_param(	IS_ADC_CHANNEL_CMD(chlCMD));
	do{
		if(chlCMD&0x01)
			break;
		chlCMD>>=1;
		i++;
	}while(i<8);
	return i;
}
/****************************************************************************************************************
 *
 *  IS_ADC_CHANNEL_CMD  格式转换成  IS_ADC_CHANNEL
 *
 *****************************************************************************************************************/
u32 adcChlCMD2adcAdcChl(u32 chlCMD){
	u32 vtAdcChl=adcChlCMD2adcIndex(chlCMD);
	vtAdcChl<<=8;
	return vtAdcChl;
}
/********************************************************
 *
 * 获取多通道转换后指定通道的数据
 *
 * * 	调用范例
 * 	userAdcStruct.channels=(u8)ADC_NTC_CHL|ADC_MOTOR_CHL|ADC_BATTDET_CHL|ADC_5V_CHL;
 * 	mutiChannesADC(&userAdcStruct);
 * 	vDaValue=readADCResultFromMutiChls(ADC_MOTOR_CHL);
 * 	vDaVolt=vDaValue*vD2A_Coe;
 *********************************************************/
//u16 readADCResultFromMutiChls(ADCChlDefType ch){
//	return userAdcStruct.DaValues[ch];
//}
u16 readADCResultFromMutiChls(u8 ch) {
	u8 i = 0;
	assert_param(IS_ADC_CHANNEL_CMD(chlCMD));
	i = adcChlCMD2adcIndex(ch);
	return userAdcStruct.DaValues[i];
}


/***********************************************************************************************************
 *  @brief  			尝试开启电池电量通道DA转换
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
void startBattVoltDect(void) {

	ADC_singleContinuousAccMode_config(ADC_BATT_CHL);

}

/***********************************************************************************************************
  *  @brief  			尝试开启温度通道DA转换
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void tryTemperatureConvert(void) {

	ADC_singleContinuousAccMode_config(ADC_TEMP_CHL);				//开始获取温度的DA值

}
/************************************************************************************************************
  *  @brief  			检测是否有被推迟的ADC转换请求,如果有就处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			调用周期建议小于50ms,以防止过多任务累计而产生遗漏.
  ************************************************************************************************************/
void checkAdcConvertRetry(void) {
	if (adcConvertParams.vPospendConvertInfo) {
		for (uint8 i = 0; i < 8; i++) {
			if (adcConvertParams.vPospendConvertInfo & (0x01 << i)) {
				adcConvertParams.callbacks[i](0x01 << i);
				return;
			}
		}
	}
}

//__WEAK void adcBatteryResultCallbacks(uint16 vtTemp){
//
//}
//__WEAK void adcTemperatureResultCallbacks(uint16 vtTemp){
//
//}
/***********************************************************************************************************
  *  @brief			ADC中断处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void DEBUG_ADC_IRQHandler(void) {
	u32 vtTemp = 0;
	if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL0_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL0_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL1_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL1_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL2_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL2_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL3_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL3_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL4_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL4_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL5_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL5_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL6_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL6_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICHANNEL7_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TICHANNEL7_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TILLT_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TILLT_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TIHHT_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TIHHT_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TIREG_FLAG) == SET)
		ADC_ClearFlag(ADC, ADC_TIREG_FLAG);
	else if (ADC_GetMISFlagStatus(ADC, ADC_TICONT_FLAG) == SET) {
		ADC_ClearFlag(ADC, ADC_TICONT_FLAG);
		vtTemp = ADC_GetAccResult(DEBUG_ADCx);
		vtTemp /= ((_ADC_SETADCCNT >> 8) & 0xff) + 1;
		uint8 i = 0;
		for (; i < 8; i++) {
			if ((adcConvertParams.vSingleChlAdcConverting >> i) & 0x01)
				break;
		}
		ADC_ContinuousConverChannelCmd(ADC, adcConvertParams.vSingleChlAdcConverting, DISABLE);
		adcConvertParams.aDaValues[i] = (uint16) vtTemp;
		adcConvertParams.vChlResultGot |= adcConvertParams.vSingleChlAdcConverting;
		adcConvertParams.vSingleChlAdcConverting = 0;
	}
	// 关闭ADC
	ADC_Cmd(DEBUG_ADCx, DISABLE);					//关闭ADC模块
}
/*********************************************END OF FILE**********************/
