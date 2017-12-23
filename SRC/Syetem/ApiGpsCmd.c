#include "AllHead.h"
#include <string.h>
#include <stdio.h>

u8 Test1=0;
typedef enum{
	GPSREV_Ack		= 0x8001,//ƽ̨ͨ��Ӧ��
	GPSREV_Puls		= 0x8002,//�ն�����
	GPSREV_Login		= 0x8100,//�ն�ע��Ӧ��
	GPSREV_Logout		= 0x0101,//�ն�ע��
	GPSREV_Authentication	= 0x0102,//�ն˼�Ȩ
	GPSREV_SetParam		= 0x8103,//�����ն˲���
	GPSREV_GetParam		= 0x8104,//��ѯ�ն˲���
	GPSREV_Position		= 0x8201,//λ����Ϣ��ѯ
	GPSREV_Photo		= 0x8801//����ͷ������������
}GpsRevCommType;

typedef enum{
	GPSCOMM_Ack		= 0x00,
	GPSCOMM_Puls		= 0x01,
	GPSCOMM_Login		= 0x02,
	GPSCOMM_Logout		= 0x03,
	GPSCOMM_Authentication	= 0x04,
	GPSCOMM_Position	= 0x05,
	GPSCOMM_PositionAck     = 0x06,
	GPSCOMM_PhotoAck	= 0x07,
	GPSCOMM_PhotoData	= 0x08,
	GPSCOMM_CheckTcp	= 0x09,
	GPSCOMM_SetIp		= 0x0A,
	GPSCOMM_CheckUdp	= 0x0B,
	GPSCOMM_SetUpu		= 0x0C,
	GPSCOMM_SendTcp		= 0x0D,
	GPSCOMM_SendUdp		= 0x0E,
	GPSCOMM_PlayZtts	= 0x0F,
	GPSCOMM_ZGSN		= 0x10
}GpsCommType;

#define GPS_GB_INFO_LEN			64+20//+512
#define GPS_GB_HEAD_LEN			12//��Ϣͷ����
#define GPS_GB_POSITION_LEN		28
#define GPS_GB_POSITION_ACK_LEN	28+2

typedef union{
  u8 ucData[GPS_GB_HEAD_LEN];//��Ϣͷȫ������
  struct{
    union{
      u8 ucData[2];
      u16 usData;
    }ucMsgID;//��ϢID
    union{
      u8 ucData[2];
      struct{
        u16 Len		: 10;//��Ϣ�峤��
        u16 Encryption	: 3;//����
        u16 Subpackage	: 1;//�ְ�
      u16		: 2;
      }Bits;
    }MsgProperty;//��Ϣ������
    union{
      u8 ucData[6];
      struct{
        u8	Num0	: 4;
        u8	Num1	: 4;
        u8	Num2	: 4;
        u8	Num3	: 4;
        u8	Num4	: 4;
        u8	Num5	: 4;
        u8	Num6	: 4;
        u8	Num7	: 4;
        u8	Num8	: 4;
        u8	Num9	: 4;
        u8	Num10	: 4;
        u8	Num11	: 4;
      }Bits;
    }ucTerminalNo;//�ն��ֻ���/�豸ID
    union{
      u8 ucData[2];
      u16 usData;
    }SerialNo;//��ˮ��
  }stParam;
}_Gb_GpsHeadInfo;//��Ϣͷ

typedef union{
  u32 ucData;
  struct{
    u32 bEmergency	: 1;
    u32 b1		: 1;
    u32 b2		: 1;
    u32 bEarlyWarning	: 1;
    u32 b4		: 1;
    u32 b5		: 1;
    u32 b6		: 1;
    u32 b7		: 1;
    u32 b8		: 1;
    u32 b9		: 1;
    u32 b10		: 1;
    u32 b11		: 1;
    u32 		: 6;
    u32 b18		: 1;
    u32 b19		: 1;
    u32 b20		: 1;
    u32 b21		: 1;
    u32 b22		: 1;
    u32 b23		: 1;
    u32 b24		: 1;
    u32 b25		: 1;
    u32 b26		: 1;
    u32 bIgnitionWarn	: 1;
    u32 b28		: 1;
    u32 b29		: 1;
    u32			: 2;
  }Bits;
}_Gb_AlarmState;//λ�û�����Ϣ���ݸ�ʽ�еı�����־

