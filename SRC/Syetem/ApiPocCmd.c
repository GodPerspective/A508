#include "ALLHead.h"
//u8 group_num_temp_count=0;


#define poc_login_info_len  100
#define poc_user_name_len   16*4  //16*4+2=66
#define poc_group_name_len  8*4  //16*4+2=66
#define poc_user_num        5
#define poc_group_num       50


u8 refresh_users_list_count=0;
u8 refresh_users_list_count_max=0;
u8 group_num_count=0;
u8 get_group_list_loss_flag=FALSE;
u8 GetMemberCount=0;
bool ApiPocCmd_PersonalCallingMode=FALSE;
u8 PresentGroupNum=0;
u8 ReadBuffer[80];//Test ��EEPROM��ȡ������ʹ��
u8 InvalidCallCount=0;
u8 ASCII_ActiveUserID[22];//Test ��EEPROM��ȡ������ʹ��
u8 Get_Unicode_ActiveUserIDBuf[45];//
u8 Get_0X_ActiveUserIDBuf[11];//
u8 UnicodeForGbk_AllUserNameBuf[poc_user_name_len];
u8 UnicodeForGbk_AllGrounpNameBuf[25];
u8 UnicodeForGbk_MainWorkNameBuf[15];
u8 UnicodeForGbk_MainUserNameBuf[25];
u8 UnicodeForGbk_SpeakerRightnowNameBuf[64];
const u8 *ucAtPocHead   = "AT+POC=";
const u8 *ucTingEnd   = "0B0000";
const u8 *ucTingStart   = "0B0001";
const u8 *ucSetIPAndID    = "010000";

u8 POC_GetAllGroupNameDone_Flag=FALSE; 
u8 POC_EnterPersonalCalling_Flag=0;
u8 POC_QuitPersonalCalling_Flag=0;
u8 POC_AtEnterPersonalCalling_Flag=0;
u8 POC_AtQuitPersonalCalling_Flag=0;
u8 POC_EnterGroupCalling_Flag=0;
u8 POC_QuitGroupCalling_Flag=0;
bool POC_ReceivedVoice_Flag=FALSE;
bool ApiPocCmd_Tone_Flag=FALSE;
bool ApiPocCmd_PlayReceivedVoice_Flag=FALSE;//��������ʹ��
bool SwitchGroupBUG=FALSE;
//bool POC_ReceivedNoVoice_Flag=FALSE;
bool EnterPersonalCalling=FALSE;
u8 POC_ReceivedVoiceStart_Flag=0;
u8 POC_ReceivedVoiceEnd_Flag=0;
u8 POC_ReceivedVoiceEndForXTSF_Flag=0;
bool POC_Receive86_Flag=FALSE;
u8 OffLineCount=0;
u8 OnlineMembership=0;
u8 KeyPttState=0;
bool PocNoOnlineMember_Flag=FALSE;
bool PocNoOnlineMember_Flag2=FALSE;
bool GettheOnlineMembersDone=FALSE;


typedef struct{
  struct{
    union{
      struct{
        u16 bUserInfo	: 3;
        u16 bUserWrite	: 1;
        u16 bPocReset	: 1;
        u16 bPocOpen	: 1;
        u16 bModeChange	: 1;
        u16	 bMode	: 3;
        u16 bNetStat	: 2;
        u16 bUnline	: 1;
        u16             : 1;
        u16             : 2;
      }Bits;
      u16 Byte;
    }Msg;
    //u8 Buf[10];
    u8 Buf2[10];
    u8 Buf3[3];
    u8 Buf4[3];
    u8 Buf5[3];
    u8 Buf6[3];
    u8 Buf7[3];
    u8 Buf8[9];
    struct{
      struct{
        u8 bSet	: 1;
        u8 Len	: 7;
      }Msg;
      u8 Buf[poc_login_info_len];
    }LoginInfo;
  }NetState;
  struct{
    struct{
      union{
        struct{
          u16 bInitial		        : 1;
          u16 bPttState		        : 1;			//0: ptt involide; 1 :ptt volide
          u16 bPttUser		        : 1;			//ptt operrtor 0: oneself; 1: other
          u16 bWorkGrpVolide	        : 3;
          u16 bEnterGroup		: 1;
          u16 bCallFail	                : 1;
          u16 bLogin                    : 1;
          u16 AlarmMode            	: 1;
          u16 PersonalCallingMode 	: 1;
          u16 			        : 5;
        }Bits;
        u16 Byte;
      }Msg;
      
      struct{
        u8 PresentGroupId;
        u8 GroupNum;
        u8 Id[8];
        u8 Name[poc_group_name_len+1];
        u8 NameLen;
      }MainWorkGroup;//��ǰ���ڵ�Ⱥ��
      
      struct{
        u8 Id[8];
        u8 Name[poc_group_name_len+1];
        u8 NameLen;
      }WorkGroup;//�м�������޾�������
      
      struct{
        //u8 Id[8];
        u8 Name[poc_group_name_len+1];
        u8 NameLen;
      }Group[poc_group_num];//Ⱥ���б�
      
      struct{
        bool UnicodeForGbk_SpeakerRightnowName_english;
        u8 PresentUserId;
        u8 Name[poc_user_name_len+1];
        u8 NameLen;
      }SpeakerRightnow;//Ⱥ��ģʽ�£���ǰ˵�����û�
      
      struct{
        bool UnicodeForGbk_english;
        u8 PresentUserId;
        u8 UserNum;
        //u8 Id[8];
        u8 Name[poc_user_name_len+1];
        u8 NameLen;
      }PttUserName;//����ģʽ�£���ǰ���е��û���
      
      struct{
        //u8 Id[8];
        u8 Name[poc_user_name_len+1];//11*4+1=45
        u8 NameLen;
      }WorkUserName;//�м����
      struct{
        bool UnicodeForGbk_AllUserName_english;
        //u8 Id[8];
        u8 Name[poc_user_name_len+1];//11*4+1=45
        u8 NameLen;
      }UserName[poc_user_num];//�û��б�
      
    }UseState;
  }WorkState;
}PocCmdDrv;

