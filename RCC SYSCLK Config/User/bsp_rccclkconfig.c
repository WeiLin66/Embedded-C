#include "bsp_rccclkconfig.h"

#if (RCC_TEST)
static void SetSysClock(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	/* Check if the HSE is ready or not */
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    
    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
		
		/* Wait till the CSR_ODRDY is ready */
    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
    {
    }
		
    PWR->CR |= PWR_CR_ODSWEN;
		
		/* Wait till the CSR_ODSW is ready */		
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
    {
    } 
		
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}

#endif /*RCC_TEST*/


/**
 * vco range: 192~432Mhz
 * Re-set system clock by myself, the maximun frequency is 216Mhz
 * @param m Division factor
 * @param n Multiplication factor
 * @param p Division factor for main system clock
 * @param q Division factor for USB OTG FS, SDIO, etc.
 * @retval None
 */
void HSE_SetSysCLK(uint32_t m, uint32_t n, uint32_t p, uint32_t q){
		RCC_DeInit(); // RCC reset to default
		RCC_HSEConfig(SET_ON);
	
		while(!RCC_WaitForHSEStartUp()){
			My_Delay(TIMES);
			RCC_HSEConfig(SET_ON);
		}
			
		RCC_PLLConfig(HSE,m,n,p,q);
		RCC_HCLKConfig(AHB_CFG); // AHB
		RCC_PCLK2Config(APB2_CFG); // APB2
		RCC_PCLK1Config(APB1_CFG); // APB1
			
		RCC_PLLCmd(ENABLE);
		while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY)){}	
		RCC_SYSCLKConfig(PLLCLK);
}
