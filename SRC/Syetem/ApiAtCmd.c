#include "AllHead.h"
u8 BootProcess_SIMST_Flag=0;
u8 BootProcess_PPPCFG_Flag=0;
u8 ApiAtCmd_TrumpetVoicePlay_Flag=0;//���ſ��Ʊ�־λ
bool ApiAtCmd_ZTTS_Flag=FALSE;
u8 CSQ_Flag=0;
u8 CSQ99Count_Flag=0;
u8 KeyDownUpChoose_GroupOrUser_Flag=0;
u8 HDRCSQValue=0;//HDRCSQ��ֵ
u8 CSQValue=0;//HDRCSQ��ֵ
//u8 KaiJi_Flag=FALSE;
const u8 *ucGD83Reset  = "at^reset";
const u8 *ucRxPASTATE1 = "PASTATE:1";
const u8 *ucRxPASTATE0 = "PASTATE:0";
const u8 *ucRxZTTS0 = "ZTTS:0";
const u8 *ucCheckRssi = "AT+CSQ?";
const u8 *ucHDRCSQ = "AT^HDRCSQ";
const u8 *ucRxCSQ = "CSQ:";
const u8 *ucRxHDRCSQ = "^HDRCSQ:";
const u8 *ucGpsPosition = "LATLON:";
const u8 *ucCDMATIME = "^CDMATIME:";
const u8 *ucGPSINFO = "^GPSINFO:";
const u8 *ucGPSCNO = "^GPSCNO:";
const u8 *ucSIMST1="^SIMST:1";
const u8 *ucSIMST255="^SIMST:255";
const u8 *ucCaretPPPCFG="^PPPCFG:";
const u8 *ucPlayZtts = "AT+ZTTS=";
const u8 *ucCheckTcp = "at^pocsockstat=";//���TCP Ip�Ƿ���������
//u8 *ucSetIp = "at^pocsetuptcp=1,";//����TCP����1 UDP:0
//u8 *ucSendTcp = "at^pocsendtcp=1,0x";//��TCPЭ�鷢������ ����TCP����1 UDP:0
//u8 *ucRxCheckTcp = "^POCSOCKSTAT: 1";//TCP���������·�ָ��
const u8 *ucSetIp = "at^POCSETUPUDP=0";//����TCP����1 UDP:0
const u8 *ucSendTcp = "at^POCSENDUDP=0,";//at^pocsendudp=0,123.56.80.107,6969,0x
const u8 *ucRxCheckTcp = "^POCSOCKSTAT: 2";//TCP���������·�ָ��
const u8 *ucZpppOpen = "at^pocnetopen";//����PPP����
const u8 *ucCheckPPP = "AT^POCNETOPEN?";//���PPP�����Ƿ���������
const u8 *ucRxCheckPppOpen = "^POCNETOPEN:1";
const u8 *ucRxCheckPppClose = "^POCNETOPEN:0";
const u8 *ucCheckCard = "AT^GETICCID";

