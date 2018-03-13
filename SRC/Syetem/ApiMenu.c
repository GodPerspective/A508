#include "AllHead.h"

#if 1
u8 ApiMenu_SwitchGroup_Flag=0;
u8 ApiMenu_SwitchCallUser_Flag=0;
u8 ApiMenu_GpsInfo_Flag=0;
u8 ApiMenu_NativeInfo_Flag=0;
u8 ApiMenu_BacklightTimeSet_Flag=0;
u8 ApiMenu_KeylockTimeSet_Flag=0;
u8 ApiMenu_BeiDouOrWritingFrequency_Flag=0;
#endif


void MenuDisplay(MenuDisplayType id)
{
  u8 Buf1[1];
  u8 Buf2[1];
  switch(id)
  {
  case Menu0:
    break;
  case Menu1:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("1/7");
     api_lcd_pwr_on_hint("Ⱥ��ѡ��        ");
    break;
  case Menu2:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("2/7");
     api_lcd_pwr_on_hint("��Աѡ��        ");
    break;
  case Menu3:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("3/7");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
     break;
     
  case Menu4:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("4/7");
     api_lcd_pwr_on_hint("���������      ");
     if(ApiMenu_BacklightTimeSet_Flag==2)
     {
       ApiMenu_BacklightTimeSet_Flag=0;
       if(BacklightTimeSetCount==7)
       {
         Buf1[0]=0;
         FILE_Write2(0x236,1,Buf1);//����ʱ�䡾�롿
       }
       else
       {
         Buf1[0]=BacklightTimeSetCount;
         FILE_Write2(0x236,1,Buf1);//����ʱ�䡾�롿
       }
     }
     break;
  case Menu5:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("5/7");
     api_lcd_pwr_on_hint("��������        ");
     if(ApiMenu_KeylockTimeSet_Flag==2)
     {
       ApiMenu_KeylockTimeSet_Flag=0;
       if(KeylockTimeSetCount==0x10)
       {
         //KeylockTimeCount=200;//���=200����Զ������
         Buf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,Buf2);//������ʱ�䡾�롿
       }
       else
       {
         //KeylockTimeCount=(KeylockTimeSetCount-0x10)*30;
         Buf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,Buf2);//������ʱ�䡾�롿
       }
     }
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("6/7");
     api_lcd_pwr_on_hint("������Ϣ        ");
    break;
  case Menu7:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("7/7");
     api_lcd_pwr_on_hint("����/дƵ�л�   ");
     break;
/*  case Menu8:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("8/8");
     api_lcd_pwr_on_hint("��������        ");
    break;*/
  case Menu_Locking_NoOperation:
    api_lcd_pwr_on_hint("                ");//����
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    break;
  case Menu_unLocking:
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
     api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
     api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    break;
  case Menu_RefreshAllIco:
    api_lcd_pwr_on_hint3("                ");//����
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
    if(VoiceType_FreehandOrHandset_Flag==0)
      api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
    else
      api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
    if(LockingState_Flag==FALSE)
      api_disp_icoid_output( eICO_IDBANDWIDTHN, TRUE, TRUE);//��������ͼ��
    else
    {
      api_disp_icoid_output( eICO_IDBANDWIDTHW, TRUE, TRUE);//����ͼ��
    }
    if(PersonCallIco_Flag==0)
      api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
    else
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
    api_disp_icoid_output( BatteryLevel, TRUE, TRUE);//��ص���ͼ��
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
    api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
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
    api_lcd_pwr_on_hint3("                ");//����
     api_lcd_pwr_on_hint("  �Ǵ˰汾����  ");
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
     api_lcd_pwr_on_hint("30��            ");
    break;
  case KeylockTimeSet_60s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("2/7");
     api_lcd_pwr_on_hint("60��            ");
    break;
  case KeylockTimeSet_90s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("3/7");
     api_lcd_pwr_on_hint("90��            ");
    break;
  case KeylockTimeSet_120s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("4/7");
     api_lcd_pwr_on_hint("120��           ");
    break;
  case KeylockTimeSet_150s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("5/7");
     api_lcd_pwr_on_hint("150��           ");
    break;
  case KeylockTimeSet_180s:
    api_lcd_pwr_on_hint3("������          ");
    api_lcd_pwr_on_hint5("6/7");
     api_lcd_pwr_on_hint("180��           ");
    break;
  default:
    break;
  }
}
