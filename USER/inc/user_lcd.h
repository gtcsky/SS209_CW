/*
 * user_lcd.h
 *
 *  Created on: 2019年10月11日
 *      Author: Bough_004
 */

#ifndef SOURCE_INC_USER_LCD_H_
#define SOURCE_INC_USER_LCD_H_
#include	"const.h"
#include	"io_define.h"

#define	LCD_POWER_ON_CONST	20		//20*5=100ms

#define 	SIZE 					16
//#define 	XLevelL					0x02
//#define 	XLevelH					0x10
#define 	Max_Column				128
#define 	Max_Row				64
#define	Brightness				0xFF
#define 	X_WIDTH 				128
#define 	Y_WIDTH 				64
#define 	OLED_CMD  				0			//写命令
#define 	OLED_DATA 				1			//写数据
#define 	OLED_MODE 			0

#define	ICON_Degree_ADDRESS			29+32//59+32
#define	ICON_Arrow_ADDRESS			30+32//60+32
#define	ICON_Colon_ADDRESS			31+32//61+32

#define	ICON_HUES_X					9
#define	ICON_HUES_Y					0
#define	ICON_HUES_ADDRESS			0
#define	Value_Hues_X					ICON_HUES_X+16

#define	Icon_Clock_X						ICON_HUES_X
#define	Icon_Clock_Y						0
#define	Icon_Clock_Table_Addr			10//30
#define	ValueOfClock_X					ICON_HUES_X+16+4


#define	ICON_Saturation_X				ICON_HUES_X
#define	ICON_Saturation_Y				2
#define	ICON_Saturation_ADDRESS		1
#define	Value_Saturation_X				ICON_HUES_X+16

#define	ICON_BRIGHTNESS_X				ICON_HUES_X
#define	ICON_BRIGHTNESS_Y				4
#define	ICON_BRIGHTNESS_ADDRESS		2
#define	Value_Brightness_X				ICON_BRIGHTNESS_X+16
#define	Value_Percent_Addr				25

#define	ICON_ColorTemp_X				ICON_HUES_X
#define	ICON_ColorTemp_Y				6
#define	ICON_ColorTemp_ADDRESS		3
#define	Value_ColorTemp_X				ICON_ColorTemp_X+18

#define	ICON_batt_X0					96
#define	ICON_batt_X0_systemOff			50
#define	ICON_batt_X1					ICON_batt_X0+16
#define	ICON_batt_X1_systemOff			ICON_batt_X0_systemOff+16
#define	ICON_batt_Y						0
#define	ICON_batt_Y_systemOff			4
#define	ICON_batt_lv0_Addr				11
#define	ICON_batt_lv1_Addr				13
#define	ICON_batt_lv2_Addr				15
#define	ICON_batt_lv3_Addr				17
#define	ICON_batt_lv4_Addr				19
#define	ICON_batt_lv5_Addr				21
#define	ICON_batt_lv6_Addr				23

#define	ICON_Hot_X						ICON_batt_X0
#define	ICON_Hot_X1					ICON_batt_X1
#define	ICON_Hot_Y						ICON_batt_Y
#define	ICON_Hot_Addr					28

#define	ICON_Percent_X					ICON_HUES_X
#define	ICON_Percent_Y					0


#define	ICON_charge_X0					ICON_batt_X0-32
#define	ICON_charge_X1					ICON_charge_X0+16
#define	ICON_charge_Y					ICON_batt_Y
#define	ICON_charge_TableAddr			26

#define	ICON_Flash_X					ICON_batt_X0
#define	ICON_Flash_Y					2
#define	ICON_Flash_ADDRESS				4

#define	ICON_Loop_X					ICON_Flash_X
#define	ICON_Loop_Y					4
#define	ICON_Loop_ADDRESS				5

#define	ICON_Lock_X						ICON_Flash_X
#define	ICON_Lock_Y						6
#define	ICON_Lock_ADDRESS				6

#define	ICON_Style1_X					ICON_Flash_X+16
#define	ICON_Style1_Y					ICON_Flash_Y
#define	ICON_Style2_X					ICON_Style1_X
#define	ICON_Style3_X					ICON_Style1_X
#define	ICON_Style2_Y					ICON_Style1_Y+2
#define	ICON_Style3_Y					6
#define	ICON_StyleA_ADDRESS			30
#define	ICON_StyleB_ADDRESS			31
#define	ICON_StyleC_ADDRESS			32
#define	ICON_StyleD_ADDRESS			33
#define	ICON_StyleE_ADDRESS			34
#define	ICON_StyleF_ADDRESS			35

#define	ICON_T_X						ICON_HUES_X+16*3
#define	ICON_T_Y						2
#define	ICON_T_Addr					10

#define	Value_CD_X						ICON_HUES_X+16*2+8
#define	Value_CD_Y						4

#define	Counting_X						ICON_HUES_X
#define	Counting_Y						4

#define	TimerBarX						Counting_X
#define	TimerBarY						6
#define	TimerBarStartAddr				62+32
#define	TimerBarEndX					Counting_X+4+100
#define	TimerBarEndAddr				63+32

#define	LOW_CASE_m					28+32
#define	LOW_CASE_h						27+32