static PocCmdDrv PocCmdDrvobj;
void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.NetState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bInitial = 0x01;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin=0;
}

void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  u8 i;
  u8 NetStateBuf[5]={0,0,0,0,0};
  u8 testData=0;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_SetParam://�����˺�����
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    DrvGD83_UART_TxCommand(ReadBuffer, strlen((char const *)ReadBuffer));
    break;
  case PocComm_GetParam:
  case PocComm_Login:
    break;
  case PocComm_Logout:
  case PocComm_Cancel:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    DrvGD83_UART_TxCommand("090000000000", 12);
    COML_HexToAsc(GroupCallingNum, NetStateBuf);
    if(strlen((char const*)NetStateBuf)==1)
    {
      NetStateBuf[1]=NetStateBuf[0];
      NetStateBuf[0]=0x30;
    }
    else
    {
      testData          =NetStateBuf[0];
      NetStateBuf[0]    =NetStateBuf[1];
      NetStateBuf[1]    =testData;
    }
    DrvGD83_UART_TxCommand(NetStateBuf, 2);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0A0000000000", 12);
    COML_HexToAsc(PersonalCallingNum+0x64,PocCmdDrvobj.NetState.Buf2);
    COML_StringReverse(2,PocCmdDrvobj.NetState.Buf2);
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf2, 2);
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_GroupListInfo://5
    DrvGD83_UART_TxCommand("0D0000", 6);
    break;
  case PocComm_UserListInfo://6
    for(i=0;i<=20;i++)
    {
      //memset(PocCmdDrvobj.WorkState.UseState.UserName[i].Name,0,
             //sizeof(PocCmdDrvobj.WorkState.UseState.UserName[i].Name));//ÿ���յ�0e������Ⱥ�����
      PocCmdDrvobj.WorkState.UseState.UserName[i].NameLen=0;
    }
    GetMemberCount=0;
    GettheOnlineMembersDone=FALSE;
    //get_online_user_list_num_flag=FALSE;
    DrvGD83_UART_TxCommand("0E0000000000", 12);
    COML_HexToAsc(PresentGroupNum, NetStateBuf);
    if(strlen((char const*)NetStateBuf)==1)
    {
      NetStateBuf[1]=NetStateBuf[0];
      NetStateBuf[0]=0x30;
    }
    else
    {
      testData          =NetStateBuf[0];
      NetStateBuf[0]    =NetStateBuf[1];
      NetStateBuf[1]    =testData;
    }
    DrvGD83_UART_TxCommand(NetStateBuf, 2);
    break;
  case PocComm_Key://7
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}

//��EEPROM�ж�ȡ���ݴ���дƵ���
u8 ApiPocCmd_user_info_get(u8 ** pBuf)
{
  u8 Len=0;
  Len = Combine2Hex(ReadBuffer, strlen((char const *)ReadBuffer), ReadBuffer);
  *pBuf = ReadBuffer;
  return (strlen((char const *)ReadBuffer))/2;
}

