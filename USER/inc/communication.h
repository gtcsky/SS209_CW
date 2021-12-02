/*
 * communication.h
 *
 *  Created on: 2020年03月09日
 *      Author:  Sky@Bough.com
 */

#ifndef USER_INC_COMMUNICATION_H_
#define USER_INC_COMMUNICATION_H_
#include "const.h"
extern	u8 fIsReceiveStart;



#define	MAX_RECEIVE_INDEX	19
#define	COMMUN_TIME_OUT_CONST	120000	//120000/24M=5ms


#define	NEG_LPT_CNT					0x3F	//(u8)_LPTIMER_IT_LOADVALUE
#define	CHARGE_SYNC_BIT_HIGH			123	//100CLK=3.2ms      3200/26.05=123	(Lptimer step=26.05us)
#define	CHARGE_LOW_BIT_HIGH			10	//10CLK=256us      256/26.05=10	(Lptimer step=26.05us)
#define	CHARGE_HIGH_BIT_HIGH			1	//2CLK=64us      64/26.05=2	(Lptimer step=26.05us)

#define	CHARGE_LOW_BIT_LOW			1	//2CLK=64us      64/26.05=2	(Lptimer step=26.05us)
#define	CHARGE_HIGH_BIT_LOW			10	//10CLK=256us      256/26.05=10	(Lptimer step=26.05us)

#define	MAX_BIT_TIMER			49		//1.3ms   (Lptimer step=26.05us)
#define	MIN_BIT_TIMER			11		//286us   (Lptimer step=26.05us)
#define	VALID_BIT_TIMER		30		//780us
#define	MASK_BYTE				0x55

#define	CHARGE_TIME_OUT				100	//5ms*100=500ms
#define	CODE_FULLY_BIT			0X20
#define	CODE_CHARGING_BIT		0X40
#define	CODE_SYNC_BIT			0X80

#if(REMOTE_TYPE_DEFINE==0)
	#define	COMM_CODE_SWITCH	0XC0
	#define	COMM_CODE_INC		0X30
	#define	COMM_CODE_DEC		0X0C
	#define	COMM_CODE_FUNC		0X03
#elif(REMOTE_TYPE_DEFINE==1)
	#define	COMM_CODE_POWER			0X01
	#define	COMM_CODE_COLOR				0X03
	#define	COMM_CODE_FlASH_INC			0X05
	#define	COMM_CODE_FlASH_DEC			0X0B
	#define	COMM_CODE_MOTOR_INC		0X07
	#define	COMM_CODE_MOTOR_DEC		0X09
	#define	COMM_CODE_1_HOUR			0X10
	#define	COMM_CODE_2_HOUR			0X11
	#define	COMM_CODE_3_HOUR			0X12
	#define	COMM_CODE_4_HOUR			0X13
	#define	COMM_CODE_5_HOUR			0X14
	#define	COMM_CODE_6_HOUR			0X15
#endif

#define	SERIES_TIME_CONST		12		//5*10ms

extern	u8	vSkipCommKeyTimer;

typedef enum{
	invalidResult=0,
	validResultBuff=1
}ChgRecResult;

typedef struct{
	UINT8  fReceivedResult;
	UINT8  vReceivedData;
	UINT8  vFirstRecData;
}ChgReceiveStts;

typedef enum{
	NoSignal=0,
//	SyncLowStart=1,
	SyncLowGot=2,
	SyncHiGot=3,
	SyncCodeGot=4
}ChgSyscStts;

typedef struct{
	ChgSyscStts  vSyncCodeStts;
	UINT8	vHiStartTimer;
	UINT8	vLowStartTimer;
	UINT8	vHiLastTimer;
	UINT8	vLowLastTimer;
	UINT8	vCommBitCnt;
	UINT8	vReceiveTemp;
	UINT8	fIsFirstHiGot;
	UINT8	vRecByteCnt;
}ChgRecevingParams;


typedef struct{
	uint8	vRemoteFuncStts;				//01 generated function Key , 02 generated power key
	uint8	vRemoteFuncKeyClickTimes;
}RemoteFuncKeyDef;
extern	void	 deinitComm(void);
extern	void  dataReceiveFunc(void);
extern	void	 dataReceivedProcess(void);
extern	void	 duplicatedRemoteKeyProcess(void);
extern	void deinitRemoteCtrlDecoder(void);
#endif /* USER_INC_COMMUNICATION_H_ */
