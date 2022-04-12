#include "./internalFlash/bsp_internalFlash.h"   

/*�ʂ䌑��Ĝyԇ����*/
#define DATA_32                 ((uint32_t)0x00000000)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Ҫ�����Ȳ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_8   
/* Ҫ�����Ȳ�FLASH�ĽY����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_12  


 
static uint32_t GetSector(uint32_t Address);


/**
  * @brief  InternalFlash_Test,���Ȳ�FLASH�M���x���yԇ
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void){
	/*Ҫ��������ʼ�ȅ^(����)���Y���ȅ^(������)����8-12����ʾ����8��9��10��11�ȅ^*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
  /* FLASH ���i ********************************/
  /* ʹ���L��FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����Ñ�^�� (�Ñ�^��ָ������]��ʹ�õĿ��g�������Զ��x)**/
  /* ������NFLASH�Ę�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* �_ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С�M�в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE){ 
      /*�������e�����أ����H�����пɼ���̎�� */
			return -1;
    }
    /* Ӌ����ָ����һ���ȅ^ */
    if (uwSectorCounter == FLASH_Sector_11){
      uwSectorCounter += 40;
    }else {
      uwSectorCounter += 8;
    }
  }

  /* �ԡ��֡��Ĵ�С���λ���딵�� ********************************/
  uwAddress = FLASH_USER_START_ADDR;

  while (uwAddress < FLASH_USER_END_ADDR){
    if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE){
      uwAddress = uwAddress + 4;
    }else{ 
      /*������e�����أ����H�����пɼ���̎�� */
			return -1;
    }
  }
	

  /* �oFLASH���i����ֹ���ݱ��۸�*/
  FLASH_Lock(); 


  /* ��FLASH���xȡ�������M��У�***************************************/
  /*  MemoryProgramStatus = 0: ����Ĕ������_
      MemoryProgramStatus != 0: ����Ĕ����e�`����ֵ���e�`�Ă��� */
  uwAddress = FLASH_USER_START_ADDR;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < FLASH_USER_END_ADDR){
    uwData32 = FLASH_M uwAddress;

    if (uwData32 != DATA_32){
      uwMemoryProgramStatus++;  
    }

    uwAddress = uwAddress + 4;
  }  
  /* ����У򞲻���_ */
  if(uwMemoryProgramStatus){    
		return -1;
  }else{ 
		return 0;   
  }
}

/**
  * @brief  ����ݔ��ĵ�ַ�o�������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
  */
static uint32_t GetSector(uint32_t Address){
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)){
    sector = FLASH_Sector_0;  
  }else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)){
    sector = FLASH_Sector_1;  
  }else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)){
    sector = FLASH_Sector_2;  
  }else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)){
    sector = FLASH_Sector_3;  
  }else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)){
    sector = FLASH_Sector_4;  
  }else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)){
    sector = FLASH_Sector_5;  
  }else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)){
    sector = FLASH_Sector_6;  
  }else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7)){
    sector = FLASH_Sector_7;  
  }else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8)){
    sector = FLASH_Sector_8;  
  }else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9)){
    sector = FLASH_Sector_9;  
  }else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)){
    sector = FLASH_Sector_10;  
  }else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11)){
    sector = FLASH_Sector_11;  
  }else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12)){
    sector = FLASH_Sector_12;  
  }else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13)){
    sector = FLASH_Sector_13;  
  }else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14)){
    sector = FLASH_Sector_14;  
  }else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15)){
    sector = FLASH_Sector_15;  
  }else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16)){
    sector = FLASH_Sector_16;  
  }else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17)){
    sector = FLASH_Sector_17;  
  }else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18)){
    sector = FLASH_Sector_18;  
  }else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19)){
    sector = FLASH_Sector_19;  
  }else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20)){
    sector = FLASH_Sector_20;  
  }else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21)){
    sector = FLASH_Sector_21;  
  }else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22)){
    sector = FLASH_Sector_22;  
  }else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/{
    sector = FLASH_Sector_23;  
  }
  return sector;
}