#define	FACTORY_MODE_LEFT_X			8
#define	FACTORY_MODE_RIGHT_X		104
#define	FACTORY_MODE_PROMPT_Y		2
/*********************************************************************
 *
 * LCD上电初始化
 *
 **********************************************************************/
extern	void 	powerOnLcdInit(void);

/*********************************************************************
 *
 * LCD进入休眠状态
 *
 **********************************************************************/
extern	void  	lcdExitSleep(void);

/*********************************************************************
 *
 * LCD进入休眠状态
 *
 **********************************************************************/
extern	void  	lcdEnterSleep(void);

/***********************************************************************************************************
  *  @brief  				电量格显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void 	batterDisplay(u8 level) ;

/***********************************************************************************************************
  *  @brief			充电状态显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void 	updateChargingIcon(void);

/*****************************************************************************************
 *
 *过热显示
 *
 ******************************************************************************************/
extern	void		updateHotDisplay(void);

/***********************************************************************************************************
  *  @brief 			显示电量百分比
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :		0为清除电量显示
  ************************************************************************************************************/
extern	void	 	batteryPercentDisplay(u8 level);

//extern	void 	updateRemainingTimeByValue(u16 data);
//extern	void		updateRemainingTime(displayParamsStruct * disParams);
//extern	void 	updateCountDownDisplay(displayParamsStruct * disParams);
/*****************************************************************************************
 *
 * 色温值显示
 *
 ******************************************************************************************/
extern	void 	updateColorTempDisplay(displayParamsStruct * disParams) ;

/*****************************************************************************************
 *
 * 亮度值显示
 *
 ******************************************************************************************/
extern	void 	updateBrightnessDisplay(displayParamsStruct * disParams);

/*****************************************************************************************
 *
 * 饱和度值显示
 *
 ******************************************************************************************/
extern	void 	updateSaturationDisplay(displayParamsStruct * disParams);

/*****************************************************************************************
 *
 * 色调值显示
 *
 ******************************************************************************************/
extern	void 	updateHuesDisplay(displayParamsStruct * disParams);

/*****************************************************************************************
 *
 * 待机画面显示
 *
 ******************************************************************************************/
extern	void 	displaySystemMenu(displayParamsStruct * disParams);

/*****************************************************************************************
 *
 * 光标位置显示
 *
 ******************************************************************************************/
extern	void 	updateArrowDisplay(displayParamsStruct * disParams);

/*****************************************************************************************
 *
 *灯效样式显示
 *
 ******************************************************************************************/
extern	void 	updateLightEffectDisplay(displayParamsStruct * disParams);

/***********************************************************************************************************
  *  @brief  				显示汉字
  *
  *  @param [in] :			x:0~127
  *						y:0~7
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void 	OLED_ShowCHinese(u8 x,u8 y,u8 no);

/***********************************************************************************************************
  *  @brief  				显示字符串
  *
  *  @param [in] :			x:0~127
  *						y:0~7
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void 	OLED_ShowString(u8 x,u8 y,u8 *chr);

/***********************************************************************************************************
  *  @brief				显示数字
  *
  *  @param [in] :			x:0~127
  *						y:0~7
  *  @param [out] :
  *
  *  @return :				数字长度
  *
  *  @note :
  ************************************************************************************************************/
u8 		OLED_ShowNum(u8 x,u8 y,u32 num);


u32 		oled_pow(u8 m,u8 n);
/***********************************************************************************************************
  *  @brief				在指定位置显示一个字符,包括部分字符
  *
  *  @param [in] :			x:0~127
  *						y:0~7
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void 	OLED_ShowChar(u8 x,u8 y,u8 chr);



void 	OLED_ShowOneSegment(u8 x,u8 y,u8  data1,u8 data2);
/***********************************************************************************************************
  *  @brief  			定位OLED 显示坐标
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
void 	OLED_Set_Pos(unsigned char x, unsigned char y);
/***********************************************************************************************************
  *  @brief  				清除所有LCD显示
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
extern	void clear(void);
/***********************************************************************************************************
  *  @brief		系统版本号显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void  versionDisplay(void);
extern	void  temperatureDisplay(void);
/***********************************************************************************************************
  *  @brief			测试模式中特定显示画面
  *
  *  @param [in] :		画面编号
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void	displayFactoryMode(u8	item);
/***********************************************************************************************************
  *  @brief		十六进制数字显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void HexDigitDis(u8 x,u8 y,u8 value);

/***********************************************************************************************************
  *  @brief		测试模式温度显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	void	 factoryModeTemperatureDisplay(void);

/***********************************************************************************************************
  *  @brief			测试模式电压显示
  *
  *  @param [in] :		待显示电压最大值为9.99v
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			大于9.99v的电压需要折算成单节电池电压显示
  ************************************************************************************************************/
extern	void	 factoryVoltDisplay(float volt);

/***********************************************************************************************************
  *  @brief			写入8条COM线的数据
  *
  *  @param [in] :		data=0:		Clear
  *  						!=0:		On
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			从指定位置开始,写到最后
  ************************************************************************************************************/
extern	void OLED_ShowOnePage(uint8 x, uint8 y, uint8 data);

#endif /* SOURCE_INC_USER_LCD_H_ */
