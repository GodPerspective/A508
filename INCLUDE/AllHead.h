#ifndef __ALLHEAD_H
#define __ALLHEAD_H
//
#include "stm8s.h"

#include "sysclock.h"
#include "McuConfig.h"
#include "stm8s_it.h"
#include "CommonLib.h"
#include "Define.h"// 
#include "Delay.h"

#include "Voice.h"

//ͨѶģ��
#include "DrvGD83.h"
#include <string.h>
#include "ApiPocCmd.h"
#include "ApiAtCmd.h"

//����
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"

//��ʾ��
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"

//BEEP
#include "Tone.h"
#include "Beep.h"

//����
#include "Task.h"
#include "System.h"

//ָʾ��
#include "ApiLED.h"

//дƵ
#include "uart3.h"
#include "Flash_eeprom.h"
#include "Config.h"//����ǿ�����
#include "DataStructAddr.h"
//����
#include "ApiGpsCmd.h"

//��ص������
#include "ADC.h"
//�ֿ�IC
#include "GT20L16P1Y.h"
#include "GT20L16P1Y_D.h"//�ֿⲹ��

//�˵���ʾ
#include "ApiMenu.h"

//����
#include "ApiBeidou.h"

//����汾����
#include "KeyCmd.h"
#endif