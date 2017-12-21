#include "AllHead.h"
#include <string.h>
#include <stdio.h>

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
  //_GpsPar GpsPar;
  //_InfoRecord InfoRecord;
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
            u8 bGeneralAck		: 1;
            u8 bSpecificAck		: 1;
            u8 bAuthenticatied	        : 1;
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

static bool GpsCmd_GbWritCommand(AtCommType id, u8 *buf, u8 len);
static void pack_data(u8 *pBuf, u16 ucLen);//��Ϣ����װ

void ApiGpsCmd_100msRenew(void)//����ʲôʱ����ʲô����
{
  if(OFF != ApiAtCmd_tcp_state())
  {}
}

static bool GpsCmd_GbWritCommand(AtCommType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  u16 i = 0;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Encryption=0;
  GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Subpackage=0;
  switch(id)
  {
  /*case GPSCOMM_Ack:
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0001;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.Message.Ack.Result = 0;
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0x0005;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+5);
    ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
    GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_Puls:
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0002;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0x0000;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN);
    ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;*/
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
  /*case GPSCOMM_Logout:
    if(GpsFunDrvObj->ucWorkState != id)
    {
      GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.usData = 0x0003;
      COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.ucMsgID.ucData);
      GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = 0;//GPS_GB_POSITION_LEN;
      COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
      pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN);//+GPS_GB_POSITION_LEN);
      ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
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
    ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_Position:
    GpsCmd_GbDataTransave(GPSCOMM_Position);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = GPS_GB_POSITION_LEN+6+4;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+GPS_GB_POSITION_LEN+6+4);
    ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;
  case GPSCOMM_PositionAck:
    GpsCmd_GbDataTransave(GPSCOMM_PositionAck);
    GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.Bits.Len = GPS_GB_POSITION_LEN+6+4+2;
    COML_StringReverse(0x02,GpsFunDrvObj->PositionSystem.GbSys.MsgBody.Param.HeadInfo.stParam.MsgProperty.ucData);
    pack_data(GpsFunDrvObj->PositionSystem.GbSys.MsgBody.ucData, GPS_GB_HEAD_LEN+GPS_GB_POSITION_LEN+6+4+2);
    ApiAtCmd_WritCommand(ATCOMM_SendTcp, GpsFunDrvObj->PositionSystem.GbSys.TempBuf.ucData,
                         GpsFunDrvObj->PositionSystem.GbSys.TempBuf.Len);
    break;*/
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