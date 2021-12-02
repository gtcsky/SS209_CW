/*
 * user_color.h
 *
 *  Created on: 2019-11-20
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_COLOR_H_
#define USER_INC_USER_COLOR_H_
#include "const.h"
extern	void		allLedIoAsInput(void);
extern	void	 	setCoolDuty(float duty);
extern	void	 	setCoolData(u16 pwmData);
extern	void	 	setWarmDuty(float duty);
extern	void	 	setWarmData(u16 pwmData);
extern	void 	updateColorTemp(displayParamsStruct * disParams);
extern	void		setTargeColorTempData(displayParamsStruct * disParams,PcaDataStruct *targetData);
extern	void 	turnOffColorTempLamp(void);
extern	void 	hsb2Rgb(colorStructType * colorStruct) ;
extern	void		setLedIoAf(void) ;
extern	void		turnOffRGBLamp(void);
extern	void		turnOffColorTempLamp(void);
extern	void	 	setRedData(u16 pwmData);
extern	void	 	setGreenData(u16 pwmData);
extern	void	 	setBlueData(u16 pwmData);
extern	void		updateRGBLamp(displayParamsStruct * disParams);
extern	void 	updateColor(colorStructType * colorStruct);
#endif /* USER_INC_USER_COLOR_H_ */
