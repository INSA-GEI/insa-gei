/*
________________________________________________________________________________________
	Cortex M3 Fault Handling
	cm3_traps.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. cm3_traps
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
_______________________________________________________________________________________
TODO
	declare proper OLYMEX MASK
	check LED_PORT conf with warnings
	add Init_Traps to ensure Exception is enable 
	And GPIO LED clock is enabled
________________________________________________________________________________________
*/

#include <stm32f10x_lib.h>
#include "cm3_traps.h"
#include <stdio.h>

#define CORE_DUMP
#ifdef CORE_DUMP

#define SCB_BASE_ADDR	0xE000ED00
#define CFSR_REG_ADDR 	0xE000ED28
#define HFSR_REG_ADDR	0xE000ED2C
#define MMFAR_REG_ADDR	0xE000ED34
#define BFAR_REG_ADDR	0xE000ED38

u32 CFSR_REG;
u32 HFSR_REG;
u32 MMFAR_REG;
u32 BFAR_REG;
u32 *ptr;

u32 R0_REG,R1_REG,R2_REG,R3_REG,R12_REG,SP_REG,LR_REG,RETURN_ADDR_REG,XPSR_REG;


#endif /* CORE_DUMP */
/*--------------------------Blink leds function --------------------*/

#ifdef USE_BLINK_LEDS

void Blink_Leds(int repet, int duree)
{
	volatile unsigned int i,j;
	volatile int dummy;

	for(j=repet;j>0;j--)
 	{ 
	   	LED_PORT->ODR &=~(LED_MASK);	  //LED ON

	  	for (i=duree;i>0;i--)
	 	{
			dummy++;
		}

	  	LED_PORT->ODR |= (LED_MASK); 		   //LED  OFF

	  	for (i=duree;i>0;i--)
	 	{
			dummy--;
		}
 	}
}
#endif 

/*---------------------- Hardware Fault Handle ------------------*/

#ifdef HANDLE_HARDWARE_FAULT

void HardFault_Handler(void)
{
 	#ifdef CORE_DUMP

		asm ("movw	r5, #:lower16:R0_REG;\n\t"
			 "movt	r5, #:upper16:R0_REG;\n\t"
			 "str 	r0 , [r5, #0];\n\t" /* Sauvegarde R0 dans R0_REG */
			 "movw	r5, #:lower16:R1_REG;\n\t"
			 "movt	r5, #:upper16:R1_REG;\n\t"
			 "str 	r1 , [r5, #0];\n\t"/* Sauvegarde R1 dans R1_REG */
			 "movw	r5, #:lower16:R2_REG;\n\t"
			 "movt	r5, #:upper16:R2_REG;\n\t"
			 "str 	r2 , [r5, #0];\n\t" /* Sauvegarde R2 dans R2_REG */
			 "movw	r5, #:lower16:R3_REG;\n\t"
			 "movt	r5, #:upper16:R3_REG;\n\t"
			 "str 	r3 , [r5, #0];\n\t"/* Sauvegarde R3 dans R3_REG */
			 "movw	r5, #:lower16:R12_REG;\n\t"
			 "movt	r5, #:upper16:R12_REG;\n\t"
			 "str 	r12 , [r5, #0];\n\t" /* Sauvegarde R12 dans R12_REG */);

		asm ("ldr	r0,[sp,#17*4];\n\t" /* Charge dans R0 la valeur de LR poussé sur la stack */
			 "movw	r5, #:lower16:LR_REG;\n\t"
			 "movt	r5, #:upper16:LR_REG;\n\t"
			 "str	r0,[r5,#0];\n\t"
			 "ldr	r0,[sp,#18*4];\n\t" /* Charge dans R0 la valeur de l'adresse fautive poussé sur la stack */
			 "movw	r5, #:lower16:RETURN_ADDR_REG;\n\t"
			 "movt	r5, #:upper16:RETURN_ADDR_REG;\n\t"
			 "str	r0,[r5,#0];\n\t"
			 "ldr	r0,[sp,#19*4];\n\t" /* Charge dans R0 la valeur de PSR poussé sur la stack */
			 "movw	r5, #:lower16:XPSR_REG;\n\t"
			 "movt	r5, #:upper16:XPSR_REG;\n\t"
			 "str	r0,[r5,#0];\n\t"
		    );

		ptr = (u32*)CFSR_REG_ADDR;
		CFSR_REG = *ptr;
		ptr = (u32*)HFSR_REG_ADDR;
		HFSR_REG = *ptr;
		ptr = (u32*)MMFAR_REG_ADDR;
		MMFAR_REG = *ptr;
		ptr = (u32*)BFAR_REG_ADDR;
		BFAR_REG = *ptr;

		printf ("\n\nHardFault Core Dump\n\n");
		printf ("R0=%08X\nR1=%08X\nR2=%08X\nR3=%08X\nR12=%08X\nLR=%08X\nFaultAddr=%08X\nPSR=%08X\n\n",
		         (unsigned int)R0_REG, 
				 (unsigned int)R1_REG,
				 (unsigned int)R2_REG, 
				 (unsigned int)R3_REG,
				 (unsigned int)R12_REG, 
				 (unsigned int)LR_REG,
				 (unsigned int)RETURN_ADDR_REG, 
				 (unsigned int)XPSR_REG);

		printf ("CFSR=%08X\nHFSR=%08X\nMMFAR=%08X\nBFAR=%08X\n",
		         (unsigned int)CFSR_REG,
				 (unsigned int)HFSR_REG,
				 (unsigned int)MMFAR_REG,
				 (unsigned int)BFAR_REG);

	#endif /* CORE_DUMP */

 	#ifdef USER_HARDWARE_FAULT_HANDLER
		 HARDWARE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(4,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 /*---------------------- Memmanage Fault Handle ------------------*/

 #ifdef HANDLE_MEMMANAGE_FAULT

 void MemManage_Handler(void)
 {
 	#ifdef USER_MEMMANAGE_FAULT_HANDLER
		 MEMMANAGE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(3,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 /*---------------------- Usage Fault Handle ------------------*/

 #ifdef HANDLE_USAGE_FAULT

 void UsageFault_Handler(void)
 {
 	#ifdef USER_USAGE_FAULT_HANDLER
		 USAGE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(2,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 
 /*---------------------- BUS Fault Handle ------------------*/

 #ifdef HANDLE_BUS_FAULT

 void BusFault_Handler(void)
 {
 	#ifdef USER_BUS_FAULT_HANDLER
		 BUS_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(1,DUREE_LENTE);

		}
	#endif 
 } 
 #endif
