#define TASKABLE
#include "AllHead.h"
u8 SSWLCount=0;
u8 StartingUpStep=0;
Key3_OptionType Key3Option;
bool NoUseNum=FALSE;
u8 AlarmCount=4;//2G3G�л�����,Ĭ��Ϊ3Gģʽ
u8 NetworkType_2Gor3G_Flag=3;
u8 PersonCallIco_Flag=0;//������ʾ�������ͼ���ж�״̬
#if 1 //test
u8 Key_PersonalCalling_Flag=0;
bool TASK_Ptt_StartPersonCalling_Flag=FALSE;
bool Task_Landing_Flag=FALSE;
u8 EnterPttMomentCount=0;
u8 LoosenPttMomentCount=0;
bool EnterPttMoment_Flag=FALSE;
bool LoosenPttMoment_Flag=FALSE;
bool EnterKeyTime_2s_Flag=FALSE;
bool KEY_4_Flag=FALSE;
u8 TaskStartDeleteDelay1=0;
u8 TaskStartDeleteDelay2=0;
u8 TaskStartDeleteDelay3=0;
u8 TaskStartDeleteDelay4=1;
u8 TaskStartDeleteDelay5=0;
u8 TaskStartDeleteDelay6=0;
#endif
u8 key_warning_flag=FALSE;
u8 key_warning_bubiao_flag=FALSE;
u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";

u8 *ucCLVL                       = "AT+CLVL=7";//��������7
u8 *ucVGR                       = "AT+VGR=7";//��������7
#if 1
u8 *ucCODECCTL                  = "at^codecctl=2300,2300,0";//T4//�ͻ�ѡ�������
//u8 *ucCODECCTL                  = "at^codecctl=2000,1800,0";//AT^codecctl=2870,8000,0�����๤����
#else
u8 *ucCODECCTL                  = "at^codecctl=5000,2300,0";//����ԭӲ�����ȷ��͸���־���Ĳ��Ȱ汾
//u8 *ucCODECCTL                  = "at^codecctl=9000,1c00,0";//Ĭ����������9000,1c00
#endif
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

void Key3_PlayVoice(void);

