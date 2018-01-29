#include "ALLHead.h"
#define DrvMC8332_GroupName_Len		16			//define UART Tx buffer length value
#define DrvMC8332_UseId_Len			100			//define UART Tx buffer length value
#define APIPOC_UserList_Len			16
#define APIPOC_UserLoad_Len			8
#define APIPOC_UserName_Len			30

u8 ReadBuffer[80];//Test ��EEPROM��ȡ������ʹ��
u8 ASCII_ActiveUserID[22];//Test ��EEPROM��ȡ������ʹ��
u8 Get_Unicode_ActiveUserIDBuf[45];//
u8 Get_0X_ActiveUserIDBuf[11];//
//u8 TestReadBuffer[300];//��EEPROM��ȡ�������ݲ���
u8 UnicodeForGbk_MainWorkNameBuf[15];
u8 UnicodeForGbk_MainUserNameBuf[15];

const u8 *ucAtPocHead   = "AT+POC=";
const u8 *ucTingEnd   = "0B0000";
const u8 *ucTingStart   = "0B0001";
const u8 *ucSetIPAndID    = "010000";
u8 POC_EnterPersonalCalling_Flag=0;
u8 POC_QuitPersonalCalling_Flag=0;
u8 POC_AtEnterPersonalCalling_Flag=0;
u8 POC_AtQuitPersonalCalling_Flag=0;
u8 POC_EnterGroupCalling_Flag=0;
u8 POC_QuitGroupCalling_Flag=0;
bool POC_ReceivedVoice_Flag=FALSE;
bool ApiPocCmd_Tone_Flag=FALSE;
u8 POC_ReceivedVoiceStart_Flag=0;
u8 POC_ReceivedVoiceEnd_Flag=0;
bool POC_Receive86_Flag=FALSE;
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
    u8 Buf[10];
    u8 Buf2[10];
    u8 Buf3[3];
    u8 Buf4[3];
    u8 Buf5[3];
    u8 Buf6[3];
    u8 Timer;
    u8 Times;
    u8 ResetTimes;
    struct{
      struct{
        u8 bSet	: 1;
        u8 Len	: 7;
      }Msg;
      u8 Buf[DrvMC8332_UseId_Len];
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
        }Bits;u16 Byte;
			}Msg;
			u8 Timer;
			struct{
                          
				u8 PresentGroupId;
                                u8 GroupNum;
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}MainWorkGroup;
                        struct{
                                u8 PresentUserId;
                                u8 UserNum;
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}PttUserName;
#if 1//POC:80 added by Tom in 2017.11.21
			struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}Group[50];//ԭ50���ڴ���Ը�Ϊ10---------------------------------------test----------------------------------------------------------
                        struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}UserName[20];
#endif
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkGroup;
			
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkUserName;
		}UseState;
	}WorkState;
}PocCmdDrv;

static PocCmdDrv PocCmdDrvobj;
/*void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.NetState.Msg.Byte = 0x00;
  PocCmdDrvobj.NetState.Timer = 0x00;
  PocCmdDrvobj.NetState.Times = 0x00;
  PocCmdDrvobj.NetState.ResetTimes = 0x00;
  PocCmdDrvobj.WorkState.InvFirstPlay = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bInitial = 0x01;
  PocCmdDrvobj.WorkState.InviteInfo.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.InviteInfo.InviteIndex = 0x00;
  PocCmdDrvobj.CfgInfo.Msg.Byte = 0x00;
  PocCmdDrvobj.CfgInfo.GroupInfoLen = 0x00;
  PocCmdDrvobj.CfgInfo.UseListLen = 0x00;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Start = 0;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Index = 0;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Stop = 16;
  PocCmdDrvobj.CfgInfo.UseIndex = 0;
}*/

void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_SetParam://�����˺�����
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    FILE_Read(0,80,ReadBuffer);//80λ
   // FILE_Read(28,22,ActiveUserID);
