/*
 * user_lcd.c
 *	LCD driver for SSD1106
 *
 *  	Created on: 2021年07月20日
 *      Author: Bough_004
 */

#include "user_lcd.h"
#include "user_ddl.h"
#include "user_i2c.h"
#include "user_adc.h"
#include "user_lptimer.h"
#include "user_character.h"
#include "user_menu.h"

u8	arrowIndex;
void lcdGpioInit(void) {

//	GPIO_PinAFConfig(SPI0_SCK_PORT,SPI0_SCK_PinSource,SPI0_SCK_FuncSel);
//	GPIO_PinAFConfig(SPI0_MOSI_PORT,SPI0_MOSI_PinSource,SPI0_MOSI_FuncSel);
//	GPIO_PinAFConfig(SPI0_CS_PORT,SPI0_CS_PinSource,SPI0_CS_FuncSel);
	set_LcdResPin;
}

//void writeLcdCommand(u8 cmd) {
////	return;
////	u8 vtTemp=cmd;
////	clr_LcdDcPin;
////	WriteData(&vtTemp,1);
////	set_LcdDcPin;
//	IC_IIC_Start();
//	Write_IIC_Data(LCD_SLAVE_ADDR);                	//Salve Adress
//	Write_IIC_Data(OP_Command);                   	//写命令
//	Write_IIC_Data(cmd);
//	IC_IIC_Stop();
//}
//
//void writeLcdData(u8 data) {
////	return;
////	u8 vtTemp=data;
////	set_LcdDcPin;
////	//return;
////	WriteData(&vtTemp,1);
//	IC_IIC_Start();
//	Write_IIC_Data(LCD_SLAVE_ADDR);                	//Salve Address
//	Write_IIC_Data(OP_Data);                      		//写数据
//	Write_IIC_Data(data);
//	IC_IIC_Stop();
//}
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
void clear(void) {
#if(LCD_INSIDE==1)
//	unsigned char x, y;
//	fIsChargeDisplaying = 0;
//	writeLcdCommand(0x00);
//	writeLcdCommand(0x10); /*set higher column address*/
//
//	for (y = 0; y < 8; y++) {
//		writeLcdCommand(0xB0 + y); /*set page address*/
//		writeLcdCommand(0x00);
//		writeLcdCommand(0x10);
//		for (x = 0; x < 130; x++)
//			writeLcdData(0x00);
//		writeLcdData(0x00);
//	}
	uint8 y=0;
	for(;y<8;y++){
		OLED_ShowOnePage(0,y,0x00);
	}
#endif
}
///*********************************************************************
// *
// * LCD进入关机状态
// *
// **********************************************************************/
//void lcdEnterPowerDownMode(void){
//
//}

/*********************************************************************
 *
 * LCD进入休眠状态
 *
 **********************************************************************/
void  lcdEnterSleep(void){
#if(LCD_INSIDE==1)
	clear();
	writeLcdCommand(0xAE); /*display off*/
	writeLcdCommand(0x8d); /*set charge pump disable*/
	writeLcdCommand(0x10);		//使用外置DC
	delay100us(5);
//	LcdPowerOff;
#endif
}
/*********************************************************************
 *
 * LCD退出休眠状态
 *
 **********************************************************************/
void  lcdExitSleep(void){
#if(LCD_INSIDE==1)
//	LcdPowerOn;
	sysTimes.vSystem5ms = 0;
	while (sysTimes.vSystem5ms <= LCD_POWER_ON_CONST);
	sysTimes.vSystem5ms = 0;
	writeLcdCommand(0xAE); 		//display off
	writeLcdCommand(0x8d);		//set charge pump disable
	writeLcdCommand(0x14);		//使用内置DC
	writeLcdCommand(0xAF); 		//display ON
	while (sysTimes.vSystem5ms <= LCD_POWER_ON_CONST);
	sysTimes.vSystem5ms = 0;
#endif
}
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
void batteryPercentDisplay(u8 level) {
#if(LCD_INSIDE==1)
	if (!level) {
		OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, ' ');
		OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, ' ');
		OLED_ShowChar(ICON_Percent_X + 16, ICON_Percent_Y, ' ');
		OLED_ShowChar(ICON_Percent_X + 24, ICON_Percent_Y, ' ');
		OLED_ShowChar(ICON_Percent_X + 32, ICON_Percent_Y, ' ');
	} else {

		if (level != 100) {
			OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, ' ');
			OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, 9 + '0');
		} else {
			OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, '1');
			OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, '0');
		}
		OLED_ShowChar(ICON_Percent_X + 16, ICON_Percent_Y, level % 10 + '0');
		OLED_ShowCHinese(ICON_Percent_X + 24, ICON_Percent_Y, Value_Percent_Addr);
	}
