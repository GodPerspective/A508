#include "AllHead.h"
u8 BootProcess_SIMST_Flag=0;
u8 BootProcess_PPPCFG_Flag=0;
u8 BootProcess_PPPCFG_Flag_Zanshi=1;//��ʱ����PPPCFG���
u8 BootProcess_OpenPoc_Flag=0;
u8 VoiceEnd_Flag=0;
u8 CSQ_Flag=0;
u8 KeyDownUpChoose_GroupOrUser_Flag=0;


const u8 *ucRxCheckCard = "GETICCID:";
const u8 *ucRxZTTS0 = "ZTTS:0";
const u8 *ucRxCSQ31 = "CSQ:31";
const u8 *ucRxCSQ99 = "CSQ:99";
const u8 *ucSIMST1="^SIMST:1";
const u8 *ucSIMST255="^SIMST:255";
const u8 *ucCaretPPPCFG="^PPPCFG:";
const u8 *ucPlayZtts = "AT+ZTTS=";
//New
u8 *ucCheckTcp = "at^pocsockstat=";//���TCP Ip�Ƿ���������
u8 *ucSetIp = "at^pocsetuptcp=1,";//����TCP Ip����
u8 *ucSendTcp = "at^pocsendtcp=1,0x";//��TCPЭ�鷢������
u8 *ucRxCheckTcp = "^POCSOCKSTAT: 1";//TCP���������·�ָ��

u8 *ucZpppOpen = "at^pocnetopen";//����PPP����
u8 *ucCheckPPP = "AT^POCNETOPEN?";//���PPP�����Ƿ���������
u8 *ucRxCheckPppOpen = "^POCNETOPEN:1";
u8 *ucRxCheckPppClose = "^POCNETOPEN:0";

u8 *ucCheckRssi = "AT+CSQ?";
u8 *ucCheckCard = "AT^GETICCID";


#define DrvMC8332_IccId_Len 30
typedef struct{
	struct{
		union{
			struct{
				u16 bFunOff	: 1;
				u16 bFun	: 1;
				u16 bEsn	: 1;
				u16 bCard	: 1;
				u16 bRssi	: 1;
				u16 bPppStep	: 2;
				u16 bPppOk	: 1;
				u16 bTcp	: 1;
				u16 bTcpOk	: 1;
				u16 bUdp	: 1;
				u16 bUdpOk	: 1;
				u16 bFirstPlay  : 1;
				u16		: 3;
			}Bits;
			u16 Byte;
		}Msg;
		u8 ucCardPassword[50];
		struct{
			u8 Buf[DrvMC8332_IccId_Len];
			u8 Len;
		}IccId;
		u8 RssiValue;
		u8 Buf[30];
		u8 Len;
		u8 Timer;
		u8 Times;
		u8 ResetTimer;
		u8 OpenPppTimer;
	}NetState;
}AtCmdDrv;
static AtCmdDrv AtCmdDrvobj;
static void AtCmd_NetParamCode(void);//��ȡTCP IP��ַ



bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM3_GD83Reset://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM0_OSSYSHWID://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM1_PPPCFG://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM2_ZTTS_Abell://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM4_GD83Mode://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM5_CODECCTL://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM6_CSQ://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM7_VGR://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM8_CheckTcp	://2
    DrvGD83_UART_TxCommand((u8*)ucCheckTcp, strlen((char const*)ucCheckTcp));
    DrvGD83_UART_TxCommand(buf, len);
    break; 
  case ATCOMM9_SetIp	://1
    DrvGD83_UART_TxCommand(ucSetIp, strlen((char const *)ucSetIp));
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM10_SendTcp	://1
    if(len <= 1024 && len != 0x00)
    {
      DrvGD83_UART_TxCommand(ucSendTcp, strlen((char const *)ucSendTcp));
      DrvGD83_UART_TxCommand(buf, len);
    }
    break; 
  case ATCOMM11_ZpppOpen ://1
    DrvGD83_UART_TxCommand(ucZpppOpen, strlen((char const *)ucZpppOpen));
    break;
  case ATCOMM12_CheckPPP ://2
    DrvGD83_UART_TxCommand(ucCheckPPP, strlen((char const *)ucCheckPPP));
    break;
  case ATCOMM13_CheckRssi:
    DrvGD83_UART_TxCommand(ucCheckRssi, strlen((char const *)ucCheckRssi));
    break;
  case ATCOMM14_CheckCard:
    DrvGD83_UART_TxCommand(ucCheckCard, strlen((char const *)ucCheckCard));
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}