void Task_RunStart(void)
{
  UART3_ToMcuMain();
  if(BootProcess_SIMST_Flag==1)//�յ�ģ�鿪��ָ��:SIMST:1
  {
    if(TaskStartDeleteDelay1==1)
    {
      api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//GPRS���ź�ͼ��
#if 1
      
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)"AT^rxfilter=0x1BA,0xFCB8,0xFAB7,0xFF11,0x9CF,0x1871,0x2C20",strlen((char const *)"AT^rxfilter=0x1BA,0xFCB8,0xFAB7,0xFF11,0x9CF,0x1871,0x2C20"));//���ӳ�����T1-ͦ�� ��Х��
      //NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)"AT^rxfilter=0x310,0xf9f0,0xf8e5,0xfc37,0xfeff,0x106b,0x2D15",strlen((char const *)"AT^rxfilter=0x310,0xf9f0,0xf8e5,0xfc37,0xfeff,0x106b,0x2D15"));//�����๤�������ߵ����Σ������˵��Եģ�
      //NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)"AT^rxfilter=0x27F,0xAB,0xFD08,0xFADC,0xFC19,0xD74,0x27DA",strlen((char const *)"AT^rxfilter=0x27F,0xAB,0xFD08,0xFADC,0xFC19,0xD74,0x27DA"));//�����๤�������ߵڶ���
      //NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)"AT^rxfilter=0xFBA8,0xF9DD,0xFD4F,0xFE10,0xFC7A,0x1071,0x2D8D",strlen((char const *)"AT^rxfilter=0xFBA8,0xF9DD,0xFD4F,0xFE10,0xFC7A,0x1071,0x2D8D"));//�����๤��������

#else 
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)"AT^RXFILTER=0xDD,0x2F3,0xFC52,0x1D,0xFDF3,0xBEB,0x2CF7",strlen((char const *)"AT^RXFILTER=0xDD,0x2F3,0xFC52,0x1D,0xFDF3,0xBEB,0x2CF7"));//Ĭ������
#endif
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucCLVL,strlen((char const *)ucCLVL));//
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR,strlen((char const *)ucVGR));//
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucCODECCTL,strlen((char const *)ucCODECCTL));//
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));//
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//�����α����Խ���
      api_lcd_pwr_on_hint("������Ǣ����Խ�");
      //api_lcd_pwr_on_hint("China Abell");
      TaskStartDeleteDelay1=2;
    }
    if(TaskStartDeleteDelay2==1)
    {
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.����PPPCFG
      BootProcess_SIMST_Flag=0;
      VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//������������
      api_lcd_pwr_on_hint("   ��������...  ");
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM15_HDRCSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
      StartingUpStep=1;
      TaskStartDeleteDelay2=2;
    }
  }
  else
  {
    if(BootProcess_SIMST_Flag==2)
    {
      if(TaskStartDeleteDelay3==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//�����첻����
        api_lcd_pwr_on_hint("    �첻����    ");
        TaskStartDeleteDelay3=2;
      }
    }
  }
  if(StartingUpStep==1)
  {
    if(HDRCSQValue>=30)//CSQ?
    {
      if(TaskStartDeleteDelay4==1)
      {
        SSWLCount=0;
        HDRCSQSignalIcons();
        api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//�����ѵ��źţ���ʾ3Gͼ��
        api_lcd_pwr_on_hint("   ���ڵ�½..     ");
        ApiPocCmd_WritCommand(PocComm_SetParam,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));//����echat�˺š�IP
        TaskStartDeleteDelay4=2;
      }
      if(TaskStartDeleteDelay5==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"636b28577b764696",16);//�������ڵ�½
        api_lcd_pwr_on_hint("   ���ڵ�½...    ");
        ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
        Task_Landing_Flag=TRUE;
        StartingUpStep=0;
        TaskStartDeleteDelay5=2;
      }
    }
    else
    {
      if(HDRCSQValue<30&&BootProcess_SIMST_Flag==0)
      {
        if(TaskStartDeleteDelay6==1)
        {
          SSWLCount++;
          if(SSWLCount==10||SSWLCount==20)
          {
            VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7ee14ff753315f",20);//���������ź���
          }
          else if(SSWLCount==30)
          {
            ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
            SSWLCount=0;
          }
          else
          {
            VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//������������
          }
          api_lcd_pwr_on_hint("   ��������...  ");
          if(NetworkType_2Gor3G_Flag==3)//�����3G����HDRCSQ��2G����CSQ
            ApiAtCmd_WritCommand(ATCOMM15_HDRCSQ, (void*)0, 0);
          else
          {
            if(NetworkType_2Gor3G_Flag==2)
            {
              ApiAtCmd_WritCommand(ATCOMM6_CSQ, (void*)0, 0);
            }
          }
          TaskStartDeleteDelay6=0;
        }
      }
    }
  }
}

