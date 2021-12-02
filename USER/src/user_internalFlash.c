/*
 * user_internalFlash.c
 *
 *  Created on: 2020-12-18
 *      Author: Achilles Zhang
 */

#include "user_internalFlash.h"
#include "user_menu.h"
#include "user_i2c.h"
#include "string.h"
#include "stdlib.h"

u32 readInternalFlashData(u32 Address){
	return	*(__IO u32*)Address;
}
u16 readInternalFlashWrod(u32 Address){
	return	*(__IO u16*)Address;
}
u8 readInternalFlashByte(u32 Address){
	return	*(__IO u8*)Address;
}

FLASH_Status EraseFlash(void){
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	/*清除所有的标志位*/
	FLASH_ClearFlag(FLASH_FLAG_ALL);

	FLASHStatus = FLASH_EraseSector(WRITE_START_ADDR);
	return	FLASHStatus;
}

///***********************************************************************************************************
//  *  @brief				clear the exceptions flag which store at the flash
//  *
//  *  @param [in] :			None
//  *
//  *  @param [out] :			None
//  *
//  *  @return :				None
//  *
//  *  @note :
//  ************************************************************************************************************/
//static void clearExceptions(void) {

//	FLASH_ProgramHalfWord(WRITE_START_ADDR, 0x0000);

//}
/***********************************************************************************************************
  *  @brief	store the system parameters when some exceptions occurred ,before  the MCU reset
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
FLASH_Status StoreExceptions2Flash(void){
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint8_t len=sizeof(displayParams);// add checkInfo
	uint16_t * p=(uint16_t *)malloc(len);
	if(p==NULL)
		return FLASH_BUSY;
	displayParams.checkCode=CHECK_INFO_DATA;
#if(EEPROM_INSIDE==1)
	writeEepromData(0x00,(uint8 *)&displayParams,len);
#else
	uint32_t Address = 0;
	uint8 retryTimes=3;
	do{
		if(EraseFlash()!=FLASH_COMPLETE)
			return FLASH_BUSY;
		memcpy(p , &displayParams, len);
		/*PROGRAM*/
		Address = WRITE_START_ADDR;
		for(uint8_t i=0;i<(len+1)/2;i++) {
			FLASHStatus =FLASH_ProgramHalfWord(Address,*(p+i));
			if(FLASHStatus!=FLASH_COMPLETE)
				break;
			Address = Address + 2;
		}

		*p=readInternalFlashWrod(WRITE_START_ADDR);
		if(*p==CHECK_INFO_DATA){
			FLASHStatus=FLASH_COMPLETE;
			break;
		}
		FLASHStatus=FLASH_BUSY;
	}while(--retryTimes);
#endif
	free(p);
	return	FLASHStatus;
}
/***********************************************************************************************************
  *  @brief			load exceptions from the flash which stored data by last Error if exist.
  *
  *
  *  @param [in] :
  *
  *  @param [out] :		disParams: if exceptions exist ,load the last system parameters  and store to
  *  							disParams.
  *  							if  not exceptions,   disParams will not be changed.
  *
  *  @return :			read flash result
  *
  *  @note :
  ************************************************************************************************************/
FLASH_Status LoadExceptionsFromFlash(displayParamsStruct * disParams){
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint8 len=sizeof(displayParams);// add checkInfo
#if(EEPROM_INSIDE==1)
	uint8 * p=(uint8 *)malloc(len);
	if(p==NULL)
		return FLASH_BUSY;
//	memset(p,0x11,5);
	readEepromData(0x00,p,len);
	if(*((uint16 *)p)!=CHECK_INFO_DATA){
		free(p);
		return FLASH_BUSY;
	}
#else
	uint32_t Address = WRITE_START_ADDR;
	uint8_t i=0;
	uint16_t * p=(uint16_t *)malloc(len);
	if(p==NULL)
		return FLASH_BUSY;
	i=0;
	while (i < (len+1)/2) {
		*(p+(i++))=*(__IO uint16_t*) Address;
		Address += 2;
	}
	if(*p!=CHECK_INFO_DATA){
		free(p);
		return FLASH_BUSY;
	}
#endif
	memcpy(disParams,p,len);
		free(p);
	return	FLASHStatus;
}
/***********************************************************************************************************
  *  @brief  						check if there are some exceptions exist
  *
  *
  *  @param [in] :					None
  *
  *  @param [out] :					if some exceptions exist ,displayParams will be load as the last
  *  									data before exceptions  occurred
  *
  *  @return :						0/1   no/have some exceptions
  *
  *  @note :
  ************************************************************************************************************/
int exceptionsCheck(void) {
	if (FLASH_COMPLETE == LoadExceptionsFromFlash(&displayParams)) {
		if (CHECK_INFO_DATA != displayParams.checkCode) {			//新Flash或未记录到异常关机信息
			return 1;
		}else{
//			clearExceptions();
			return 0;
		}
	}else {
		return 1;
	}
}

