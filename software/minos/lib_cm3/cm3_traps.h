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
#include "cm3_traps_config.h"

#ifndef _CM3_TRAPS
#define _CM3_TRAPS
			
	#ifdef USE_BLINK_LEDS
		//___________________________________________________
		extern void Blink_Leds(int repet, int duree) ;
		// Blink choosen leds (in ./cm3_traps_config.h)
		// repet : number of ON/OFF blinks
		// duree : something connected to time of blink (use following defines) 
		//_____________________________________________________
		#define DUREE_RAPIDE (0x1FFF*20) //Approx 0.1s @40MHz core
		#define DUREE_LENTE (0x1FFF*150) //Approx 0.5s @40MHz core
		#define DUREE_1S (0x1FFF*300) //Approx 1s @40MHz core
	#endif


	//______________________________________________________________________
	// Use following propotypes	to call standard fault handlers from soft
	// or custom fault handlers hooks.
	// Those handlers are infinite loops which do
	// NOTHING IF you have commented USE_BLINK_LEDS  (in ./cm3_traps_config.h)
	// ELSE alternate fast blink on LEDS with
	//  4 slow blinks for Hardware Fault
	//  3 slow blinks for Memmanage Fault
	//  2 slow blinks for Usage Fault
	//  1 slow blink for Bus Fault
	// 

 	/*---------------------- Hardware Fault Handle ------------------*/
 	#ifdef HANDLE_HARDWARE_FAULT
 		void HardFault_Handler(void);
 	#endif
 	
 	/*---------------------- Memmanage Fault Handle ------------------*/
 	#ifdef HANDLE_MEMMANAGE_FAULT
 		void MemManage_Handler(void); 
 	#endif
 	
 	/*---------------------- Usage Fault Handle ------------------*/
 	#ifdef HANDLE_USAGE_FAULT
 		void UsageFault_Handler(void);
	 #endif
	
	 
	 /*---------------------- BUS Fault Handle ------------------*/
	 #ifdef HANDLE_BUS_FAULT									   	
 		void BusFault_Handler(void);
 	#endif

#endif