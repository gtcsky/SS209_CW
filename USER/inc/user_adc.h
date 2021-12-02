#ifndef __ADC_H
#define	__ADC_H

#include "cx32l003_adc.h"
#include "cx32l003_rcc.h"
#include "cx32l003_gpio.h"
#include "cx32l003_spi.h"
#include "cx32l003_lpuart.h"
#include "cx32l003_uart.h"
#include "cx32l003_uart.h"
#include "const.h"
#include "user_menu.h"

// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    DEBUG_ADCx         					ADC
#define   _ADC_CIRCLEMODE     				ADC_Mode_Independent
#define	  _ADC_CTMODE         					ADC_Ct_Contine
#define	  _ADC_SETADCCNT      				0x00000F00//0x00000100
//#define	  _ADC_SETADCCNT_SIGNAL      		0x00000300//0x00000100
//#define	  _ADC_SETADCCNT_SIGNAL      		0x00000500//0x00000100
//#define	  _ADC_SETADCCNT_SIGNAL      		0x0000FF00//	255次
#define   _ADC_TRIGS0SEL      					ADC_Trigs0_DISABLE
#define 	_ADC_TRIGS1SEL      					ADC_Trigs1_DISABLE
	
#define   _ADC_ChannelSel     					ADC_Channel0
#define   _ADC_CLKSEL         					ADC_Prescaler_Div1
#define   _ADC_SAMSEL         					ADC_SAMPLE_4CYCLE

// ADC GPIO宏定义
#define    ADC_GPIOx                     				GPIOC
#define    ADC_PIN_SOURCE                			GPIO_PinSource6
#define    ADC_AF_VALUE                  				GPIO_AF_AIN0_PC6


#define    ADC_IT_FLAG                   				ADC_TICHANNEL0_FLAG
#define    ADC_IT_STATE                  				ENABLE//DISABLE

#define    ADC_SOFTWARECONV_ENABLE       	ENABLE
#define    ADC_SOFTWARECONV_CYCLE           	12

#define    ADC_EXITTRIE_SEL                         		0x01

#define     ADC_CHANNEL_EN                			ADC_Channel0_ENABLE


// ADC 中断相关宏定义
#define    DEBUG_ADC_IRQ                       			ADC0_IRQn
#define    DEBUG_ADC_IRQHandler                   	ADC0_IRQHandler


extern	adcConvertStruct adcConvertParams;


//-----------------define function--------------

/***********************************************************************************************************
  *  @brief  		start convert DA value of Battery Detect
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
extern	void startBattVoltDect(void);

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
void tryTemperatureConvert(void);

/***********************************************************************************************************
  *  @brief  		检测是否有被推迟的ADC转换请求,如果有就处理
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
void checkAdcConvertRetry(void);

/***********************************************************************************************************
  *  @brief     				单通道连续累加转换模式
  *
  *  @param [in] :			试图开启转换的通道
  *
  *  @param [out] :			true:开启转换成功
  *  						false:开启新的转换失败，新的转换任务会被加入待准换队列
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern uint8 ADC_singleContinuousAccMode_config(u8 vtADCChl);

//void ADC_MutiMode_Config(u8 vtADCChls);
void ADC_SignalMode_Config(u8 vtADCChl);
void ADC_NVIC_Config(void);
//void mutiChannesADC(UserADCTypeDef *adcStruct);
//u16 readADCResultFromMutiChls(ADCChlDefType ch);
u16 readADCResultFromMutiChls(u8 ch);
u16 getSignalChlADC(u8 channel);
u32 adcChlCMD2adcAdcChl(u32 chlCMD);
returnStatus checkAd(void);
extern returnStatus idleAdDect(void);
extern void adcThresholdGenerate(void);
extern void ADC_MutiMode_Config(u8 vtADCChls);
extern u8 adcChlCMD2adcIndex(u32 chlCMD);
#endif /* __ADC_H */

