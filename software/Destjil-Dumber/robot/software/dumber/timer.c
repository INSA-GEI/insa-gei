/**
 * \file timer.c
 * \brief header for timer and motor driver
 * \author Sebastien DI MERCURIO
 * 
 * timer.c file contains the driver for motor and timer functionalities
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "dumber.h"
#include "io.h"
#include "motors.h"

#define __WITH_WDT__

/**
 * \brief Used to start windowed watchdog
 *
 *  To start watchdog, set this variable to 1 
 */
char WDT_demarre;

/**
 * \brief Counter used for watchdog
 *
 * The watchdog should be reset when this counter is between SEUIL_MIN_WDT and SEUIL_MAX_WDT.
 * @see SEUIL_MIN_WDT
 * @see SEUIL_MAX_WDT
 */
unsigned int WDT_compteur;

/**
 * \brief Counter for watchdog fault
 *
 * Each time watchdog is no reset correctly, this counter increases. When it reach WDT_MAX_FAULT, robot stop functioning.
 * @see WDT_MAX_FAULT
 */
char WDT_faults;

/**
 * \brief Low level counter for WDT 
 *
 * This constant is equal to a time of 950ms.
 * @see WDT_compteur
 */
#define SEUIL_MIN_WDT	34200

/**
 * \brief High level counter for WDT 
 *
 * This constant is equal to a time of 1050ms.
 * @see WDT_compteur
 */
#define SEUIL_MAX_WDT   37800

/**
 * \brief Max allow fault for WDT
 *
 * When WDT_faults counter reaches this value, robot stops working.
 * @see WDT_faults
 */
#define WDT_MAX_FAULT	10

/**
 * \brief Timer initialization
 *
 * Initialization of the 3 timers
 * - Timer 1: Channel A set as PWM, used for IR led modulation, frequency 36 Khz, used also for main system timer
 * - Timer 2: Used to control left motor, channels A and B set as PWM, frequency 31250 hz  
 * - Timer 3: Used to control right motor, channels A and B set as PWM, frequency 31250 hz 
 */
void TIMERInit(void)
{
	/* Rappel: FCPU = 7372800 Hz */
	/* Reglage du Timer 1 (16 bits), canal A en pwm */
	/* Utilisation pour moduler la led IR. Periode = 36 Khz */

	/* Connecte OC1A sur le port, Regle le timer en mode fast PWM 14*/
	TCCR1A = (1<<COM1A1)+ (1<<WGM11); 
	TCCR1B = (1<<WGM13) + (1<<WGM12) + (1<<CS10);
	//ICR1 = 204; /* -> Permet une periode de 36000 Hz à 7372800 Hz */
	ICR1 = 222+1; /* -> Permet une periode de 36000 Hz à 8000000 Hz */
	OCR1A = 5; /* set correct power */

	TIMSK1 = (1<<TOIE1);

	TCCR0A = (0<<WGM00);
	TCCR0B = (1<<CS00);

	DDRD = DDRD | (1<<PIN5) | (1<<PIN6);

	TIMSK0 = (1<<TOIE0);
	
	/* Watchdog */
	WDT_compteur = 0;
	DUMBERSetRobotState(STATE_IDLE);
	WDT_demarre=0;
	WDT_faults = 0;
}			

/**
 * \brief Interruption handler for timer 1
 *
 * Interruption handler of timer 1 called every 27,7 µs. Used as main timing functionalities.   
 */
ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
static unsigned int compteur=0;

#ifdef __WITH_WDT__
	if (WDT_demarre==1)
	{
		WDT_compteur ++;

		if (WDT_compteur > SEUIL_MAX_WDT) 
		{
			if (WDT_faults> WDT_MAX_FAULT)
			{
				DUMBERSetRobotState(STATE_DISABLED);
				IOSwitchBallDetectOff();
				MOTORReset();
			}
			else
			{
				WDT_faults++;
				WDT_compteur = WDT_compteur - 36000; /* on recharge le compteur avec la difference par rapport à 1s */ 	
			}
		}
	}
#endif /* __WITH_WDT__ */

	compteur ++;
	if (compteur >=3600) /* environ 100 ms */
	{
		compteur =0;
		DUMBERCallbackActivity();
	}	
}

/**
 * \brief Interruption handler for timer 0
 *
 * Interruption handler of timer 1 called every (7372800/128) => ms. Used as main timing functionalities.   
 */
ISR(TIMER0_OVF_vect, ISR_BLOCK)
{
static unsigned int compteur=0;

	/* Called every interrupt */
	MOTORUpdateCounter();
	
	compteur++;
	
	if (compteur>=29)
	{
		compteur=0;
		/* called only every 1 ms for regulating purpose */
		MOTORControlInterrupt();
	}		
}

/**
 * \brief Used to take robot ready to run
 *
 * When robot is in IDLE state, this function enable full functionalities.
 * @param[in] wdt activate WDT when starting system. 0: WDT not active, 1: WDT active.  
 */
void TIMERStartSystem(char wdt)
{
	if (wdt == WITH_WDT) WDT_demarre = 1;
	WDT_compteur = 0;
	WDT_faults = 0;

	DUMBERSetRobotState(STATE_RUNNING);

	/* Demarrage de la scrutation de la balle */
	IOSwitchBallDetectOn();
}

/**
 * \brief Check if WDT is valid
 *
 * When receiving WDT reset command, this function is called. If WDT can be reset, the function returns STATE_RUNNING, otherwise it returns STATE_DISABLED.
 * @return New state for the robot: can be STATE_ENABLED or STATE_DISABLED.
 * @see STATE_ENABLED
 * @see STATE_DISABLED    
 */
char TIMERAckWDT(void)
{
#ifdef __WITH_WDT__
	if (WDT_demarre==1)
	{
		if (WDT_compteur < SEUIL_MIN_WDT) 
		{
			if (WDT_faults > WDT_MAX_FAULT)
			{
				DUMBERSetRobotState(STATE_DISABLED);
				IOSwitchBallDetectOff();
				MOTORReset();
			}
			else
			{
				WDT_faults++;
				WDT_compteur=0;	
			}
		}
		else 
		{
			WDT_faults=0; /* RAZ du compteur de fautes */
			//WDT_compteur =0;
		}
	}	

	return DUMBERGetRobotState();
#else
	return STATE_RUNNING;
#endif /* __WITH_WDT__ */ 
}

/**
 * \brief Reinit WDT
 *
 * WDT is stopped, and robot state is set to STATE_IDLE
 * @see STATE_IDLE
 */
void TIMERResetWDT(void)
{
	WDT_demarre=0;
	WDT_compteur = 0;
	WDT_faults = 0;
	DUMBERSetRobotState(STATE_IDLE);
	//OCR1A = 0;
	IOSwitchBallDetectOff();
}




