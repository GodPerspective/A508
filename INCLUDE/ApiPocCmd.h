#ifndef __APIPOCCMD_H
#define __APIPOCCMD_H

#include "AllHead.h"

typedef enum{
  PocComm_OpenPOC               = 0x00,
  PocComm_SetParam		= 0x01,
  PocComm_GetParam		= 0x02,
  PocComm_Login			= 0x03,
  PocComm_Logout		= 0x04,
  PocComm_Cancel		= 0x05,
  PocComm_ModifyPwd		= 0x06,
  PocComm_EnterGroup		= 0x09,
  PocComm_Invite                = 0x0A,
  PocComm_StartPTT		= 0x0B,
  PocComm_EndPTT		= 0x0C,
  PocComm_GroupListInfo	        = 0x0D,
  PocComm_UserListInfo          = 0x0E,
  PocComm_Key			= 0x10
}PocCommType;


extern u8 POC_EnterPersonalCalling_Flag;
extern u8 POC_QuitPersonalCalling_Flag;

extern u8 POC_AtEnterPersonalCalling_Flag;
extern u8 POC_AtQuitPersonalCalling_Flag;

extern u8 POC_EnterGroupCalling_Flag;
extern u8 POC_QuitGroupCalling_Flag;

extern u8 POC_ReceivedVoiceStart_Flag;
extern u8 POC_ReceivedVoiceEnd_Flag;
extern bool POC_ReceivedVoice_Flag;
extern bool POC_Receive86_Flag;
extern bool ApiPocCmd_Tone_Flag;
//void ApiPocCmd_PowerOnInitial(void);
extern void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len);
//extern bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len);
extern bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len);//cTxBufΪ���ip�˺��������Ϣ
extern void ApiPocCmd_10msRenew(void);
//extern void ApiPocCmd_83_1msRenew(void);
extern bool ApiPocCmd_GetPttState(void);//�ж�PPT״̬���Ƿ��л�Ȩ;
extern bool ApiAtCmd_GetLoginState(void);
extern bool GetPlayState(void);
extern bool GetPersonalCallingMode(void);//��ȡ�Ƿ�Ϊ����ģʽ
extern void ApiGetPocBuf(void);
extern u8 *ApiAtCmd_GetMainWorkName(void);
extern u8 ApiAtCmd_GetMainWorkNameLen(void);
extern u8 ApiAtCmd_GetGroupNum(void);
extern u8 ApiAtCmd_GetMainGroupId(void);
extern u8 *ApiAtCmd_GetGroupName(u8 n);
extern u8 ApiAtCmd_GetGroupNameLen(u8 n);

extern u8 *ApiAtCmd_GetUserName(u8 n);//��ȡ���������û�����������
extern u8 ApiAtCmd_GetUserNameLen(u8 n);
extern u8 *ApiAtCmd_GetMainUserName(void);//��ȡ��ǰ�û�����������
extern u8 ApiAtCmd_GetMainUserNameLen(void);
extern u8 ApiAtCmd_GetUserNum(void);//��ȡ���߳�Ա����������
extern u8 ApiAtCmd_GetMainUserId(void);//��ȡ��ǰ�û�id��������

extern u8 *HexToChar_MainGroupId(void);//16����ת�ַ��� ��ǰȺ��ID ��ʾ������ʹ��
extern u8 *HexToChar_GroupCallingNum(void);//16����ת�ַ��� ����������ǰȺ��ID ��ʾ������ʹ��
extern u8 *HexToChar_MainUserId(void);//16����ת�ַ��� ��ǰ�û�ID ��ʾ������ʹ��
extern u8 *HexToChar_PersonalCallingNum(void);//16����ת�ַ��� ����������ǰ�û�ID ��ʾ������ʹ��

extern u8 *UnicodeForGbk_MainWorkName(void);
extern u8 *UnicodeForGbk_MainUserName(void);
extern u8 *UnicodeForGbk_SpeakerRightnowName(void);
extern u8 *Get_Unicode_ActiveUserID(void);
extern u8 *Get_GBK_ActiveUserID(void);
#endif