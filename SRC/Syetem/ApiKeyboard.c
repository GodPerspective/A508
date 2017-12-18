#include "AllHead.h"
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
u8 *ucQuitPersonalCalling  = "0A0000ffffffff";
s8 GroupCallingNum=1;
s8 PersonalCallingNum=0;
s8 KeyUpDownCount=0;//������¼�����
s8 KeyPersonalCallingCount=0;//�������¼�����
u32 get_key_value(u8 scan_value);
u8 Key_Flag_1=0;
void Keyboard_Test(void)
{
  u8 r=0;

  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;
  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
  switch(ulAllKeyID)
  {
  case 0x00000002://1
    //api_lcd_pwr_on_hint("ŷ�갴��:1     ");
    break; 
  case 0x00000008://3
    //api_lcd_pwr_on_hint("ŷ�갴��:3     ");
    break;
  case 0x00000080://4
    //api_lcd_pwr_on_hint("ŷ�갴��:4     ");
    break;
  case 0x00000200://6
    //api_lcd_pwr_on_hint("ŷ�갴��:6     ");
    break;
  case 0x00002000://7
    //api_lcd_pwr_on_hint("ŷ�갴��:7     ");
    break;
  case 0x00008000://9
    //api_lcd_pwr_on_hint("ŷ�갴��:9     ");
    break;
  case 0x00010000://dn
    if(Key_PersonalCalling_Flag==1)//������¸�����
    {
      KeyPersonalCallingCount--;
      PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
      if(PersonalCallingNum<0)
      {
        PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
        KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
      }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//�������ϼ�֮���Ӧ���û���
      api_lcd_pwr_on_hint("����:   ѡ�����");
      api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());
      KeyDownUpChoose_GroupOrUser_Flag=2;
    }
    else
    {
      KeyUpDownCount--;
      GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
      if(GroupCallingNum<=0)
      {
        GroupCallingNum=ApiAtCmd_GetGroupNum();
        KeyUpDownCount=ApiAtCmd_GetGroupNum()-ApiAtCmd_GetMainGroupId();//
      }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
      api_lcd_pwr_on_hint("Ⱥ��:   ѡ��Ⱥ��");//��ʾ����
      api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//��ʾ����
      KeyDownUpChoose_GroupOrUser_Flag=1;
    }
    Key_Flag_1=1;
    //api_lcd_pwr_on_hint("ŷ�갴��:Down  ");
    break;  
  case 0x00000010://ok
    //api_lcd_pwr_on_hint("ŷ�갴��:OK     ");
    break;
  case 0x00800000://menu   
    //api_lcd_pwr_on_hint("ŷ�갴��:Menu   ");
    break;   
  case 0x00000004://2
    //api_lcd_pwr_on_hint("ŷ�갴��:2      ");
    break;  
  case 0x00080000://*
    //api_lcd_pwr_on_hint("ŷ�갴��:*      ");
    break;  
  case 0x00000100://5
    //api_lcd_pwr_on_hint("ŷ�갴��:5      ");
    break;  
  case 0x00100000://0
    //api_lcd_pwr_on_hint("ŷ�갴��:0      ");
    break;  
  case 0x00004000://8
    //api_lcd_pwr_on_hint("ŷ�갴��:8      ");
    break;  
  case 0x00200000://#
    //api_lcd_pwr_on_hint("ŷ�갴��:#      ");
    break;  
  case 0x00000400://up
    if(Key_PersonalCalling_Flag==1)//������¸�����
    {
    KeyPersonalCallingCount++;
    PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
    if(PersonalCallingNum>ApiAtCmd_GetUserNum()-1)
    {
      KeyPersonalCallingCount=0;
      PersonalCallingNum=0;
    }
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//�������ϼ�֮���Ӧ���û���
    api_lcd_pwr_on_hint("����:   ����ѡ��");
    api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());
    KeyDownUpChoose_GroupOrUser_Flag=2;
    }
    else
    {
      KeyUpDownCount++;
    GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
    if(GroupCallingNum>ApiAtCmd_GetGroupNum())
    {
      GroupCallingNum=1;
      KeyUpDownCount=1-ApiAtCmd_GetMainGroupId();
    }
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
    api_lcd_pwr_on_hint("Ⱥ��:   Ⱥ��ѡ��");//��ʾ����
    api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//��ʾ����
    KeyDownUpChoose_GroupOrUser_Flag=1;
    }
    Key_Flag_1=1;
   
    break;
  case 0x00400000://cancel
    api_lcd_pwr_on_hint("    �˳�����    ");
    Delay_100ms(5);
    r=ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
    api_lcd_pwr_on_hint("Ⱥ��:   ���ģʽ");
    Key_Flag_1=1;//������ʱ��־λ
    Key_PersonalCalling_Flag=0;//���������־λ
    break;  
  default:
    break;
  }
    if(Key_Flag_1==1)
      {
        DEL_SetTimer(1,100);
        while(1)
        {
          Key_Flag_1=0;
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
      }
}



u32 get_key_value(u8 scan_value)
{
  u32 value = 0;
  switch (scan_value)
  {
  case 0x11://"7"
    value = 0x00002000;
    break;
  case 0x12://"4"
    value = 0x00000080;
    break;
  case 0x13://"1"
    value = 0x00000002;
    break;
  case 0x14://OK
    value = 0x00000010;
    break;
  case 0x21://"8"
    value = 0x00004000;
    break;
  case 0x22://"5"
    value = 0x00000100;
    break;
  case 0x23://"2"
    value = 0x00000004;
    break;
  case 0x24://"DN"
    value = 0x00010000;
    break;
  case 0x31://"9"
    value = 0x00008000;
    break;
  case 0x32://"6"
    value = 0x00000200;
    break;
  case 0x33://"3"
    value = 0x00000008;
    break;
  case 0x34://"UP"
    value = 0x00000400;
    break;
  case 0x41://"#"
    value = 0x00200000;
    break;
  case 0x42://"0"
    value = 0x00100000;
    break;
  case 0x43://"*"
    value = 0x00080000;
    break;
  case 0x44://"Cancel"
    value = 0x00400000;
    break;
  case 0x54://"Menu"
    value = 0x00800000;
    break;
  default:
    break;
  }
  return value;
}