typedef union{
  u32 ucData;
  struct{
    u32 bAcc		: 1;
    u32 bGpsVolid	: 1;
    u32 bNorthOrSouth	: 1;
    u32 bEastOrWest	: 1;
    u32 bWorking	: 1;
    u32 bEncryption	: 1;
    u32 		: 4;
    u32 bOilWay		: 1;
    u32 bPowerSupply	: 1;
    u32 bLock		: 2;
    u32			: 18;
  }Bits;
}_Gb_WorkState;//λ�û�����Ϣ���ݸ�ʽ�е�״̬λ

typedef union{
  u8 ucData[GPS_GB_POSITION_LEN];
  struct{
    _Gb_AlarmState Warning;//������־
    _Gb_WorkState WorkStatus;//��λ״̬
    u32 ulLatitude;//γ��
    u32 ulLongitude;//����
    u16 usAltitude;// �߶�Geoid Separation
    u16 usSpeed;//�ٶ�
    u16 usDirection;//����
    struct{
      u8 ucData[3];
    }Date;//����
    struct{
      u8 ucData[3];
    }Time;//ʱ��
  }stParam;
}_PositInfo;

typedef union{
  u8 ucData[50+20];
  struct{
    _Gb_GpsHeadInfo HeadInfo;//��Ϣͷ
    union{
      struct{
        u16 SerialNo;
        u16 Command;
        u8	Result;
      }Ack;
      struct{
        union{
          u8 ucData[2];
          u16 usData;
        }Province;//ʡ��ID
        union{
          u8 ucData[2];
          u16 usData;
        }County;//������ID
        u8 Manufacturer[5];//������ID
        u8 TerminalType[8];//�ն��ͺ�
        u8 TerminalID[7];//�ն�ID
        u8 TerminalColor;//������ɫ
        u8 LicensePlate[20];//���ƺ�
      }LoginInfo;
      struct{
        u8 ucData[20];
      }Authentication;
      _PositInfo PositInfo;
      struct{
        u8 ucAckNo[2];
        _PositInfo PositInfo;
      }PositInfoAck;
      struct{
        u32 MediaId;
        u8 MediaMode;
        u8 MediaFormat;
        u8 MediaEven;
        u8 Number;
      }PhotoAck;
    }Message;
  }Param;
}_Gb_MsgBody;

typedef struct{
  struct{
    union{
      struct{
        u8 Enable	: 1;
        u8 Protocal	: 2;
        u8 Servicer	: 4;
        u8 WorkMode	: 1;
      }Bits;
      u8 Byte;
    }Cfg;
    u8 PulseTime;
    u8 ID[14];
    union{
      u8 ucData[19];//+512];
      struct{
        u8 IP[15];
        u8 Port[4];
      }Param;
    }NetParam;
  }Gps;
  u8 Vehicle;
  struct{
    union{
      u8 ucData[2];
      u16 usData;
    }Province;
    union{
      u8 ucData[2];
      u16 usData;
    }County;
    u8 Manufacturer[5];
    u8 TerminalType[8];
    u8 TerminalID[7];
    u8 TerminalColor;
    u8 LicensePlate[15];
    u8 ucRev[10];
  }LoginInfo;
  struct{
    union{
      struct{
        u8 Enable	: 1;
        u8 Polarity	: 1;
        u8		: 6;
      }Bits;
      u8 Byte;
    }Cfg;
    u8 OnReportTime;
    u8 OffReportTime;
    u8 CountdownTimer;
    u8 Res[3];
  }Acc;
}_GpsPar;

