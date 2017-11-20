#define TASKABLE
#include "AllHead.h"

u8 *ucOSSYSHWID         = "AT^OSSYSHWID=1";
u8 *ucPPPCFG            = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";

u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
u8 *ucPocOpenConfig     = "0000000101";

void Task_RunStart(void)
{
  u8 v;
  if(BootProcess_SIMST_Flag==1)//�յ�ģ�鿪��ָ��:SIMST:1
  {
    v=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));
    BootProcess_SIMST_Flag=0;
    v=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.����PPPCFG
  }
  if(BootProcess_PPPCFG_Flag==1)//����յ�^PPPCFG
  {
    BootProcess_PPPCFG_Flag=0;
    ApiPocCmd_WritCommand(PocComm_SetParam,ucSetParamConfig,strlen((char const *)ucSetParamConfig));//����echat�˺š�IP
    DEL_SetTimer(0,500);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    v=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
  }
  if(ApiAtCmd_GetLoginState()==TRUE)//��¼�ɹ�
  {
    BEEP_Time(50);
  }
  
   /* if()//����յ�^PPPCFG
    {
    //2.
      if()//�յ�+POC:00000000
      {
        //3.��echatӦ��
      }
    }
    if()//+POC:9000 ����Ҫ����
    {
      //�ȴ���¼
      if()//��¼�ɹ�
      {
        if()//����Ⱥ��
        {
          //�˳�Task_Start��־λ��������һ��־λ
        }
      }
    }
    else//90001��90002
    {
      //����Task_Start��־λ
    }
    */
 
}

void TASK_WriteFreq(void)
{

}