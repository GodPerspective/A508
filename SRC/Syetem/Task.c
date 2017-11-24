#define TASKABLE
#include "AllHead.h"

#if 1 //test
u8 Key_Flag_0=0;
#endif

#if 0//2��
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437323B7077643D3131313131313B00";
#endif
#if 0//3��
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437333B7077643D3131313131313B00";
#endif
#if 0//4��
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
#endif
#if 1//5��
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437353B7077643D3131313131313B00";
#endif
#if 0//6��
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437363B7077643D3131313131313B00";
#endif

u8 *ucStartPTT          = "0B0000";
u8 *ucEndPTT            = "0C0000";
u8 *ucOSSYSHWID         = "AT^OSSYSHWID=1";
u8 *ucPrefmode         = "AT^prefmode=4";
u8 *ucPPPCFG            = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
u8 *ucZTTS_Abell        = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";
u8 *ucPocOpenConfig     = "0000000101";

void Task_RunStart(void)
{
  u8 v;
  if(BootProcess_SIMST_Flag==1)//�յ�ģ�鿪��ָ��:SIMST:1
  {
    BEEP_Time(50);
    v=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));//
    v=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode,strlen((char const *)ucPrefmode));//1.����PPPCFG
    v=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//�����α����Խ���
    
    DEL_SetTimer(0,250);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    
    v=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.����PPPCFG
    BootProcess_SIMST_Flag=0;

  }
  if(BootProcess_PPPCFG_Flag==1)//����յ�^PPPCFG
  {
    BootProcess_PPPCFG_Flag=0;
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    ApiPocCmd_WritCommand(PocComm_SetParam,ucSetParamConfig,strlen((char const *)ucSetParamConfig));//����echat�˺š�IP
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    v=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
  }

}

void Task_RunNormalOperation(void)
{
  u8 v;
  if(ReadInput_KEY_PTT==0)
  {
    if(KeyDownUpChooseGroup_Flag==1)
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      v=ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));

      Key_Flag_0=1;
      KeyDownUpChooseGroup_Flag=0;
    }
    else
    {
      Set_RedLed(LED_ON);
      v=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      while(ReadInput_KEY_PTT==0);
      v=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
    }
    
        if(Key_Flag_0==1)
      {
        DEL_SetTimer(0,100);
        while(1)
        {
          Key_Flag_0=0;
          if(DEL_GetTimer(0) == TRUE) {break;}
        }
      }
  }
  else
  {
    Set_RedLed(LED_OFF);
  }
  
  if(ReadInput_KEY_3==0)//��ĳ����������ǰȺ��Ϊ
  {
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
  }
  Keyboard_Test();
}

void TASK_WriteFreq(void)
{

}