typedef struct{
  struct{
    u8 bAcc		: 1;
    u8 bRotot		: 2;
    u8 Protocol	        : 3;
    u8			: 2;
  }Msg;
  u16	 OilMass;
  u16	 TotalOilMass;
  struct{
    struct{
      u8 bGpsVolid	: 1;
      u8 bNorthOrSouth	: 1;
      u8 bEastOrWest	: 1;
      u8		: 5;
    }Msg;
    u32 ulLatitude;
    u32 ulLongitude;
    u16 usAltitude;// Geoid Separation
    u16 usSpeed;
    u16 usDirection;
    u8 ucDate[3];
    u8 ucTime[3];
  }Position;
  u8 rev[6];
}_InfoRecord;

typedef struct{
  union{
    struct{
      u8 bCountDown		: 1;
      u8 bGpsHardWareOk		: 1;
      u8			: 6;
    }Bits;
    u8 Byte;
  }Msg;
  struct{
    u8 uc100Ms;
    u8 Second;
    u8 Minute;
    u8 Hour;
    u8 LoadCount;
  }CountdownTimer;
  u8 ucWorkState;
  u16 usPulseTimer;
  u16 usReportTimer;
  u8 ucCommand[3];
  u8 ucBlock;
  _GpsPar GpsPar;
  _InfoRecord InfoRecord;
  union{
    struct{
      union{
        struct{
          u8 bLogin		: 1;
          u8 bPhotoCommand	: 1;
          u8 bDataTransmit	: 1;
          u8			: 5;
        }Bits;
        u8 Byte;
      }Msg;
      u8 ucStep;
      //_Xcy_tagGPSPositInfo  stHxkPostInfo;
      //_Xcy_tagGPSPositInfo_Y stHxkPostInfoY;
    }XcySys;
    struct{
      struct{
        union{
          struct{
            u8 bGeneralAck		: 1;//ͨ��Ӧ��
            u8 bSpecificAck		: 1;//����Ӧ��
            u8 bAuthenticatied	        : 1;//��֤
            u8 bLen			: 5;
          }Bits;
          u8 ucData;
        }Msg;
        u16 ulAckStep;
        u8 ucStep;
        u32 MediaId;
      }State;
      union{
        u8 ucData[21];
        struct{
          union{
            struct{
              u8 bLoginSuccess	: 1;
              u8 bLen		: 7;
            }Bits;
            u8 ucData;
          }Msg;
          u8 AuthenticationId[20];
        }ucParam;
      }LoginInfo;
      _Gb_MsgBody MsgBody;//�ն�ע��
      struct{
        u16 Len;
        u8 ucData[GPS_GB_INFO_LEN];
      }TempBuf;//����õ����ݰ����������ͣ�
    }GbSys;
  }PositionSystem;
}GpsFunDrv;
static GpsFunDrv *GpsFunDrvObj;
//static GpsFunDrv stGpsFunDrvObj;

static void GpsCmd_GbAnalytical(u8 *pBuf, u8 len);//�յ������ݽ��з���
static void GpsCmd_GbDataTransave(GpsCommType GpsComm);//��λ��Ϣת�����Ȼ�Ҫ�õ���POC��ȡGPS��Ϣת������TCP
static bool GpsCmd_GbWritCommand(GpsCommType id, u8 *buf, u8 len);
static void pack_data(u8 *pBuf, u16 ucLen);//��Ϣ����װ