bool PositionInformationSendToATPORT_Flag=FALSE;
bool PositionInfoSendToATPORT_RedLed_Flag=FALSE;
bool PositionInfoSendToATPORT_SetPPP_Flag=FALSE;
bool PositionInfoSendToATPORT_InfoDisplay_Flag=FALSE;
//#define DrvMC8332_IccId_Len 30
typedef struct{
  struct{
    union{
      struct{
        //u16 bFunOff	: 1;
        //u16 bFun	: 1;
        //u16 bEsn	: 1;
        //u16 bCard	: 1;
        //u16 bRssi	: 1;
        //u16 bPppStep	: 2;
        u16 bPppOk	: 1;
        u16 bTcp	: 1;
        u16 bTcpOk	: 1;
        u16 bUdp	: 1;
        u16 bUdpOk	: 1;
        u16 bFirstPlay  : 1;
      u16		: 10;
      }Bits;
      u16 Byte;
    }Msg;
    struct{
      u8 Buf[21];//���AT�յ��ľ�γ����Ϣ
      u8 BufLen;//���վ�γ����Ϣ�����ݳ���
      u8 Longitude_Minute;//С����ǰ����
      u32 Longitude_Second;//С��������
      u8 Latitude_Minute;
      u32 Latitude_Second;
    }Position;
    u8 SouXingConut;
    u16 SignalToNoiseMax;//����ź�ֵ
    bool EffectiveLocation;//�Ƿ��յ���γ����Ϣ
    u8 TimeBuf[20];//���AT�յ���ʱ����Ϣ
    u8 TimeBufLen;//����ʱ����Ϣ�����ݳ���
    u8 GpsInfoBuf[30];//���AT�յ����ٶ�����
    u8 GpsInfoBufLen;//�����ٶȵ����ݳ���
    u8 GpsCnoBuf[10];//���CNO�յ����ٶ�����
    u8 GpsCnoBufLen;//����Cno�����ݳ���
    u8 ucDate[3];//������
    u8 ucTime[3];//ʱ����
    u16 ucSpeed;
    u8 Buf[30];
    u8 Len;
    u8 HDRCSQBuf[2];
    u8 HDRCSQLen;
    u8 CSQBuf[2];
    u8 CSQLen;
    bool PppOk;
    bool Tcp;
    bool TcpOk;
  }NetState;
}AtCmdDrv;

static AtCmdDrv AtCmdDrvobj;
static void AtCmd_NetParamCode(void);//��ȡTCP IP��ַ

bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  u8 Buf1[4];
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM3_GD83StartupReset://1
    DrvGD83_UART_TxCommand((u8*)ucGD83Reset,strlen((char const *)ucGD83Reset));
    //KaiJi_Flag=FALSE;
    break;
  case ATCOMM3_GD83Reset://1
    //DrvGD83_UART_TxCommand((u8*)ucGD83Reset,strlen((char const *)ucGD83Reset));
    api_lcd_pwr_on_hint("                ");//
    main_init();
    ApiAtCmd_SetLoginState();
    BootProcess_SIMST_Flag=0;
    CSQ_Flag=0;
    TaskStartDeleteDelay1=0;
    TaskStartDeleteDelay2=0;
    TaskStartDeleteDelay3=0;
    TaskStartDeleteDelay4=1;
    TaskStartDeleteDelay5=0;
    TaskStartDeleteDelay6=0;
    TaskStartDeleteDelay1Count=0;
    TaskStartDeleteDelay3Count=0;
    TaskStartDeleteDelay4Count=0;
    TaskStartDeleteDelay6Count=0;
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
    DrvGD83_UART_TxCommand((u8*)ucCheckRssi, strlen((char const*)ucCheckRssi));
    break;
  case ATCOMM15_HDRCSQ:
    DrvGD83_UART_TxCommand((u8*)ucHDRCSQ, strlen((char const*)ucHDRCSQ));
    break;
  case ATCOMM7_VGR://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM8_CheckTcp://2
    DrvGD83_UART_TxCommand((u8*)ucCheckTcp, strlen((char const*)ucCheckTcp));
    DrvGD83_UART_TxCommand(buf, len);
    break; 
  case ATCOMM9_SetIp	://1
    DrvGD83_UART_TxCommand((u8*)ucSetIp, strlen((char const *)ucSetIp));
    //DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM10_SendTcp://1
    if(len <= 1024 && len != 0x00)
    {
      DrvGD83_UART_TxCommand((u8*)ucSendTcp, strlen((char const *)ucSendTcp));
      AtCmd_NetParamCode();//��ȡIP��ַ
      DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, AtCmdDrvobj.NetState.Len);
      Buf1[0]=',';
      Buf1[1]='0';
      Buf1[2]='x';
      DrvGD83_UART_TxCommand(Buf1, 3);
      DrvGD83_UART_TxCommand(buf, len);
    }
    break; 
  case ATCOMM11_ZpppOpen ://1
    DrvGD83_UART_TxCommand((u8*)ucZpppOpen, strlen((char const *)ucZpppOpen));
    break;
  case ATCOMM12_CheckPPP ://2
    DrvGD83_UART_TxCommand((u8*)ucCheckPPP, strlen((char const *)ucCheckPPP));
    break;
  case ATCOMM13_CheckRssi:
    DrvGD83_UART_TxCommand((u8*)ucCheckRssi, strlen((char const *)ucCheckRssi));
    break;
  case ATCOMM14_CheckCard:
    DrvGD83_UART_TxCommand((u8*)ucCheckCard, strlen((char const *)ucCheckCard));
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
  if(AtCmdDrvobj.NetState.SignalToNoiseMax>=350&&
     AtCmdDrvobj.NetState.SignalToNoiseMax<=1000&&
     AtCmdDrvobj.NetState.SouXingConut>=3&&
     AtCmdDrvobj.NetState.EffectiveLocation==TRUE)
  {
    PositionInfoSendToATPORT_RedLed_Flag=TRUE;
    PositionInformationSendToATPORT_Flag=TRUE;
    PositionInfoSendToATPORT_SetPPP_Flag=TRUE;
    PositionInfoSendToATPORT_InfoDisplay_Flag=TRUE;
  }

    if(GetTaskId()==Task_NormalOperation)//��¼�ɹ�����Ⱥ��״̬
    {
      if(PositionInfoSendToATPORT_SetPPP_Flag==TRUE)//�����λ�ɹ�(�����յ�[��γ��])�����¼�����¼IP��������λ��
      {
        if(AtCmdDrvobj.NetState.PppOk == FALSE)//���PPPδ������
        {
          if(ApiAtCmd_WritCommand(ATCOMM11_ZpppOpen, (void*)0, 0) == TRUE)//pocnetopen
          {
            if(ApiAtCmd_WritCommand(ATCOMM12_CheckPPP, (void*)0, 0) == TRUE)//POCNETOPEN?
            {}
          }
        }
        else//PPP��������
        {
            if(AtCmdDrvobj.NetState.TcpOk == FALSE)//��ѯ�󷵻ص�TCP״̬
            {
              if(AtCmdDrvobj.NetState.Tcp == FALSE)//���TCP�ǹرյ�
              {
               // AtCmd_NetParamCode();//��ȡIP��ַ
                if(ApiAtCmd_WritCommand(ATCOMM9_SetIp, (void*)AtCmdDrvobj.NetState.Buf, AtCmdDrvobj.NetState.Len) == TRUE)//����IP
                {
                  AtCmdDrvobj.NetState.Tcp = TRUE;
                }
              }
              else//���TCP�Ǵ򿪵ģ�����TCP�����Ƿ�����
              {
                AtCmdDrvobj.NetState.Buf[0] = 0x30;
                if(ApiAtCmd_WritCommand(ATCOMM8_CheckTcp, (void*)AtCmdDrvobj.NetState.Buf, 1) == TRUE)
                {}
              }
            }
        }
      }
    }
}
void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len,i;
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
    /***********HDRCSQ�źŻ�ȡ���ж�****************************************************************/
    ucRet = memcmp(pBuf, ucRxHDRCSQ, 8);
    if(ucRet == 0x00)
    {
      //UpgradeNoATReturn_Flag=FALSE;
      if(Len > 8)//ȥ�^
      {
        Len -= 8;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.HDRCSQBuf[i] = pBuf[i + 8];//
       }
       AtCmdDrvobj.NetState.HDRCSQLen = i;
      if(NetworkType_2Gor3G_Flag==3)
       HDRCSQValue=CHAR_TO_Digital(AtCmdDrvobj.NetState.HDRCSQBuf,AtCmdDrvobj.NetState.HDRCSQLen);
    } 
    /*********************************************************/
    ucRet = memcmp(pBuf, ucSIMST1, 8);//^SIMST:1
    if(ucRet == 0x00)
    {
      api_lcd_pwr_on_hint("                ");//
      ApiAtCmd_SetLoginState();
      CSQ_Flag=0;
      //KaiJi_Flag=TRUE;
      SetTaskId(Task_Start);
      BootProcess_SIMST_Flag=1;
      //UpgradeNoATReturn_Flag2=FALSE;
      TaskStartDeleteDelay1=1;
#if 1//����ģ�������󣬲�����д�˺ŵ�����
      CSQ_Flag=0;
      TaskStartDeleteDelay2=0;
      TaskStartDeleteDelay3=0;
      TaskStartDeleteDelay4=1;
      TaskStartDeleteDelay5=0;
      TaskStartDeleteDelay6=0;
      TaskStartDeleteDelay1Count=0;
      TaskStartDeleteDelay3Count=0;
      TaskStartDeleteDelay4Count=0;
      TaskStartDeleteDelay6Count=0;
#endif
    }
    ucRet = memcmp(pBuf, ucSIMST255, 10);//^SIMST:255
    if(ucRet == 0x00)
    {
      //KaiJi_Flag=TRUE;
      //UpgradeNoATReturn_Flag2=FALSE;
      BootProcess_SIMST_Flag=2;
      TaskStartDeleteDelay3=1;
    }
    ucRet = memcmp(pBuf, ucCaretPPPCFG, 8);//GETICCID
    if(ucRet == 0x00)
    {
      BootProcess_PPPCFG_Flag=1;
    }
    if(AtCmdDrvobj.NetState.PppOk == FALSE)//���PPP����״̬Ϊδ����
    {
      ucRet = memcmp(pBuf, ucRxCheckPppOpen, 13);
      if(ucRet == 0x00)
      {
        AtCmdDrvobj.NetState.PppOk = TRUE;
      }
      else
      {
        ucRet = memcmp(pBuf, ucRxCheckPppClose, 13);//strlen(ucRxTcpDataHeadInfo));
        if(ucRet == 0x00)
        {
          AtCmdDrvobj.NetState.PppOk = FALSE;
        }
      }
    }
    else//���PPP����״̬Ϊ����
    {
      ucRet = memcmp(pBuf, ucRxCheckPppClose, 13);//strlen(ucRxTcpDataHeadInfo));
      if(ucRet == 0x00)
      {
        AtCmdDrvobj.NetState.PppOk = FALSE;//���Ӻ�Ͽ�
      }
      else//���PPP����״̬����
      {
        ucRet = memcmp(pBuf, ucRxCheckTcp, 13);//strlen(ucRxTcpDataHeadInfo));
        if(ucRet == 0x00)
        {
          ucRet = memcmp(pBuf, ucRxCheckTcp, 15);//strlen(ucRxTcpDataHeadInfo));
          if(ucRet == 0x00)
          {
            AtCmdDrvobj.NetState.TcpOk = TRUE;
          }
          else
          {
            AtCmdDrvobj.NetState.TcpOk = FALSE;
          }
        }
      }
    }
