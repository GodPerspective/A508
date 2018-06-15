#include "AllHead.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 Macro Define (macro definitions used in this file internally)
-----------------------------------------------------------------------------*/
const u8 CHAR_HIGH[2][8]  = { 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
							  0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02 };

const u8 CHAR_WIDTH[2][8] = { 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10,
							  0x08, 0x10, 0x06, 0x08, 0x08, 0x08, 0x10, 0x10 };


#define DISPRELAOD_TIMER_PERIOD ( 10*OS_TICKS_PER_10MS )  /* ��ʱ���ڣ�ÿ100ms�ص�һ�κ��� */
#define LCD_ALIAS_LEN_MAX 10
u16 CharCode;
typedef struct
{
	union
	{
		struct
		{
			u32	bRefresh	: 1;
			u32	bAllRef		: 1;
		}Bit;
		u32	Byte;
	}Msg;
}DIS_DRV;

//static DIS_DRV DisDrvObj;

static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData);
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData);
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData);//UNICODE��ʾ
/* ------------------------------------------------------------------------------- */
/* ���д���ϴ���ʱ,���º�����δ���壬����ʱ���壨���ڱ���ͨ���������������±�д   */
/* ------------------------------------------------------------------------------- */
const u8 BitTab[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


u8 DisDataBit[64]  = {0};
u8 DisDataBuf[512] = {0};
const u8 Ico_DataBuf[32][32]=
{
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//0

{0x00,0xF8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//1

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//2

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,   //3
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//4

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//5

/*
{0x00,0x00,0xE0,0x3C,0x22,0xA2,0xA2,0xA2,0xA2,0xA2,0x22,0x3C,0xE0,0x00,0x00,0x00,
0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x0B,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00},//6

{0x00,0x00,0xFC,0x0C,0x14,0x24,0x44,0x84,0x84,0x44,0x24,0x14,0x0C,0xFC,0x00,0x00,
0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x07,0x00,0x00},//7
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x8E,0x02,0x00,0xF0,0x00,0x00,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00 },//8
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x0E,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },//9
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },//10
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x42,0x42,0xF2,0x42,0x42,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x09,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },//11
 
{0x00,0xF8,0x08,0xF8,0x04,0x02,0xFE,0x00,0x10,0xE0,0x08,0xF0,0x04,0xF8,0x00,0x00,
0x00,0x03,0x02,0x03,0x04,0x08,0x0F,0x00,0x01,0x00,0x02,0x01,0x04,0x03,0x00,0x00 },//12*/

{0X00,0X00,0X10,0X11,0XF2,0X00,0X28,0X26,0XE4,0X3F,0XE4,0X24,0X20,0X00,0X00,0X00,
0X00,0X00,0X08,0X04,0X03,0X04,0X0A,0X09,0X08,0X08,0X09,0X0A,0X0B,0X00,0X00,0X00,//ѡ//6
},
{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//0���ź�//7

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//1���ź�//8

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//2���ź�//9

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00},//3���ź�//10

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0XF0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00},//4���ź�//11

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0XF0,0X00,0XFC,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X00},//5���ź�//12

 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//�޷����޽���״̬����ͼ�꣩//13
 
{0X00,0X7C,0X82,0X39,0X44,0X82,0X10,0XB8,0X10,0X82,0X44,0X39,0X82,0X7C,0X00,0X00,
0X00,0X00,0X00,0X01,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X01,0X00,0X00,0X00,0X00},//������//14

{0X82,0X44,0X39,0X82,0X7C,0X00,0X10,0XB8,0X10,0X00,0X7C,0X82,0X39,0X44,0X82,0X00,
0X00,0X00,0X01,0X00,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X00,0X01,0X00,0X00,0X00},//������//15

 {0X00,0X00,0X00,0X00,0X48,0X48,0XB8,0X00,0XE0,0X10,0X08,0X48,0X48,0XD0,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X02,0X01,0X00,0X00,0X01,0X02,0X02,0X02,0X03,0X00,0X00},//3G//16

{0X00,0X00,0X00,0X10,0X08,0X88,0X70,0X00,0XE0,0X10,0X08,0X48,0X48,0XD0,0X00,0X00,
0X00,0X00,0X00,0X02,0X03,0X02,0X02,0X00,0X00,0X01,0X02,0X02,0X02,0X03,0X00,0X00},//2G//17

 
{0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XC0,0X80,0XC0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,
0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X01,0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X00},//���//18
 
{0X00,0X00,0X00,0X00,0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X03,0X03,0X01,0X00,0X00,0X00,0X00},//����//19

{0X00,0X00,0XF0,0XF0,0XF0,0XF8,0XFC,0XFE,0X00,0X08,0XF0,0X02,0X04,0XF8,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X07,0X00,0X01,0X00,0X04,0X02,0X01,0X00,0X00},//����ģʽ//20

{0X00,0X00,0X00,0X0E,0X1F,0X3F,0X66,0XC0,0X80,0X00,0X80,0XC0,0XC0,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X01,0X00,0X00},//��Ͳģʽ//21
 
{0X00,0X00,0XF0,0X08,0X0E,0X09,0X69,0XE9,0X69,0X09,0X0E,0X08,0XF0,0X00,0X00,0X00,
0X00,0X00,0X03,0X04,0X04,0X04,0X04,0X05,0X04,0X04,0X04,0X04,0X03,0X00,0X00,0X00},//����ͼ��//22
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//��ͼ��//23
 
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,//�հ�ͼ�꣨��ѡico��Ӧ��//24
},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//25
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//26
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,//�հ�ͼ�꣨��ѡico��Ӧ��//27
},
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//28
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//29
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//30
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//31
};
/*******************************************************************************
* Description	: char-data display process
* Input			: CharInfo	: char data location information and dispaly char type
				: CharData	: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_char_output(DISP_CHAR CharInfo, u8 *CharData)
{	
	DISP_MulTypePro(CharInfo, CharData);
	return;
}
void api_disp_char_output2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE��ʾ��Ⱥ����Ϣ��ʾ����ʾʹ��
{	
	DISP_MulTypePro2(CharInfo, CharData);
	return;
}
void api_lcd_pwr_on_hint(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 0;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint2(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816B;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 5;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint3(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x00;//���Ͻ���ʾ����
	stCharInfo.DispAddX  = 0;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint4(u8 *CharData)//UNICODE��ʾ
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;//���Ͻ���ʾ����
	stCharInfo.DispAddX  = 0;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output2(stCharInfo,CharData);//UNICODE��ʾ��Ⱥ����Ϣ��ʾ����ʾʹ��

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}

void api_lcd_pwr_on_hint5(u8 *CharData)//�˵�ҳ���Ͻ����
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0;//���Ͻ���ʾ����
	stCharInfo.DispAddX  = 13;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}

void api_lcd_pwr_on_hint6(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 12;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint7(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x00;
	stCharInfo.DispAddX  = 5;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint8(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 0x09;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
void api_lcd_pwr_on_hint9(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x00;
	stCharInfo.DispAddX  = 11;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// ȫ��ͳһˢ��
}
/*******************************************************************************
* Description	: ˢ����Ļ������ʾ
* Input		: void
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_all_screen_refresh(void)
{
	u8 Page, XAlias;
	u16 iPoint;

	for (Page = 0; Page < PAGE_MAX; Page++)
	{
		iPoint = Page * 0x80;
		drv_htg_set_address(0, Page);
		for (XAlias = 0; XAlias < COL_MAX; XAlias++)
		{
			drv_htg_write_data(DisDataBuf[iPoint]);
			iPoint++;
		}
	}
	//bDisDataBufUsed = FALSE;
}


/*******************************************************************************
* Description	: Mixed type display process
* Input			: CharInfo : char display location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData)
{
  u8 CharCodeH;
  u8 CharCodeL;
  u16 CharCode;
  DISP_CHAR DisInfo;
  u8  iLen = 0;//34
  u8  CharBuf[34];
  DisInfo = CharInfo;
  if ((CharInfo.DispType & 0x80) != 0x00)
  {
    DisInfo.DispLenth = 0x00;
    for (; *CharData != 0x00; DisInfo.DispLenth++)
    {
      if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//���޸ĵ���ʾ����Ϊ16ʱ����ʾ���⣩
      CharCode = *CharData;
      DisInfo.DispAddX = CharInfo.DispAddX + iLen;
      DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
      if (*CharData >= 0x80)//Ϊ�����ַ�
      {
        iLen++;
        CharData++;
        CharCode <<= 0x08;
        CharCode |= (*CharData);
        DisInfo.DispType = DISP_IDCN1516;
        DisInfo.DispLenth++;//����ʾ���ģ�����Ϊ16ʱ����ʾ����
        CharCodeH=(CharCode&0xff00)>>8;
        CharCodeL=CharCode&0x00ff;
      }
      //GB2312_16_GetData(0xa3,0x65+0x80,CharBuf);//CharData[0]
      //GB2312_16_GetData(0xa3,0x42+0x80,CharBuf);
      drv_gt20_data_output(DisInfo.DispType, CharCode, CharBuf);
      //UNICODE_16_GetData(0xff42,CharBuf);
			DISP_DataBuf(DisInfo, CharBuf);
                        
			CharData++;
			iLen++;
		}
	}

	return;
}
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE��ʾ
{
  u8  CharBuf2[32];
  DISP_CHAR DisInfo;
  u8  iLen = 0;//34
  DisInfo = CharInfo;
  if ((CharInfo.DispType & 0x80) != 0x00)
  {
    DisInfo.DispLenth = 0x00;
    for (; (*CharData != 0x00)||(*(CharData+1) != 0x00); DisInfo.DispLenth++)//���*(CharData+1)�жϣ�����0x00Ҳ�����ѭ��������ѭ���ڴ���0x0000
    {
      if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//���޸ĵ���ʾ����Ϊ16ʱ����ʾ���⣩
      CharCode = *CharData;
      if(CharCode!=0x0000)//���0x00��ʾ��ռ����ַ�������
      {
        DisInfo.DispAddX = CharInfo.DispAddX + iLen;
        DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
        if (*CharData >= 0x4E)//Ϊ�����ַ�//UNICODE��Χ��4E00-9FA5
        {
          iLen++;
          CharData++;
          CharCode <<= 0x08;
          CharCode |= (*CharData);
          DisInfo.DispType = DISP_IDCN1516;
          DisInfo.DispLenth++;//����ʾ���ģ�����Ϊ16ʱ����ʾ����
        }
        drv_gt20_data_output2(DisInfo.DispType, CharCode, CharBuf2);
        DISP_DataBuf(DisInfo, CharBuf2);
        iLen++;
      }
      CharData++;
    }
  }
  return;
}
/*******************************************************************************
* Description	: display data process
* Input			: DisInfo : display char location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData)
{
	u16 iPt;
	u8  High, Width, xCol, yPage, nType, bType;
        //DisInfo.DispType = DISP_IDCN1516;//��Ӻ����ֺ���ĸ������ʾһ��
	//bDisDataBufUsed = TRUE;
	yPage = DisInfo.DispAddY;
	nType = (DisInfo.DispType & NROWTYPE);//���֣�0x01&0x10=0 ��ĸ��0x05&0x10=0
	bType = (DisInfo.DispType & BASETYPE);//���֣�0x01&0x0f=1 ��ĸ��0x05&0x0f=5
	// if (DISP_OverBound(DisInfo) != TRUE) return;
	xCol  = (nType != 0x00) ? DisInfo.DispAddX : (DisInfo.DispAddX * 0x08);//һֱȡDisInfo.DispAddX * 0x08
	for (High = 0; High < CHAR_HIGH[0][bType]; High++)//���֣�CHAR_HIGH=2 ��ĸ��CHAR_HIGH=2
	{
		iPt = xCol + yPage * 0x80;//iPtΪ8*8����
		for (Width = 0; Width < CHAR_WIDTH[0][bType]; Width++)//���֣�CHAR_WIDTH=16 ��ĸ��CHAR_WIDTH=8
		{
			if (iPt < 512)//�޸ĵ���ʾ����Ϊ16ʱ����ʾ����
			{
				if (DisDataBuf[iPt] != (*CharData))
				{
					if (iPt == 3*0x80)
					{
						iPt = iPt;
					}
					DisDataBuf[iPt] = *CharData;//[512]
					DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];//[64]
				}
				CharData++;
				iPt++;
			}
		}
		yPage++;
	}
	//bDisDataBufUsed = FALSE;
}

//��ʾ��ͼ��
/*******************************************************************************
* Description   : ico-id display process
* Input		: IcoInfo : ico data location information
		: IcoID : display ico-id
		: IcoDefault : ico display default location
* Output	: void
* Return	: void
* Others	: void
********************************************************************************/
void api_disp_icoid_output(u8 IcoIdIndex, bool IcoDefault,bool on_off)
{
	u16 IcoDataLen;
	ICO_INF	IcoInf;
	DISP_ICO IcoInfo;
	u8 IcoDataBuf[LCD_ICO_BUF_LEN];
        /* �����ֽڴ�һ������ */
        
        IcoDataLen = ((u16)(IcoInf.xLen) * (u16)(IcoInf.yLen)) / 0x08;
        if(IcoDataLen > LCD_ICO_BUF_LEN)
        {
          IcoDataLen = LCD_ICO_BUF_LEN;
        }
        if (on_off == TRUE)
        {
          //api_read_eeprom_data(IcoInf.iAdr, IcoDataLen, &IcoDataBuf[0]);
          memcpy(IcoDataBuf, &Ico_DataBuf[IcoIdIndex][0], LCD_ICO_BUF_LEN);
        }
        else
        {
          memset(&IcoDataBuf[0], 0x00, LCD_ICO_BUF_LEN);
        }
	if (IcoDefault == TRUE)	//ʹ��Ĭ��ͼ��λ��������Ϣ
	{
          api_diap_ico_pos_get(&IcoInfo, IcoIdIndex);
	}
	
	api_disp_ico_output(IcoInfo, &IcoDataBuf[0]);
	//api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���Σ���Ӵ�ָ���ʹ����ʱ��ȡȺ������������
	return;
}

