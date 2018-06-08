#include "AllHead.h"
u8 *ucNetworkMode2                  = "AT^prefmode=2";//����ģʽ2G
u8 *ucNetworkMode3                  = "AT^prefmode=4";//����ģʽ3G
u8 network_mode_2gor3g_SetCount=0x18;
  u8 BacklightTimeBuf1[1]={0};
  u8 BacklightTimeBuf2[1]={0};
#if 1
u8 ApiMenu_SwitchGroup_Flag=0;
u8 ApiMenu_SwitchCallUser_Flag=0;
u8 ApiMenu_SwitchOnlineUser_Flag=0;
u8 ApiMenu_GpsInfo_Flag=0;
u8 ApiMenu_NativeInfo_Flag=0;
u8 ApiMenu_BacklightTimeSet_Flag=0;
u8 ApiMenu_KeylockTimeSet_Flag=0;
u8 ApiMenu_BeiDouOrWritingFrequency_Flag=0;
#endif


void MenuDisplay(MenuDisplayType id)
{
  switch(id)
  {
  case Menu0:
    break;
  case Menu1:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("1/8");
     api_lcd_pwr_on_hint("Ⱥ��ѡ��        ");
    break;
  case Menu2:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("2/8");
     api_lcd_pwr_on_hint("��Աѡ��        ");
    break;
  case Menu3:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("3/8");
     api_lcd_pwr_on_hint("���߳�Ա�б�    ");
    break;
  case Menu4:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("4/8");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
     break;
     
  case Menu5:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("5/8");
     api_lcd_pwr_on_hint("���������      ");
     if(ApiMenu_BacklightTimeSet_Flag==2)
     {
       ApiMenu_BacklightTimeSet_Flag=0;
       if(BacklightTimeSetCount==7)
       {
         BacklightTimeBuf1[0]=0;
         FILE_Write2(0x236,1,BacklightTimeBuf1);//����ʱ�䡾�롿
       }
       else
       {
         BacklightTimeBuf1[0]=BacklightTimeSetCount;
         FILE_Write2(0x236,1,BacklightTimeBuf1);//����ʱ�䡾�롿
       }
     }
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("6/8");
     api_lcd_pwr_on_hint("��������        ");
     if(ApiMenu_KeylockTimeSet_Flag==2)
     {
       ApiMenu_KeylockTimeSet_Flag=0;
       if(KeylockTimeSetCount==0x10)
       {
         //KeylockTimeCount=200;//���=200����Զ������
         BacklightTimeBuf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,BacklightTimeBuf2);//������ʱ�䡾�롿
       }
       else
       {
         //KeylockTimeCount=(KeylockTimeSetCount-0x10)*30;
         BacklightTimeBuf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,BacklightTimeBuf2);//������ʱ�䡾�롿
       }
     }
     break;
  case Menu7:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("7/8");
     api_lcd_pwr_on_hint("������Ϣ        ");
    break;
  case Menu8:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("8/8");
     api_lcd_pwr_on_hint("�л�����ģʽ    ");
    if(ApiMenu_BeiDouOrWritingFrequency_Flag==2)
    {
      ApiMenu_BeiDouOrWritingFrequency_Flag=0;
      if(network_mode_2gor3g_SetCount==0x18)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e33004700216a0f5f",36);//�����л�Ϊ3Gģʽ
        NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucNetworkMode3,strlen((char const *)ucNetworkMode3));//
        NetworkType_2Gor3G_Flag=3;
      }
      else if(network_mode_2gor3g_SetCount==0x17)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e32004700216a0f5f",36);//�����л�Ϊ2Gģʽ
        NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucNetworkMode2,strlen((char const *)ucNetworkMode2));//
        NetworkType_2Gor3G_Flag=2;
      }
      else
      {
        
      }
    }
     break;
  case Menu_Locking_NoOperation:
    if(PersonCallIco_Flag==0)
    {
      MenuDisplay(Menu_RefreshAllIco);
      api_lcd_pwr_on_hint("                ");//����
      //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    }
    else
    {
      api_lcd_pwr_on_hint3("                ");//����
      MenuDisplay(Menu_RefreshAllIco);
      api_lcd_pwr_on_hint("                ");//����
      if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
      {
        api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
      else
      {
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
    }
    
    break;
  case Menu_unLocking:
    if(PersonCallIco_Flag==0)
    {
      api_lcd_pwr_on_hint3("                ");//����
      MenuDisplay(Menu_RefreshAllIco);
      api_lcd_pwr_on_hint("                ");//����
      //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    }
    else
    {
      api_lcd_pwr_on_hint3("                ");//����
      MenuDisplay(Menu_RefreshAllIco);
      api_lcd_pwr_on_hint("                ");//����
      if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
      {
        api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
      else
      {
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
      }
    }
    break;
  case Menu_RefreshAllIco:
    //api_lcd_pwr_on_hint3("                ");//����
    //api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
    if(HDRCSQValue>=80)//5��
    {
      api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5���ź�
    }
    else if(HDRCSQValue>=70&&HDRCSQValue<80)
    {
      api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4���ź�
    }
    else if(HDRCSQValue>=60&&HDRCSQValue<70)
    {
      api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3���ź�
    }
    else if(HDRCSQValue>=40&&HDRCSQValue<60)
    {
      api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2���ź�
    }
    else if(HDRCSQValue>=20&&HDRCSQValue<40)
    {
      api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1���ź�
    }
    else
    {
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0���ź�
    }
    if(NetworkType_2Gor3G_Flag==3)
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
    else
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
    if(LockingState_Flag==FALSE)
      api_disp_icoid_output( eICO_IDBANDWIDTHN, TRUE, TRUE);//��������ͼ��
    else
    {
      api_disp_icoid_output( eICO_IDBANDWIDTHW, TRUE, TRUE);//����ͼ��
    }
    
    api_disp_icoid_output( BatteryLevel, TRUE, TRUE);//��ص���ͼ��
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    if(ShowTime_Flag==FALSE)
    {
      if(VoiceType_FreehandOrHandset_Flag==0)
        api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
      else
        api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
      if(PersonCallIco_Flag==0)
        api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
      else
        api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��

      if(KeyDownUpChoose_GroupOrUser_Flag==0)
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
      else
        api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
    }
      break;
  case Menu_UnlockStep1_Ok:
    //MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
    api_lcd_pwr_on_hint("�ٰ�#��         ");
    break;
  default:
    break;
  }
}

void SubmenuMenuDisplay(SubmenuMenuDisplayType id)
{
  u8 Buf1[16];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  u8 Buf2[16];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  switch(id)
  {
  case GroupSwitch: 
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
    //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    break;
  case GpsInfoMenu:
     api_lcd_pwr_on_hint("                ");//����
    api_lcd_pwr_on_hint3("                ");//����
    if(PositionInfoSendToATPORT_InfoDisplay_Flag==FALSE)//δ��λ��ʾ000
    {
      api_lcd_pwr_on_hint3("����:000.000000 ");//����
       api_lcd_pwr_on_hint("γ��:00.000000  ");//����
    }
    else
    {
    //���㲢��ʾ����
    Buf1[0]=0xbe;
    Buf1[1]=0xad;
    Buf1[2]=0xb6;
    Buf1[3]=0xc8;
    Buf1[4]=0x3a;
    COML_DecToAsc(Data_Latitude_Minute(), Buf1+5);
    COML_StringReverse(3,Buf1+5);
    Buf1[8]=0x2e;
    COML_DecToAsc(Data_Latitude_Second(), Buf1+9);
    COML_StringReverse(6,Buf1+9);
    Buf1[15]='\0';
    api_lcd_pwr_on_hint3(Buf1);
    //���㲢��ʾγ��
    Buf2[0]=0xce;
    Buf2[1]=0xb3;
    Buf2[2]=0xb6;
    Buf2[3]=0xc8;
    Buf2[4]=0x3a;
    COML_DecToAsc(Data_Longitude_Minute(), Buf2+5);
    COML_StringReverse(2,Buf2+5);
    Buf2[7]=0x2e;
    COML_DecToAsc(Data_Longitude_Second(), Buf2+8);
    COML_StringReverse(6,Buf2+8);
    Buf2[14]='\0';
    api_lcd_pwr_on_hint(Buf2);
    }
    break;
  case NativeInfoMenu:
    MCU_VERSIONForMenu();
    break;
  case BacklightTimeSet:
    Level3MenuDisplay(BacklightTimeSetCount);
    break;
  case KeylockTimeSet:
    Level3MenuDisplay(KeylockTimeSetCount);
    break;
  case BeiDouOrWritingFrequencySwitch:
    if(NetworkType_2Gor3G_Flag==2)
    {
      network_mode_2gor3g_SetCount=0x17;
    }
    else
    {
      network_mode_2gor3g_SetCount=0x18;
    }
    Level3MenuDisplay(network_mode_2gor3g_SetCount);
    break;
  }
}

void Level3MenuDisplay(Level3MenuDisplayType id)
{
  switch(id)
  {
  case BacklightTimeSet_0s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("7/7");
     api_lcd_pwr_on_hint("�ر�            ");
    break;
  case BacklightTimeSet_10s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("1/7");
     api_lcd_pwr_on_hint("5��            ");
    break;
  case BacklightTimeSet_20s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("2/7");
     api_lcd_pwr_on_hint("10��            ");
    break;
  case BacklightTimeSet_30s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("3/7");
     api_lcd_pwr_on_hint("15��            ");
    break;
  case BacklightTimeSet_40s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("4/7");
     api_lcd_pwr_on_hint("20��            ");
    break;
  case BacklightTimeSet_50s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("5/7");
     api_lcd_pwr_on_hint("25��            ");
    break;
  case BacklightTimeSet_60s:
    api_lcd_pwr_on_hint3("�����          ");
    api_lcd_pwr_on_hint5("6/7");
     api_lcd_pwr_on_hint("30��            ");
    break;
  case KeylockTimeSet_0s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("7/7");
     api_lcd_pwr_on_hint("�ر�            ");
    break;
  case KeylockTimeSet_30s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("1/7");
     api_lcd_pwr_on_hint("5��            ");
    break;
  case KeylockTimeSet_60s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("2/7");
     api_lcd_pwr_on_hint("10��            ");
    break;
  case KeylockTimeSet_90s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("3/7");
     api_lcd_pwr_on_hint("15��            ");
    break;
  case KeylockTimeSet_120s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("4/7");
     api_lcd_pwr_on_hint("20��           ");
    break;
  case KeylockTimeSet_150s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("5/7");
     api_lcd_pwr_on_hint("25��           ");
    break;
  case KeylockTimeSet_180s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("6/7");
     api_lcd_pwr_on_hint("30��           ");
    break;
  case network_mode_2g:
    api_lcd_pwr_on_hint3("����ģʽ        ");
    api_lcd_pwr_on_hint5("2/2");
     api_lcd_pwr_on_hint("2G����          ");

    break;
  case network_mode_3g:
    api_lcd_pwr_on_hint3("����ģʽ        ");
    api_lcd_pwr_on_hint5("1/2");
     api_lcd_pwr_on_hint("3G����          ");

    break;
  default:
    break;
  }
      
}