/***********��ȡ��ǰʱ�䣨����ʹ�ã�****************************************************************/
    ucRet = memcmp(pBuf, ucCDMATIME, 10);
    if(ucRet == 0x00)
    {
      //UpgradeNoATReturn_Flag=FALSE;
      if(Len > 10)//ȥ�^
      {
        Len -= 10;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.TimeBuf[i] = pBuf[i + 10];//
       }
       AtCmdDrvobj.NetState.TimeBufLen = i;
    } 
/*********��ȡ��ǰ�ٶ�*************************************************************************************/
    ucRet = memcmp(pBuf, ucGPSINFO, 9);
    if(ucRet == 0x00)
    {
      //UpgradeNoATReturn_Flag=FALSE;
      if(Len > 9)//ȥ�^
      {
        Len -= 9;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.GpsInfoBuf[i] = pBuf[i + 9];//
       }
       AtCmdDrvobj.NetState.GpsInfoBufLen = i;
    }
/*******��ȡGPSCNO����������**********************************************************************************************/
    ucRet = memcmp(pBuf, ucGPSCNO, 8);
    if(ucRet == 0x00)
    {
      //UpgradeNoATReturn_Flag=FALSE;
      if(Len > 8)//ȥ�^
      {
        Len -= 8;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.GpsCnoBuf[i] = pBuf[i + 8];//
       }
       AtCmdDrvobj.NetState.GpsCnoBufLen = i;
    }