void ApiGpsCmd_PowerOnInitial(void)//bubiao
{
//  u16 i;
//  u8 ucIndex0 = 0, ucIndex1 = 0;

  /*GpsFunDrvObj->GpsPar.LoginInfo.Province.usData=0x0010;//ʡ��ID
  GpsFunDrvObj->GpsPar.LoginInfo.County.usData=0x0300;//������ID
  GpsFunDrvObj->GpsPar.LoginInfo.Manufacturer[4]=0x00;//������ID
  GpsFunDrvObj->GpsPar.LoginInfo.Manufacturer[3]=0x00;
  GpsFunDrvObj->GpsPar.LoginInfo.Manufacturer[2]='1';
  GpsFunDrvObj->GpsPar.LoginInfo.Manufacturer[1]='Y';
  GpsFunDrvObj->GpsPar.LoginInfo.Manufacturer[0]='V';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[7]=' ';//�ն��ͺ�
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[6]=' ';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[5]=' ';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[4]=' ';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[3]=' ';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[2]=' ';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[1]='B';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalType[0]='J';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[6]='A';//�ն�ID
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[5]='B';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[4]='E';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[3]='L';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[2]='L';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[1]='0';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalID[0]='1';
  GpsFunDrvObj->GpsPar.LoginInfo.TerminalColor=0x01;//������ɫ
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[0]='1';//���ƺ�
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[1]='0';
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[2]='0';
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[3]='0';
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[4]='0';
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[5]='6';
  GpsFunDrvObj->GpsPar.LoginInfo.LicensePlate[6]=0x00;*/

 /* GpsFunDrvObj->GpsPar.Acc.Cfg.Bits.Enable = 0x01;
  GpsFunDrvObj->GpsPar.Acc.Cfg.Bits.Polarity = 0x00;
  GpsFunDrvObj->GpsPar.Acc.OnReportTime = 10;
  GpsFunDrvObj->GpsPar.Acc.OffReportTime = 0x01;
  GpsFunDrvObj->GpsPar.Acc.CountdownTimer = 0x01;*/



  //GpsFunDrvObj->usReportTimer = 0;//����ʱ������Ϊ0
  //����
  //GpsFunDrvObj->PositionSystem.GbSys.State.MediaId = 0x01;
  //GpsFunDrvObj->PositionSystem.GbSys.State.ulAckStep=0x00;
  GpsFunDrvObj->PositionSystem.GbSys.State.ucStep=0x00;
  //GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLoginSuccess = OFF;
  //GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Encryption=0;
  //GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Subpackage=0;
  /*ucIndex0 = strlen((char const *)GpsFunDrvObj->GpsPar.Gps.ID)-1;//�豸ID
  if(ucIndex0 > 11)
  {
    ucIndex0 = 11;
  }
  for(i = 0; i < 12; i++)
  {
    if(i < sizeof(GpsFunDrvObj->GpsPar.Gps.ID))
    {
      if(i % 2 == 0x00)
      {
        GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[ucIndex1]
          =GpsFunDrvObj->GpsPar.Gps.ID[ucIndex0] - 0x30;
      }
      else
      {
        GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[ucIndex1]
          +=(GpsFunDrvObj->GpsPar.Gps.ID[ucIndex0] - 0x30) << 0x04;
        ucIndex1++;
      }
      ucIndex0--;
    }
    else
    {
      if(i % 2 == 0x00)
      {
        GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[ucIndex1]=0;
      }
      else
      {
        GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[ucIndex1]+=0;
        ucIndex1++;
      }
    }
  }
  COML_StringReverse(0x06,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData);
*/
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[0]=0x00;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[1]=0x98;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[2]=0x00;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[3]=0x30;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[4]=0x53;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucTerminalNo.ucData[5]=0x88;
		
 /* GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.SerialNo.usData=0x0000;
  memcpy((u8*)&(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.LoginInfo),
         (u8*)&(GpsFunDrvObj->GpsPar.LoginInfo), sizeof(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.LoginInfo));*/
}

/*void ApiGpsCmd_1sRenew(void)
{
  if (SYS_GetCurTaskBuf() == TASK_mPowerOff || SYS_GetCurTaskBuf() == TASK_IDLE)
  {
    if(GpsFunDrvObj->GpsPar.Gps.Cfg.Bits.WorkMode != 0x00 &&
       GpsFunDrvObj->GpsPar.Acc.CountdownTimer != 0x00 &&
         GpsFunDrvObj->Msg.Bits.bCountDown == OFF)
    {
      GpsFunDrvObj->CountdownTimer.Second++;
      if(GpsFunDrvObj->CountdownTimer.Second >= 60)
      {
        GpsFunDrvObj->CountdownTimer.Second = 0x00;
        GpsFunDrvObj->CountdownTimer.Minute++;
        if(GpsFunDrvObj->CountdownTimer.Minute >= 60)
        {
          GpsFunDrvObj->CountdownTimer.Minute = 0x00;
          GpsFunDrvObj->CountdownTimer.Hour++;
          if(GpsFunDrvObj->CountdownTimer.Hour >= ((u16)GpsFunDrvObj->GpsPar.Acc.CountdownTimer*5))
          {
            GpsFunDrvObj->Msg.Bits.bCountDown = ON;
          }
        }
      }
    }
  }
  else
  {
    GpsFunDrvObj->Msg.Bits.bCountDown = OFF;
    GpsFunDrvObj->CountdownTimer.Hour = 0x00;
    GpsFunDrvObj->CountdownTimer.Minute = 0x00;
  }
}*/