void Task_RunNormalOperation(void)
{
  if(allow_key_operation_flag==TRUE)
  {
    if(huo_qu_zhong_flag==TRUE)
    {}
    else
    {
      Keyboard_Test();
    }
  }
  
  UART3_ToMcuMain();
/***********PTT״̬���*************************************************************************************************************************/
  if(ReadInput_KEY_PTT==0)//�жϰ���PTT��˲��
  {
    //AUDIO_IOAFPOW(ON);//�򿪹��ţ����DIDI��ʾ��������
    EnterPttMomentCount++;
    if(EnterPttMomentCount==1)
      EnterPttMoment_Flag=TRUE;
    else
    {
      EnterPttMoment_Flag=FALSE;
      EnterPttMomentCount=3;
    }
    LoosenPttMoment_Flag=FALSE;
    LoosenPttMomentCount=0;
  }
  else
  {
    EnterPttMomentCount=0;
    EnterPttMoment_Flag=FALSE;
    LoosenPttMomentCount++;
    if(LoosenPttMomentCount==1)
      LoosenPttMoment_Flag=TRUE;
    else
    {
      LoosenPttMoment_Flag=FALSE;
      LoosenPttMomentCount=3;
    }
  }
//���дƵʱ��Ӱ����������ʹ�ã������������ڽ���״̬��
  if(WriteFreq_Flag==TRUE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
  {
    ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
  }

  switch(KeyPttState)//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
  {
  case 0://δ��PTT
    POC_ReceivedVoiceEndForXTSF_Flag=0;
    if(KeyDownUpChoose_GroupOrUser_Flag==0)
    {
      //if(POC_ReceivedVoice_Flag==FALSE)
      {
        if(WriteFreq_Flag==FALSE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
        {
          if(EnterPttMoment_Flag==TRUE)
          {
            ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
          }
        }
      }
    }
    break;
  case 1://1:����ptt˲��
    KeyPttState=2;
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      Set_RedLed(LED_OFF);
    }
    break;
  case 2://2����סPTT״̬
    if(POC_ReceivedVoice_Flag==TRUE)//������ڽ�������
    {
    }
    else
    {
      //�������ʱ�䵽ʱ��������ϵͳ�ͷš���������ֹͣ���䡣����ʾ��������Ų���ʧ����ɫָʾ�Ʋ�Ϩ��
      if(POC_ReceivedVoiceEndForXTSF_Flag==2)
      {
        //Set_RedLed(LED_OFF);
        KeyDownUpChoose_GroupOrUser_Flag=0;
      }
      else
      {
        Set_RedLed(LED_ON);
        Set_GreenLed(LED_OFF);
      if(TheMenuLayer_Flag!=0)//�������ʱӰ��˵�������Ϣ��ʾ������ֻҪ��PTT�ͻ��˳��˵�
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
        MenuModeCount=1;
        TheMenuLayer_Flag=0;
        MenuMode_Flag=0;
        ApiMenu_SwitchGroup_Flag=0;
        ApiMenu_SwitchCallUser_Flag=0;
        ApiMenu_SwitchOnlineUser_Flag=0;
        ApiMenu_GpsInfo_Flag=0;
        ApiMenu_BacklightTimeSet_Flag=0;
        ApiMenu_KeylockTimeSet_Flag=0;
        ApiMenu_NativeInfo_Flag=0;
        ApiMenu_BeiDouOrWritingFrequency_Flag=0;
      }
      api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
    }
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      
      Set_RedLed(LED_OFF);
    }
    break;
  case 3://3���ɿ�PTT˲��
    POC_ReceivedVoiceEndForXTSF_Flag=0;
    KeyPttState=0;
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
#if 1//������ٰ�����PTT�쳣������
    if(EnterPttMoment_Flag==TRUE)
    {
      ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
    }
#endif
    break;
  default:
    break;
  }
  
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://Ĭ��PTT״̬
      break;
    case 1://=1������ĳȺ��
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
      Delay_100ms(4);//DEL_SetTimer(0,40);
      //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      KeyDownUpChoose_GroupOrUser_Flag=0;
      EnterKeyTimeCount=0;
      KeyUpDownCount=0;
      break;
    case 2://=2,����ĳ�û�
      if(GettheOnlineMembersDone==TRUE)
      {
        //GettheOnlineMembersDone=FALSE;
        VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
        Delay_100ms(6);//DEL_SetTimer(0,60);
        //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
        ApiPocCmd_WritCommand(PocComm_Invite,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
        KeyDownUpChoose_GroupOrUser_Flag=0;
        TASK_Ptt_StartPersonCalling_Flag=TRUE;//�ж���������״̬��0a��
        EnterKeyTimeCount=0;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      break;
    }
  }
  else
  {
    //Set_RedLed(LED_OFF);

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
      if(TheMenuLayer_Flag!=0)//��������Ӱ��˵�������Ϣ��ʾ������ֻҪ��������ͻ��˳��˵�
      {
        MenuDisplay(Menu_RefreshAllIco);
        MenuModeCount=1;
        TheMenuLayer_Flag=0;
        MenuMode_Flag=0;
        ApiMenu_SwitchGroup_Flag=0;
        ApiMenu_SwitchCallUser_Flag=0;
        ApiMenu_SwitchOnlineUser_Flag=0;
        ApiMenu_GpsInfo_Flag=0;
        ApiMenu_BacklightTimeSet_Flag=0;
        ApiMenu_KeylockTimeSet_Flag=0;
        ApiMenu_NativeInfo_Flag=0;
        ApiMenu_BeiDouOrWritingFrequency_Flag=0;
      }
      Key3_PlayVoice();
    }
  }
