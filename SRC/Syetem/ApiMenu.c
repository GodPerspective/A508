#include "AllHead.h"

void MenuDisplay(MenuDisplayType id)
{
  switch(id)
  {
  case Menu0:
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
    api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
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
     api_lcd_pwr_on_hint("���������      ");
     break;
  case Menu4:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("4/8");
     api_lcd_pwr_on_hint("����/дƵ�л�   ");
     break;
  case Menu5:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("5/8");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("6/8");
     api_lcd_pwr_on_hint("����ģʽ        ");
     break;
  case Menu7:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("7/8");
     api_lcd_pwr_on_hint("��������        ");
    break;
  case Menu8:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("8/8");
     api_lcd_pwr_on_hint("��������        ");
    break;
  case Menu_Locking_NoOperation:
    api_lcd_pwr_on_hint3("        ");//��������ֹ��ʾ�������̺š�
     api_lcd_pwr_on_hint("                ");
     api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
     if(NetworkType_2Gor3G_Flag==3)
       api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
     else
       api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
    api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
    
    break;
  case Menu_UnlockStep1_Ok:
    //api_lcd_pwr_on_hint3("        ");//��������ֹ��ʾ�������̺š�
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS�����ź�ͼ��
    if(NetworkType_2Gor3G_Flag==3)
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
    else
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
     api_lcd_pwr_on_hint("�ٰ�*��         ");
    break;
  default:
    break;
  }
}