void ApiGpsCmd_100msRenew(void)//����ʲôʱ����ʲô����
{
  if(GetTaskId()==Task_Start)//���ڳ�ʼ״̬
  {
    if(OFF != ApiAtCmd_tcp_state())
    {
      if(GpsCmd_GbWritCommand(GPSCOMM_Logout, (void *)0, 0) == TRUE)
      {
      }
    }
  }
  else//��¼�ɹ�״̬
  {
    if(GetTaskId()==Task_NormalOperation)
    {
      if(++Test1 == 10)//GPS��ʱ1S
      {
        Test1 = 0x00;//1s��һ��
        if((GpsFunDrvObj->usPulseTimer) == 0x00)//�����嶨ʱ=0
        {
          if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bSpecificAck == 0x00//�յ�����ָ���־λ=0
             &&(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bGeneralAck == 0x00))//���û���յ�����
          {
            GpsFunDrvObj->usPulseTimer = (GpsFunDrvObj->GpsPar.Gps.PulseTime * 10);//�����嶨ʱ��Ϊһ��ֵ
            if(OFF != ApiAtCmd_tcp_state())//���ʣ���TCP������������ʱ
            {
              if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bAuthenticatied != OFF)//��֤��ȷʱ
              {
                GpsCmd_GbWritCommand(GPSCOMM_Puls, (void*)0, 0);//�����ն�����
              }
            }
          }
        }
        else//�����嶨ʱ��0
        {
          GpsFunDrvObj->usPulseTimer--;
        }
        if((GpsFunDrvObj->usReportTimer) == 0x00)//����ʱ��=0ʱ
        {
          GpsFunDrvObj->usReportTimer =(GpsFunDrvObj->GpsPar.Acc.OnReportTime);//������ʱ����Ϊһ��ֵ
          if(OFF != ApiAtCmd_tcp_state())//���ʣ���TCP������������ʱ
          {
            if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bGeneralAck != 0x00)//�����ͨ��Ӧ��
            {
              GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bGeneralAck = 0x00;
              GpsCmd_GbWritCommand(GPSCOMM_Ack, (void*)0, 0);
              GpsFunDrvObj->usReportTimer = 3;//����ʱ����Ϊ3
            }
            else
            {
              if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bSpecificAck != 0x00)//���������Ӧ��
              {
                GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bSpecificAck = 0x00;
                switch(GpsFunDrvObj->PositionSystem.GbSys.State.ulAckStep)//AckStepΪ�ж��յ���ָ������
                {
                case GPSREV_Ack://ͨ��Ӧ��
                case GPSREV_Puls://����Ӧ��
                case GPSREV_Login://��¼Ӧ��
                case GPSREV_Logout://ע��Ӧ��
                case GPSREV_Authentication://��ȨӦ��
                  break;
                case GPSREV_Position://��λӦ��
                  GpsCmd_GbWritCommand(GPSCOMM_PositionAck, (void*)0, 0);
                  break;
                case GPSREV_Photo:
                  break;
                default:
                  break;
                }
              }
              else//����Ȳ�����ָͨ��Ҳ��������ָ��
              {
                switch(GpsFunDrvObj->PositionSystem.GbSys.State.ucStep)
                {
                case 0x00:
                  if(GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLoginSuccess==OFF)
                  {
                    GpsCmd_GbWritCommand(GPSCOMM_Login, (void*)0, 0);
                    GpsFunDrvObj->usReportTimer = 0x05;
                  }
                  else
                  {
                    GpsFunDrvObj->PositionSystem.GbSys.State.ucStep++;
                    GpsFunDrvObj->usReportTimer = 0x00;
                  }
                  break;
                case 0x01:
                  if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bAuthenticatied == OFF)
                  {
                    GpsCmd_GbWritCommand(GPSCOMM_Authentication, (void*)0, 0);
                    GpsFunDrvObj->usReportTimer = 0x05;
                  }
                  else
                  {
                    GpsFunDrvObj->PositionSystem.GbSys.State.ucStep++;
                    GpsFunDrvObj->usReportTimer = 0x00;
                  }
                  break;
                case 0x02:
                  if(GpsFunDrvObj->CountdownTimer.LoadCount < 0x05)
                  {
                      GpsFunDrvObj->CountdownTimer.LoadCount++;
                      GpsFunDrvObj->usReportTimer =(GpsFunDrvObj->GpsPar.Acc.OnReportTime);
                  }
                  else
                  {
                    GpsFunDrvObj->CountdownTimer.LoadCount = 0x00;
                    GpsFunDrvObj->usReportTimer =(GpsFunDrvObj->GpsPar.Acc.OnReportTime);
                  }
                  GpsCmd_GbWritCommand(GPSCOMM_Position, (void*)0, 0);
                  break;
                case 0x03:
                  GpsCmd_GbWritCommand(GPSCOMM_PhotoAck, (void*)0, 0);
                  GpsFunDrvObj->PositionSystem.GbSys.State.ucStep = 0x04;
                  GpsFunDrvObj->usReportTimer = 2;
                  break;
                case 0x04:
                  break;
                default:
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
}

void ApGpsCmd_10msRenew(void)
{
  u8 * pBuf, Len;
  if(GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bGeneralAck != 0x00
     || GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bSpecificAck != 0x00)//����յ���ͨ������ָ��
  {
    return;
  }
  while((Len = DrvMC8332_GpsNotify_Queue_front(&pBuf)) != 0)//
  {
    GpsCmd_GbAnalytical(pBuf, Len);
  }
}

static void GpsCmd_GbAnalytical(u8 *pBuf, u8 len)//�յ������ݽ��з���
{
  u8 i, j, k;
  u16 Len, usSeralNo;
  u32 ulParamId, ulParamData;
  pBuf+=0x05;
  GpsFunDrvObj->PositionSystem.GbSys.State.ulAckStep = (u16)(pBuf[1]) << 0x08 | pBuf[2];
  Len= (u16)(pBuf[3]) << 0x08 | pBuf[4];
  switch(GpsFunDrvObj->PositionSystem.GbSys.State.ulAckStep)
  {
  case GPSREV_Ack:
    if(pBuf[17] == 0x00)
    {
      usSeralNo = (u16)(pBuf[15]) << 0x08 | pBuf[16];
      switch(usSeralNo)
      {
      case 0x0000:
      case 0x0001:
        break;
      case 0x0102:
        GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bAuthenticatied = ON;
        break;
      case 0x0801:
        break;
      default:
        break;
      }
    }
    break;
  case GPSREV_Puls:
    break;
  case GPSREV_Login:
    if(pBuf[15] == 0x00)
    {
      pBuf+=16;
      Len-=0x03;
      for(i = 0x00; i < Len; i++)
      {
        GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.AuthenticationId[i] = pBuf[i];
      }
      GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLen = Len;
      GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLoginSuccess = ON;
    }
    break;
  case GPSREV_Logout:
    break;
  case GPSREV_Authentication:
    break;
  case GPSREV_SetParam:
    for(i = 0x00, j = 14; i < pBuf[13]; i++)
    {
      ulParamId = 0;
      for(k = 0x00; k < 4;k++, j++)
      {
        ulParamId <<= 0x08;
        ulParamId += pBuf[j];
      }
      len = pBuf[j++];
      for(k = 0x00; k < len;k++, j++)
      {
        ulParamData <<= 0x08;
        ulParamData += pBuf[j];
      }
      switch(ulParamId)
      {
      case 0x00000001:
        GpsFunDrvObj->GpsPar.Gps.PulseTime = ulParamData/10;
        break;
      case 0x00000027:
        GpsFunDrvObj->GpsPar.Acc.OffReportTime = ulParamData;
        break;
      case 0x00000029:
        GpsFunDrvObj->GpsPar.Acc.OnReportTime = ulParamData;
        break;
      default:
        break;
      }
    }
    break;
  case GPSREV_GetParam:
    break;
  case GPSREV_Position:
    GpsFunDrvObj->PositionSystem.GbSys.State.Msg.Bits.bSpecificAck = ON;
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.PositInfoAck.ucAckNo[0] = pBuf[11];
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.PositInfoAck.ucAckNo[1] = pBuf[12];
    GpsFunDrvObj->usReportTimer = 0x00;
    break;
  case GPSREV_Photo:
    break;
  default:
    break;
  }
}


static void GpsCmd_GbDataTransave(GpsCommType GpsComm)//��λ��Ϣת�����Ȼ�Ҫ�õ���POC��ȡGPS��Ϣת������TCP
{
  u8 i = 0;
  u8 ucIndex = 0;
  _PositInfo *pPositInfo;
  
  switch(GpsComm)
  {
  case GPSCOMM_PositionAck://λ����Ϣ��ѯӦ��
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0201;
    pPositInfo = &GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.PositInfoAck.PositInfo;
    break;
  case GPSCOMM_Position://λ����Ϣ�㱨
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0200;
    pPositInfo = &GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.PositInfo;
    break;
  default:
    break;
  }
  COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
  pPositInfo->stParam.Warning.ucData = 0;
  pPositInfo->stParam.WorkStatus.ucData = 0;
  pPositInfo->stParam.WorkStatus.Bits.bAcc = GpsFunDrvObj->InfoRecord.Msg.bAcc;
  pPositInfo->stParam.WorkStatus.Bits.bGpsVolid = GpsFunDrvObj->InfoRecord.Position.Msg.bGpsVolid;
  pPositInfo->stParam.WorkStatus.Bits.bNorthOrSouth = GpsFunDrvObj->InfoRecord.Position.Msg.bNorthOrSouth;
  pPositInfo->stParam.WorkStatus.Bits.bEastOrWest = GpsFunDrvObj->InfoRecord.Position.Msg.bEastOrWest;
  pPositInfo->stParam.ulLatitude = GpsFunDrvObj->InfoRecord.Position.ulLatitude;
  pPositInfo->stParam.ulLongitude = GpsFunDrvObj->InfoRecord.Position.ulLongitude;
  pPositInfo->stParam.usAltitude = GpsFunDrvObj->InfoRecord.Position.usAltitude;
  pPositInfo->stParam.usSpeed = GpsFunDrvObj->InfoRecord.Position.usSpeed;
  pPositInfo->stParam.usDirection = GpsFunDrvObj->InfoRecord.Position.usDirection;
  for(i = 0x00; i < 0x03; i++)
  {
    pPositInfo->stParam.Time.ucData[i] = GpsFunDrvObj->InfoRecord.Position.ucTime[i];
    pPositInfo->stParam.Date.ucData[i] = GpsFunDrvObj->InfoRecord.Position.ucDate[i];
  }
  COML_StringReverse(0x04, (u8 *)(&pPositInfo->stParam.Warning));
  COML_StringReverse(0x04, (u8 *)(&pPositInfo->stParam.WorkStatus));
  COML_StringReverse(0x04, (u8 *)(&pPositInfo->stParam.ulLatitude));
  COML_StringReverse(0x04, (u8 *)(&pPositInfo->stParam.ulLongitude));
  COML_StringReverse(0x02, (u8 *)(&pPositInfo->stParam.usAltitude));
  COML_StringReverse(0x02, (u8 *)(&pPositInfo->stParam.usSpeed));
  COML_StringReverse(0x02, (u8 *)(&pPositInfo->stParam.usDirection));
  COML_StringReverse(0x03, (u8 *)(&pPositInfo->stParam.Date));
  i = pPositInfo->stParam.Time.ucData[0];
  ucIndex = ((i >> 0x04) * 10);
  i &= 0x0F;
  i = (i + ucIndex + 8) % 24;
  ucIndex = (i / 10) << 0x04;
  i %= 10; 
  pPositInfo->stParam.Time.ucData[0] = i | ucIndex; 
}


static bool GpsCmd_GbWritCommand(GpsCommType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  u16 i = 0;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Encryption=0;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Subpackage=0;
  switch(id)
  {
  case GPSCOMM_Ack:
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0001;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.Ack.Result = 0;
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0x0005;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+5);
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
    GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_Puls:
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0002;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0x0000;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN);
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_Login:
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData=0x0100;//�ն�ע����ϢID
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);//��ϢIDת��Ϊ�ַ���
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len=31;//��Ϣ�峤��
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);//��Ϣ������
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.SerialNo.ucData);//��ˮ��
    i = strlen((char const *)GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.LoginInfo.LicensePlate);//���ƺŵĳ���
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, 37+i);//������Ϣͷ����Ϣ����ܳ���len=12+25+���ƺų��ȣ��������ݰ��ղ���Ҫ����
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);//��ʱ��������ż��ܺ���豸ע����Ϣ��
    break;
  case GPSCOMM_Logout:
    if(GpsFunDrvObj->ucWorkState != id)
    {
      GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0003;
      COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
      GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0;//GPS_GB_POSITION_LEN;
      COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
      pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN);//+GPS_GB_POSITION_LEN);
      ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    }
    break;
  case GPSCOMM_Authentication:
  for(i = 0x00; i < GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLen; i++)
    {
      GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.Authentication.ucData[i] =
        GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.AuthenticationId[i];
    }
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0102;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 
      GpsFunDrvObj->PositionSystem.GbSys.LoginInfo.ucParam.Msg.Bits.bLen;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+i);
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_Position:
    GpsCmd_GbDataTransave(GPSCOMM_Position);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = GPS_GB_POSITION_LEN+6+4;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+GPS_GB_POSITION_LEN+6+4);
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_PositionAck:
    GpsCmd_GbDataTransave(GPSCOMM_PositionAck);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = GPS_GB_POSITION_LEN+6+4+2;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+GPS_GB_POSITION_LEN+6+4+2);
    ApiAtCmd_WritCommand(ATCOMM10_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_PhotoAck:
    break;
  case GPSCOMM_PhotoData:
    break;
  default:
    break;
  }
  GpsFunDrvObj->ucWorkState = id;
  return r;
}

