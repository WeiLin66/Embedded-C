#include "./internalFlash/bsp_internalFlash.h"   

/*準備寫入的測試數據*/
#define DATA_32                 ((uint32_t)0x00000000)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除內部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_8   
/* 要擦除內部FLASH的結束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_12  


 
static uint32_t GetSector(uint32_t Address);


/**
  * @brief  InternalFlash_Test,對內部FLASH進行讀寫測試
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void){
	/*要擦除的起始扇區(包含)及結束扇區(不包含)，如8-12，表示擦除8、9、10、11扇區*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
  /* FLASH 解鎖 ********************************/
  /* 使能訪問FLASH控制寄存器 */
  FLASH_Unlock();
    
  /* 擦除用戶區域 (用戶區域指程序本身沒有使用的空間，可以自定義)**/
  /* 清除各種FLASH的標志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* 開始擦除操作 */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) {
    /* VoltageRange_3 以“字”的大小進行操作 */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE){ 
      /*擦除出錯，返回，實際應用中可加入處理 */
			return -1;
    }
    /* 計數器指向下一個扇區 */
    if (uwSectorCounter == FLASH_Sector_11){
      uwSectorCounter += 40;
    }else {
      uwSectorCounter += 8;
    }
  }

  /* 以“字”的大小為單位寫入數據 ********************************/
  uwAddress = FLASH_USER_START_ADDR;

  while (uwAddress < FLASH_USER_END_ADDR){
    if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE){
      uwAddress = uwAddress + 4;
    }else{ 
      /*寫入出錯，返回，實際應用中可加入處理 */
			return -1;
    }
  }
	

  /* 給FLASH上鎖，防止內容被篡改*/
  FLASH_Lock(); 


  /* 從FLASH中讀取出數據進行校驗***************************************/
  /*  MemoryProgramStatus = 0: 寫入的數據正確
      MemoryProgramStatus != 0: 寫入的數據錯誤，其值為錯誤的個數 */
  uwAddress = FLASH_USER_START_ADDR;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < FLASH_USER_END_ADDR){
    uwData32 = FLASH_M uwAddress;

    if (uwData32 != DATA_32){
      uwMemoryProgramStatus++;  
    }

    uwAddress = uwAddress + 4;
  }  
  /* 數據校驗不正確 */
  if(uwMemoryProgramStatus){    
		return -1;
  }else{ 
		return 0;   
  }
}

/**
  * @brief  根據輸入的地址給出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
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