//дƵд�����ݴ���EEPROM
bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len)//cTxBufΪ���ip�˺��������Ϣ
{
	bool r;
	u8 i, uRet = 0;
	//ADRLEN_INF	adr;

	for(i = 0; i < len; i++)
	{
		if(pBuf[i] == ';')
		{
			uRet++;
		}
	}
	if(uRet >= 2)
	{
		Dec2Hex(pBuf, len, PocCmdDrvobj.NetState.LoginInfo.Buf);//���յ�����ת��Ϊ�ַ���//LoginInfo.BufΪ���
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len << 0x01;//ΪʲôҪlen*2��
		PocCmdDrvobj.NetState.LoginInfo.Msg.bSet = ON;
		//adr = CFG_GetCurAdr(ADR_IDLocalUserInfo);
		//FILE_Write(adr.Adr,adr.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                FILE_Write(0,PocCmdDrvobj.NetState.LoginInfo.Msg.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                
		for(i = 0; i < len; i++)
		{
			PocCmdDrvobj.NetState.LoginInfo.Buf[i] = pBuf[i];
		}
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len;
		
		//SYS_BufReset();
		PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0;
		PocCmdDrvobj.NetState.Msg.Bits.bUserInfo = 3;
		PocCmdDrvobj.NetState.Msg.Bits.bUserWrite = ON;
		r = TRUE;
	}
	else
	{
		r = FALSE;
	}
	return r;
}

