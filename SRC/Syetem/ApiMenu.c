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
    api_lcd_pwr_on_hint5("1/3");
     api_lcd_pwr_on_hint("�����ʱ��      ");
    break;
  case Menu2:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("2/3");
     api_lcd_pwr_on_hint("������          ");
    break;
  case Menu3:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("3/3");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
    break;
  case Menu_Lock:
    api_lcd_pwr_on_hint3("�˵�            ");
    api_lcd_pwr_on_hint5("3/3");
     api_lcd_pwr_on_hint("GPS��Ϣ         ");
    break;
  default:
    break;
  }
}