/*****************************************************************************************************/
  }
}

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
#if 0//��ʱ����
    ucRet = memcmp(pBuf, ucRxPASTATE0, 9);// +PASTATE:0
    if(ucRet == 0x00)
    {
      ApiAtCmd_TrumpetVoicePlay_Flag=2;
    }
#endif
    ucRet = memcmp(pBuf, ucRxPASTATE1, 9);// +PASTATE:1
    if(ucRet == 0x00)
    {
      ApiAtCmd_ZTTS_Flag=TRUE;
      AUDIO_IOAFPOW(ON);
    }
    ucRet = memcmp(pBuf, ucRxZTTS0, 6);// +PASTATE:0
    if(ucRet == 0x00)
    {
      ApiAtCmd_ZTTS_Flag=FALSE;
    }
/***********CSQ�źŻ�ȡ���ж�****************************************************************/
    ucRet = memcmp(pBuf, ucRxCSQ, 4);
    if(ucRet == 0x00)
    {
      if(Len > 4)//ȥ�^
      {
        Len -= 4;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.CSQBuf[i] = pBuf[i + 4];//
       }
       AtCmdDrvobj.NetState.CSQLen = i;
      CSQValue=CHAR_TO_Digital(AtCmdDrvobj.NetState.CSQBuf,2);
      if(NetworkType_2Gor3G_Flag==2)
      {
        if(CSQValue==99)
        {
          HDRCSQValue=0;
        }
        else
        {
          HDRCSQValue=CSQValue*2;
        }
      }
    } 
/***********GPS��γ�Ȼ�ȡ������ʹ�ã�****************************************************************/
    ucRet = memcmp(pBuf, ucGpsPosition, 7);
    if(ucRet == 0x00)
    {
      if(Len > 7)//ȥ�^
      {
        Len -= 0x07;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.Position.Buf[i] = pBuf[i + 7];//-----------------------------��AT��GPS���ݻ�ȡ
       }
       AtCmdDrvobj.NetState.Position.BufLen = i;
    }
  }
}

