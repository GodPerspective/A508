#define TASKABLE
#include "AllHead.h"
bool NoUseNum=FALSE;
u8 AlarmCount=4;//2G3G�л�����,Ĭ��Ϊ3Gģʽ

#if 1 //test
u8 Key_Flag_0=0;
u8 Key_PersonalCalling_Flag=0;
bool TASK_Ptt_StartPersonCalling_Flag=FALSE;
u8 x=0;
#endif

u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";
u8 *ucRequestUserListInfo       = "0E000000000064";
u8 *ucCLVL                       = "AT+CLVL=7";//��������7
u8 *ucVGR                       = "AT+VGR=7";//��������7
u8 *ucCODECCTL                  = "at^codecctl=9000,1c00,0";//��������4000 3c00
u8 *ucOSSYSHWID                 = "AT^OSSYSHWID=1";
u8 *ucPrefmode2                  = "AT^prefmode=2";//����ģʽ2G
u8 *ucPrefmode4                  = "AT^prefmode=4";//����ģʽ3G
u8 *ucPrefmode8                  = "AT^prefmode=8";//����ģʽ2/3G�Զ��л�
u8 *ucCSQ                       = "AT+CSQ?";
u8 *ucPPPCFG                    = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
//u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";ŷ�����Խ���
u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"2d4e745113663d6d20007f5edf57f95bb28b\"";
u8 *ucPocOpenConfig             = "0000000101";
//u8 *ucPocOpenConfig             = "00000001010101";//˫ȫ��

void Task_RunStart(void)
{
  
  UART3_ToMcuMain();
  if(BootProcess_SIMST_Flag==1)//�յ�ģ�鿪��ָ��:SIMST:1
  {
    api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//GPRS���ź�ͼ��
    
    BEEP_Time(50);
    
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucCLVL,strlen((char const *)ucCLVL));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR,strlen((char const *)ucVGR));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucCODECCTL,strlen((char const *)ucCODECCTL));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//�����α����Խ���
    api_lcd_pwr_on_hint("������Ǣ����Խ�");
      //api_lcd_pwr_on_hint("��1 b c ����Խ�");
    Delay_100ms(25);//2.5s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.����PPPCFG
    BootProcess_SIMST_Flag=0;
    VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//������������
    //DEL_SetTimer(0,100);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    api_lcd_pwr_on_hint("   ��������...  ");
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
    //Delay_100ms(100);//0.1s
    //DEL_SetTimer(0,100);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  }
  else
  {
    if(BootProcess_SIMST_Flag==2)
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//�����첻����
      api_lcd_pwr_on_hint("    �첻����    ");
      Delay_100ms(100);//10s
    }
  }
  if(CSQ_Flag==1)//CSQ?
  {
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
    api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//�����ѵ��źţ���ʾ3Gͼ��
    api_lcd_pwr_on_hint("   ���ڵ�½..     ");
    if(BootProcess_PPPCFG_Flag_Zanshi==1)//����յ�^PPPCFG//��Ϊ��ʱ�ղ�����ָ���ʱ���Σ����ڼ���
    {
      Delay_100ms(10);//1s
      ApiPocCmd_WritCommand(PocComm_SetParam,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));//����echat�˺š�IP
      Delay_100ms(40);//4s
      VOICE_SetOutput(ATVOICE_FreePlay,"636b28577b764696",16);//�������ڵ�½
     api_lcd_pwr_on_hint("   ���ڵ�½...    ");
    //  Delay_100ms(10);//1s
      ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      BootProcess_PPPCFG_Flag_Zanshi=0;
    }
  }
  else
  {
    if(CSQ_Flag==2)
    {
      Delay_100ms(50);//5s
      VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//������������
      api_lcd_pwr_on_hint("   ��������...  ");
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
    }
  }
}

void Task_RunNormalOperation(void)
{
  Keyboard_Test();
  UART3_ToMcuMain();
/***********PTT״̬���*************************************************************************************************************************/
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://Ĭ��PTT״̬
      Set_RedLed(LED_ON);
      Set_GreenLed(LED_OFF);
      ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      while(ReadInput_KEY_PTT==0)
      {
        api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
      api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      break;
    case 1://=1������ĳȺ��
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
      KeyUpDownCount=0;
      break;
    case 2://=2,����ĳ�û�
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_Invite,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
      TASK_Ptt_StartPersonCalling_Flag=TRUE;//�ж���������״̬��0a��
      break;
    default:
      break;
    }
    if(Key_Flag_0==1)
    {
     // DEL_SetTimer(0,100);
      //while(1)
      {
        //Key_Flag_0=0;
        //if(DEL_GetTimer(0) == TRUE) {break;}
      }
    }
  }
  else
  {
    Set_RedLed(LED_OFF);

  }
/*******�����״̬���***********************************************************************************************************************************/
  if(ReadInput_KEY_3==0)//�����
  {
    if(GetPersonalCallingMode()==1)//����ǵ���ģʽ�����˳�����
    {
      api_lcd_pwr_on_hint("    �˳�����    ");
      Delay_100ms(5);
      ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
      api_lcd_pwr_on_hint("Ⱥ��:   ���ģʽ");
    }
    else
    {
      api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
      api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    
      Key_PersonalCalling_Flag=0;
      VOICE_SetOutput(ATVOICE_FreePlay,"535f4d52A47FC47E",16);//��ǰȺ��
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      //VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),strlen((char const *)ApiAtCmd_GetMainWorkName()));
      // VOICE_SetOutput(ATVOICE_FreePlay,"13663d6d4b6dd58bc47e3100",strlen((char const *)"13663d6d4b6dd58bc47e3100"));
      
    }
  }
