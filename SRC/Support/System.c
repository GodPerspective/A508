#define SYSABLE
#include "AllHead.h"

#define SYS_IDLE	0x00
#define SYS_RUN		0x01

//u32 T1[63];//MAX=6144

typedef struct {
  struct{
    union{
      struct{
        u8 WorkMode	: 2;
        u8 UnlineArm	: 1;
        u8      	: 5;
      }Bits;
      u8 Byte;
    }Cfg;
    u8 UnlineArmCountdown;
    u8 Res[3];
  }Par;
  union {
    struct {
      u16 bLoad		: 1;
      u16 bRenew	: 1;
      u16 bTxRxRun	: 1;
      u16 bDebug	: 3;
      u16 bRxNullCh	: 1;
      u16 bTxNullCh	: 1;
      u16 bTalk		: 1;
      u16 bSq		: 1;
      u16 bSqMome	: 1;
      u16 bMoni		: 1;
      u16 bMoniMome	: 1;
      u16 bTot		: 1;
      u16 bUnlineArm	: 1;
      u16 		: 1;
    }Bit;
    u16 Byte;
  }STA;
  union {
    struct {
      u16 TxError		: 1;
      u16 			: 15;
    }Bit;
    u16 Byte;
  }Error;
  u16 cChannel;
  u8 KeyCode;
  POW_MANTYPE   PowMan;
  IO_ONOFF	  PowOnOff;
  TASK_CODE 	  NewId;
  TASK_CODE  	  OldId;
  MSG_STATUS	  Msg;
  SW_CODE 	  Sw;
  u16 UnlineArmTimer;
}TASK_DRV;

static TASK_DRV	TaskDrvObj;

u8 *ucSwitch            = "10000002";
u8 *ucGroupListInfo     = "0D0000";
u8 *ucGD83Reset         = "at^reset";

#if 1//��ص�������
//u16 Test2=0;
u8 TestBuf[5];
#endif

void main_app(void)
{
  //T1[62]=0;//Test
  disableInterrupts();
  SystemClock_Init(HSE_Clock);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_RX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART3_RX,ITC_PRIORITYLEVEL_2);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF,ITC_PRIORITYLEVEL_1);
  LED_Init();
  //��ʱ��ʼ��
  DEL_PowerOnInitial();
  //ͨѶģ���ʼ��
  DrvGD83_Init();
  DrvMC8332_Software_Initial();
 ApiGpsCmd_PowerOnInitial();
  //
  Uart1_Init();//ģ��ͨѶʹ��
  Uart3_Init(); //����дƵʹ��
  //��Ƶ��ʼ��
  AF_Mute_Init();
  Noise_Mute_Init();
  MIC_GPIO_Init();
  //BEEP
  BEEP_PowerOnInitial();
  TONE_PowerOnInitial();
  //��ʾ��
  drv_gt20_pwr_on_init();
  drv_htg_pwr_on_init();
  
  Key_Init();
  ADC_Init();//��ص�ѹ�ɼ�
  Set_RedLed(LED_OFF);
  Set_GreenLed(LED_OFF);
  enableInterrupts();

#if 1//EEPROM TEST
  
  TestBuf[0]=0x6b;
  TestBuf[1]=0x27;
  TestBuf[2]=0x68;
  TestBuf[3]=0x07;
  TestBuf[4]='\0';