#endif
}

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
void updateChargingIcon(void){
#if(LCD_INSIDE==1)
	if(fIsCharging&&!fIsChargeDisplaying){
		if(!fIsSystemOff){
			fIsChargeDisplaying=1;
		}else{
			if(displayParams.DisplayModeIndex==ChargingAtPowerDown){
				fIsChargeDisplaying=1;						//当LCD处于唤醒状态时，才更新显示状态
			}
		}
	}
	else if(!fIsCharging&&fIsChargeDisplaying){
		if(!fIsSystemOff){
			fIsChargeDisplaying=0;
//			OLED_ShowString(ICON_charge_X0,ICON_charge_Y,"    ");
		}
	}
#endif
}

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
void batterDisplay(u8 level) {
#if(LCD_INSIDE==1)
	u8 vtBattIconTableAddr = 0;
	if (level > Max_Batt_level)
	level = Max_Batt_level;
	switch (level) {
		case 0:
		vtBattIconTableAddr = ICON_batt_lv0_Addr;
		break;
		case 1:
		vtBattIconTableAddr = ICON_batt_lv1_Addr;
		break;
		case 2:
		vtBattIconTableAddr = ICON_batt_lv2_Addr;
		break;
		case 3:
		vtBattIconTableAddr = ICON_batt_lv3_Addr;
		break;
		case 4:
		vtBattIconTableAddr = ICON_batt_lv4_Addr;
		break;
		case 5:
		vtBattIconTableAddr = ICON_batt_lv5_Addr;
		break;
		case 6:
		vtBattIconTableAddr = ICON_batt_lv6_Addr;
		break;
	}
	OLED_ShowCHinese(ICON_batt_X0, ICON_batt_Y, vtBattIconTableAddr);
	OLED_ShowCHinese(ICON_batt_X1, ICON_batt_Y, vtBattIconTableAddr + 1);
	updateChargingIcon();
#endif
}

/*****************************************************************************************
 *
 * 光标位置显示
 *
 ******************************************************************************************/
void updateArrowDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	for (uint8 i = 0; i <= Max_Arrow_Index; i++) {
		if (i == disParams->arrowIndex) {
//			if (disParams->fIsBrightnessAdj) {
//				OLED_ShowChar(ArrowArray[disParams->arrowIndex][0], ArrowArray[disParams->arrowIndex][1], ' ');
//			} else {
				OLED_ShowChar(ArrowArray[disParams->arrowIndex][0], ArrowArray[disParams->arrowIndex][1], ICON_Arrow_ADDRESS);
//			}
		} else {
			OLED_ShowChar(ArrowArray[i][0], ArrowArray[i][1], ' ');

		}
	}
//	if (disParams->fIsBrightnessAdj)
//		OLED_ShowChar(ArrowArray[2][0], ArrowArray[2][1], ICON_Arrow_ADDRESS);
#endif
}
/*****************************************************************************************
 *
 * 色调值显示
 *
 ******************************************************************************************/
#if(RGB_INSIDE==1)
void updateHuesDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	u8 len = 0;
	//OLED_ShowString(Value_Hues_X,ICON_HUES_Y,"    ");
	len = OLED_ShowNum(Value_Hues_X, ICON_HUES_Y, disParams->hues);
	OLED_ShowChar(Value_Hues_X + 8 * len, ICON_HUES_Y, ICON_Degree_ADDRESS);
	if (len < 3)
		OLED_ShowChar(Value_Hues_X + 8 * 3, ICON_HUES_Y, ' ');
	if (len < 2)
		OLED_ShowChar(Value_Hues_X + 8 * 2, ICON_HUES_Y, ' ');
#endif
}
#endif
/*****************************************************************************************
 *
 * 饱和度值显示
 *
 ******************************************************************************************/
#if(RGB_INSIDE==1) 
void updateSaturationDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	u8 len = 0;
	len = OLED_ShowNum(Value_Saturation_X, ICON_Saturation_Y, disParams->saturation);
	if (len < 3)
		OLED_ShowChar(Value_Saturation_X + 8 * 2, ICON_Saturation_Y, ' ');
	if (len < 2)
		OLED_ShowChar(Value_Saturation_X + 8 * 1, ICON_Saturation_Y, ' ');
#endif
}
#endif
/*****************************************************************************************
 *
 * 亮度值显示
 *
 ******************************************************************************************/
void updateBrightnessDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	u8 len = 0;
	len = OLED_ShowNum(Value_Brightness_X, ICON_BRIGHTNESS_Y, disParams->brightness);
	OLED_ShowCHinese(Value_Brightness_X + 8 * len, ICON_BRIGHTNESS_Y, Value_Percent_Addr);
	if (len < 3)
		OLED_ShowChar(Value_Brightness_X + 8 * 4, ICON_BRIGHTNESS_Y, ' ');
	if (len < 2)
		OLED_ShowChar(Value_Brightness_X + 8 * 3, ICON_BRIGHTNESS_Y, ' ');
#endif
}
/*****************************************************************************************
 *
 * 色温值显示
 *
 ******************************************************************************************/
void updateColorTempDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	u8 len = 0;
	len = OLED_ShowNum(Value_ColorTemp_X, ICON_ColorTemp_Y, disParams->colorTemperature * 100);
	OLED_ShowChar(Value_ColorTemp_X + 8 * len, ICON_ColorTemp_Y, 'K');
#endif
}
/*****************************************************************************************
 *
 *灯效样式显示
 *
 ******************************************************************************************/
#if(RGB_INSIDE==1)
void updateLightEffectDisplay(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	if (Style1Setting == disParams->arrowIndex)
		OLED_ShowCHinese(ICON_Style1_X, ICON_Style1_Y, ICON_StyleA_ADDRESS + disParams->style1Value);
	else if (Style2Setting == disParams->arrowIndex)
		OLED_ShowCHinese(ICON_Style2_X, ICON_Style2_Y, ICON_StyleA_ADDRESS + disParams->style2Value);
	else if (Style3Setting == disParams->arrowIndex)
		OLED_ShowCHinese(ICON_Style3_X, ICON_Style3_Y, ICON_StyleA_ADDRESS + disParams->style3Value);
#endif
}
#endif
/*****************************************************************************************
 *
 *计时条更新
 *
 ******************************************************************************************/
/*
void updateTimerBar(displayParamsStruct * disParams) {
	u8 i;
	OLED_ShowChar(TimerBarX, TimerBarY, TimerBarStartAddr);
	for(i=0;i<MAX_CD_TIMER;i++){
		if(i==0)
			OLED_Set_Pos(TimerBarX+4,TimerBarY);
		if(disParams->countDownTimer>i){
			writeLcdData(0xEC);
		}
		else
			//OLED_ShowOneSegment(TimerBarX+3+i,TimerBarY,0x0C,0x30);
			writeLcdData(0x0C);
	}
	for(i=0;i<MAX_CD_TIMER;i++){
		if(i==0)
			OLED_Set_Pos(TimerBarX+4,TimerBarY+1);
		if(disParams->countDownTimer>i){
			writeLcdData(0x37);
		}
		else
			writeLcdData(0x30);
	}
	OLED_ShowChar(TimerBarEndX, TimerBarY, TimerBarEndAddr);
}
*/
/***********************************************************************************************************
  *  @brief  					浮点小数显示
  *
  *
  *  @param [in] : 				x,y:起始坐标
  * 								x:0~127
  *								y:0~7
  * 							size: 小数点后显示几位
  * 							per:显示单位
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void displayFloat(UINT8 x, UINT8 y, float fNum, UINT8 size, UINT8 per) {
#if(LCD_INSIDE==1)
	UINT16 vtInt = 0;
	UINT8 len = 0, dotPos = 0, i = 0;
	vtInt = (UINT16) fNum;
	len = OLED_ShowNum(x, y, vtInt);
	dotPos = x + 8 * len;
	if (size) {
		OLED_ShowChar(dotPos, y, '.');
		for (i = 1; i <= size; i++) {
			OLED_ShowChar(dotPos + 5 + (i - 1) * 8, y, ((u16) (fNum * oled_pow(10, i))) % 10 + '0');
		}
		OLED_ShowChar(dotPos + 5 + (size) * 8, y, per);
	} else {
		OLED_ShowChar(dotPos, y, per);
		OLED_ShowChar(dotPos + 8, y, ' ');
	}
	if (size <= 2) {
		OLED_ShowString(dotPos + 5 + (size + 1) * 8, y, " ");
	}
#endif
}
/*****************************************************************************************
 *
 *剩余时间显示
 *
 ******************************************************************************************/
