/*
________________________________________________________________________________________
	Clock configuration files for stm32f103
	stm_clock_config.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. stm_clock
	RELPATH is the relative path from your projetc directory to lib_cm3 directory	
	
   	include RELPATH/lib_cm3/???.c file in your makefile
	copy    RELPATH/lib_cm3/???_config_TEMPLATE.h in your project directory	(noted ./)
	rename  ./???_config_TEMPLATE.h as ./???_config.h in your project directory
	edit    ./???_config.h file for your project (lines with //CONF tags)  
	ensure that ./ path is visible in CC compiler options 
	add "#include "RELPATH/lib_cm3/???.h" in app. code 
	add a call to "Init_???();" at initialisation step of your APP
________________________________________________________________________________________
REVS
	[Acco 06/01/2010] finalisation et commentaires de la première version
		Testée en réel et simulé (utilisation de PWM et USART et CAN)		
________________________________________________________________________________________
TODO
	test _SYSCLK_SOURCE IS_HSI and IS_HSE
	+ Peripheral clock enable  functions
	+ USBCLK, MCO, CSS fonctionalities
	+ RTCCLK IWDGCLK functionalities
	+ TIM1 and TIM2  
 	+ stm32_GetPCLK2() et autres fonctions pour assurer 
	  la compatibilité avec stm32f10x lib
________________________________________________________________________________________
*/

#include "stm_regs.h"
#include "stm_clock.h"

#define __RCC_CR_RESET    	0x00000083
#define __RCC_CFGR_RESET	0x00000000
//#define __RCC_CIR_VALUE		(HSERDYIE | PLLRDYIE)  //TODO DEBUG car polling ! 0x00001800
#define __RCC_CIR_RESET		0x00000000

//_______________________________
//determine what should be used
#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSE) )
	#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(__PLLMULL)| 	\
	                            _PLLXTPRE| 						\
								RCC_PLLSRC_IS_HSE| 				\
								RCC_ADCPRE_VAL(_ADCPRE)| 		\
								RCC_SW_IS_HSI| 					\
								RCC_PPRE1_VAL(_PPRE1)| 			\
								RCC_PPRE2_VAL(_PPRE2)| 			\
								RCC_HPRE_VAL(_HPRE))
	#define HSE_IS_USED
	#define PLL_IS_USED
#elif ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSI) )
	#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(__PLLMULL)| 	\
	                            _PLLXTPRE| 						\
								RCC_PLLSRC_IS_HSI| 				\
								RCC_ADCPRE_VAL(_ADCPRE)| 		\
								RCC_SW_IS_HSI| 					\
								RCC_PPRE1_VAL(_PPRE1)| 			\
								RCC_PPRE2_VAL(_PPRE2)| 			\
								RCC_HPRE_VAL(_HPRE))
	#define HSI_IS_USED
	#define PLL_IS_USED
#elif (_SYSCLK_SOURCE  == IS_HSI)
	#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 		\
								RCC_SW_IS_HSI| 					\
								RCC_PPRE1_VAL(_PPRE1)| 			\
								RCC_PPRE2_VAL(_PPRE2)| 			\
								RCC_HPRE_VAL(_HPRE))
	#define HSI_IS_USED
#elif (_SYSCLK_SOURCE  == IS_HSE)
	#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 		\
								RCC_SW_IS_HSI| 					\
								RCC_PPRE1_VAL(_PPRE1)| 			\
								RCC_PPRE2_VAL(_PPRE2)| 			\
								RCC_HPRE_VAL(_HPRE))
	#define HSE_IS_USED
#endif

//____________________________
void Init_Clock_System()
{ 
	RCC->CR		=__RCC_CR_RESET;
	RCC->CFGR	=__RCC_CFGR_RESET;
	RCC->CIR	=__RCC_CIR_RESET;

	RCC->CFGR 	=__RCC_CFGR_VALUE;
	
	/* Reglage de la flash (waitstates) */
	FLASH->ACR = 0x0; /* reset du controleur de flash -> 0 WS, pas de buffer de prefetch  */

	#if (__SYSCLK <=24000000UL)
		FLASH->ACR |= FLASH_LATENCY_0_WS;
	#elif (__SYSCLK <=48000000UL) 
		FLASH->ACR |= FLASH_LATENCY_1_WS;
	#else
		FLASH->ACR |= FLASH_LATENCY_2_WS;
	#endif 	 
	 
	#ifdef HSE_IS_USED
		//activate HSE
		RCC->CR |= RCC_HSEON;
		//wait for HSE to be ready
	  	while((RCC->CR & RCC_HSERDY) == 0);
	
		#ifndef PLL_IS_USED
		// switch clock to HSE
		RCC->CFGR |= RCC_SW_IS_HSE;
		#endif
	#endif  

	#ifdef HSI_IS_USED
		//activate HSI
		RCC->CR |= RCC_HSION;
		//wait for HSI to be ready
	  	while((RCC->CR & RCC_HSIRDY) == 0);

		#ifndef PLL_IS_USED
		// switch clock to HSI
		RCC->CFGR |= RCC_SW_IS_HSI;
		#endif
	#endif  

	#ifdef PLL_IS_USED
		//activate PLL
		RCC->CR |= RCC_PLLON;
		//wait for PLL to be ready
		while((RCC->CR & RCC_PLLRDY) == 0);

		// switch clock to PLL
		RCC->CFGR |= RCC_SW_IS_PLL;
	#endif 
	
	FLASH->ACR |= FLASH_PRFTBE; /* Buffer de prefetch activé */
	
	#if ((_HPRE == 1) && (__SYSCLK <8000000UL))
		FLASH->ACR |= FLASH_HLFCYA; /* Half cycle flash access (possible seulement si sysclk < 8Mhz 
		                               et HCLK == SYSCLK*/	
	#endif	 	 
}

//__________________________________________
//ensure compatibility with STM32f10x lib
unsigned int stm32_GetPCLK1 (void) __attribute__ ((weak));
unsigned int stm32_GetPCLK1 (void) 
{
	return (unsigned int) (__PCLK1) ;
}

