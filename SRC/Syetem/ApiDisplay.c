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

static DIS_DRV DisDrvObj;

static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData);
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData);

/* ------------------------------------------------------------------------------- */
/* ���д���ϴ���ʱ,���º�����δ���壬����ʱ���壨���ڱ���ͨ���������������±�д   */
/* ------------------------------------------------------------------------------- */
const u8 BitTab[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


u8 DisDataBit[64]  = {0};
u8 DisDataBuf[512] = {0};

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

void api_lcd_pwr_on_hint(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 0;//һ��16��Ӣ���ַ�
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	MCU_LCD_BACKLIGTH(ON);
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
	u16 CharCode;
	DISP_CHAR DisInfo;
	u8  iLen = 0, CharBuf[34];

	DisInfo = CharInfo;
	if ((CharInfo.DispType & 0x80) != 0x00)
	{
		DisInfo.DispLenth = 0x00;
		for (; *CharData != 0x00; DisInfo.DispLenth++)
		{
			if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//�޸ĵ���ʾ����Ϊ16ʱ����ʾ����
			CharCode = *CharData;
			DisInfo.DispAddX = CharInfo.DispAddX + iLen;
			DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
			
			if (*CharData >= 0x80)
			{
				iLen++;
				CharData++;
				CharCode <<= 0x08;
				CharCode |= (*CharData);
				DisInfo.DispType = DISP_IDCN1516;
				DisInfo.DispLenth++;//����ʾ���ģ�����Ϊ16ʱ����ʾ����
			}
#ifdef FONT_DRIVER_IC_EN			
			drv_gt20_data_output(DisInfo.DispType, CharCode, CharBuf);
#else
			//api_SPI_Flash_Wake_up();
			//drv_Font_GetData(DisInfo.DispType, CharCode, CharBuf);
			//api_SPI_Flash_power_down();
#endif
			DISP_DataBuf(DisInfo, CharBuf);
			CharData++;
			iLen++;
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

	//bDisDataBufUsed = TRUE;
	yPage = DisInfo.DispAddY;
	nType = (DisInfo.DispType & NROWTYPE);
	bType = (DisInfo.DispType & BASETYPE);
	// if (DISP_OverBound(DisInfo) != TRUE) return;
	xCol  = (nType != 0x00) ? DisInfo.DispAddX : (DisInfo.DispAddX * 0x08);
	for (High = 0; High < CHAR_HIGH[0][bType]; High++)
	{
		iPt = xCol + yPage * 0x80;
		for (Width = 0; Width < CHAR_WIDTH[0][bType]; Width++)
		{
			if (iPt < 512)//�޸ĵ���ʾ����Ϊ16ʱ����ʾ����
			{
				if (DisDataBuf[iPt] != (*CharData))
				{
					if (iPt == 3*0x80)
					{
						iPt = iPt;
					}
					DisDataBuf[iPt] = *CharData;
					DisDrvObj.Msg.Bit.bRefresh = DISP_RUN;
					DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];
				}
				CharData++;
				iPt++;
			}
		}
		yPage++;
	}
	//bDisDataBufUsed = FALSE;
}