//
//void updateRemainingTimeByValue(u16 data){
//	float vtTimes=0;
//	displayParams.remainingTime=data;
//	if(data<60){
//		displayFloat(ValueOfClock_X,Icon_Clock_Y,data,0,LOW_CASE_m);
//	}else if(data<6000){						//<100h
//		vtTimes=(float) (data)/60;
//		displayFloat(ValueOfClock_X,Icon_Clock_Y,vtTimes+0.05,1,LOW_CASE_h);
//	}else{
//		vtTimes=data/60;						//>100小时,不显示小数
//		displayFloat(ValueOfClock_X,Icon_Clock_Y,(u16)vtTimes,0,LOW_CASE_h);
//	}
//}
//
//void	updateRemainingTime(displayParamsStruct * disParams){
//	updateRemainingTimeByValue(disParams->remainingTime);
//}
/*****************************************************************************************
 *
 *过热显示
 *
 ******************************************************************************************/
void updateHotDisplay(void){
#if(LCD_INSIDE==1)
	if(fIsHotNow){
		if(!fIsHotDisplayOn){
			OLED_ShowCHinese(ICON_Hot_X, ICON_Hot_Y,ICON_Hot_Addr);
			OLED_ShowCHinese(ICON_Hot_X1, ICON_Hot_Y,ICON_Hot_Addr+1);
			fIsHotDisplayOn=TRUE;
		}else{
			OLED_ShowCHinese(ICON_Hot_X, ICON_Hot_Y,ICON_batt_lv0_Addr);
			OLED_ShowCHinese(ICON_Hot_X1, ICON_Hot_Y,ICON_batt_lv0_Addr+1);
			fIsHotDisplayOn=FALSE;
		}
	}
#endif
}

/*****************************************************************************************
 *
 * 待机画面显示
 *
 ******************************************************************************************/
void displaySystemMenu(displayParamsStruct * disParams) {
#if(LCD_INSIDE==1)
	clear(); 										/* Clear Screen */
#if(RGB_INSIDE==1)  
	batterDisplay(disParams->battIndex);
#endif  
//	updateHotDisplay();
	if (IdleIamgeDisplay==disParams->DisplayModeIndex ) {
		OLED_ShowChar(ArrowArray[disParams->arrowIndex][0], ArrowArray[disParams->arrowIndex][1], ICON_Arrow_ADDRESS);
#if(RGB_INSIDE==1)    
		OLED_ShowCHinese(ICON_HUES_X, ICON_HUES_Y, ICON_HUES_ADDRESS);//色调图标
		updateHuesDisplay(disParams);
		OLED_ShowCHinese(ICON_Saturation_X, ICON_Saturation_Y, ICON_Saturation_ADDRESS);				//饱和度图标
		updateSaturationDisplay(disParams);
#endif    
		OLED_ShowCHinese(ICON_BRIGHTNESS_X, ICON_BRIGHTNESS_Y, ICON_BRIGHTNESS_ADDRESS);			//亮度图标
		updateBrightnessDisplay(disParams);
		OLED_ShowCHinese(ICON_BRIGHTNESS_X, ICON_ColorTemp_Y, ICON_ColorTemp_ADDRESS);			//色温图标
		updateColorTempDisplay(disParams);
#if(RGB_INSIDE==1)
		OLED_ShowCHinese(ICON_Flash_X, ICON_Flash_Y, ICON_Flash_ADDRESS);							//右侧闪光灯图标
		OLED_ShowCHinese(ICON_Style1_X, ICON_Style1_Y, ICON_StyleA_ADDRESS+disParams->style1Value);

		OLED_ShowCHinese(ICON_Loop_X, ICON_Loop_Y, ICON_Loop_ADDRESS);								//右侧循环图标
		OLED_ShowCHinese(ICON_Style2_X, ICON_Style2_Y, ICON_StyleA_ADDRESS+disParams->style2Value);

		OLED_ShowCHinese(ICON_Lock_X, ICON_Lock_Y, ICON_Lock_ADDRESS);								//右侧警报图标
		OLED_ShowCHinese(ICON_Style3_X, ICON_Style3_Y, ICON_StyleA_ADDRESS+disParams->style3Value);
#endif
	}else if(ModeTDisplay==disParams->DisplayModeIndex){
		OLED_ShowCHinese(ICON_BRIGHTNESS_X, ICON_BRIGHTNESS_Y, ICON_BRIGHTNESS_ADDRESS);			//亮度图标
		updateBrightnessDisplay(disParams);
		OLED_ShowCHinese(ICON_BRIGHTNESS_X, ICON_ColorTemp_Y, ICON_ColorTemp_ADDRESS);			//色温图标
		updateColorTempDisplay(disParams);
	}else if(CountDownDisplay==disParams->DisplayModeIndex){
//		OLED_ShowCHinese(ICON_T_X, ICON_T_Y, ICON_T_Addr);					//反显T图标
//		OLED_ShowString(Value_CD_X, Value_CD_Y, "    ");
	}else if(ChargingAtPowerDown==disParams->DisplayModeIndex){
		return;
	}
#endif
}