void ApiAtCmd_100msRenew(void)
{
/*  if(AtCmdDrvobj.NetState.Msg.Bits.bRssi == OFF)//���û�������ź�
  {
    if(AtCmdDrvobj.NetState.Msg.Bits.bCard == ON)//�Ƿ��п�
    {
    }
    else
    {
    }
  }
  else//����������ź�*/
  {
    if(GetTaskId()==Task_NormalOperation)//��¼�ɹ�����Ⱥ��״̬
    {
    if(AtCmdDrvobj.NetState.Msg.Bits.bPppOk == OFF)//���PPPδ������
    {
      if(ApiAtCmd_WritCommand(ATCOMM11_ZpppOpen, (void*)0, 0) == TRUE)
      {
      if(ApiAtCmd_WritCommand(ATCOMM12_CheckPPP, (void*)0, 0) == TRUE)
      {}
      }
    }
    else//PPP��������
    {

        //if()//������궨λ��
        {
          if(AtCmdDrvobj.NetState.Msg.Bits.bTcpOk == OFF)//��ѯ�󷵻ص�TCP״̬
          {
            if(AtCmdDrvobj.NetState.Msg.Bits.bTcp == OFF)//���TCP�ǹرյ�
            {
              AtCmd_NetParamCode();//��ȡIP��ַ
              if(ApiAtCmd_WritCommand(ATCOMM9_SetIp, (void*)AtCmdDrvobj.NetState.Buf, AtCmdDrvobj.NetState.Len) == TRUE)//����IP
              {
                AtCmdDrvobj.NetState.Msg.Bits.bTcp = ON;
              }
            }
            else//���TCP�Ǵ򿪵ģ�����TCP�����Ƿ�����
            {
              AtCmdDrvobj.NetState.Buf[0] = 0x31;
              if(ApiAtCmd_WritCommand(ATCOMM8_CheckTcp, (void*)AtCmdDrvobj.NetState.Buf, 1) == TRUE)
              {}
            }
          }
         
        }
      }
    }
  }
}
void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len;//, i
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
    ucRet = memcmp(pBuf, ucSIMST1, 8);//^SIMST:1
    if(ucRet == 0x00)
    {
      BootProcess_SIMST_Flag=1;
    }
    ucRet = memcmp(pBuf, ucSIMST255, 10);//^SIMST:255
    if(ucRet == 0x00)
    {
      BootProcess_SIMST_Flag=2;
    }
    ucRet = memcmp(pBuf, ucCaretPPPCFG, 8);//GETICCID
    if(ucRet == 0x00)
    {
      BootProcess_PPPCFG_Flag=1;
    }
    if(AtCmdDrvobj.NetState.Msg.Bits.bPppOk == OFF)//���PPP����״̬Ϊδ����
    {
      ucRet = memcmp(pBuf, ucRxCheckPppOpen, 13);
      if(ucRet == 0x00)
      {
        AtCmdDrvobj.NetState.Msg.Bits.bPppOk = ON;
      }
      else
      {
        ucRet = memcmp(pBuf, ucRxCheckPppClose, 13);//strlen(ucRxTcpDataHeadInfo));
        if(ucRet == 0x00)
        {
          AtCmdDrvobj.NetState.Msg.Bits.bPppOk = OFF;
        }
      }
    }
    else//���PPP����״̬Ϊ����
    {
      ucRet = memcmp(pBuf, ucRxCheckPppClose, 13);//strlen(ucRxTcpDataHeadInfo));
      if(ucRet == 0x00)
      {
        AtCmdDrvobj.NetState.Msg.Bits.bPppOk = OFF;//���Ӻ�Ͽ�
      }
      else//���PPP����״̬����
      {
        ucRet = memcmp(pBuf, ucRxCheckTcp, 13);//strlen(ucRxTcpDataHeadInfo));
        if(ucRet == 0x00)
        {
          ucRet = memcmp(pBuf+10, ucRxCheckTcp+10, 2);//strlen(ucRxTcpDataHeadInfo));
          if(ucRet == 0x00)
          {
            AtCmdDrvobj.NetState.Msg.Bits.bTcpOk = ON;
          }
          else
          {
            AtCmdDrvobj.NetState.Msg.Bits.bTcpOk = OFF;
          }
        }
      }
    }
    
  }
}

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
    ucRet = memcmp(pBuf, ucRxCheckCard, 9);//GETICCID
    if(ucRet == 0x00)
    {
      if(Len > 0x09)//ȥ�^
      {
        Len -= 0x09;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.IccId.Buf[i] = pBuf[i + 9];
       }
       AtCmdDrvobj.NetState.IccId.Len = i;
    }
    ucRet = memcmp(pBuf, ucRxZTTS0, 6);//GETICCID
    if(ucRet == 0x00)
    {
      VoiceEnd_Flag=1;
    }
    else
    {
      VoiceEnd_Flag=0;
    }
    ucRet = memcmp(pBuf, ucRxCSQ31, 6);//CSQ:31
    if(ucRet == 0x00)
    {
      CSQ_Flag=1;
    }
    ucRet = memcmp(pBuf, ucRxCSQ99, 6);//CSQ:31
    if(ucRet == 0x00)
    {
      CSQ_Flag=2;
    }
  }
}

bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8*)ucPlayZtts, strlen((char const *)ucPlayZtts));
  AtCmdDrvobj.NetState.Buf[0] = 0x31;	// 1
  AtCmdDrvobj.NetState.Buf[1] = 0x2C;	// ,
  AtCmdDrvobj.NetState.Buf[2] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, 3);
  switch(id)
  {
  case ATVOICE_FreePlay :
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  AtCmdDrvobj.NetState.Buf[0] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, 1);
  r  = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}


u8 ApiAtCmd_GetIccId(u8 **pBuf)
{
	*pBuf = AtCmdDrvobj.NetState.IccId.Buf;
	return AtCmdDrvobj.NetState.IccId.Len;
}
void ApiGetIccidBuf(void)
{
  DrvGD83_UART_TxCommand((u8 *)"AT+Printf=",strlen((char const *)"AT+Printf="));
  DrvGD83_UART_TxCommand((u8 *)AtCmdDrvobj.NetState.IccId.Buf,strlen((char const *)AtCmdDrvobj.NetState.IccId.Buf));
  DrvGD83_UART_TxCommand((u8 *)"\r\n",strlen((char const *)"\r\n"));                      
}

static void AtCmd_NetParamCode(void)//��ȡTCP IP��ַ
{
  AtCmdDrvobj.NetState.Buf[0]  = '1';
  AtCmdDrvobj.NetState.Buf[1]  = '2';
  AtCmdDrvobj.NetState.Buf[2]  = '3';
  AtCmdDrvobj.NetState.Buf[3]  = '.';
  AtCmdDrvobj.NetState.Buf[4]  = '5';
  AtCmdDrvobj.NetState.Buf[5]  = '6';
  AtCmdDrvobj.NetState.Buf[6]  = '.';
  AtCmdDrvobj.NetState.Buf[7]  = '8';
  AtCmdDrvobj.NetState.Buf[8]  = '0';
  AtCmdDrvobj.NetState.Buf[9]  = '.';
  AtCmdDrvobj.NetState.Buf[10] = '1';
  AtCmdDrvobj.NetState.Buf[11] = '0';
  AtCmdDrvobj.NetState.Buf[12] = '7';
  AtCmdDrvobj.NetState.Buf[13] = ',';
  AtCmdDrvobj.NetState.Buf[14] = '6';
  AtCmdDrvobj.NetState.Buf[15] = '9';
  AtCmdDrvobj.NetState.Buf[16] = '7';
  AtCmdDrvobj.NetState.Buf[17] = '3';
  AtCmdDrvobj.NetState.Len=18;
}

u8 ApiAtCmd_tcp_state(void)
{
	return AtCmdDrvobj.NetState.Msg.Bits.bTcpOk;
}

u8 ApiAtCmd_Ppp_state(void)
{
	return AtCmdDrvobj.NetState.Msg.Bits.bPppOk;
}