/*******������״̬���***************************************************************************************************************************************/
  if(ReadInput_KEY_2==0)//������
  {
    ApiPocCmd_PersonalCallingMode=TRUE;
    if(POC_EnterPersonalCalling_Flag==1)//�������״̬�£�����������Ч��Ӧ���Ǳ���״̬�£��ø�����ʧЧ��
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"ab887c542d4e",12);//������
      Delay_100ms(5);//DEL_SetTimer(0,50);
      //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    }
    else
    {
      if(TheMenuLayer_Flag!=0)//���������Ӱ��˵�������Ϣ��ʾ������ֻҪ���������ͻ��˳��˵�
      {
          MenuDisplay(Menu_RefreshAllIco);
          MenuModeCount=1;
          TheMenuLayer_Flag=0;
          MenuMode_Flag=0;
          ApiMenu_SwitchGroup_Flag=0;
          ApiMenu_SwitchCallUser_Flag=0;
          ApiMenu_SwitchOnlineUser_Flag=0;
          ApiMenu_GpsInfo_Flag=0;
          ApiMenu_BacklightTimeSet_Flag=0;
          ApiMenu_KeylockTimeSet_Flag=0;
          ApiMenu_NativeInfo_Flag=0;
          ApiMenu_BeiDouOrWritingFrequency_Flag=0;
      }
              api_lcd_pwr_on_hint("    ����ģʽ    ");
              PersonalCallingNum=0;//�����������ֱ��ѡ�У������û������ǲ������û�
              Key_PersonalCalling_Flag=1;
              VOICE_SetOutput(ATVOICE_FreePlay,"C5627C54216A0F5F",16);//����ģʽ
              Delay_100ms(6);//DEL_SetTimer(0,65);
              //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
              ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
              KeyDownUpChoose_GroupOrUser_Flag=2;
              KeyPersonalCallingCount=0;//�������ģʽ�����¼���Ա������˳�򣬵�һ����Ա���л�ʱ��ڶ�������������
    }
  }
/*******������״̬���********************************************************************************************************************************************/
  if(ReadInput_KEY_4==0)//������
  {
#if 1//���Բ��걨������
    key_warning_flag=TRUE;
    key_warning_bubiao_flag=TRUE;
#else
    switch(AlarmCount)
    {
    case 4://�л�Ϊ2Gģʽ
      NetworkType_2Gor3G_Flag=2;
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e32004700216a0f5f",36);//�����л�Ϊ2Gģʽ
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0���ź�
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      Delay_100ms(10);//DEL_SetTimer(0,100);
      //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode2,strlen((char const *)ucPrefmode2));//
      AlarmCount=8;
      break;
    case 8://�л�Ϊ3Gģʽ
      NetworkType_2Gor3G_Flag=3;
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e33004700216a0f5f",36);//�����л�Ϊ3Gģʽ
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//ͼ�꣺3G
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0���ź�
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      Delay_100ms(10);//DEL_SetTimer(0,100);
      //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
      AlarmCount=4;
      break;
    default:
      break;
    }
#endif
  }
  
/***********�ж��������飻�����˳���;������ģʽ���˳�����ģʽ��������ʼ�����������������˳�����*************/
  if(POC_EnterPersonalCalling_Flag==2)//1�ձ���
  {
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
    if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
    {
    api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
    }
    else
    {
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
    }
    api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
#if 1//�������ʱ��ʱ����ʾ�ĵ�һ�������ӳ���ʧ����ˢ������ͼ�������
    if(VoiceType_FreehandOrHandset_Flag==0)
      api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
    else
      api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
#endif
    PersonCallIco_Flag=1;
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_EnterPersonalCalling_Flag=1;//�ڵ���ģʽ
    POC_EnterGroupCalling_Flag=1;
  }
  else if(POC_EnterPersonalCalling_Flag==1)//2����״̬
  {
    //api_lcd_pwr_on_hint("   2����״̬     ");
  }
  else//���顢���ڡ����顢��ʼ�����������С���������
  {
    if(POC_EnterGroupCalling_Flag==2)//1����
    {
      if(POC_AtEnterPersonalCalling_Flag==2)//������ʼ����ģʽ
      {
      }
      else if(POC_AtEnterPersonalCalling_Flag==1)//������
      {
      }
      else
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
        api_disp_icoid_output( BatteryLevel, TRUE, TRUE);
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
        //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�--2
        PersonCallIco_Flag=0;
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
      }
      POC_EnterGroupCalling_Flag=1;//��������
    }
    else if(POC_EnterGroupCalling_Flag==1)//2����
    {
      if(POC_AtEnterPersonalCalling_Flag==2)//1������
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
      if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
      {
        api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
      else
      {
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
        PersonCallIco_Flag=1;
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
        POC_AtEnterPersonalCalling_Flag=1;
      }
      else if(POC_AtEnterPersonalCalling_Flag==1)//2������
      {
      }
      else//3��������
      {
      }
    }
    else//����
    {
      if(POC_QuitGroupCalling_Flag==2)
      {
        if(POC_QuitPersonalCalling_Flag==2)//����ģʽ����
        {
          POC_QuitPersonalCalling_Flag=0;
          Key_PersonalCalling_Flag=0;
        }
        if(POC_AtQuitPersonalCalling_Flag==2)//����ģʽ����
        {
          POC_AtQuitPersonalCalling_Flag=0;
          Key_PersonalCalling_Flag=0;
        }
        
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//����л�����,��PTTȷ�ϣ���������ģʽʱ���м䲻Ӧ��ʾһ�������Ϣ������ʾ����
        {
          //api_lcd_pwr_on_hint("   ����BUG     ");
        }
        else
        {
          api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
          PersonCallIco_Flag=0;
        }
        POC_QuitGroupCalling_Flag=1;
      }
    }

  }
