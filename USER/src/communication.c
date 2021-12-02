/*
 * communication.c
 *
 *  Created on: 2019年12月6日
 *      Author: Bough_004
 */
#include <string.h>
#include "communication.h"
#include "const.h"
#include "io_define.h"
#include "user_exti.h"
#include "user_basetime.h"
#include "user_menu.h"
#include "user_ddl.h"
#include "user_lptimer.h"
#include "user_key.h"

//u8	vCommBitCnt=0;
//u8	receivedData=0;
//u16	vTimeoutCnt=0;
//ChgReceiveStts			chgRecStts;
//ChgRecevingParams		chgRecingParams;
//
//u8	vSkipCommKeyTimer=0;
//RemoteFuncKeyDef remoteKeyStruct={0};
//u16 vSyncCnt=0;
//void	deinitComm(void){
//	DDL_ZERO_STRUCT(chgRecingParams);
//}
//void	initChargeComm(void){
//	DDL_ZERO_STRUCT(chgRecingParams);
//	DDL_ZERO_STRUCT(chgRecStts);
//}
///***************************************************************************************
// *
// * 通信接收函数
// *
// ****************************************************************************************/
//void dataReceiveFunc(void) {
////	u32 vtCurrentTimer = 0;
//	u8 vtCurrentTimer = 0;
//	u16	vtTt0=0;
//	u16	vtTt1=0;
//	u16 vtACc = 0;
//	if (CommunPort->DI & CommunPin) {			//rising
//		vtTt0=TIM_GetCountRegister(TIM11);
//		vtTt1=(vSyncCnt+vtTt0)&0xffff;
//		if ( vtTt1>=6818) {						//step 1.33us
//			set_Debug_Pin;
//			vtCurrentTimer = LPTIMER_ReadCnt() & 0XFF;
//			chgRecingParams.vHiStartTimer = ~vtCurrentTimer + 1;//正脉冲开始时间
//			vTimeoutCnt = 0x10000 - sysTimes.vSystemLIRC5ms;
//			chgRecingParams.fIsFirstHiGot = TRUE;
//			chgRecingParams.vReceiveTemp = 0;
//			chgRecingParams.vCommBitCnt = 0;
//			chgRecingParams.vSyncCodeStts = SyncCodeGot;
//			return;
//		}
//		else {
//			if (!chgRecingParams.vSyncCodeStts) {
//				deinitComm();				//ERROR
//				return;
//			}
//		}
//		if (chgRecingParams.vSyncCodeStts == SyncCodeGot) {
//			vtCurrentTimer = LPTIMER_ReadCnt() & 0XFF;
//			vtACc = vtCurrentTimer + chgRecingParams.vLowStartTimer;
//			if (vtACc >= 0x0100)
//				chgRecingParams.vLowLastTimer = vtACc & 0xff;
//			else {
//				chgRecingParams.vLowLastTimer = vtACc - NEG_LPT_CNT;
//			}
//			if ((chgRecingParams.vLowLastTimer < MIN_BIT_TIMER) || (chgRecingParams.vLowLastTimer > MAX_BIT_TIMER)) {
//				deinitComm();				//ERROR
//				return;
//			}
//
//			if (chgRecingParams.vLowLastTimer > VALID_BIT_TIMER) {
//				chgRecingParams.vReceiveTemp = chgRecingParams.vReceiveTemp << 1;//low bit
//				chgRecingParams.vCommBitCnt++;
//			} else {
//				chgRecingParams.vReceiveTemp = (chgRecingParams.vReceiveTemp << 1) | 0x01;//high bit
//				chgRecingParams.vCommBitCnt++;
//			}
//			if (chgRecingParams.vCommBitCnt >= 8) {	//一帧数据接收完成
//				if(chgRecingParams.vRecByteCnt<2){
////					if(chgRecingParams.vReceiveTemp==MASK_BYTE){
//						chgRecingParams.vRecByteCnt++;
//						chgRecingParams.vCommBitCnt=0;
////					}
//				}else{
//					chgRecStts.vReceivedData = chgRecingParams.vReceiveTemp;
//					chgRecStts.fReceivedResult = validResultBuff;
//					deinitComm();
//				}
//			}
//			chgRecingParams.vHiStartTimer = ~vtCurrentTimer + 1;//正脉冲开始时间
//			vTimeoutCnt = 0x10000 - sysTimes.vSystemLIRC5ms;
//		}
//	} else {						//falling
//		clr_Debug_Pin;
////		clrPowerLed;
//		vtCurrentTimer = LPTIMER_ReadCnt() & 0XFF;
//		chgRecingParams.vLowStartTimer = ~vtCurrentTimer + 1;//负脉冲开始时间
//		//vTimeoutCnt = vtCurrentTimer;
////		vSyncCnt = 10000 - vTime1msCnt;
//		vSyncCnt=0x10000-TIM_GetCountRegister(TIM11);
//		if (!chgRecingParams.vSyncCodeStts) {
//			return;
//		} else if (chgRecingParams.vSyncCodeStts == SyncCodeGot)  {
//			vtACc = chgRecingParams.vHiStartTimer + vtCurrentTimer;
//			if (vtACc >= 0x0100)
//				chgRecingParams.vHiLastTimer = vtACc & 0xff;
//			else {
//				chgRecingParams.vHiLastTimer = vtACc - NEG_LPT_CNT;
//			}
//			if ((chgRecingParams.vHiLastTimer < MIN_BIT_TIMER) || (chgRecingParams.vHiLastTimer > MAX_BIT_TIMER)) {
//				deinitComm();				//ERROR
//				return;
//			}
//		}
//	}
//}
//
//
//void dataReceivedProcess(void) {
//	if (validResultBuff == chgRecStts.fReceivedResult) {
//		if (!vSkipCommKeyTimer) {
//#if(REMOTE_TYPE_DEFINE==0)
//			if (COMM_CODE_SWITCH == chgRecStts.vReceivedData) {
//				vKeyValue = SWITCH_KEY_SHORT_PRESSED;
//				chgRecStts.fReceivedResult = 0;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//			} else if (COMM_CODE_INC == chgRecStts.vReceivedData) {
//				vKeyValue = INC_KEY_SHORT_PRESSED;
//				vSkipCommKeyTimer = 0;
//				chgRecStts.fReceivedResult = invalidResult;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//				return;
//			} else if (COMM_CODE_DEC == chgRecStts.vReceivedData) {
//				vKeyValue = DEC_KEY_SHORT_PRESSED;
//				vSkipCommKeyTimer = 0;
//				chgRecStts.fReceivedResult = invalidResult;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//				return;
//			} else if (COMM_CODE_FUNC == chgRecStts.vReceivedData) {
//				if (!remoteKeyStruct.vRemoteFuncStts) {
////					vKeyValue = FUNC_KEY_SHORT_PRESSED;
//					remoteKeyStruct.vRemoteFuncStts = 1;
//				}
//			} else if (0x02 == chgRecStts.vReceivedData) {
//				if(fIsSystemOff){
//					vKeyValue = POWER_KEY_LONG_PRESSED;
//				}else{
//					vKeyValue = FUNC_KEY_SHORT_PRESSED;
//				}
//			}
//#elif(REMOTE_TYPE_DEFINE==1)
//			if (COMM_CODE_FlASH_INC == chgRecStts.vReceivedData) {
//				vKeyValue = FUNC_KEY_SHORT_PRESSED;
//				chgRecStts.fReceivedResult = 0;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//			}else if (COMM_CODE_FlASH_DEC == chgRecStts.vReceivedData) {
//				vKeyValue = SWITCH_KEY_SHORT_PRESSED;
//				chgRecStts.fReceivedResult = 0;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//			} else if (COMM_CODE_MOTOR_INC == chgRecStts.vReceivedData) {
//				vKeyValue = INC_KEY_SHORT_PRESSED;
//				vSkipCommKeyTimer = 0;
//				chgRecStts.fReceivedResult = invalidResult;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//				return;
//			} else if (COMM_CODE_MOTOR_DEC == chgRecStts.vReceivedData) {
//				vKeyValue = DEC_KEY_SHORT_PRESSED;
//				vSkipCommKeyTimer = 0;
//				chgRecStts.fReceivedResult = invalidResult;
//				remoteKeyStruct.vRemoteFuncKeyClickTimes = 0;
//				remoteKeyStruct.vRemoteFuncStts=0;
//				return;
//
//#if(POWER_ON_BY_REMOTE_CTRL==1)
//			} else if (COMM_CODE_POWER == chgRecStts.vReceivedData) {
//				if (fIsSystemOff) {
//					vKeyValue = POWER_KEY_LONG_PRESSED;
//					remoteKeyStruct.vRemoteFuncStts = 2;
//					vSkipCommKeyTimer = SERIES_TIME_CONST*5;
//					chgRecStts.fReceivedResult = invalidResult;
//					return;
//				} else {
//					if (!remoteKeyStruct.vRemoteFuncStts) {
//						remoteKeyStruct.vRemoteFuncStts = 1;
//					}
//				}
//			}
//#else
//			} else if (COMM_CODE_POWER == chgRecStts.vReceivedData) {
////				if (!fIsSystemOff && !remoteKeyStruct.vRemoteFuncStts) {
////					remoteKeyStruct.vRemoteFuncStts = 1;
////				}
//				if (!fIsSystemOff ) {
//					vKeyValue = POWER_KEY_LONG_PRESSED;
//				}
//			}
//#endif
//#endif
//		} else {
//#if(REMOTE_TYPE_DEFINE==0)
//			if (COMM_CODE_FUNC == chgRecStts.vReceivedData)
//#elif(REMOTE_TYPE_DEFINE==1)
//			if (COMM_CODE_POWER == chgRecStts.vReceivedData)
//#endif
//			{
//				if (remoteKeyStruct.vRemoteFuncStts == 1) {
////					if (remoteKeyStruct.vRemoteFuncKeyClickTimes++ >= 20) {
////						remoteKeyStruct.vRemoteFuncStts = 2;					//power key got
////						vKeyValue = POWER_KEY_LONG_PRESSED;
////						remoteKeyStruct.vRemoteFuncKeyClickTimes = 20;
////					}
//				}else if(COMM_CODE_POWER == chgRecStts.vReceivedData){
//#if(POWER_ON_BY_REMOTE_CTRL==1)
//					if (fIsSystemOff) {
//						vKeyValue = POWER_KEY_LONG_PRESSED;
//						remoteKeyStruct.vRemoteFuncStts = 2;
//						vSkipCommKeyTimer = SERIES_TIME_CONST*5;
//						chgRecStts.fReceivedResult = invalidResult;
//						return;
//					} else {
//						if (!remoteKeyStruct.vRemoteFuncStts) {
//							remoteKeyStruct.vRemoteFuncStts = 1;
//						}
//					}
//#else
////					if(!fIsSystemOff&&!remoteKeyStruct.vRemoteFuncStts){
////						remoteKeyStruct.vRemoteFuncStts = 1;
////					}
//#endif
//				}
//				vSkipCommKeyTimer = SERIES_TIME_CONST*4;
//				chgRecStts.fReceivedResult = invalidResult;
//				return;
//			}
//		}
//		if(remoteKeyStruct.vRemoteFuncStts){
//			vSkipCommKeyTimer = SERIES_TIME_CONST*5;
//		}else{
//			vSkipCommKeyTimer = SERIES_TIME_CONST;
//		}
//		chgRecStts.fReceivedResult = invalidResult;
//	}
//}
//
//void deinitRemoteCtrlDecoder(void) {
//
//	vSkipCommKeyTimer = 0;
//	memset(&remoteKeyStruct, 0, sizeof(remoteKeyStruct));
//	memset(&chgRecingParams, 0, sizeof(chgRecingParams));
//	memset(&chgRecStts, 0, sizeof(chgRecStts));
//
//}
//
//void	 duplicatedRemoteKeyProcess(void){
////	if(vSkipCommKeyTimer){
////		vSkipCommKeyTimer--;
////	}
//	if(vSkipCommKeyTimer&&!--vSkipCommKeyTimer){
//#if(POWER_ON_BY_REMOTE_CTRL==1)
//		if(remoteKeyStruct.vRemoteFuncStts == 1){
//			vKeyValue = POWER_KEY_LONG_PRESSED;
//		}
//#endif
//		remoteKeyStruct.vRemoteFuncStts=0;
//		remoteKeyStruct.vRemoteFuncKeyClickTimes=0;
//	}
//}

