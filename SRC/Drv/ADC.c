#include "AllHead.h"

void ADC_Init(void)
{
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_CHANNEL_2, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL10,DISABLE);
  ADC2_Cmd(ENABLE);
}

//ADC2��ͨ��ѡ���ȡADֵ
 u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
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