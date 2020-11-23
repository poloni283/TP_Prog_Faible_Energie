#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
void Systick_Init_Exp1(void);
void Systick_Init_24MHz(void);
static void RTC_wakeup_init( int delay );
void RTC_wakeup_init_from_stop( int delay );
void Config_Experience_1(void);
void RTC_wakeup_init_from_standby_or_shutdown( int delay );
void RTC_WKUP_IRQHandler(void);


//expe = 0;
Interruption_Systick = 0;


int main(void)
{

	// Reset of all peripherals, Initializes the Flash interface and the Systick.
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    SystemClock_Config();
    MX_GPIO_Init();
	//MX_RTC_Init();

  if(LL_RCC_LSE_IsReady() != 0)//démarrage à chaud
  {
	  MX_RTC_Init();

	  	//Configuration RTC
	  	LL_RTC_DisableWriteProtection (RTC);
	  	LL_RTC_EnableInitMode(RTC);

	  	//initialiser l'interface RTC-MPU
	  	LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );
	  	LL_PWR_EnableBkUpAccess();


	  	LL_RTC_EnableWriteProtection(RTC);

	  //if (LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_13) == 0)
	  //{//Si on appuie sur le bouton bleu
		  if (expe==8)
		  {
			expe=1;
		  }
		  else
		  {
			expe = LL_RTC_BAK_GetRegister(RTC,LL_RTC_BKP_DR0); //Incrémentation de expe
			expe = expe +1;
		  }
		  LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0,expe);
	  //}
	  //else
	  //{
	   // expe = LL_RTC_BAK_GetRegister(RTC,LL_RTC_BKP_DR0);
	  //}
	  //LL_GPIO_SetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);
  }

  if(LL_RCC_LSE_IsReady() == 0)//démarrage à froid
  {
	  MX_RTC_Init();
	//Configuration du LSE
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();
	LL_RCC_ReleaseBackupDomainReset();
	LL_RCC_LSE_Enable();

	//Configuration RTC
	LL_RTC_DisableWriteProtection (RTC);
	LL_RTC_EnableInitMode(RTC);

	//initialiser l'interface RTC-MPU
	LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );
	LL_PWR_EnableBkUpAccess();

	LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0,expe);

	LL_RTC_EnableWriteProtection(RTC);
	while(LL_RCC_LSE_IsReady() != 1){} //attendre l'initialisation du LSE

	expe = 0;
    LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0,expe);
  }

  blue_mode = 0;
  int endormi = 0;
  int periode_LED = 0;

  if (expe == 1)
  {
	  Config_Experience_1();
	  Systick_Init_Exp1();
  }
  if(expe == 2)
  {
	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1){}
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	  Systick_Init_24MHz();
  }

  if(expe == 3)
  {
	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
	  Systick_Init_24MHz();
  }

  if(expe == 4)
  {

	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
	  Systick_Init_24MHz();
  }

  if(expe == 5)
	{

	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

	  LL_RTC_WAKEUP_Enable(RTC); // RTC wake up init
	  Systick_Init_24MHz();
	}

  if(expe == 6)
	{

	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

	  LL_RTC_WAKEUP_Enable(RTC); // RTC wake up init
	  Systick_Init_24MHz();
	}

	if(expe == 7)
	{
		LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
		while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

		LL_RTC_WAKEUP_Enable(RTC); // RTC wake up init
		Systick_Init_24MHz();
	}

	if(expe == 8)
	{
		LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
		while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3){}
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

		LL_RTC_WAKEUP_Enable(RTC); // RTC wake up init
		Systick_Init_24MHz();
	}

  while(1)
  {
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_13) == 0)
	{
	  blue_mode = 1;
	  endormi=1;
	}

	if (expe == 1)
	{
		if(Interruption_Systick == 1)
		{
			periode_LED = periode_LED + 1;

			  //Période de la LED : 2s, durée d'impulsion : expe * 50 ms
			  if(periode_LED <= 5*expe)
			  {
				  LL_GPIO_SetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);
			  }
			  else if(periode_LED > 5*expe && periode_LED < 20)
			  {
				  LL_GPIO_ResetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);
			  }
			  else
			  {
				  periode_LED = 0;
			  }

			  Interruption_Systick = 0;
		}
		if(blue_mode == 1)
		{
		  LL_LPM_EnableSleep();
		  __WFI();
		}
	  }

	  if(expe == 2)
	  {
		  if(Interruption_Systick == 1)
		  {
			  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
			  Interruption_Systick = 0;
	 	  }

	 	  if(blue_mode == 1)
	 	  {
	 		  LL_RCC_MSI_EnablePLLMode();
	 	  }
	  }

	  if(expe == 3)
	  {
		  //LL_GPIO_SetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);

		  if(Interruption_Systick == 1)
		  {
			  periode_LED = periode_LED + 1;

			  //Période de la LED : 2s, durée d'impulsion : expe * 50 ms
			  if(periode_LED <= 5*expe)
			  {
				  LL_GPIO_SetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);
			  }
			  else if(periode_LED > 5*expe && periode_LED < 200)
			  {
				  LL_GPIO_ResetOutputPin(LED_Verte_GPIO_Port,LED_Verte_Pin);
			  }
			  else
			  {
				  periode_LED = 0;
			  }

			  Interruption_Systick = 0;
	 	  }

	 	  if(blue_mode == 1)
	 	  {
	 		  LL_LPM_EnableSleep();
	 		  __WFI();

	 		  //LL_GPIO_ResetOutputPin(LED_Verte_GPIO_Port, LED_Verte_Pin);
	 	  }
	  }

	  if(expe == 4)
	  {

		if(Interruption_Systick == 1)
		{
		  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
		  Interruption_Systick = 0;
		}

		if(blue_mode == 1)
		{
		  LL_RCC_MSI_EnablePLLMode();
		}
	  }

	  if (expe == 5)
	  {
		if(Interruption_Systick == 1)
		{
		  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
		  Interruption_Systick = 0;
		}

		if((blue_mode == 1) && (endormi == 1))
		{
			endormi = 0;
			LL_PWR_SetPowerMode(LL_PWR_MODE_STOP0);//Set Power Mode (stop0)
			RTC_wakeup_init_from_stop(20);
			LL_LPM_EnableDeepSleep(); // Enable deep sleep
			__WFI();// <3 s'arrête et le µC reprends les instructions */
		}
	  }
	  if (expe == 6)
	  {
		if(Interruption_Systick == 1)
		{
		  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
		  Interruption_Systick = 0;
		}

		if((blue_mode == 1) && (endormi == 1))
		{
			endormi = 0;
			LL_PWR_SetPowerMode(LL_PWR_MODE_STOP1);//Set Power Mode (stop0)
			RTC_wakeup_init_from_stop(20);
			LL_LPM_EnableDeepSleep(); // Enable deep sleep
			__WFI();// <3 s'arrête et le µC reprends les instructions */
		}
	  }
	  if (expe == 7)
	  {
		if(Interruption_Systick == 1)
		{
		  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
		  Interruption_Systick = 0;
		}

		if((blue_mode == 1) && (endormi == 1))
		{
			endormi = 0;
			LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);//Set Power Mode (stop0)
			RTC_wakeup_init_from_stop(20);
			LL_LPM_EnableDeepSleep(); // Enable deep sleep
			__WFI();// <3 s'arrête et le µC reprends les instructions */

		}
	  }

	  if (expe == 8)
	  {
		if(Interruption_Systick == 1)
		{
		  LL_GPIO_TogglePin(Derive_GPIO_Port,Derive_Pin); //Si on ne change pas la période d'interruption du Systick c'est bon
		  Interruption_Systick = 0;
		}

		if((blue_mode == 1) && (endormi == 1))
		{
			endormi = 0;
			LL_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);//Set Power Mode (stop0)
			RTC_wakeup_init_from_standby_or_shutdown(10);
			LL_LPM_EnableDeepSleep(); // Enable deep sleep
			__WFI();// <3 s'arrête et le µC reprends les instructions */

		}
	  }
  }
}