static void pack_data(u8 *pBuf, u16 ucLen)//��Ϣ����װ
{
  u16 i, ucTemp;
  GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len = 0x01;
  GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[0] = 0x7E;
  for(i = 0; i < ucLen; i++)//У���롢��Ϣͷ����Ϣ���г���0x7e����Ҫ����ת�崦��
  {
    if(i != 0x00 && pBuf[i] == 0x7e)
    {
      ucTemp = pBuf[0];
    }
  }
  if(ucLen != 0)
  {
    for(i = 0; i < ucLen; i++)
    {
      if(i == 0x00)
      {
        ucTemp = pBuf[0];
      }
      else
      {
        ucTemp ^= pBuf[i];//��� ucTempΪУ����
      }
      switch(pBuf[i])
      {
      case 0x7e://0x7e <��������> 0x7d �����һ�� 0x02
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len] = 0x7d;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x02;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
        break;
      case 0x7d://0x7d <��������> 0x7d �����һ�� 0x01
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x7d;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x01;
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
        break;
      default:
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= pBuf[i];
        GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
        break;
      }
    }
    switch(ucTemp)//���У������0x7e��0x7d����ת�壬�������У����
    {
    case 0x7e:
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len] = 0x7d;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x02;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
      break;
    case 0x7d:
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x7d;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= 0x01;
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
      break;
    default:
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len]= ucTemp;//����У����
      GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
      break;
    }
  }
  GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData[GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len] = 0x7E;//������һ��0x7e��ʶλ
  GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len++;
}


