#define TASKABLE
#include "AllHead.h"

void Task_RunStart(void)
{
  if(SIMST_Flag==1)//�յ�ģ�鿪��ָ��:SIMST:1
  {
    BEEP_Time(50);
    SIMST_Flag==0;
    //1.����PPPCFG
   /* if()//����յ�^PPPCFG
    {
    //2.����echat�˺š�IP
      if()//�յ�+POC:00000000
      {
        //3.��echatӦ��
      }
    }
    if()//+POC:9000 ����Ҫ����
    {
      //�ȴ���¼
      if()//��¼�ɹ�
      {
        if()//����Ⱥ��
        {
          //�˳�Task_Start��־λ��������һ��־λ
        }
      }
    }
    else//90001��90002
    {
      //����Task_Start��־λ
    }
    */
  }
}

void TASK_WriteFreq(void)
{

}