#endif //EEPROMTEST

  GD83_ON();
  AUDIO_IOAFMUT(ON);
  AUDIO_IOAFPOW(ON);
  GPIO_Init(GPIOB,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//LOC MIC MUTE
  MIC_IOMUT(OFF); 
  MCU_LCD_BACKLIGTH(ON);//�򿪱����
  api_disp_icoid_output( eICO_IDBATT5, TRUE, TRUE);//��ʾ�������ͼ��
  api_lcd_pwr_on_hint2("eChat");

  BEEP_Time(1);
  NoUseNum=ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(u8 *)ucGD83Reset,strlen((char const *)ucGD83Reset));
  TaskDrvObj.NewId=Task_Start;//Task_Start

  while(1)
  {
#if 0//��ص�ѹ�ɼ�����
    
    
    /*case eICO_IDEmergency://ͼ�꣺3G
	case eICO_IDPOWERL://ͼ�꣺2G
		pIcoInfo->DispAddX = 0x02;
		break;
	case eICO_IDPOWERM://ͼ�꣺���
	case eICO_IDPOWERH://ͼ�꣺����
		pIcoInfo->DispAddX = 0x0c;//0x04
		break;
	case eICO_IDLOCKED://����ͼ��
	case eICO_IDLOCKEDOff://��ͼ��
		pIcoInfo->DispAddX = 0x06;
		break;
	case eICO_IDTALKAR://�޷����޽���״̬
        case eICO_IDTX://�����ź�ͼ��
	case eICO_IDVOX://ͼ�꣺���գ���״̬
          pIcoInfo->DispAddX = 0x08;*/
 api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//��ص���3��
 api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ص���3��
 api_disp_icoid_output( eICO_IDLOCKEDOff, TRUE, TRUE);//��ص���3��
 api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//��ص���3��
 api_lcd_pwr_on_hint4(Unicode2Gbk_MainWorkName());

#else
    LowVoltageDetection();
    DEL_Renew();
    switch(TaskDrvObj.NewId)
    {
    case Task_Start:
      Task_RunStart();
      if(ApiAtCmd_GetLoginState()==TRUE)//��¼�ɹ�
      {
        api_lcd_pwr_on_hint("���ڻ�ȡȺ����Ϣ");
        TaskDrvObj.NewId=Task_NormalOperation;
      }
      break;
    case Task_NormalOperation:
      Task_RunNormalOperation();
      break;
    case TASK_WRITEFREQ:
      TASK_WriteFreq();
      break;
    case TASK_LOBATT:		//task LO battery process
      TASK_RunLoBattery();
      break;
    default:
      break;
      
    }
#endif   
    
/*
#if 1//�������Ƶ�����   
    Keyboard_Test();
#endif

    
#if 1//�Խ�&����
    DEL_Renew();
    ListenState();//�������̵�
    if(ReadInput_KEY_PTT==0)
    {
      Set_RedLed(LED_ON);
      
      r=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      AUDIO_IOAFPOW(OFF);
      while(ReadInput_KEY_PTT==0);
      r=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ReadInput_KEY_4==0)
      {
        r=ApiPocCmd_WritCommand(PocComm_Key,ucSwitch,strlen((char const *)ucSwitch));
        Key_Flag_0=1;
      }
      else
      {
      if(ReadInput_KEY_2==0)
      {
        api_lcd_pwr_on_hint("ŷ�갴��:K2 �˳�����     ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A0000ffffffff",strlen((char const *)"0A0000ffffffff"));
        Key_Flag_0=1;
      }
      if(ReadInput_KEY_3==0)
      {
#if 1//1:һ�Ż���¼  0:���Ż���¼
        api_lcd_pwr_on_hint("����:K3 ����       ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0E000000000001",strlen((char const *)"0E000000000001"));
        Key_Flag_0=1;
        DEL_SetTimer(1,100);
        while(1)
        {
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
        
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A00000000006401",strlen((char const *)"0A00000000006401"));
#else//
        api_lcd_pwr_on_hint("����:K3 ����һ��   ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0E000000000001",strlen((char const *)"0E000000000001"));
        Key_Flag_0=1;
        DEL_SetTimer(1,20);
        while(1)
        {
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
        
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A00000000006401",strlen((char const *)"0A00000000006401"));
#endif
      }
      }
      if(Key_Flag_0==1)
      {
        DEL_SetTimer(0,200);
        while(1)
        {
          Key_Flag_0=0;
          if(DEL_GetTimer(0) == TRUE) {break;}
        }
      }
     
      Set_RedLed(LED_OFF);
      Set_GreenLed(LED_OFF);
    }

#endif
  }*/
}
}

void SYS_McuReset(void)
{
	//POW_SetPowSwitch(OFF);
	DEL_Soft1ms(8000);

}

TASK_CODE GetTaskId(void)
{
  return TaskDrvObj.NewId;
}
void SetTaskId(TASK_CODE a)
{
  TaskDrvObj.NewId=a;
}