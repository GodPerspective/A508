#include "AllHead.h"

#if 1
u8 ApiMenu_GpsInfo_Flag=0;
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
    api_lcd_pwr_on_hint5("5/8");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
     break;
  case Menu4:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("3/8");
     api_lcd_pwr_on_hint("���������      ");
     break;
  case Menu5:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("6/8");
     api_lcd_pwr_on_hint("����ģʽ        ");
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("7/8");
     api_lcd_pwr_on_hint("������Ϣ        ");
    break;
  case Menu7:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("4/8");
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
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
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
      api_disp_icoid_output( eICO_IDBANDWIDTHW, TRUE, TRUE);//����ͼ��
    if(PersonCallIco_Flag==0)
      api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
    else
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
    api_disp_icoid_output( BatteryLevel, TRUE, TRUE);//��ص���ͼ��
    break;
  case Menu_UnlockStep1_Ok:
    //MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
    api_lcd_pwr_on_hint("�ٰ�*��         ");
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
  case GpsInfoMenu:
     api_lcd_pwr_on_hint("                ");//����
    api_lcd_pwr_on_hint3("                ");//����
    if(PositionInfoSendToATPORT_InfoDisplay_Flag==FALSE)//δ��λ��ʾ000
    {
      api_lcd_pwr_on_hint3("����:00.000000  ");//����
       api_lcd_pwr_on_hint("γ��:000.000000 ");//����
    }
    else
    {
    //Longitude����Data_Latitude_Minute()*1000000+Data_Latitude_Second();
    //Latitudewγ��Data_Longitude_Minute()*1000000+Data_Longitude_Second();
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
    api_lcd_pwr_on_hint(Buf2);
    }
    break;
  }
}