/*********************************************************************
 *
 * LCD上电初始化
 *
 **********************************************************************/
void powerOnLcdInit(void) {
//	userSpiInit();
	lcdGpioInit();
	set_LcdResPin;
	delay100us(10);
	clr_LcdResPin;
	delay100us(10);
	set_LcdResPin;
	delay100us(10);
	writeLcdCommand(0xAE); /*display off*/

	writeLcdCommand(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency
	writeLcdCommand(0x80);

	writeLcdCommand(0xA8);	//Set Multiplex Ratio
	writeLcdCommand(0x3F);

	writeLcdCommand(0xD3); 	/*set display offset*/
	writeLcdCommand(0x00);						//设置后产生上下画面移动

	writeLcdCommand(0x40); 	//set Start Line		设置后产生上下画面移动

	writeLcdCommand(0xAD); //Set Charge Pump
	writeLcdCommand(0x8B);	//使用内置DC

	writeLcdCommand(0xA1); 	/*set segment remap*/
	writeLcdCommand(0xC8); 	/*Com scan direction*/		//正向显示

//	writeLcdCommand(0xA0); /*set segment remap*/
//	writeLcdCommand(0xC0); /*Com scan direction*/		//逆向显示

	writeLcdCommand(0xDA); 	/*set COM pins*/
	writeLcdCommand(0x12);

	writeLcdCommand(0x81); 	//Contrast Control
	writeLcdCommand(0xFF);

	writeLcdCommand(0xD9); 	//Set pre-charge period
	writeLcdCommand(0x1F);

	writeLcdCommand(0xDB); 	//Vcom Deselect Mode
	writeLcdCommand(0x40);

	writeLcdCommand(0x33);	//set VPP

	writeLcdCommand(0xA6);	//Set Normal/Inverse Display

//	writeLcdCommand(0x21); 	//set column start addr
//	writeLcdCommand(0x10); 	//从00开始.
//	writeLcdCommand(0x50); 	//到7F(127)结束.
//	writeLcdCommand(0x22); 	//set column start addr
//	writeLcdCommand(0x20); 	//从00开始.
//	writeLcdCommand(0x50); 	//到7F(127)结束.

	clear(); /* Clear Screen */
//	delay100us(10);
	writeLcdCommand(0xAF); /*display ON*/
	//---------------------------------------------------------
//	writeLcdCommand(0xAE); /*display off*/
//
//	writeLcdCommand(0x02); 	/*set lower column address*/
//	writeLcdCommand(0x10);	/*set higher column address*/
//	writeLcdCommand(0x40);	/*set display start line*/
//	writeLcdCommand(0xB0);	/*set page address*/
//	writeLcdCommand(0x81); 	/*contract control*/
//	writeLcdCommand(0xFF);	/*128*/
//	writeLcdCommand(0xA1); 	/*set segment remap*/
//	writeLcdCommand(0xA6); 	/*normal / reverse*/
//	writeLcdCommand(0xA8);	/*multiplex ratio*/
//	writeLcdCommand(0x3F); 	/*duty = 1/64*/
//	writeLcdCommand(0xAD); 	/*set charge pump enable*/
//	writeLcdCommand(0x8B); 	/* 0x8B 内部升压 */
//	writeLcdCommand(0x33);	/*0X30---0X33 set VPP 9V */
//	writeLcdCommand(0xC8); 	/*Com scan direction*/
//	writeLcdCommand(0xD3);	/*set display offset*/
//	writeLcdCommand(0x00); 	/* 0x20 */
//	writeLcdCommand(0xD5);	/*set osc division*/
//	writeLcdCommand(0x80);
//	writeLcdCommand(0xD9);	/*set pre-charge period*/
//	writeLcdCommand(0x1F);	/*0x22*/
//	writeLcdCommand(0xDA);	/*set COM pins*/
//	writeLcdCommand(0x12);
//	writeLcdCommand(0xDB);	/*set vcomh*/
//	writeLcdCommand(0x40);
//	writeLcdCommand(0x81); 	/*contract control*/
//	writeLcdCommand(0xFF);	/*128*/
//	writeLcdCommand(0xDB);	/*set vcomh*/
//	writeLcdCommand(0x40);
//
//	clear(); /* Clear Screen */
//	delay100us(50);
//	writeLcdCommand(0xAF); /*display ON*/
	//---------------------------------------------------------
//	writeLcdCommand(0xAE); //Set Display Off
//	writeLcdCommand(0xAE); //Set Display Off
//	writeLcdCommand(0xD5); //Display divide ratio/osc. freq. mode
//	writeLcdCommand(0x80); //
//	writeLcdCommand(0xA8); //Multiplex ration mode:63
//	writeLcdCommand(0x3F);
//	writeLcdCommand(0xD3); //Set Display Offset
//	writeLcdCommand(0x00);
//	writeLcdCommand(0x40); //Set Display Start Line
//	writeLcdCommand(0xAD); //DC-DC Control Mode Set
//	writeLcdCommand(0x8B); //8A:External 8b:Built-in DC-DC is used
//	writeLcdCommand(0x32); //Set Pump voltage value
//	writeLcdCommand(0xA1); //Segment Remap
//	writeLcdCommand(0xC8); //Set COM Output Scan Direction
//	writeLcdCommand(0xDA); //Common pads hardware: alternative
//	writeLcdCommand(0x12);
//	writeLcdCommand(0x81); //Contrast control
//	writeLcdCommand(0xff);
//	writeLcdCommand(0xD9); //Set pre-charge period
//	writeLcdCommand(0x1F);
//	writeLcdCommand(0xDB); //VCOM deselect level mode
//	writeLcdCommand(0x40);
//	writeLcdCommand(0xA4); //Set Entire Display On/Off
//	writeLcdCommand(0xA6); //Set Normal Display
//	writeLcdCommand(0x81); //Contrast control
//	writeLcdCommand(0xff);
//	clear();
//	writeLcdCommand(0xAF); //Set Display On

	sysTimes.vSystem5ms = 0;
	while (sysTimes.vSystem5ms <= LCD_POWER_ON_CONST);
	sysTimes.vSystem5ms = 0;
	if(!fIsSystemOff){
		displaySystemMenu(&displayParams);
	}
//	writeLcdCommand(0xA4);		//LCD显示内容跟随RAM
//	writeLcdCommand(0xA5);		//LCD全屏点亮,无视RAM


//-------------------------------------------------------------------------
//	横向滚屏设置
//-------------------------------------------------------------------------
//	writeLcdCommand(0x26);		//设置水平向右移动
////	writeLcdCommand(0x27);		//设置水平向左移动
////	writeLcdCommand(0x00)		//空字节 SS1106不需要,SS1136需要
//	writeLcdCommand(0x00);		//起始y轴(含)
//	writeLcdCommand(0x07);		//移动速度
//	writeLcdCommand(0x03);		//结束y轴(含)		//0~1 即移动第一行
//	writeLcdCommand(0x2F);		//激活滚屏

}


//void resetLcdSetting(void) {
//
//	writeLcdCommand(0x81); 		/*contract control*/
//	writeLcdCommand(0xFF); 		/*128*/
//	writeLcdCommand(0xDB);		/*set vcomh*/
//	writeLcdCommand(0x40);
//	writeLcdCommand(0xAF); 		/*display ON*/
//}
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
void OLED_Set_Pos(unsigned char x, unsigned char y) {
	writeLcdCommand(0xb0 + y);
	writeLcdCommand(((x & 0xf0) >> 4) | 0x10);
//	writeLcdCommand((x & 0x0f) | 0x01);
	writeLcdCommand((x & 0x0f) );
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
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
void OLED_ShowChar(u8 x, u8 y, u8 chr) {
#if(LCD_INSIDE==1)
	unsigned char c = 0, i = 0;
	c = chr - ' ';	//得到偏移后的值
	if (x > Max_Column - 1) {
		x = 0;
		y = y + 2;
	}
	OLED_Set_Pos(x, y);
	for (i = 0; i < 8; i++)
		writeLcdData(F8X16[c * 16 + i]);
	OLED_Set_Pos(x, y + 1);
	for (i = 0; i < 8; i++)
		writeLcdData(F8X16[c * 16 + i + 8]);
#endif
}
/**********************************************************************
 *
 * 写单条Segment 数据(16条com线)数据
 *
 ***********************************************************************/
void OLED_ShowOneSegment(u8 x, u8 y, u8 data1, u8 data2) {

	OLED_Set_Pos(x, y);
	writeLcdData(data1);
	OLED_Set_Pos(x, y + 1);
	writeLcdData(data2);

}
/***********************************************************************************************************
  *  @brief			写入条8COM线的数据
  *
  *  @param [in] :		data=0:		Clear
  *  						!=0:		On
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			从指定位置开始,写到最后
  ************************************************************************************************************/
void OLED_ShowOnePage(uint8 x, uint8 y, uint8 data) {
	uint8 i = 0;
	OLED_Set_Pos(x, y);
	do {
		if (data)
			writeLcdData(data);
		else
			writeLcdData(0);
	} while (i++ < (Max_Column + 3));
}

//m^n函数
uint32 oled_pow(uint8 m, uint8 n) {
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}

/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
uint8 getNumLen(uint32 num) {
	u8 len = 1;
	while (1) {
		if (num < 10)
			return len;
		else
			num /= 10;
		len++;
	}
}
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
#if(LCD_INSIDE==1)
u8 OLED_ShowNum(u8 x, u8 y, u32 num) {
	u8 t, temp;
	u8 enshow = 0;
	u8 len = getNumLen(num);
	for (t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				OLED_ShowChar(x + 8 * t, y, ' ');
				continue;
			} else
				enshow = 1;
		}
		OLED_ShowChar(x + 8 * t, y, temp + '0');
	}
	return len;
}
#endif

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
void OLED_ShowString(u8 x, u8 y, u8 *chr) {
#if(LCD_INSIDE==1)
	unsigned char j = 0;
	while (chr[j] != '\0') {
		OLED_ShowChar(x, y, chr[j]);
		x += 8;
		if (x > 120) {
			x = 0;
			y += 2;
		}
		j++;
	}
#endif
}
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
void OLED_ShowCHinese(u8 x, u8 y, u8 no) {
#if(LCD_INSIDE==1)
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * no][t]);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * no + 1][t]);
		adder += 1;
	}