void Config_Experience_1(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
	while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_MSI_Enable();

	 /* Wait till MSI is ready */
	while(LL_RCC_MSI_IsReady() != 1)
	{

	}
	LL_RCC_MSI_EnableRangeSelection();
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
	LL_RCC_MSI_SetCalibTrimming(0);
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
	LL_RCC_PLL_EnableDomain_SYS();
	LL_RCC_PLL_Enable();

	 /* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1)
	{

	}
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	 /* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{

	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(80000000);

	LL_SetSystemCoreClock(80000000);
}

// wakeup timer interrupt Handler (inutile mais doit etre defini)
void RTC_WKUP_IRQHandler()
{
	LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_20 );
}

void Systick_Init_Exp1(void)
{
	//Configuration du Systick pour avoir des interruptions toutes les 10ms
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	LL_SYSTICK_EnableIT();
	SysTick->LOAD=SystemCoreClock/10;
	SysTick->VAL=0;
	NVIC_SetPriority(SysTick_IRQn,7);
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
}


void Systick_Init_24MHz(void)
{
	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

	  //Configuration du Systick pour avoir des interruptions toutes les 10ms
	  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	  LL_SYSTICK_EnableIT();
	  SysTick->LOAD=240000-1;
	  SysTick->VAL=0;
	  NVIC_SetPriority(SysTick_IRQn,7);
	  SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
}

// partie commune a toutes les utilisations du wakeup timer
static void RTC_wakeup_init( int delay )
{
	LL_RTC_DisableWriteProtection( RTC );
	LL_RTC_WAKEUP_Disable( RTC );
	while	( !LL_RTC_IsActiveFlag_WUTW( RTC ) )
		{ }
	// connecter le timer a l'horloge 1Hz de la RTC
	LL_RTC_WAKEUP_SetClock( RTC, LL_RTC_WAKEUPCLOCK_CKSPRE );
	// fixer la duree de temporisation
	LL_RTC_WAKEUP_SetAutoReload( RTC, delay );	// 16 bits
	LL_RTC_ClearFlag_WUT(RTC);
	LL_RTC_EnableIT_WUT(RTC);
	LL_RTC_WAKEUP_Enable(RTC);
	LL_RTC_EnableWriteProtection(RTC);
}

void RTC_wakeup_init_from_stop( int delay )
{
	RTC_wakeup_init( delay );
	// valider l'interrupt par la ligne 20 du module EXTI, qui est réservée au wakeup timer
	LL_EXTI_EnableIT_0_31( LL_EXTI_LINE_20 );
	LL_EXTI_EnableRisingTrig_0_31( LL_EXTI_LINE_20 );
	// valider l'interrupt chez NVIC
	NVIC_SetPriority( RTC_WKUP_IRQn, 1 );
	NVIC_EnableIRQ( RTC_WKUP_IRQn );
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
  {
  }
  /*LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while(LL_RCC_LSI_IsReady() != 1)
    {

    }*/

  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */

  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(24000000);

  LL_SetSystemCoreClock(24000000);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/

  /**/
  LL_GPIO_ResetOutputPin(Derive_GPIO_Port, Derive_Pin);

  /**/
  GPIO_InitStruct.Pin = Bouton_bleu_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Bouton_bleu_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_Verte_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_Verte_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Derive_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Derive_GPIO_Port, &GPIO_InitStruct);

}

// Dans le cas des modes STANDBY et SHUTDOWN, le MPU sera reveille par reset
// causé par 1 wakeup line (interne ou externe) (le NVIC n'est plus alimenté)
void RTC_wakeup_init_from_standby_or_shutdown( int delay )
{
RTC_wakeup_init( delay );
// enable the Internal Wake-up line
LL_PWR_EnableInternWU();	// ceci ne concerne que Standby et Shutdown, pas STOPx
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
