#ifndef __APIBEIDOU_H
#define __APIBEIDOU_H
#define COML_FUNENABLE

//#define BEIDOU//Ĭ������GPS��ע��ȥ��Ϊ���ñ�����������������
#define BEIDOU_FUNENABLE

#ifdef	BEIDOU_FUNENABLE
  #define BEIDOUAPI extern
#else
  #define BEIDOUAPI 
#endif

#if 1
BEIDOUAPI u8  BDValid;
BEIDOUAPI u32 BDLongitude_Degree;//��
BEIDOUAPI u32 BDLongitude_Minute;//��
BEIDOUAPI u32 BDLongitude_Second;//С��������
BEIDOUAPI u32 BDLatitude_Degree;//��
BEIDOUAPI u32 BDLatitude_Minute;//��
BEIDOUAPI u32 BDLatitude_Second;//С��������
BEIDOUAPI u16 BDSpeed;//�ٶ�
BEIDOUAPI u16 BDDirection;//����
#endif

BEIDOUAPI u8 BeidouRxData[75];
BEIDOUAPI u8 BeidouRxDataLen;
BEIDOUAPI u8 RxStartFlag[6];
BEIDOUAPI void ApiBeidou_Get_location_Information(void);
#endif