/*******������״̬���***************************************************************************************************************************************/
  if(ReadInput_KEY_2==0)//������
  {
    api_lcd_pwr_on_hint("����:   ѡ�����");
    api_lcd_pwr_on_hint2(HexToChar_MainUserId());
    
    x++;
    
    if(x>=2)
    {
      Key_PersonalCalling_Flag=1;
      VOICE_SetOutput(ATVOICE_FreePlay,"2a4e7c542000106258540990e962",28);//������Աѡ��
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    x=1;
    }

    
    ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(0),ApiAtCmd_GetUserNameLen(0));
    KeyDownUpChoose_GroupOrUser_Flag=2;
  }
/*******������״̬���********************************************************************************************************************************************/
  if(ReadInput_KEY_4==0)//������
  {
    switch(AlarmCount)
    {
    case 4://�л�Ϊ2Gģʽ
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e32004700216a0f5f",36);//�����л�Ϊ2Gģʽ
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode2,strlen((char const *)ucPrefmode2));//
      AlarmCount=8;
      break;
    /*case 2://�л�Ϊ�Զ�ģʽ
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4eea81a852216a0f5f",36);//�����л�Ϊ�Զ�ģʽ
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode8,strlen((char const *)ucPrefmode8));//
      AlarmCount=8;
      break;*/
    case 8://�л�Ϊ3Gģʽ
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e33004700216a0f5f",36);//�����л�Ϊ3Gģʽ
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//ͼ�꣺3G
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
      AlarmCount=4;
      break;
    default:
      break;
    }
  }
  
/***********�ж��������飻�����˳���;������ģʽ���˳�����ģʽ��������ʼ�����������������˳�����*********************************************************************************************************************/
  /*if(POC_AtEnterPersonalCalling_Flag==2)//������ʼ����ģʽ
  {
      api_lcd_pwr_on_hint(HexToChar_MainUserId());//��ʾ��ǰȺ��ID
      api_lcd_pwr_on_hint4("             ");//����
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      POC_AtEnterPersonalCalling_Flag=1;
  }
  else
  {
    if(POC_AtQuitPersonalCalling_Flag==2)
    {
      POC_AtQuitPersonalCalling_Flag=0;
      //�˳��������账������Ⱥ�鴦��
    }
  }*/



  if(POC_EnterPersonalCalling_Flag==2)//��������ڽ��뵥��-------------------------------
    {
      
      api_lcd_pwr_on_hint(HexToChar_MainUserId());//��ʾ��ǰȺ��ID
      api_lcd_pwr_on_hint4("             ");//����
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      POC_EnterPersonalCalling_Flag=1;//�ڵ���ģʽ
    }
    else//������������飻���ڣ������˳���;�������˳�����ģʽ��
    {
      if(POC_EnterPersonalCalling_Flag==0)//������������飻���ڣ������˳���
      {
        if(POC_EnterGroupCalling_Flag==2)//���ڽ���Ⱥ��
        {

          if(POC_AtEnterPersonalCalling_Flag==2)//������ʼ����ģʽ
          {
          api_lcd_pwr_on_hint("                ");//����
          api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
          api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
          api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
          api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
            POC_AtEnterPersonalCalling_Flag=1;
          }
          else
          {
            if(POC_AtEnterPersonalCalling_Flag==1)
            {}
            else
            {
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
            }
          }
          POC_EnterGroupCalling_Flag=1;
          
        }
        else if(POC_EnterGroupCalling_Flag==1)//����
        {
        }
        else//�˳����
        {
          if(POC_QuitGroupCalling_Flag==2)
          {
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
              POC_QuitGroupCalling_Flag=0;
          }

        }
      }
      else//����ģʽ���˳�����ģʽ��
      {
        if(POC_QuitPersonalCalling_Flag==2)//������˳�����ģʽ----------------------------
        {
          POC_QuitPersonalCalling_Flag=0;
          //�˳��������账������Ⱥ�鴦�� 
        }
        else//����ģʽ
        {

        }
      }
    }

/*********�жϷ������ͼ��״̬****************************************************************************************************************************/
if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
{
  api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
  api_disp_all_screen_refresh();// ȫ��ͳһˢ��
  POC_ReceivedVoiceStart_Flag=1;//��������״̬
}
else//0����״̬��1����״̬
{
  if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
  {
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
  }
  else//����״̬
  {}
}
}

void TASK_WriteFreq(void)
{
  UART3_ToMcuMain();
}
void TASK_RunLoBattery(void)
{
  api_lcd_pwr_on_hint(" ������  ����  ");
  VOICE_SetOutput(ATVOICE_FreePlay,"3575606c3575cf914e4f0cfff78b45513575",36);//Ⱥ��ѡ��
  DEL_SetTimer(0,1000);
  while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  BEEP_Time(10);
}
void Delay_100ms(u8 T)
{
  u16 i;
  u8 j,k,l;
    for(j = 0; j < 83; j++)
    for(l = 0; l < 10; l++)
        for(k = 0; k < 100; k++)
          for(i = 0; i < T; i++)
      {
        nop();
      }
  return;
}

