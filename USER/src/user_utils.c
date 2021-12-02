/*
 * user_utils.c
 *
 *  Created on: 2021年7月23日
 *      Author: Sky
 */
#include "user_utils.h"
#include "stdlib.h"
#include "string.h"
#include "user_basetime.h"
///***********************************************************************************************************
//  *  @brief					array data shift left
//  *
//  *
//  *  @param [in] :				arr:	 	待移动数据
//  *  							len：	参与移动的数据总长度
//  *							offset：	需要移动的偏移量
//  *							loop:	是否循环移动
//  *
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :
//  ************************************************************************************************************/
//void uint32ArrayShiftLeft(uint32 * arr, uint16 len, uint8 offset, uint8 loop) {
//	if (!len || !offset)
//		return;
//	uint32 * temp = (uint32 *) malloc(sizeof(uint32) * offset);
//	if (loop) {
//		if (temp != NULL)
//			memcpy(temp, arr, offset * sizeof(uint32));
//	}
//	//arr += len - 1 - offset;
//	//uint32 *tar = arr + offset;
//	uint32 *tar = arr ;
//	arr +=  offset;
//	for (uint8 i = 0; i < len - offset; i++) {
//		*tar++ = *arr++;
//	}
////	tar += offset - 1;
//	if (loop && (temp != NULL)) {
//		memcpy(tar, temp, offset * sizeof(uint32));
//		free(temp);
//	} else {
//		memset(tar, 0x00, offset * sizeof(uint32));
//	}
//	free(temp);
//}
//
//
///***********************************************************************************************************
//  *  @brief					array data shift right
//  *
//  *
//  *  @param [in] :				arr:	 	待移动数据
//  *  							len：	参与移动的数据总长度
//  *							offset：	需要移动的偏移量
//  *							loop:	是否循环移动
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :
//  ************************************************************************************************************/
//void uint32ArrayShiftRight(uint32 * arr, uint16 len, uint8 offset, uint8 loop) {
//	if (!len || !offset)
//		return;
//	uint32 * temp = (uint32 *) malloc(sizeof(uint32) * offset);
//	if (loop) {
//		if (temp != NULL)
//			memcpy(temp, arr + len - offset, offset * sizeof(uint32));
//	}
//	arr += len - 1 - offset;
//	uint32 *tar = arr + offset;
//	for (uint8 i = 0; i < len - offset; i++) {
//		*tar-- = *arr--;
//	}
//	tar -= offset - 1;
//	if (loop && (temp != NULL)) {
//		memcpy(tar, temp, offset * sizeof(uint32));
//		free(temp);
//	} else {
//		memset(tar, 0x00, offset * sizeof(uint32));
//	}
//	free(temp);
//}
//
///***********************************************************************************************************
//  *  @brief					array data shift right
//  *
//  *
//  *  @param [in] :				arr:	 	待移动数据
//  *  							len：	参与移动的数据总长度
//  *  							blockLen: 单元长度
//  *							offset：	需要移动的偏移量
//  *							loop:	是否循环移动
//  *  @param [out] :
//  *
//  *  @return :					0/1	fail:success
//  *
//  *  @note :					len需要能被blockLen整除
//  ************************************************************************************************************/
//uint8 uint32ArrayShiftRightByBlock(uint32 * arr, uint16 len, uint8 blockLen,uint8 offset, uint8 loop) {
//	if(len%blockLen)
//		return 1;				//fail
//	for(uint8 i=0;i<len/blockLen;i++){
//		uint32ArrayShiftRight(arr+i*blockLen,blockLen,offset,loop);
//	}
//	return 0;
//}
///***********************************************************************************************************
//  *  @brief					array data shift left
//  *
//  *
//  *  @param [in] :				arr:	 	待移动数据
//  *  							len：	参与移动的数据总长度
//  *  							blockLen: 单元长度
//  *							offset：	需要移动的偏移量
//  *							loop:	是否循环移动
//  *  @param [out] :
//  *
//  *  @return :					0/1	fail:success
//  *
//  *  @note :					len需要能被blockLen整除
//  ************************************************************************************************************/
//uint8 uint32ArrayShiftLeftByBlock(uint32 * arr, uint16 len, uint8 blockLen,uint8 offset, uint8 loop) {
//	if(len%blockLen)
//		return 1;				//fail
//	for(uint8 i=0;i<len/blockLen;i++){
//		uint32ArrayShiftLeft(arr+i*blockLen,blockLen,offset,loop);
//	}
//	return 0;
//}
//
//void	 memsetUint32(uint32* arr,uint32 data,uint8 len){
//	for(uint8 i=0;i<len;i++){
//		*arr++=data;
//	}
//}
///***********************************************************************************************************
//  *  @brief  			从右侧找到非0数据，并把这个数据填满左侧
//  *
//  *
//  *  @param [in] :				arr:指向待处理数据的指针
//  *							len:参与处理的数据长度
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :
//  ************************************************************************************************************/
//void setData2ArrayRight(uint32* arr,uint8 len){
//	uint32 *tp=arr+len-1;
//	uint32 data=0;
//	for(uint8 i=0;i<len;i++){
//		if(*tp){
//			data=*tp;
//		}else{
//			*tp=data;
//		}
//		tp--;
//	}
//}
///***********************************************************************************************************
//  *  @brief  			从左侧找到非0数据，并把这个数据填满右侧
//  *
//  *
//  *  @param [in] :				arr:指向待处理数据的指针
//  *							len:参与处理的数据长度
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :
//  ************************************************************************************************************/
//void setData2ArrayByLeft(uint32* arr,uint8 len){
//	uint32 *tp=arr;
//	uint32 data=0;
//	for(uint8 i=0;i<len;i++){
//		if(*tp){
//			data=*tp;
//		}else{
//			*tp=data;
//		}
//		tp++;
//	}
//}
//
//
//void setData2AllPosition(uint32 *arr) {
//
//	setData2ArrayRight(arr, SINGLE_SERIAL_LED_NO);
//	setData2ArrayRight(arr + SINGLE_SERIAL_LED_NO, SINGLE_SERIAL_LED_NO);
//	setData2ArrayRight(arr + SINGLE_SERIAL_LED_NO * 2, SINGLE_SERIAL_LED_NO);
//	setData2ArrayRight(arr + SINGLE_SERIAL_LED_NO * 3, SINGLE_SERIAL_LED_NO);
//	setData2ArrayRight(arr + SINGLE_SERIAL_LED_NO * 4, SINGLE_SERIAL_LED_NO);
//	setData2ArrayRight(arr + SINGLE_SERIAL_LED_NO * 5, SINGLE_SERIAL_LED_NO);
//
//}
/*************************************************************************************************************
  *  @brief  					获取1~ram之间的伪随机数
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
uint8 randomUint8(uint8 ram) {
	if (ram) {
		uint32 cnt = TIM_GetCountRegister(BASIC_TIM);
		return cnt % ram + 1;
	} else {
		return 0;
	}
}


/***********************************************************************************************************
  *  @brief  			求两个uint8 数差的绝对值
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
uint8 userAbsSubUint8(uint8 data1,uint8 data2){
	if(data1>=data2)
		return data1-data2;
	else
		return data2-data1;
}