#endif
}


///***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
// {
//	unsigned int j = 0;
//	unsigned char x, y;
//
//	if (y1 % 8 == 0)
//		y = y1 / 8;
//	else
//		y = y1 / 8 + 1;
//	for (y = y0; y < y1; y++) {
//		OLED_Set_Pos(x0, y);
//		for (x = x0; x < x1; x++) {
//			writeLcdData(BMP[j++]);
//		}
//	}
//}
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
void HexDigitDis(u8 x,u8 y,u8 value){
#if(LCD_INSIDE==1)
	u8 vtTemp;
	vtTemp=(value>>4)&0x0F;
	if(vtTemp<=9)
		OLED_ShowChar(x, y,vtTemp+'0');
	else
		OLED_ShowChar(x, y,vtTemp+'A'-10);

	vtTemp=value&0x0F;
	if(vtTemp<=9)
		OLED_ShowChar(x+8, y,vtTemp+'0');
	else
		OLED_ShowChar(x+8, y,vtTemp+'A'-10);
#endif
}

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
void versionDisplay(void) {
#if(LCD_INSIDE==1)
	clear();
	OLED_ShowString(8, 2, SOFTWARE_VERSION);
	OLED_ShowString(8, 4, HARDWARE_VERSION);
#endif
}

#if(DEBUG_VOLT_INFO==1)
void temperatureDisplay(void) {
#if(LCD_INSIDE==1)
	u8 vtTemperature_X = 0;
	u8 vtTemperature_Y = 0;
	u8 vtVolt_X = 0;
	u8 vtVolt_Y = 0;
	u8 vtCVolt_X = 0;
	u8 vtCVolt_Y = 0;
	fIsSystemTempGot=TRUE;
	if (fIsSystemTempGot) {
		fIsSystemTempGot = 0;
		if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
			vtTemperature_X = 50;
			vtTemperature_Y = 0;
			vtVolt_X = 76;
			vtVolt_Y = 2;
			vtCVolt_X =vtVolt_X ;
			vtCVolt_Y = 4;
		} else {
			vtTemperature_X = 76;
			vtTemperature_Y =6;
			vtVolt_X = 76;
			vtVolt_Y = 2;
			vtCVolt_X =vtVolt_X ;
			vtCVolt_Y = 4;
		}
		OLED_ShowChar(vtTemperature_X, vtTemperature_Y,vSystemTemperature/10+'0');
		OLED_ShowChar(vtTemperature_X+8, vtTemperature_Y,vSystemTemperature%10+'0');
		//---------------------------------------------------------------------
		displayFloat(vtVolt_X,vtVolt_Y,vAdVoltOfBatt,3,'V');
		//---------------------------------------------------------------------
		displayFloat(vtCVolt_X,vtCVolt_Y,vTestCompBatt,3,'V');
	}
