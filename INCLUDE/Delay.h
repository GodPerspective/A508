/******************************************************************************
; File Name		: Delay.h
; Author		: ZhangYaLing
; Version		: 1.0
; Description	: delay driver program process
; Date			: 2009.05.04
; Modify		:
******************************************************************************/
#ifndef	DELAY_H
#define DELAY_H

#ifdef	DELLABEL
	#define DELAPI
#else
	#define DELAPI	extern
#endif
extern u8 ApiAtCmd_ZTTSCount;
extern u8 ShowTime_Flag;
extern u8 TaskStartDeleteDelay1Count;
extern u8 TaskStartDeleteDelay3Count;
extern u8 TaskStartDeleteDelay4Count;
extern u8 TaskStartDeleteDelay6Count;
extern bool LockingState_Flag;
extern u16 TimeCount;//��ʱʱ��
extern u16 TimeCount3;
extern u8 EnterKeyTimeCount;
extern u8 BacklightTimeCount;//�����ʱ��(��Ҫ���ý���eeprom)
extern u16 KeylockTimeCount;//������ʱ��(��Ҫ���ý���eeprom)
extern u8 UpDownSwitchingCount;//û��һ�λ��黻��������һ��
extern void DEL_PowerOnInitial(void);
extern void DEL_Interrupt(void);
extern void DEL_Interrupt1(void);
extern void DEL_Renew(void);
extern void DEL_Soft1ms(u16 iLen); 
extern bool DEL_SetTimer(u8 cId,u16 iLen);
extern bool DEL_GetTimer(u8 cId);
extern u8 allow_key_operation_flag;
//DELAPI bool UpgradeNoATReturn_Flag;
//DELAPI bool UpgradeNoATReturn_Flag2;
#endif
/******************************************************************************
;------------end page
******************************************************************************/
