#define  TONELABEL
#include "AllHead.h"
#ifdef TONE_FUNENABLE

#define TONE_IDLE 	0x00
#define TONE_RUN 	0x01
#define TONE_MAXFREQ	0x02

typedef struct {						//DA tone freq parameter process
	u8 cLevel;
	u16  iStep;
	WordType iStepSum;
}TONE_PAR;

typedef struct 
	{
	union 
		{
		struct {
			u8 bSyn 	: 1;
			u8 bSet 	: 1;
			u8 bRun		: 1;
			u8 bMute	: 1;
			u8 			: 4;
		}Bit;
		u8 Byte;
	}Msg;
	u8 Number;
	u16  Freq0;
	u8 Level0;
	u16  Freq1;
	u8 Level1;
	TONE_PAR TonePar[TONE_MAXFREQ];
}TONE_DRV;

static void Delay(u32 nCount);
static TONE_DRV	ToneDrvObj;
void TIM1_PWM_Init(void)
{
	GPIO_Init(GPIO_BEEP, GPIO_PIN_BEEP, GPIO_MODE_OUT_PP_LOW_FAST);
        TIM1_DeInit();
        /*
	 * TIM2 Frequency = TIM2 counter clock/(ARR + 1) 
	 * ��������TIM2�ļ���Ƶ��Ϊ 24M/8/(999+1)=1K
	 */
        TIM1_TimeBaseInit(1, TIM1_COUNTERMODE_UP, 4000, 0);
	
	/* 
	 * PWM1 Mode configuration: Channel1
	 * TIM2 Channel1 duty cycle = [TIM2_CCR1/(TIM2_ARR + 1)] * 100 = 50%
	 * TIM2 Channel2 duty cycle = [TIM2_CCR2/(TIM2_ARR + 1)] * 100 = 50%
	 */ 

	/* ����ͨ��1 */
	TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE ,2000, TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_RESET);
	TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);  
        TIM1_OC1PreloadConfig(ENABLE);
        TIM1_CtrlPWMOutputs(ENABLE);
        TIM1_Cmd(ENABLE);
}

/*******************************************************************************
 * ����: Set_TIM2_PWM_Frequency
 * ����: ����TIM2-PWMƵ��
 * �β�: TIM2_Period -> Ҫ���õ�Ƶ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void Set_TIM1_PWM_Frequency(uint16_t TIM1_Period)
{
	/* ����PWMƵ�� */
	TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
	TIM1->ARRL = (uint8_t)(TIM1_Period);
}

/*******************************************************************************
 * ����: Set_TIM1_PWM1_DutyCycle
 * ����: ����TIM1-PWMͨ��1ռ�ձ�
 * �β�: TIM1_Pulse -> Ҫ���õ�ռ�ձ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void Set_TIM1_PWM1_DutyCycle( uint16_t TIM1_Pulse)
{
    /* ����ͨ��1ռ�ձ� */
    TIM1->CCR1H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR1L = (uint8_t)(TIM1_Pulse);
}

/*******************************************************************************
 * ����: Test_PWM_LED
 * ����: ����ͨ��2
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void Test_PWM_LED(void)
{
	u16 Duty_Val;
	for(Duty_Val = 0; Duty_Val < 4000; Duty_Val++)
	{
		Set_TIM1_PWM1_DutyCycle(Duty_Val);
		Delay(0xff);
	}
}

static void Delay(u32 nCount)
{
	/* Decrement nCount value */
	while (nCount != 0)
	{
		nCount--;
	}
}

void TONE_PowerOnInitial(void)
{
	u8 i;
	
        TIM1_PWM_Init();
	
	for(i = 0; i < TONE_MAXFREQ; i++)
	{
		ToneDrvObj.TonePar[i].cLevel  = 0;
		ToneDrvObj.TonePar[i].iStep   = 0;  
		ToneDrvObj.TonePar[i].iStepSum.Word   = 0;
	}
	ToneDrvObj.Number   = 0x00;
	ToneDrvObj.Freq0	= 0x00;
	ToneDrvObj.Level0	= 0x00;
	ToneDrvObj.Freq1	= 0x00;
	ToneDrvObj.Level1	= 0x00;
	ToneDrvObj.Msg.Byte = TONE_IDLE;
	return;
}

void TONE_Start(void)
{

	if (ToneDrvObj.Msg.Bit.bRun == TONE_IDLE)
	{
		ToneDrvObj.Msg.Bit.bRun = TONE_RUN;
		ToneDrvObj.Msg.Bit.bSet = TONE_RUN;
		TIM1_CtrlPWMOutputs(ENABLE);
	}
	else
	{
		TONE_Stop();
	}
	return;
}
void TONE_Stop(void)
{
	ToneDrvObj.Msg.Byte = TONE_IDLE;
	TIM1_CtrlPWMOutputs(DISABLE);
	return;
}

void TONE_SetMute(IO_ONOFF OnOff)
{
	if (OnOff == OFF)
	{
		GPIO_WriteLow(GPIOD, GPIO_PIN_7);//Noise-Mute
	}
	else
	{
		GPIO_WriteHigh(GPIOD, GPIO_PIN_7);//Noise-Mute
	}
	return;
}

#endif