void ApiPocCmd_10msRenew(void)
{
  u8 ucId,user_list_ucId,i, Len;
  u8 * pBuf, ucRet;
  u8 NumCount;
  u8 i_count1=0;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0A://�жϽ���״̬
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//�������PTT������ĳ�û�
        {
          EnterPersonalCalling=TRUE;
        }
      }
      break;
    case 0x0B://�жϰ���PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=1;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;
    case 0x0C://�ж��ɿ�PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=3;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;  
    case 0x0e://�����û�����
      //0E000000000001000066745186963e6dfa51406236003
      //0E000000002a000000671152a
      ucId = COML_AscToHex(pBuf+8, 0x04);//
      PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;//
      refresh_users_list_count_max=(u8)(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum/poc_get_once_group_and_user_num);
      if(Len==12)
      {
        if(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum==0)
        {
          ApiPocCmd_PersonalCallingMode=FALSE;
          PocNoOnlineMember_Flag=TRUE;
        }
        else
        { 
          PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
          PocNoOnlineMember_Flag=FALSE;
        }
      }
      break;
    case 0x0d://Ⱥ�����
      group_num_count=0;
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum = ucId;
      if(PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum>=poc_group_num)
      {
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum=poc_group_num;
      }
      break;
    case 0x80://��ȡ�������б�
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId<=PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum)//Ⱥ�����������Ⱥ�����򲻴�
      {
        if(Len >= 24)//���Ⱥ��id���滹��Ⱥ����
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 24;
          if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen>=poc_group_name_len)
          {
            PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen=poc_group_name_len;
          }
        }
        else//��Ⱥ����
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
        }
        for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+24];//�����ȡ��Ⱥ����
          PocCmdDrvobj.WorkState.UseState.Group[ucId-1].Name[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
        }
        PocCmdDrvobj.WorkState.UseState.Group[ucId-1].NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
        group_num_count++;
        if(group_num_count<PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum)
        {
          get_group_list_loss_flag=TRUE;
        }
        else
        {
          get_group_list_loss_flag=FALSE;
        }
      }
      break;
    case 0x81://��ȡ���ڳ�Ա�б�
      ucId = COML_AscToHex(pBuf+10, 0x02);//
      refresh_users_list_count=(u8)(PersonalCallingNum/poc_get_once_group_and_user_num);
      user_list_ucId=ucId-refresh_users_list_count*poc_get_once_group_and_user_num;
      if(user_list_ucId<poc_get_once_group_and_user_num)
      {
        if(Len >= 20)//����û�ID���滹���û���
        {
          PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = Len - 20;
          if(PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen>=poc_user_name_len)
          {
            PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = poc_user_name_len;
          }
        }
        else//���û���
        {
          PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0x00;
        }
        for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen; i++)
        {
          PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i] = pBuf[i+20];//�����ȡ��Ⱥ����
          PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].Name[i]=PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i];
        }
        PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen;

        
        
        i_count1=0;
        NumCount=0;
        while(1)
        {
            if(4*i_count1<PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].NameLen)
            {
              if(PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[4*i_count1+2]==0x30&&
                 PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[4*i_count1+3]==0x30)
              {
                NumCount++;
              }
              i_count1++;
            }
            else
            {
              if(NumCount*4>=PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].NameLen)
              {
                PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].UnicodeForGbk_AllUserName_english=TRUE;
              }
              else
              {
                PocCmdDrvobj.WorkState.UseState.UserName[user_list_ucId].UnicodeForGbk_AllUserName_english=FALSE;
              }
              break;
            }
        }
        GetMemberCount++;
        if(refresh_users_list_count==refresh_users_list_count_max)//��������һ����Ϣ
        {
          if(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum!=0
             &&GetMemberCount!=0
             &&GetMemberCount>=(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum%poc_get_once_group_and_user_num))
          {
            GetMemberCount=0;
            GettheOnlineMembersDone=TRUE;
          }
        }
        else//�����һ������������
        {
          if(GetMemberCount>=poc_get_once_group_and_user_num)
          {
            GetMemberCount=0;
            GettheOnlineMembersDone=TRUE;
          }
        }
        
        if(GettheOnlineMembersDone==TRUE&&huo_qu_zhong_flag==TRUE)//���¼���ѯ���߽��ʱ���ٷ�һ��0eָ���ʾ��������ǰѡ��Ⱥ��
        {
          huo_qu_zhong_flag=FALSE;
          
          api_lcd_pwr_on_hint4("                ");//����
          if(UnicodeForGbk_AllUserName_english_flag(PersonalCallingNum%poc_get_once_group_and_user_num)==TRUE)
          {
            api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else
          {
            api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum%poc_get_once_group_and_user_num),ApiAtCmd_GetUserNameLen(PersonalCallingNum%poc_get_once_group_and_user_num));//�������ϼ�֮���Ӧ���û���
        }
        
        if(get_online_user_list_num_flag==TRUE)
        {
          api_lcd_pwr_on_hint2(HexToChar_AllOnlineMemberNum());
          
          if(GettheOnlineMembersDone==TRUE
             ||GetMemberCount>=poc_get_once_group_and_user_num-1
             ||GetMemberCount>=PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum)//����ڲ˵����߳�Ա�б�״̬�»�ȡ��һ�г�Ա�б������ٵڶ��������
          {
            get_online_user_list_num_flag=FALSE;
                    PersonalCallingNum=0;//�����������ֱ��ѡ�У������û������ǲ������û�
                    VOICE_SetOutput(ATVOICE_FreePlay,"2857bf7e106258547065",20);//���߳�Ա��
                    Delay_100ms(15);//DEL_SetTimer(0,150);
                    VOICE_SetOutput(ATVOICE_FreePlay,VoiceAllOnlineMemberNum(),8);//
                    Delay_100ms(10);//DEL_SetTimer(0,100);
                    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(0),ApiAtCmd_GetUserNameLen(0));//�״λ�ȡ���ڳ�Ա������һ����Ա
                    api_lcd_pwr_on_hint3("���߳�Ա�б�  ");
                    api_lcd_pwr_on_hint4("                ");//����
                    if(UnicodeForGbk_AllUserName_english_flag(0)==TRUE)
                    {
                      api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(0));//��ʾ��ǰѡ�е�Ⱥ����
                    }
                    else
                    {
                      api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(0));//��ʾ��ǰѡ�е�Ⱥ����
                    }
                    TheMenuLayer_Flag=2;
                    KeyPersonalCallingCount=0;//�������ģʽ�����¼���Ա������˳�򣬵�һ����Ա���л�ʱ��ڶ�������������
          }
        }
      }
      break;
    case 0x82://�ж��Ƿ��¼�ɹ�
      ucId = COML_AscToHex(pBuf+3, 0x01);
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x01;
        OffLineCount=0;
      }
      else if(ucId == 0x00)
      {
        OffLineCount++;
        if(OffLineCount>=5)
        {
          ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
          OffLineCount=0;
        }
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      else
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      break;
/********�ж��Ƿ��Ǳ���״̬******************************/
    case 0x83:
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        if(Len >= 12)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = Len - 12;
          if(InvalidCallCount==1)
          {
            InvalidCallCount=0;
          }
          else
          {
            if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen > poc_user_name_len)
            {
              PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = poc_user_name_len;
            }
            POC_ReceivedVoice_Flag=TRUE;
            POC_ReceivedVoiceStart_Flag=2;//0:���� 1���յ����� 2���տ�ʼ����
            POC_ReceivedVoiceEnd_Flag=1;//0:���� 1���յ����� 2���ս�������
            POC_ReceivedVoiceEndForXTSF_Flag=1;
          }
        }
        for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen; i++)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[i] = pBuf[i+12];//���뵱ǰ˵���˵�����
        }
        i_count1=0;
        NumCount=0;
        while(1)
        {
          if(4*i_count1<PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen)
          {
            if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i_count1+2]==0x30&&
               PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i_count1+3]==0x30)
            {
              NumCount++;
            }
            i_count1++;
          }
          else
          {
            if(NumCount*4>=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen)
            {
              PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.UnicodeForGbk_SpeakerRightnowName_english=TRUE;
            }
            else
            {
              PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.UnicodeForGbk_SpeakerRightnowName_english=FALSE;
            }
            break;
          }
        }
        }
      if(ucId == 0xff)
      {
        if(SwitchGroupBUG==TRUE)
        {
        }
        else
        {
          POC_ReceivedVoice_Flag=FALSE;
          POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
          POC_ReceivedVoiceEndForXTSF_Flag=2;
        }
      }
      break;
