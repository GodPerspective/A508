#include "Flash_eeprom.h"
  u32 AdrSumTest=0x00;
/*******************************************************************************
          ÿ���ͺŵ�EEPROM�Ĵ�С��һ�������ô˺�����ʱ��Ҫע�⽫Ҫд�����ֽ�����
         �Ĵ�С�Ƿ񳬹����ͺŵ�EEPROM�ĵ�ַ��
         ��������EEPROM���ͺ���STM8S208, STM8S207, STM8S007, STM8AF52Ax, STM8AF62Ax 
         EEPROM�ĵ�ַ�Ǵ�0x004000��0x0047ff����2048Byte,ÿ��Block��128Byte,һ��16��Block.
         ��������EEPROM���ͺ���STM8S105, STM8S005, STM8AF626x
         EEPROM�ĵ�ַ�Ǵ�0x004000��0x0043ff����1024Byte,ÿ��Block��128Byte,һ��8��Block.
         С������EEPROM���ͺ���STM8S103, STM8S003, STM8S903 
         EEPROM�ĵ�ַ�Ǵ�0x004000��0x00427f����1024Byte,ÿ��Block��64Byte,һ��10��Block.

 ******************************************************************************/
void WriteMultiBlockByte(uint8_t block_count,FLASH_MemType_TypeDef FLASH_MemType, 
                FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer,uint8_t BlockNum)
{
  uint8_t  BlockNum_Temp;
  /* ���� flash data eeprom memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  /* �ȴ� Data EEPROM area ������־λ��λ*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
        ;
  for(BlockNum_Temp=block_count;BlockNum_Temp<BlockNum;BlockNum_Temp++)
  {
    if(BlockNum_Temp>FLASH_DATA_BLOCKS_NUMBER)
      break;
    FLASH_ProgramBlock(BlockNum_Temp, FLASH_MemType, FLASH_ProgMode,Buffer+BlockNum_Temp*FLASH_BLOCK_SIZE);
    FLASH_WaitForLastOperation(FLASH_MemType);
  }
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*������Ҫ����*/

}

void Flash_WriteDataBlock(uint8_t block_count, uint8_t *Buffer)
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);//���Բ�дEEPROM��Flash:FLASH_Unlock(FLASH_MemType_Program);
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
    {}
    FLASH_ProgramBlock(block_count-1, FLASH_MEMTYPE_DATA, FLASH_PROGRAMMODE_STANDARD, Buffer);

    while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
    {}
    //FLASH_WaitForLastOperation();
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void ReadMultiBlockByte(uint8_t BlockNum,uint8_t ReadBlockByte[])
{
  uint32_t add, start_add, stop_add;
  start_add = FLASH_DATA_START_PHYSICAL_ADDRESS+(u32)((BlockNum-1)*FLASH_BLOCK_SIZE);
  stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + (u32)(BlockNum*FLASH_BLOCK_SIZE);
 
  for (add = start_add; add < stop_add; add++)
      ReadBlockByte[add-FLASH_DATA_START_PHYSICAL_ADDRESS]=FLASH_ReadByte(add);
 
  
}
void WriteEEPROMByte(uint32_t Address, uint8_t Data)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);//���Բ�дEEPROM��Flash:FLASH_Unlock(FLASH_MemType_Program);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);//�ȴ������ɹ�
  FLASH_ProgramByte(Address,Data);//��ָ����ַд������Data
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);//д�����
  FLASH_Lock(FLASH_MEMTYPE_DATA);//����
}


bool FILE_Write(u16 iAdr,u16 iLen,u8 *pBuf)//���pBuf���ַ���,�����һλΪ\0
{
  u16 EEPROM_WriteCount;
  u32 AdrSum=0x00;
  for(EEPROM_WriteCount=0x01;EEPROM_WriteCount<iLen+1;EEPROM_WriteCount++)
  {
    AdrSum=0x4000+iAdr+EEPROM_WriteCount-1;
    WriteEEPROMByte(AdrSum, pBuf[EEPROM_WriteCount]);
  }
  return TRUE;
}
bool FILE_Write2(u16 iAdr,u16 iLen,u8 *pBuf)//�粻ȷ���޸��Ƿ����˺Ŷ�д��Ӱ�죬ֱ���½�һ������
{
  u16 EEPROM_WriteCount;
  u32 AdrSum=0x00;
  for(EEPROM_WriteCount=0x01;EEPROM_WriteCount<iLen+1;EEPROM_WriteCount++)
  {
    AdrSum=0x4000+iAdr+EEPROM_WriteCount-1;
    WriteEEPROMByte(AdrSum, pBuf[EEPROM_WriteCount-1]);
  }
  return TRUE;
}
bool FILE_Read(u16 iAdr,u16 iLen,u8 *pBuf)//���pBuf���ַ���,�����һλΪ\0
{
  u16 i;
  u32 AdrSum=0x00;
  for(i=0x00;i<iLen;i++)
  {
    AdrSum=0x4000+iAdr+i;
    pBuf[i]=FLASH_ReadByte(AdrSum);
  }
  return TRUE;
}
bool FILE_Read2(u16 iAdr,u16 iLen,u8 *pBuf)//�粻ȷ���޸��Ƿ����˺Ŷ�д��Ӱ�죬ֱ���½�һ������
{
  u16 i;
  u32 AdrSum=0x00;
  for(i=0x00;i<iLen;i++)
  {
    AdrSum=iAdr+i;
    pBuf[i]=FLASH_ReadByte(AdrSum);
  }
  return TRUE;
}