/*********�жϷ������ͼ��״̬****************************************************************************************************************************/
if(PersonCallIco_Flag==1)
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    if(ShowTime_Flag==FALSE)
    {
      api_disp_icoid_output(eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
    }
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
else
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_lcd_pwr_on_hint("                ");//����
    if(UnicodeForGbk_SpeakerRightnowName_english_flag()==TRUE)
    {
      api_lcd_pwr_on_hint(UnicodeForGbk_SpeakerRightnowName());//��ʾ��ǰ˵���˵��ǳ�
    }
    else
    {
      api_lcd_pwr_on_hint4(UnicodeForGbk_SpeakerRightnowName());//��ʾ��ǰ˵���˵��ǳ�
    }
    //api_lcd_pwr_on_hint4("1234567890123");//��ʾ��ǰ˵���˵��ǳ�
    POC_ReceivedVoiceStart_Flag=1;//��������״̬
    //�޸�BUG�� A������״̬��B����A����A����PTTȴ�ǻ��飨������A��Ӧ�÷���Ĭ��״̬����
    KeyDownUpChoose_GroupOrUser_Flag=0;
    KeyUpDownCount=0;
    //�޸�BUG���ڲ˵����棬B����A������ʾ����ʾ���ҵ����⣨��Ϊ����A���˳��˵���
    if(TheMenuLayer_Flag!=0)
    {
      MenuDisplay(Menu_RefreshAllIco);
      MenuModeCount=1;
      TheMenuLayer_Flag=0;
      MenuMode_Flag=0;
      ApiMenu_SwitchGroup_Flag=0;
      ApiMenu_SwitchCallUser_Flag=0;
      ApiMenu_SwitchOnlineUser_Flag=0;
      ApiMenu_GpsInfo_Flag=0;
      ApiMenu_BacklightTimeSet_Flag=0;
      ApiMenu_KeylockTimeSet_Flag=0;
      ApiMenu_NativeInfo_Flag=0;
      ApiMenu_BeiDouOrWritingFrequency_Flag=0;
    }
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    if(ShowTime_Flag==FALSE)
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//�����˵��ڱ�����״̬��δˢ�µ�BUG--------------------
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��

  }
  else if(POC_ReceivedVoiceStart_Flag==1)//0����״̬��1����״̬//���Խ����������
  {
    if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
    {
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_lcd_pwr_on_hint("                ");//����
    //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�--3
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceStart_Flag=0;//�����յ�ff�����㣬���յ�endFLAG���������
    POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
    Key_PersonalCalling_Flag=0;//��������������󣬰����¼���Ȼ���л�������Ա
    }
    else//����״̬
    {}
  }
  else
  {
  }
}

/********���ƹ�������*************************************/
#if 1
if(ApiPocCmd_PlayReceivedVoice_Flag==TRUE)
{
  AUDIO_IOAFPOW(ON);
}
else
{
  if(key_warning_flag==TRUE)
  {
     AUDIO_IOAFPOW(ON);
  }
  else
  {
    if(ApiAtCmd_ZTTS_Flag==TRUE)
    {
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ApiPocCmd_Tone_Flag==TRUE)
      {
        AUDIO_IOAFPOW(ON);
      }
      else
      {
        AUDIO_IOAFPOW(OFF);
      }
    }
  }
}