/*******************************************************************************
* Description	: ico data display process
* Input			: IcoInfo : ico display location information
				: *IcoData: display ico data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_ico_output(DISP_ICO IcoInfo, u8 *IcoData)
{
	u16 iPt;
	u8  High, Width, yPage, xAxis;
	u8 *ptr;
        
	yPage = IcoInfo.DispAddY;
	xAxis = IcoInfo.DispAddX * 0x08;
	ptr = IcoData;
	for (High = 0; High < IcoInfo.DispHigh / 0x08; High++)
	{
		iPt = xAxis + yPage * 0x80;
		for (Width = 0; Width < IcoInfo.DispWidth; Width++)
		{
			if (DisDataBuf[iPt] != (*ptr))
			{
				DisDataBuf[iPt] = *ptr;
				//DisDrvObj.Msg.Bit.bRefresh = DISP_RUN;
				DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];
			}
			ptr++;
			iPt++;
		}
		yPage++;
	}
        
}
/*******************************************************************************
* Description	: ico default location display process
* Input			: IcoInfo : ico display location information
				: IcoID : ico-id
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_diap_ico_pos_get(DISP_ICO *pIcoInfo, u16 IcoID)
{
	switch (IcoID)
	{
        case eICO_IDMESSAGE://0���ź�
        case eICO_IDRXFULL://1���ź�
        case eICO_IDRXNULL://2���ź�
        case eICO_IDSCAN://3���ź�
        case eICO_IDSCANPA://4���ź�
        case eICO_IDSPEAKER://5���ź�
          pIcoInfo->DispAddX = 0x00;
          break;
	case eICO_IDEmergency://ͼ�꣺3G
	case eICO_IDPOWERL://ͼ�꣺2G
          pIcoInfo->DispAddX = 0x02;
          break;
        case eICO_IDTemper://����ģʽ
        case eICO_IDMONITER://��Ͳģʽ
        case eICO_IDSCANOff://�յ��ͼ��
          pIcoInfo->DispAddX = 0x04;//0x04
          break;
	case eICO_IDPOWERM://ͼ�꣺���
	case eICO_IDPOWERH://ͼ�꣺����
          pIcoInfo->DispAddX = 0x06;//0x04
          break;
	case eICO_IDBANDWIDTHW://����ͼ��
	case eICO_IDBANDWIDTHN://��ͼ��
          pIcoInfo->DispAddX = 0x0c;
          break;
	case eICO_IDTALKAR://�޷����޽���״̬����ͼ�꣩
        case eICO_IDTX://�����ź�ͼ��
	case eICO_IDVOX://ͼ�꣺���գ���״̬
          pIcoInfo->DispAddX = 0x0a;
          break;
        
	

	case eICO_IDVOXOff://�յ��ͼ��
	
	case eICO_IDSPEAKEROff://�յ��ͼ��
	case eICO_IDMONITEROff://�յ��ͼ��

		
	
	case eICO_IDMESSAGEOff://��ͼ��
        case eICO_IDLOCKED://ѡ
		pIcoInfo->DispAddX = 0x08;
		break;
		
	
	case eICO_IDTALKAROff://�յ��ͼ��
		pIcoInfo->DispAddX = 0x0c;
		break;
		
	case eICO_IDBATT://�յ��ͼ��
	case eICO_IDBATT1:
	case eICO_IDBATT2:
	case eICO_IDBATT3:
	case eICO_IDBATT4:
	case eICO_IDBATT5://--------------------
		pIcoInfo->DispAddX = 0x0e;
		break;
		
	default:
		break;
	}
	
	pIcoInfo->DispAddY = 0x00;
	pIcoInfo->DispWidth= 0x10;
	pIcoInfo->DispHigh = 0x10;
}
//�������ֿ��ж����ݺ���˵�� u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)ʵ�ֲο����롣
/****************************************************
u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
Address  �� ��ʾ�ַ�������оƬ�е��ֽڵ�ַ��
byte_long�� �Ƕ����������ֽ�����
*p_arr   �� �Ǳ�������ĵ������ݵ����顣
*****************************************************/
//u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
	unsigned int j=0;
	MCU_GT20_CS(LO);
	SendByte(address); //����ָ���ַ
	for(j=0;j<byte_long;j++)
	{
	 p_arr[j]=ReadByte();//��ȡ���ݵ�������
	}
	MCU_GT20_CS(HI);
	return p_arr[0];	
}
void SendByte(u32 cmd)
{
	u8 i;
	cmd=cmd|0x03000000;
	for(i=0;i<32;i++)
	{
		MCU_GT20_CLK(LO);
		if(cmd&0x80000000)
			MCU_GT20_SI(HI);
		else 
			MCU_GT20_SI(LO);
		MCU_GT20_CLK(HI);
		cmd=cmd<<1;
	}					
}

u8 ReadByte(void)
{
	u8 i;
	u8 dat=0;
	MCU_GT20_CLK(HI); 	
	for(i=0;i<8;i++)
	{
		MCU_GT20_CLK(LO); 
		dat=dat<<1;
		if(MCU_GT20_SO_Read)
			dat=dat|0x01;
		else 
			dat&=0xfe;
		MCU_GT20_CLK(HI); 	
	}	
	return dat;
        
        

}
