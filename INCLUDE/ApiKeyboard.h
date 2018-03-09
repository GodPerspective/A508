#ifndef __APIKEYBOARD_H
#define __APIKEYBOARD_H
#include "DrvKeyboard.h"
extern u8 *ucQuitPersonalCalling;
extern s8 GroupCallingNum;
extern s8 PersonalCallingNum;
extern s8 KeyPersonalCallingCount;
extern s8 KeyUpDownCount;
extern void Keyboard_Test(void);
extern bool NumberKeyboardPressDown_flag;
extern bool LockingState_EnterOK_Flag;
extern u8 VoiceType_FreehandOrHandset_Flag;
extern u8 BacklightTimeSetCount;
extern u8 KeylockTimeSetCount;
extern u8 TheMenuLayer_Flag;//�����˵��㼶��Ĭ��״̬��1 һ���˵���1 �����˵���2
extern u8 MenuMode_Flag;
extern u8 MenuModeCount;
extern bool UpDownSwitching_Flag;
#endif