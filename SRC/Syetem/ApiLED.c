#include "AllHead.h"
u8 LED_Conut=0;
void LED_IntOutputRenew(void)
{

  if( GetTaskId()==Task_Start)
  {
    LED_Conut++;
    if(LED_Conut==5)
    {
      Set_GreenLed(LED_ON);
    }
    if(LED_Conut==8)
    {
      Set_GreenLed(LED_OFF);
      LED_Conut=0;
    }
  }
  else
  {
    if(POC_ReceivedVoice_Flag==TRUE)
    {
      Set_GreenLed(LED_ON);
    }
    else
    {
      LED_Conut++;
      if(LED_Conut>=49)
      {
#ifdef BEIDOU//ʹ�����ñ���
        if(BDValid==1)
#else//ʹ������GPS
        if(PositionInfoSendToATPORT_RedLed_Flag==TRUE)
#endif
        {
          Set_RedLed(LED_ON);
        }
        else
        {
          Set_GreenLed(LED_ON);
        }
        
      }
      if(LED_Conut>=53)
      {
        PositionInfoSendToATPORT_RedLed_Flag=FALSE;
#if 1 //����������ģʽ����סPTT�����ͣ���������һ�β����̵�ȴ�ܽ��յ�����������(�����̵�����)
        if(POC_ReceivedNoVoice_Flag==TRUE)
        {}
        else
        {
          Set_GreenLed(LED_OFF);
        }
#else
        Set_GreenLed(LED_OFF);
#endif

        Set_RedLed(LED_OFF);
        LED_Conut=0;
      }
      if(LED_Conut<=48)
      {
#if 1 //����������ģʽ����סPTT�����ͣ���������һ�β����̵�ȴ�ܽ��յ�����������(�����̵�����)
        if(POC_ReceivedNoVoice_Flag==TRUE)
        {}
        else
        {
          Set_GreenLed(LED_OFF);
        }
#else
        Set_GreenLed(LED_OFF);
#endif

        if(WriteFreq_Flag==TRUE)//�����Ƶ��Ƴ��������⣬Ӧ���������
        {
          Set_RedLed(LED_OFF);
        }
        else
        {
        }
        
        //Set_RedLed(LED_OFF);
      }
    }
  }
}