void ApiAtCmd_Get_location_Information(void)
{
  u8 *pBuf;
  u8 i=0;
  u8 cHead2=0;
  u8 cInfoCount1=0;
  u8 cInfoCount2=0;
/*****��ȡGPSCNO������******************************************************************************************************************************/
  pBuf=AtCmdDrvobj.NetState.GpsCnoBuf;
  for(i=0;i<AtCmdDrvobj.NetState.GpsCnoBufLen;i++)
  {
    if(','==pBuf[i])
    {
      AtCmdDrvobj.NetState.SouXingConut = CHAR_TO_Digital(&pBuf[i+1], AtCmdDrvobj.NetState.GpsCnoBufLen-i-1);//������
    }
  }
/********��ȡ��ǰ�ٶ�gpsinfo***************************************************************************************************************************/
//2,394,22.517246,113.917666,0
  pBuf=AtCmdDrvobj.NetState.GpsInfoBuf;
  for(i=0;i<AtCmdDrvobj.NetState.GpsInfoBufLen;i++)
  {
    if(','==pBuf[i])
    {
      switch(cInfoCount1)
      {
      case 0:
        break;
      case 1:
        AtCmdDrvobj.NetState.SignalToNoiseMax=CHAR_TO_Digital(&pBuf[cHead2], i-cHead2);//����ź�ǿ��
        break;
      case 2:
        AtCmdDrvobj.NetState.Position.Longitude_Second = CHAR_TO_Digital(&pBuf[cHead2], i-cHead2);//����С��λ
        break;
      case 3:
        AtCmdDrvobj.NetState.Position.Latitude_Second = CHAR_TO_Digital(&pBuf[cHead2],i-cHead2);//γ��С��λ
#if 1//test,CHAR_TO_Digital�ٶ���ȷ
        AtCmdDrvobj.NetState.ucSpeed = CHAR_TO_Digital(&pBuf[i+1],AtCmdDrvobj.NetState.GpsInfoBufLen-i-1);//speed
#else
        //AtCmdDrvobj.NetState.ucSpeed = COML_AscToHex(&pBuf[i+1],AtCmdDrvobj.NetState.GpsInfoBufLen-i-1);//speed
#endif
        break;
      default:
        break;
      }
      cInfoCount1++;
      cHead2=i+1;
    }
    else
    {
      if('.' == pBuf[i])
      {
        switch(cInfoCount2)
        {
        case 0:
          if((i - cHead2) >= 2)//i-cHead=2
          {
            AtCmdDrvobj.NetState.Position.Longitude_Minute = CHAR_TO_Digital(&pBuf[cHead2], i-cHead2);//��������λ
            AtCmdDrvobj.NetState.EffectiveLocation=TRUE;
          }
          else
          {
            AtCmdDrvobj.NetState.EffectiveLocation=FALSE;
          }
          break;
        case 1:
          if((i - cHead2) >= 2)
          {
            AtCmdDrvobj.NetState.Position.Latitude_Minute = CHAR_TO_Digital(&pBuf[cHead2], i-cHead2);//γ������λ
            
          }
          break;
        default:
          break;
        }
        cInfoCount2++;
        cHead2 = i+1;
      }
    }
  }
}
/***********��ȡ������ʱ������Ϣ*********************************************************************************************************************/
//2018- 1 -18  1  9  :  4   5  :  1
void ApiAtCmd_Get_DateTime_Information(void)
{
  u8 *pBuf;
  u8 i=0,cDot2=0,cHead2=0;
  u8 cCount=0;//Time
  pBuf=AtCmdDrvobj.NetState.TimeBuf;
  for(i=0;i<AtCmdDrvobj.NetState.TimeBufLen;i++)
  {
    if('-'==pBuf[i])
    {
      switch(cCount)
      {
      case 0:
        AtCmdDrvobj.NetState.ucDate[0] = COML_AscToHex(&pBuf[cHead2+2], i-2);//��
        break;
      case 1:
        AtCmdDrvobj.NetState.ucDate[1] = COML_AscToHex(&pBuf[cHead2], i-cHead2);//��
        break;
      default:
        break;
        
      }
      cHead2 = i+1;
      cCount++;
    }
    else 
    {
      if(':' == pBuf[i])//2
      {
        switch(cDot2)
        {
        case 0:
          if((i - cHead2) >= 1)//i-cHead=2
          {
            AtCmdDrvobj.NetState.ucTime[0] = COML_AscToHex(&pBuf[cHead2], i-cHead2);//ʱ
          }
          break;
        case 1:
          if((i - cHead2) >= 1)
          {
            AtCmdDrvobj.NetState.ucTime[1] = COML_AscToHex(&pBuf[cHead2], i-cHead2);//��
            AtCmdDrvobj.NetState.ucTime[2] = COML_AscToHex(&pBuf[i+1],AtCmdDrvobj.NetState.TimeBufLen-i-1);//��
          }
          break;
        default:
          break;
        }
        cHead2 = i+1;
        cDot2++;
      }
      if(' '==pBuf[i])//ʶ��ո�
      {
        AtCmdDrvobj.NetState.ucDate[2] = COML_AscToHex(&pBuf[cHead2], i-cHead2);//��
        cHead2 = i+1;
      }
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




static void AtCmd_NetParamCode(void)//��ȡTCP IP��ַ
{
  u8 i,TcpIpLen,TcpPortLen;

  
  
  GetIPPORT();
  TcpPortLen=strlen((char const *)TcpPortBuf);
  TcpIpLen=strlen((char const *)TcpIpBuf);

  if(ApiGps_GetTcpPortAddress_No5()==0x00)
  {
  }
  else
  {
    TcpPortBuf[4]=ApiGps_GetTcpPortAddress_No5();
    TcpPortLen=5;
  }
  
  for(i=0;i<TcpIpLen;i++)
  {
    AtCmdDrvobj.NetState.Buf[i]=TcpIpBuf[i];
  }
  AtCmdDrvobj.NetState.Buf[TcpIpLen]=',';
  for(i=0;i<TcpPortLen;i++)
  {
    AtCmdDrvobj.NetState.Buf[TcpIpLen+1+i]=TcpPortBuf[i];
  }
  AtCmdDrvobj.NetState.Len=TcpIpLen+TcpPortLen+1;
}

bool ApiAtCmd_tcp_state(void)
{
	return AtCmdDrvobj.NetState.TcpOk;
}

bool ApiAtCmd_Ppp_state(void)
{
	return AtCmdDrvobj.NetState.PppOk;
}
u32  CHAR_TO_Digital(u8 * pBuf, u8 Len)
{
	u8 i;
	u32 buf = 0;
	for(i = 0; i < Len; i++)
	{
		buf *= 10;
		buf += (pBuf[i] - 0x30);
	}
	return buf;
}

u8 Data_Longitude_Minute(void)//��������λ
{
  return AtCmdDrvobj.NetState.Position.Longitude_Minute;
}
u32 Data_Longitude_Second(void)//����С��λ
{
  return AtCmdDrvobj.NetState.Position.Longitude_Second;
}
u8 Data_Latitude_Minute(void)//γ������λ
{
  return AtCmdDrvobj.NetState.Position.Latitude_Minute;
}
u32 Data_Latitude_Second(void)//γ��С��λ
{
  return AtCmdDrvobj.NetState.Position.Latitude_Second;
}

u8 Data_Time0(void)
{
  return AtCmdDrvobj.NetState.ucTime[0];
}
u8 Data_Time1(void)
{
  return AtCmdDrvobj.NetState.ucTime[1];
}
u8 Data_Time2(void)
{
  return AtCmdDrvobj.NetState.ucTime[2];
}
u8 Data_Date0(void)
{
  return AtCmdDrvobj.NetState.ucDate[0];
}
u8 Data_Date1(void)
{
  return AtCmdDrvobj.NetState.ucDate[1];
}
u8 Data_Date2(void)
{
  return AtCmdDrvobj.NetState.ucDate[2];
}
u16 Data_Speed(void)
{
  return AtCmdDrvobj.NetState.ucSpeed;
}

void HDRCSQSignalIcons(void)
{
  if(MenuMode_Flag==0)
  {
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
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
  }

}

void csq_value_for_display(void)
{
  u8 csq_value_buf[5];
  csq_value_buf[0]=0;
  csq_value_buf[1]=0;
  csq_value_buf[2]=0;
  csq_value_buf[3]=0;
  csq_value_buf[4]=0;
  if(HDRCSQValue==0)
  {
    api_lcd_pwr_on_hint("�ź�����:0");//
  }
  else
  {
    COML_DecToAsc(HDRCSQValue,csq_value_buf);
    COML_StringReverse(strlen((char const *)csq_value_buf),csq_value_buf);
    api_lcd_pwr_on_hint("�ź�����:");//
    api_lcd_pwr_on_hint8("      ");//5
    api_lcd_pwr_on_hint8(csq_value_buf);//5
  }
}


void sou_xing_count_for_display(void)
{
  u8 sou_xing_count_buf[5];
  sou_xing_count_buf[0]=0;
  sou_xing_count_buf[1]=0;
  sou_xing_count_buf[2]=0;
  sou_xing_count_buf[3]=0;
  sou_xing_count_buf[4]=0;
  if(AtCmdDrvobj.NetState.SouXingConut==0)
  {
    api_lcd_pwr_on_hint3("GPS������: 0");//
  }
  else
  {
    COML_DecToAsc(AtCmdDrvobj.NetState.SouXingConut,sou_xing_count_buf);
    COML_StringReverse(strlen((char const *)sou_xing_count_buf),sou_xing_count_buf);
    api_lcd_pwr_on_hint3("GPS������:");//
    api_lcd_pwr_on_hint9("  ");//5
    api_lcd_pwr_on_hint9(sou_xing_count_buf);//5
  }
}