#endif
}
#endif

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
void factoryModeTemperatureDisplay(void) {
#if(LCD_INSIDE==1)
	OLED_ShowChar(0, 0, vSystemTemperature / 10 + '0');
	OLED_ShowChar(8, 0, vSystemTemperature % 10 + '0');
	OLED_ShowChar(16, 0, ICON_Degree_ADDRESS);

#endif
}

/***********************************************************************************************************
  *  @brief	中文字符串显示
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void oledTestModeChineseString(u8 x, u8 y, const u8 *str, u8 len) {
	u8 i = 0;
	for (; i < len; i++) {
		OLED_ShowCHinese(x + 16 * i, y, *(str + i));
	}
}

//void fullyChargeTest(void){
//#if(LCD_INSIDE==1)
//	if(fIsSystemOff&&fIsCharging){
//		HexDigitDis(16,2,vChgResultForDisplay);
//		if(!vNearBattFullCnt){
//			OLED_ShowString(16,4,"    ");
//		}else{
//			if(!fIsBattFully)
//				OLED_ShowNum(16,4,(vNearBattFullCnt+vSystem1s)&0xffff);
//		}
//	}
//#endif
//}
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
void	displayFactoryMode(u8 item){
#if(LCD_INSIDE==1)
	switch(item){
	case Red_Current:
		clear();
		oledTestModeChineseString(16+16,4,RED_LED_CURRENT_STRING,4);
		break;
	case Green_Current:
//		clear();
		oledTestModeChineseString(16+16,4,GREEN_LED_CURRENT_STRING,4);
		break;
	case Blue_Current:
//		clear();
		oledTestModeChineseString(16+16,4,BLUE_LED_CURRENT_STRING,4);
		break;
	case Cw_Current:
//		clear();
		oledTestModeChineseString(16+16,4,CW_LED_CURRENT_STRING,4);
		break;
	case Mw_Current:
//		clear();
		oledTestModeChineseString(16+16,4,MW_LED_CURRENT_STRING,4);
		break;
	case Standby_Current:
		clear();
		//oledTestModeChineseString(16,2,BATT_VOLT,4);
		oledTestModeChineseString(16+16,4,STANDBY_CURRENT_STRING,4);
		break;
	case Sleep_Current:
		clear();
		oledTestModeChineseString(16+16,4,SLEEP_CURRENT_STRING,4);
		break;
	case Press_Func_key:
		clear();
		oledTestModeChineseString(16,4,PRESS_FUNC_KEY,6);
		oledTestModeChineseString(16,6,TEST,2);
//		oledTestModeChineseString(16+32,4,RED_LED_CURRENT_STRING,4);
//		oledTestModeChineseString(16,6,LEVEL,5);
//		OLED_ShowString(16+5*16,6,":A");
		break;
	case Press_Binc_key:
		clear();
		oledTestModeChineseString(16,2,PRESS_BDEC_KEY,7);
		oledTestModeChineseString(16,4,TEST,2);
		oledTestModeChineseString(16+32,4,STANDBY_CURRENT_STRING,4);
		oledTestModeChineseString(16,6,LEVEL,5);
		OLED_ShowString(16+5*16,6,":mA");
		break;
	case Press_Dec_key:
		clear();
//		oledTestModeChineseString(16,2,PRESS_DEC_KEY,7);
		oledTestModeChineseString(16,2,PRESS_INC_KEY,7);
		oledTestModeChineseString(16,4,TEST,2);
		oledTestModeChineseString(16+32,4,SLEEP_CURRENT_STRING,4);
		oledTestModeChineseString(16,6,LEVEL,5);
		OLED_ShowString(16+5*16,6,":uA");
		break;
	case System_Volt_Display:
		clear();
		oledTestModeChineseString(32,4,BATT_VOLT,4);
		break;
	case TempErrorDisplay:
		clear();
		oledTestModeChineseString(32,4,TEMP_ERROR,4);
		break;
	case VoltageErrorDisplay:
		clear();
		oledTestModeChineseString(32, 4, VOLT_ERROR, 4);
		break;
	}
#endif
}

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
void factoryVoltDisplay(float volt) {
#if(LCD_INSIDE==1)
	OLED_ShowChar(88, 2, ((u8) volt) + '0');
	OLED_ShowChar(88 + 8, 2, '.');
	OLED_ShowChar(88 + 13, 2, ((u8) (volt * 10)) % 10 + '0');
	OLED_ShowChar(88 + 21, 2, ((u16) (volt * 100)) % 10 + '0');
	OLED_ShowChar(88 + 29, 2, 'V');
#endif
}