/**************************************/
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x00)//����ͨ������
      {
        ApiPocCmd_PlayReceivedVoice_Flag=FALSE;
      }
      if(ucId==0x01)//����ͨ����ʼ
      {
        ApiPocCmd_PlayReceivedVoice_Flag=TRUE;
      }
      if(ucId==0x03)//tone��
      {
        ApiPocCmd_Tone_Flag=TRUE;
        AUDIO_IOAFPOW(ON);
#if 1//���յ�Tone������ZTTS��0
        ApiAtCmd_ZTTS_Flag=FALSE;
#endif
      }

      break;
    case 0x86:
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId!=0x00&&ucId!=0xff)
      {
        PresentGroupNum=ucId;
      }
      POC_GetAllGroupNameDone_Flag=TRUE;
      POC_Receive86_Flag=TRUE;
/****************�жϽ��뵥��**************************************************************/
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x0a)//���뵥��
      { 
        POC_EnterPersonalCalling_Flag=2;//�����жϽ��뵥��,0:���� 2�����ڽ��뵥�� 1������״̬
        POC_QuitPersonalCalling_Flag=1;//�����ж��˳�����
      }
      else
      {
        if(POC_QuitPersonalCalling_Flag==1)//�յ����˳��������˳�����������״̬��
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
      }
/****************Ⱥ��״̬�жϼ�Ⱥ����Ϣ��ȡ**************************************************************/
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        if(POC_EnterPersonalCalling_Flag==1)
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=0;
        }
        if(POC_AtEnterPersonalCalling_Flag==1)
        {
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=2;
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=0;
        }
        POC_EnterGroupCalling_Flag=0;//0Ĭ�� 1��Ⱥ���� 2���ڽ���
        POC_QuitGroupCalling_Flag=2;//0Ĭ�� 1��Ⱥ���� 2�����˳�
      }
      else//���ڽ���Ⱥ��򵥺�
      {
          if(EnterPersonalCalling==TRUE)
          {
            POC_AtEnterPersonalCalling_Flag=2;
            POC_AtQuitPersonalCalling_Flag=1;
            EnterPersonalCalling=FALSE;
            TASK_Ptt_StartPersonCalling_Flag=FALSE;
          }
#if 1//�������״̬�»����PTT��ʾ�������̵���
          if(POC_AtEnterPersonalCalling_Flag==0)//�������ģʽ����ʾ������ǰ����ʾһ��Ⱥ������BUG
          {
            POC_ReceivedVoice_Flag=FALSE;
            POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
            POC_ReceivedVoiceEndForXTSF_Flag=2;
            POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
          }

#endif
        if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode == 0x01)//����ǽ��뵥��ģʽ��86���뵥����
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen > poc_user_name_len)
              {
                PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = poc_user_name_len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
            }
            i_count1=0;
            NumCount=0;
            while(1)
            {
              if(4*i_count1<PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen)
              {
                if(PocCmdDrvobj.WorkState.UseState.PttUserName.Name[4*i_count1+2]==0x30&&
                   PocCmdDrvobj.WorkState.UseState.PttUserName.Name[4*i_count1+3]==0x30)
                {
                  NumCount++;
                }
                i_count1++;
              }
              else
              {
                if(NumCount*4>=PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen)
                {
                  PocCmdDrvobj.WorkState.UseState.PttUserName.UnicodeForGbk_english=TRUE;
                }
                else
                {
                  PocCmdDrvobj.WorkState.UseState.PttUserName.UnicodeForGbk_english=FALSE;
                }
                break;
              }
            }
        }
        else//���������
        {
          ApiPocCmd_PersonalCallingMode=FALSE;
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
          PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;
          ucId = 0x00;
          for(i = 0x00; i < 0x08; i++)
          {
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
            if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=fΪ�뿪Ⱥ��
              ucId++;
          }
          if(ucId==0x00)//���Ϊָ������뿪Ⱥ��
          {}
          else//r���Ϊ��Ⱥ����
          {
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > poc_group_name_len)
              {
                PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = poc_group_name_len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
              PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
            }
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
          }
        }
      }
      break;
    case 0x91://֪ͨ����ĳ��ģʽ�������˳�һ���澯������ģʽ��
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x00;//�˳�һ���澯
      }
      if(ucId == 0x01)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x01;//����һ���澯
      }
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x01;//���뵥��
      }
      if(ucId == 0x03)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x00;//�˳����� 
        //AUDIO_IOAFPOW(ON);//�򿪹��ţ����'���ģʽ'��������

      }
      break;
    default:
      break;
    }
  }
}

