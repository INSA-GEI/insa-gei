/*
________________________________________________________________________________________
	Cortex M3 Fault Handling
	cm3_traps_config[_TEMMPLATE].h
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

#ifndef _CM3_TRAPS_CONFIG
#define _CM3_TRAPS_CONFIG

//COMMENT line below to disable LED blink function
#define USE_BLINK_LEDS
#ifdef USE_BLINK_LEDS

	#define LOCO_PORT GPIOC
	#define LOCO_MASK ((1<<7)|(1<<9))

	#define MCB_STM32_PORT GPIOB
	#define MCB_STM32_MASK (0xFF00) 

	#define OLYMEX_PORT GPIOB //TODO
	#define OLYMEX_MASK (0) //TODO

	//CONFIGURE leds port (GPIOA,GPIOB,GPIOC or predifned)
	#define LED_PORT (LOCO_PORT)
	//CONFIGURE leds mask to select which leds of the port to blink
	#define LED_MASK (LOCO_MASK)	
#endif


//__________________________________________________
// FAULT handler
// Select IF error is handled by this lib :
//  alternate fast blink on LEDS with
//  4 slow blinks for Hardware Fault
//  3 slow blinks for Memmanage Fault
//  2 slow blinks for Usage Fault
//  1 slow blink for Bus Fault
// OR IF error is hooked to your custom function
// OR IF error is not handled
  
//COMMENT line below to disable this Fault Handling
#define HANDLE_HARDWARE_FAULT
#ifdef 	HANDLE_HARDWARE_FAULT

	//COMMENT line below to use your own Fault Function
	//#define USER_HARDWARE_FAULT_HANDLER
	#ifdef USER_HARDWARE_FAULT_HANDLER
		extern void Arret_Urgence(); 					//example of an hook function
		#define HARDWARE_FAULT_FUNCTION Arret_Urgence();
	#endif
#endif

//COMMENT line below to disable this Fault Handling
//#define HANDLE_MEMMANAGE_FAULT
#ifdef 	HANDLE_MEMMANAGE_FAULT

	//COMMENT line below to use your own Fault Function
	//#define USER_MEMMANAGE_FAULT_HANDLER
	#ifdef USER_MEMMANAGE_FAULT_HANDLER
		extern void Arret_Urgence();
		#define MEMMANAGE_FAULT_FUNCTION Arret_Urgence();
	#endif
#endif

//COMMENT line below to disable this Fault Handling
//#define HANDLE_USAGE_FAULT
#ifdef 	HANDLE_USAGE_FAULT

	//COMMENT line below to use your own Fault Function
	//#define USER_USAGE_FAULT_HANDLER
	#ifdef USER_USAGE_FAULT_HANDLER
		extern void Arret_Urgence();
		#define USAGE_FAULT_FUNCTION Arret_Urgence();
	#endif
#endif


//COMMENT line below to disable this Fault Handling
//#define HANDLE_BUS_FAULT
#ifdef 	HANDLE_BUS_FAULT

	//COMMENT line below to use your own Fault Function
	//#define USER_BUS_FAULT_HANDLER
	#ifdef USER_BUS_FAULT_HANDLER
		extern void Arret_Urgence();
		#define BUS_FAULT_FUNCTION Arret_Urgence();
	#endif
#endif

#endif