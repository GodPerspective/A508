#include "AllHead.h"

u8 BatteryLevel=0;
bool LowVoltageDetection_Flag;

static u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
  ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel);
void ADC_Init(void)
{
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_CHANNEL_2, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL10,DISABLE);
  ADC2_Cmd(ENABLE);
}

//ADC2��ͨ��ѡ���ȡADֵ
static u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
  ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel)
{
  uint16_t ADConversion_Value;
  /**< ����ת��ģʽ */
  /**< ʹ��ͨ�� */
  /**< ADCʱ�ӣ�fADC2 = fcpu/18 */
  /**< ���������˴�TIM TRGO ����ת������ʵ����û���õ���*/
  /**  ��ʹ�� ADC2_ExtTriggerState**/
  /**< ת�������Ҷ��� */
  /**< ��ʹ��ͨ��10��˹���ش����� */
  /**  ��ʹ��ͨ��10��˹���ش�����״̬ */
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_Channel, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SchmittTriggerChannel,DISABLE);
  ADC2_Cmd(ENABLE);
  ADC2_StartConversion();
  ADConversion_Value = ADC2_GetConversionValue();
  return ADConversion_Value;
}
void LowVoltageDetection(void)
{
  u16 ADValue;
  ADValue=OneChannelGetADValue(ADC2_CHANNEL_2,ADC2_SCHMITTTRIG_CHANNEL2);
  
  if(GetTaskId()==Task_Start)
  {}
  else
  {
    if(ADValue<=340&&ADValue>=300)
    {
      api_disp_icoid_output( eICO_IDBATT, TRUE, TRUE);//��ص���3��
      SetTaskId(TASK_LOBATT);
      
      LowVoltageDetection_Flag=TRUE;
    }
    else
    {
      if(ADValue<=350&&ADValue>=300)
      {
        api_disp_icoid_output( eICO_IDBATT , TRUE, TRUE);
        BatteryLevel=0;
      }//��ص���0��
      else if(ADValue<=360&&ADValue>350)
      {
        api_disp_icoid_output( eICO_IDBATT1, TRUE, TRUE);
        BatteryLevel=1;
      }//��ص���1��
      else if(ADValue<=375&&ADValue>365)
      {
        api_disp_icoid_output( eICO_IDBATT2, TRUE, TRUE);
        BatteryLevel=2;
      }//��ص���2��
      else if(ADValue<=390&&ADValue>380)
      {
        api_disp_icoid_output( eICO_IDBATT3, TRUE, TRUE);
        BatteryLevel=3;
      }//��ص���3��
      else if(ADValue<=405&&ADValue>395)
      {
        api_disp_icoid_output( eICO_IDBATT4, TRUE, TRUE);
        BatteryLevel=4;
      }//��ص���4��
      else if(ADValue<=500&&ADValue>410)
      {
        api_disp_icoid_output( eICO_IDBATT5, TRUE, TRUE);
        BatteryLevel=5;
      }//��ص���5��
      else{}
      if(LowVoltageDetection_Flag==1)//ʶ��ӵ͵������ߵ�����״̬
      {
        ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
        LowVoltageDetection_Flag=FALSE;
      }
      SetTaskId(Task_NormalOperation);
    }
  }
}

void KeyBatteryReport(void)
{
  switch(BatteryLevel)
  {
  case 0:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e3500",24);//�ٷ�֮��
    break;
  case 1:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e32004153",28);//�ٷ�֮20
    break;
  case 2:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e34004153",28);//�ٷ�֮40
    break;
  case 3:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e36004153",28);//�ٷ�֮60
    break;
  case 4:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e38004153",28);//�ٷ�80
    break;
  case 5:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606527e76",20);//�ٷְ�
    break;
  default:
    break;
  }

}