u8 *ApiAtCmd_GetGroupName(u8 n)//��ȡ����Ⱥ����
{
  return PocCmdDrvobj.WorkState.UseState.Group[n-1].Name;
}
u8 ApiAtCmd_GetGroupNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.Group[n-1].NameLen;
}
/*********************************/
//���黻�����¼���ʾ����ʾѡ��Ⱥ����
u8 *UnicodeForGbk_AllGrounpName(u8 n)
{
  u8 *GrounpBuf1;
  u8 Buf2[poc_user_name_len];

  u8 GrounpLen=0;
  u8 i=0;
  GrounpBuf1=PocCmdDrvobj.WorkState.UseState.Group[n-1].Name;
  GrounpLen=PocCmdDrvobj.WorkState.UseState.Group[n-1].NameLen;
  while(1)
  {
    if(4*i<=GrounpLen)
    {
      Buf2[4*i+0]=GrounpBuf1[4*i+2];
      Buf2[4*i+1]=GrounpBuf1[4*i+3];
      Buf2[4*i+2]=GrounpBuf1[4*i+0];
      Buf2[4*i+3]=GrounpBuf1[4*i+1];
      UnicodeForGbk_AllGrounpNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_AllGrounpNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[GrounpLen]='\0';
      return UnicodeForGbk_AllGrounpNameBuf;
    }
  }
}
/********************************/

u8 *ApiAtCmd_GetUserName(u8 n)//��ȡ���������û�����������
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].Name;
}
u8 ApiAtCmd_GetUserNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].NameLen;
}

/*********************************/
//���黻�����¼���ʾ����ʾѡ���û���
u8 *UnicodeForGbk_AllUserName(u8 n)
{
  u8 *UserBuf1;
  u8 Buf3[poc_user_name_len];

  u8 UserLen2=0;
  u8 i=0;
  u8 j=0;
  u8 NumCount=0;
  UserBuf1=PocCmdDrvobj.WorkState.UseState.UserName[n].Name;//read_all_user_name_from_flash(n);
  UserLen2=strlen((char const *)PocCmdDrvobj.WorkState.UseState.UserName[n].Name);
  while(1)
  {
    if(4*i<UserLen2)
    {
/****����û���Ϊ������ʱ����ʾλ��ֻ��8λ������****************/
      if(UserBuf1[4*i+2]==0x30&&UserBuf1[4*i+3]==0x30)
      {
        NumCount++;
      }
/**************************************************************/
      Buf3[4*i+0]=UserBuf1[4*i+2];
      Buf3[4*i+1]=UserBuf1[4*i+3];
      Buf3[4*i+2]=UserBuf1[4*i+0];
      Buf3[4*i+3]=UserBuf1[4*i+1];
      UnicodeForGbk_AllUserNameBuf[2*i+0]=COML_AscToHex(Buf3+(4*i), 0x02);
      UnicodeForGbk_AllUserNameBuf[2*i+1]=COML_AscToHex(Buf3+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf3[UserLen2]='\0';
      if(NumCount*4>=UserLen2)
      {
        for(j=0;4*j<UserLen2;j++)
        {
          UnicodeForGbk_AllUserNameBuf[j]=COML_AscToHex(UserBuf1+(4*j), 0x02);
        }
        return UnicodeForGbk_AllUserNameBuf;
      }
      else
      {
        return UnicodeForGbk_AllUserNameBuf;
      }
    }
      
  }
}
/********************************/


u8 *ApiAtCmd_GetMainWorkName(void)//��ȡ����Ⱥ����
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name;
}
u8 ApiAtCmd_GetMainWorkNameLen(void)
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen;
}

u8 *ApiAtCmd_GetMainUserName(void)//��ȡ��ǰ�û�����������
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.Name;
}
u8 ApiAtCmd_GetMainUserNameLen(void)
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen;
}

u8 ApiAtCmd_GetGroupNum(void)//��ȡȺ����
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum;
}
u8 ApiAtCmd_GetUserNum(void)//��ȡ���߳�Ա����������
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum;
}
u8 ApiAtCmd_GetMainGroupId(void)//��ȡ��ǰȺ��id
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId;
}
u8 ApiAtCmd_GetMainUserId(void)//��ȡ��ǰ�û�id��������
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.PresentUserId;
}

