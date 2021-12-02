/*
 * user_internalFlash.h
 *
 *  Created on: 2019-08-12
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_INTERNALFLASH_H_
#define USER_INC_USER_INTERNALFLASH_H_
#include "const.h"
#include "misc.h"
#include "cx32l003_flash.h"

u32 readInternalFlashData(u32 Address);
u16 readInternalFlashWrod(u32 Address);
u8 readInternalFlashByte(u32 Address);


#define WRITE_START_ADDR  ((uint32_t)0x0000FE00)
#define WRITE_END_ADDR    ((uint32_t)0x00010000)		//64K
#define FLASH_PAGE_SIZE    512
#define  		CHECK_INFO_DATA	0x5A3A
#define		CHECK_INFO_ADDR	((uint32_t)0x00000000)

/***********************************************************************************************************
  *  @brief	store the system parameters when some exceptions occurred ,before reset the MCU
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
extern	FLASH_Status StoreExceptions2Flash(void);
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
extern	FLASH_Status LoadExceptionsFromFlash(displayParamsStruct * disParams);
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
extern	int exceptionsCheck(void);
#endif /* USER_INC_USER_INTERNALFLASH_H_ */