//    FILE_Read(0x230,250,TestReadBuffer);//0x260-0x2cc
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
    PocCmdDrvobj.NetState.Buf[0] = ((GroupCallingNum&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf[1] = (GroupCallingNum&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf, 2);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0A0000000000", 12);
    PocCmdDrvobj.NetState.Buf2[0] = (((PersonalCallingNum+0x64)&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf2[1] = ((PersonalCallingNum+0x64)&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf2, 2);
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_GroupListInfo://5
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_UserListInfo://6
    DrvGD83_UART_TxCommand(buf, len);
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

#if 1//�̵�bUG
/*void ApiPocCmd_83_1msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    if(ucId==0x83)
    {
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        POC_ReceivedVoice_Flag=TRUE;
        POC_ReceivedVoiceStart_Flag=2;//0:���� 1���յ����� 2���տ�ʼ����
        POC_ReceivedVoiceEnd_Flag=1;//0:���� 1���յ����� 2���ս�������
      }
      if(ucId == 0xff)
      {
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
        POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
      }
    }
  }
}*/
#endif

void ApiPocCmd_10msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0A://�жϽ���״̬
#if 1
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//�������PTT������ĳ�û�
        {
          POC_AtEnterPersonalCalling_Flag=2;
          POC_AtQuitPersonalCalling_Flag=1;
          TASK_Ptt_StartPersonCalling_Flag=FALSE;
        }
      }
#else
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//�������PTT������ĳ�û�
        {
          POC_AtEnterPersonalCalling_Flag=2;
          POC_AtQuitPersonalCalling_Flag=1;
          TASK_Ptt_StartPersonCalling_Flag=FALSE;
        }
        else//���յ�����������ָ��
        {
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=2;
        }
      }
#endif
      break;

    case 0x0E://�����û�����
      ucId = COML_AscToHex(pBuf+8, 0x04);
      PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
      break;
    case 0x0d://Ⱥ�����
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum = ucId;
      break;
    case 0x80://��ȡ�������б�
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(Len >= 24)//���Ⱥ��id���滹��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 24;
      }
      else//��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+24];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.Group[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.Group[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      break;
    case 0x81://��ȡ���ڳ�Ա�б�
      ucId = COML_AscToHex(pBuf+10, 0x02);//
      if(Len >= 20)//����û�ID���滹���û���
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = Len - 20;
      }
      else//���û���
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i] = pBuf[i+20];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.UserName[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.UserName[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen;
      break;
    case 0x82://�ж��Ƿ��¼�ɹ�
      ucId = COML_AscToHex(pBuf+3, 0x01);
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x01;
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
        POC_ReceivedVoice_Flag=TRUE;
        POC_ReceivedVoiceStart_Flag=2;//0:���� 1���յ����� 2���տ�ʼ����
        POC_ReceivedVoiceEnd_Flag=1;//0:���� 1���յ����� 2���ս�������
      }
      if(ucId == 0xff)
      {
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
        POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
      }
      break;
/**************************************/
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x03)
      {
        ApiPocCmd_Tone_Flag=TRUE;
      }
      break;
    case 0x86:
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
        if(ucId==0x00&&POC_QuitPersonalCalling_Flag==1)//�յ����˳��������˳�����������״̬��
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
      }
/****************Ⱥ��״̬�жϼ�Ⱥ����Ϣ��ȡ**************************************************************/
#if 1//��Ⱥ�����뵥�����ֿ�
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0Ĭ�� 1��Ⱥ���� 2���ڽ���
        POC_QuitGroupCalling_Flag=2;//0Ĭ�� 1��Ⱥ���� 2�����˳�
#if 1//��������������������ʾȺ����BUG
        POC_EnterPersonalCalling_Flag=0;
        POC_QuitPersonalCalling_Flag=2;
#endif
      }
      else//���ڽ���Ⱥ��򵥺�
      {
        if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode == 0x01)//����ǽ��뵥��ģʽ��86���뵥����
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;

            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = APIPOC_UserName_Len;
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
        }
        else//���������
        {
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
              if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
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

#else
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0Ĭ�� 1��Ⱥ���� 2���ڽ���
        POC_QuitGroupCalling_Flag=2;//0Ĭ�� 1��Ⱥ���� 2�����˳�
      }
      else//���ڽ���Ⱥ��
      {
        POC_EnterGroupCalling_Flag=2;
        POC_QuitGroupCalling_Flag=1;
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;

      ucId = 0x00;
      for(i = 0x00; i < 0x08; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
      if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=fΪ�뿪Ⱥ��
        ucId++;
      }
      if(ucId==0x00)//���Ϊָ������뿪Ⱥ��
      {
        
      }
      else//r���Ϊ��Ⱥ����
      {
        if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
        if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
        }
      }
      else//��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      }
      }
#endif
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
        AUDIO_IOAFPOW(ON);//�򿪹��ţ����'���ģʽ'��������

      }
      break;
    default:
      break;
    }
  }
}

u8 *ApiAtCmd_GetGroupName(u8 n)//��ȡ����Ⱥ����
{
  return PocCmdDrvobj.WorkState.UseState.Group[n].Name;
}
u8 ApiAtCmd_GetGroupNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.Group[n].NameLen;
}

u8 *ApiAtCmd_GetUserName(u8 n)//��ȡ���������û�����������
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].Name;
}
u8 ApiAtCmd_GetUserNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].NameLen;
}

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

//��ʾ����ʾ��ǰȺ����
u8 *UnicodeForGbk_MainWorkName(void)
{
  u8 *Buf1;
  u8 Buf2[30];

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
      UnicodeForGbk_MainWorkNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_MainWorkNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';

      return UnicodeForGbk_MainWorkNameBuf;
    }
      
  }
}


//��ʾ����ʾ��ǰ�û��ǳ�
u8 *UnicodeForGbk_MainUserName(void)
{
  u8 *Buf1;
  u8 Buf2[30];

  u8 Len=0,i=0;
  Buf1=ApiAtCmd_GetMainUserName();
  Len=strlen((char const *)ApiAtCmd_GetMainUserName());
  while(1)
  {
    if(4*i<=Len)
    {
      Buf2[4*i+0]=Buf1[4*i+2];
      Buf2[4*i+1]=Buf1[4*i+3];
      Buf2[4*i+2]=Buf1[4*i+0];
      Buf2[4*i+3]=Buf1[4*i+1];
      UnicodeForGbk_MainUserNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_MainUserNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';

      return UnicodeForGbk_MainUserNameBuf;
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