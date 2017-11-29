#include "AllHead.h"

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
void WriteEEPROMByte(uint32_t Address, uint8_t Data)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);//���Բ�дEEPROM��Flash:FLASH_Unlock(FLASH_MemType_Program);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);//�ȴ������ɹ�
  FLASH_ProgramByte(Address,Data);//��ָ����ַд������Data
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);//д�����
  FLASH_Lock(FLASH_MEMTYPE_DATA);//����
}