bool ApiPocCmd_GetPttState(void)//�ж�PPT״̬���Ƿ��л�Ȩ
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPttState;
}
bool ApiAtCmd_GetLoginState(void)
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin;
}
void ApiAtCmd_SetLoginState(void)
{
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin=0;
}
/*
void ApiGetPocBuf(void)
{
  DrvGD83_UART_TxCommand((u8 *)"AT+Printf=",strlen((char const *)"AT+Printf="));
  DrvGD83_UART_TxCommand((u8 *)PocCmdDrvobj.PocDateTest.Buf,strlen((char const *)PocCmdDrvobj.PocDateTest.Buf));
  DrvGD83_UART_TxCommand((u8 *)"\r\n",strlen((char const *)"\r\n"));                      
}
*/


bool GetPersonalCallingMode(void)
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode;
}

u8 *HexToChar_MainGroupId(void)//16����ת�ַ��� ��ǰȺ��ID ��ʾ������ʹ��
{
  u8 i;
  i=ApiAtCmd_GetMainGroupId();
  PocCmdDrvobj.NetState.Buf3[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf3[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf3[2]='\0';
  return PocCmdDrvobj.NetState.Buf3;
}
u8 *HexToChar_GroupCallingNum(void)//16����ת�ַ��� ����������ǰȺ��ID ��ʾ������ʹ��
{
  u8 i;
  i=GroupCallingNum;
  PocCmdDrvobj.NetState.Buf4[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf4[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf4[2]='\0';
  return PocCmdDrvobj.NetState.Buf4;
}

u8 *HexToChar_MainUserId(void)//16����ת�ַ��� ��ǰ�û�ID ��ʾ������ʹ��
{
  u8 i;
  i=ApiAtCmd_GetMainUserId()+0x01;
  PocCmdDrvobj.NetState.Buf5[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf5[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf5[2]='\0';
  return PocCmdDrvobj.NetState.Buf5;
}
u8 *HexToChar_PersonalCallingNum(void)//16����ת�ַ��� ����������ǰ�û�ID ��ʾ������ʹ��
{
  u8 i;
  i=PersonalCallingNum+0x01;
  PocCmdDrvobj.NetState.Buf6[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf6[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf6[2]='\0';
  return PocCmdDrvobj.NetState.Buf6;
}

u8 *HexToChar_AllOnlineMemberNum(void)//16����ת�ַ��� ��ʾ����ʾ���߳�Ա����
{
  u8 i;
  i=PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum;
  COML_DecToAsc(i,PocCmdDrvobj.NetState.Buf7);
  COML_StringReverse(strlen((char const *)PocCmdDrvobj.NetState.Buf7),PocCmdDrvobj.NetState.Buf7);
  return PocCmdDrvobj.NetState.Buf7;
}

u8 *VoiceAllOnlineMemberNum(void)// �����������߳�Ա����
{
  PocCmdDrvobj.NetState.Buf8[0]='3';
  PocCmdDrvobj.NetState.Buf8[1]=PocCmdDrvobj.NetState.Buf7[0];
  PocCmdDrvobj.NetState.Buf8[2]='0';
  PocCmdDrvobj.NetState.Buf8[3]='0';
  PocCmdDrvobj.NetState.Buf8[4]='3';
  PocCmdDrvobj.NetState.Buf8[5]=PocCmdDrvobj.NetState.Buf7[1];
  PocCmdDrvobj.NetState.Buf8[6]='0';
  PocCmdDrvobj.NetState.Buf8[7]='0';
  PocCmdDrvobj.NetState.Buf8[8]='\0';
  return PocCmdDrvobj.NetState.Buf8;
}

//��ʾ����ʾ��ǰȺ����
u8 *UnicodeForGbk_MainWorkName(void)
{
  u8 *Buf1;
  u8 Buf2[poc_group_name_len];

  u8 Len=0;
  u8 i=0;
  Buf1=ApiAtCmd_GetMainWorkName();
  Len=strlen((char const *)ApiAtCmd_GetMainWorkName());
  while(1)
  {
    if(4*i<=Len)
    {
      Buf2[4*i+0]=Buf1[4*i+2];
      Buf2[4*i+1]=Buf1[4*i+3];
      Buf2[4*i+2]=Buf1[4*i+0];
      Buf2[4*i+3]=Buf1[4*i+1];
      UnicodeForGbk_AllGrounpNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_AllGrounpNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';

      return UnicodeForGbk_AllGrounpNameBuf;
    }
  }
}


//��ʾ����ʾ��ǰ�û��ǳ�
u8 *UnicodeForGbk_MainUserName(void)
{
  u8 *Buf0;
  u8 Buf2[poc_user_name_len];

  u8 Len=0,i=0;
  u8 j=0;
  u8 NumCount=0;
  Buf0=PocCmdDrvobj.WorkState.UseState.PttUserName.Name;
  Len=PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen;
  while(1)
  {
    if(4*i<Len)
    {
/****����û���Ϊ������ʱ����ʾλ��ֻ��8λ������****************/
      if(Buf0[4*i+2]==0x30&&Buf0[4*i+3]==0x30)
      {
        NumCount++;
      }
/**************************************************************/
      Buf2[4*i+0]=Buf0[4*i+2];
      Buf2[4*i+1]=Buf0[4*i+3];
      Buf2[4*i+2]=Buf0[4*i+0];
      Buf2[4*i+3]=Buf0[4*i+1];
      UnicodeForGbk_MainUserNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_MainUserNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';
      if(NumCount*4>=Len)
      {
        for(j=0;4*j<Len;j++)
        {
          UnicodeForGbk_MainUserNameBuf[j]=COML_AscToHex(Buf0+(4*j), 0x02);
        }
        return UnicodeForGbk_MainUserNameBuf;
      }
      else
      {
        return UnicodeForGbk_MainUserNameBuf;
      }
    }
  }
}

//��ʾ����ʾ���ģʽ�µ�ǰ˵���˵��ǳ�
u8 *UnicodeForGbk_SpeakerRightnowName(void)
{
  u8 Buf2[poc_user_name_len+4];
  u8 i=0;
  u8 j=0;
  u8 NumCount=0;
  memset(UnicodeForGbk_SpeakerRightnowNameBuf,0,sizeof(UnicodeForGbk_SpeakerRightnowNameBuf));
  while(1)
  {
    if(4*i<PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen)
    {
/****����û���Ϊ������ʱ����ʾλ��ֻ��8λ������****************/
      if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+2]==0x30&&
         PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+3]==0x30)
      {
        NumCount++;
      }
        Buf2[4*i+0]=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+2];
        Buf2[4*i+1]=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+3];
        Buf2[4*i+2]=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+0];
        Buf2[4*i+3]=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[4*i+1];

      UnicodeForGbk_SpeakerRightnowNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_SpeakerRightnowNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen]='\0';
      if(NumCount*4>=PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen)
      {
        for(j=0;4*j<PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen;j++)
        {
          UnicodeForGbk_SpeakerRightnowNameBuf[j]=COML_AscToHex(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name+(4*j), 0x02);
        }
        return UnicodeForGbk_SpeakerRightnowNameBuf;
      }
      else
      {
        return UnicodeForGbk_SpeakerRightnowNameBuf;
      }
    }
  }
}
   
//��ǰ�û�ID������ʹ�ã�
u8 *Get_Unicode_ActiveUserID(void)
{
  u8 Len=0,i=0;
  FILE_Read(28,22,ASCII_ActiveUserID);//3139383030333037343732
  Len=strlen((char const *)ASCII_ActiveUserID);
  for(i=0;i<Len;i++)
  {
    if(i%2==0)//ż��
    {
      Get_Unicode_ActiveUserIDBuf[2*i]=ASCII_ActiveUserID[i];
    }
    else//����
    {
      Get_Unicode_ActiveUserIDBuf[2*i-1]=ASCII_ActiveUserID[i];
      Get_Unicode_ActiveUserIDBuf[2*i]=0x30;
      Get_Unicode_ActiveUserIDBuf[2*i+1]=0x30;
    }
  }
  //31003900380030003000330030003700340037003200
  return Get_Unicode_ActiveUserIDBuf;
}

//��ǰ�û�ID����ʾ��ʹ�ã�
u8 *Get_GBK_ActiveUserID(void)
{
  u8 i=0,Len=0;
  FILE_Read(28,22,ASCII_ActiveUserID);//3139383030333037343732
  Len=strlen((char const *)ASCII_ActiveUserID);
  for(i=0;i<Len;i++)
  {
    if(i%2!=0)
    {
      Get_0X_ActiveUserIDBuf[(i-1)/2]=ASCII_ActiveUserID[i];
    }
  }
  return Get_0X_ActiveUserIDBuf;
}

bool UnicodeForGbk_SpeakerRightnowName_english_flag(void)
{
  return PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.UnicodeForGbk_SpeakerRightnowName_english;
}
bool UnicodeForGbk_MainUserName_english_flag(void)
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.UnicodeForGbk_english;
}
bool UnicodeForGbk_AllUserName_english_flag(u8 a)
{
  return PocCmdDrvobj.WorkState.UseState.UserName[a].UnicodeForGbk_AllUserName_english;
}

/*u8 *read_all_user_name_from_flash(u8 ucId)//��flash�ж�ȡ�û���
{
  file_flash_read(0x6800+ucId*(poc_user_name_len+1),poc_user_name_len+1,read_all_user_name_from_flash_buf);
  return read_all_user_name_from_flash_buf;
}*/

/*void select_which_line_users_list(void)
{
  if(PersonalCallingNum%20==0)
  {
    //at+POC=0e00000000001
  }
  
  
  
}*/
