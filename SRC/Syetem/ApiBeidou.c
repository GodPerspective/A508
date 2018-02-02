#include "AllHead.h"
u8 BeidouRxData[75];
u8 BeidouRxDataLen=0;
u8 RxStartFlag[6];
//������Ϣ��
#if 1
u8  BDValid=0;
u32 BDLongitude_Degree;//��
u32 BDLongitude_Minute;//��
u32 BDLongitude_Second;//С��������
u32 BDLatitude_Degree;//��
u32 BDLatitude_Minute;//��
u32 BDLatitude_Second;//С��������
u16 BDSpeed;//�ٶ�
u16 BDDirection;//����
#endif

void ApiBeidou_Get_location_Information(void)
{
  u8 *pBuf;
  u8 i=0,Ccomma=0,cDot=0,cHead=0;
/***************��ȡ��γ������*********************************************************************************************************/
  ////$GNRMC,000829.799,V,,,,,0.00,0.00,060180,,,N*58 \r
 // $GNRMC,025357.000,A,2231.0305,N,11355.0570,E,0.44,273.66,310118,,,A*7F
  //      0          1 2         3 4          5 6    7      8      
  //if(UART_BeidouGNRMCCommand()==TRUE)
  pBuf=BeidouRxData;
  for(i=0;i<BeidouRxDataLen;i++)
  {
      if(',' == pBuf[i])//2
      {
        switch(Ccomma)
        {
        case 0:
          break;
        case 1:
          if(pBuf[i+1]=='A')
            BDValid=1;
          else
            BDValid=0;
          break;
        case 2:
          break;
        case 3:
          if((i - cHead) >= 4)
          {
           BDLatitude_Second  = CHAR_TO_Digital(&pBuf[cHead], i-cHead);//γ�Ⱥ���λ
          }
          break;
        case 4://N
          break;
        case 5:
          if((i - cHead) >= 4)
          {
            BDLongitude_Second = CHAR_TO_Digital(&pBuf[cHead], i-cHead);//���Ⱥ���λ
          }
          break;
        case 6:
          break;
        case 7:
          break;
        default:
          break;
        }
        cHead = i+1;
        Ccomma++;
      }
    else
    {
      if('.' == pBuf[i])//2
      {
        switch(cDot)
        {
        case 0:
          break;
        case 1://γ��22
          if((i - cHead) >= 4)
          {
            BDLatitude_Degree  = CHAR_TO_Digital(&pBuf[cHead],2);//γ��ǰ2λ
            BDLatitude_Minute  = CHAR_TO_Digital(&pBuf[cHead+2],2);//γ����2λ   
          }
          break;
        case 2:
          if((i - cHead) >= 5)
          {
            BDLongitude_Degree = CHAR_TO_Digital(&pBuf[cHead], 3);//����ǰ2λ
            BDLongitude_Minute = CHAR_TO_Digital(&pBuf[cHead+3],2);//������2λ
          }
          break;
        case 3:
          BDSpeed = (u16)(1.85*CHAR_TO_Digital(&pBuf[cHead], i-cHead));//�ٶȣ�1.85*����/Сʱ��
          //BDSpeed = 20;//�ٶȣ�1.85*����/Сʱ��
          break;
        case 4:
          BDDirection=CHAR_TO_Digital(&pBuf[cHead], i-cHead);
          break;
        default:
          break;
        }
        cHead = i+1;
        cDot++;
      }
    }
  }
}