#else
AUDIO_IOAFPOW(ON);
#endif

/*****���û�����߳�Ա******************************************/
if(PocNoOnlineMember_Flag==TRUE)
{
  if(get_online_user_list_num_flag==TRUE)
  {
    get_online_user_list_num_flag=FALSE;
    api_lcd_pwr_on_hint2(HexToChar_AllOnlineMemberNum());
    PersonalCallingNum=0;//�����������ֱ��ѡ�У������û������ǲ������û�
    TheMenuLayer_Flag=2;
    KeyPersonalCallingCount=0;//�������ģʽ�����¼���Ա������˳�򣬵�һ����Ա���л�ʱ��ڶ�������������
  }
  else
  {
    MenuMode_Flag=0;
    api_lcd_pwr_on_hint("                ");//����
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    //����PTT�������¼�����Ĭ��״̬
    KeyUpDownCount=0;
    Key_PersonalCalling_Flag=0;//���������־λ
    KeyDownUpChoose_GroupOrUser_Flag=0;//����������������ؼ���OK��PTT����Ļ��ʾ�����BUG
  }
  PocNoOnlineMember_Flag=FALSE;
}
}
void TASK_WriteFreq(void)
{
  UART3_ToMcuMain();
}
void TASK_RunLoBattery(void)
{
#if 1
  api_lcd_pwr_on_hint(" ������  ����  ");
  VOICE_SetOutput(ATVOICE_FreePlay,"3575606c3575cf914e4f0cfff78b73513a6745513575",44);//����������
  Delay_100ms(100);//DEL_SetTimer(0,1000);
  BEEP_Time(10);
#else
  ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)"at+GPSFUNC=0",strlen((char const *)"at+GPSFUNC=0"));//
  ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)"at+pwroff",strlen((char const *)"at+pwroff"));//
#endif
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

void Key3_PlayVoice(void)
{
  switch(Key3Option)
  {
  case Key3_OptionZero://���������˺š���ǰȺ�顢��ص���
    //��ǰ�û���
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(Get_GBK_ActiveUserID());//��ʾ��ǰ�û���
    VOICE_SetOutput(ATVOICE_FreePlay,Get_Unicode_ActiveUserID(),strlen((char const *)Get_Unicode_ActiveUserID()));//������ǰ�û��ֻ���
    Delay_100ms(28);//DEL_SetTimer(0,285);
    //��ǰȺ��
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),strlen((char const *)ApiAtCmd_GetMainWorkName()));
    Delay_100ms(20);//DEL_SetTimer(0,200);
    //��������
    KeyBatteryReport();
    break;
  case Key3_OptionOne://���������˺š���ص���
    //��ǰ�û���
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(Get_GBK_ActiveUserID());//��ʾ��ǰ�û���
    VOICE_SetOutput(ATVOICE_FreePlay,Get_Unicode_ActiveUserID(),strlen((char const *)Get_Unicode_ActiveUserID()));//������ǰ�û��ֻ���
    Delay_100ms(28);//DEL_SetTimer(0,285);
    //��ǰȺ��
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    //��������
    KeyBatteryReport();
    break;
  case Key3_OptionTwo://���������˺�
    //��ǰ�û���
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(Get_GBK_ActiveUserID());//��ʾ��ǰ�û���
    VOICE_SetOutput(ATVOICE_FreePlay,Get_Unicode_ActiveUserID(),strlen((char const *)Get_Unicode_ActiveUserID()));//������ǰ�û��ֻ���
    Delay_100ms(20);//DEL_SetTimer(0,200);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //��ǰȺ��
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    break;
  case Key3_OptionThree://������ǰȺ��
    //��ǰȺ��
    api_lcd_pwr_on_hint("                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),strlen((char const *)ApiAtCmd_GetMainWorkName()));
    Delay_100ms(20);//DEL_SetTimer(0,20);
    break;
  case Key3_OptionFour://������ص���
    //��������
    KeyBatteryReport();
    Delay_100ms(2);//DEL_SetTimer(0,20);
    break;
  default:
    break;
  }
}

