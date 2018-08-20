/**
 * \file dumber.c
 * \brief Main program for Dumber project
 * \author Sebastien DI MERCURIO
 * 
 * dumber.c file contains main code ans initialization runtime for the project
 */

/**
 * \mainpage Dumber project
 * \section intro_sec Presentation
 *
 * Dumber is a robot used during practical training in real time at INSA, Toulouse (France).
 * Th goal of dumber is to serve as a real actuator in a real time control loop. Other part of the loop are a webcam, 
 * to detect robot position and a real time server/computer to process picture and control the robot.
 *
 * This documentation describe firmware code fitted inside each robot.
 *
 * Documentation is written in French as doxygen doesn't handle correctly French comment ...
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "uart.h"
#include "timer.h"
#include "eeprom.h"
#include "dumber.h"
#include "io.h"
#include "motors.h"

#include <stdio.h>
#include <string.h>

/*
 * Constants used for battery monitoring
 */ 
#define VBAT_SEUIL_ALERTE		185
#define VBAT_SEUIL_STOP 		174

/*
 * Software version
 * Read as FW_MAJOR_VER.FW_MINOR_VER 
 */ 
#define FW_MAJOR_VER			1
#define FW_MINOR_VER			5

/*
 * Command understood by this firmware
 */
#define CMD_PING 				'p'
#define CMD_RESET				'r'
#define CMD_SET_MOTORS			'm'
#define CMD_START_WATCHDOG		'W'
#define CMD_RESET_WATCHDOG  	'w'
#define CMD_GET_SENSOR 			's'
#define CMD_GET_ODO				'o'
#define CMD_GET_VBAT			'v'

/* Commands added in version 1.1 */
#define CMD_GET_VERSION			'V'

/* Commands added in version 1.2 */
#define CMD_SET_LEFT_NORMAL		'Y'
#define CMD_SET_LEFT_TURBO		'y'
#define CMD_SET_RIGHT_NORMAL	'Z'
#define CMD_SET_RIGHT_TURBO		'z'
#define CMD_RECORD_PARAMS		'R'
#define CMD_GET_PARAMS			'G'
#define CMD_UNSECURE_START		'u'

/* Commands added in version 1.3 */
#define CMD_MOVE				'M'
#define CMD_TURN				'T'
#define CMD_GET_BUSY			'b'

/*
 * Constant for command buffer size
 */
#define BUFFER_CMD_SIZE			32

/**
 * \brief Inactivity constant
 *
 * This constant set inactivity duration. Expressed in 100 ms ticks
 * When the constant is set to -1, inactivity power off is not supported
 */

#define INACTIVITY_DELAY		600*2 /* About 2 minutes */
//#define INACTIVITY_DELAY		-1

/**
 * \brief Normal speed for left motor.
 *
 * Value used on left motor in order to get a normal speed.
 * The value is retrieved from EEPROM in structure params
 * @see params 
 */
unsigned char motorLeftNormal;

/**
 * \brief High speed for right motor.
 *
 * Value used on right motor in order to get a high speed.
 * The value is retrieved from EEPROM in structure params
 * @see params 
 */	
unsigned char motorRightNormal; 

/**
 * \brief Normal speed for left motor.
 *
 * Value used on left motor in order to get a normal speed.
 * The value is retrieved from EEPROM in structure params
 * @see params 
 */
unsigned char motorLeftTurbo;   

/**
 * \brief High speed for right motor.
 *
 * Value used on right motor in order to get a high speed.
 * The value is retrieved from EEPROM in structure params
 * @see params 
 */
unsigned char motorRightTurbo;  

/**
 * \brief OK answer 
 *
 * String used when there were no error during a command processing 
 */
const char OK_ANS[]="O\n";		

/**
 * \brief Error answer 
 *
 * String used when an error as occurred during command processing
 */
const char ERR_ANS[]="E\n";   

/**
 * \brief Command buffer 
 *
 * Buffer used to store command received for PC or to store answer to PC
 * Size of the buffer is set by BUFFER_CMD_SIZE constant
 * @see BUFFER_CMD_SIZE
 */
unsigned char buffer_cmd[BUFFER_CMD_SIZE];

/**
 * \brief Store WDT faults
 *
 * @see WDT_faults
 */
extern char WDT_faults;

/**
 * \brief Low battery indicator
 *
 * Indicate if battery is too low. This changes led behavior.
 */
char low_bat;

/**
 * \brief Inactivity counter
 *
 * When this counter reach INACTIVITY_DELAY, the robot is switched off
 */

int inactivity_counter;

/**
 * \brief File for input and output 
 *
 * File used for input and output. Use USART to send or get data.
 */
static FILE mystdinout = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,_FDEV_SETUP_RW); 

/**
 * \brief Current state for the robot
 *
 * Store the current state of the robot. Valid values are STATE_RUNNING, STATE_DISABLED, STATE_IDLE, STATE_LOW_BAT, STATE_POWER_OFF  
 * @see STATE_RUNNING 
 * @see STATE_DISABLED 
 * @see STATE_IDLE 
 * @see STATE_LOW_BAT 
 * @see STATE_POWER_OFF
 */
char ROBOT_State;


volatile int length;
volatile int i;
volatile unsigned char checksum;
volatile unsigned char *ptr_loc;

/**
 * \brief Main function for dumber
 *
 * Main part of the program: get commands send by PC, check them, process them and send answer to PC  
 */ 
int main (void)
{
unsigned char c;
unsigned char *ptr_cmd;
unsigned char cmd_received;

int moteur_droit, moteur_gauche;
int odo_gauche, odo_droit;

char system;

char battery_level;
int distance;

	wdt_enable(WDTO_4S);
	
	stdout = &mystdinout;
	stdin = &mystdinout;
	
	DUMBERInitPeriph();
	
	cmd_received= 0;
	ptr_cmd = buffer_cmd;

	moteur_droit =0;
	moteur_gauche = 0;

	odo_gauche=0;
	odo_droit=0;
	
	low_bat =0;
	inactivity_counter=0;
	
	system = STATE_IDLE;
	
	//set_sleep_mode(SLEEP_MODE_IDLE);
	sei();
	
	//MOTORWalk(MOTOR_RIGHT, 400, MOTOR_FORWARD);
	//MOTORWalk(MOTOR_LEFT, 400, MOTOR_FORWARD);
	
	//MOTORMove(1000);
	
	while (IO_GET_PIN(PUSHBUTTON))
	{
		/* wait until pushbutton is released */
	}
	
	for (;;)
	{
		//sleep_mode();
		
			wdt_reset();
	
		
		/* Reset du watchdog hardware */
		sei();
		
		/* Battery verification */
		battery_level = DUMBERVbatLevel();
		if (battery_level==BAT_EMPTY)
		{
			/* if battery is too low, switch off robot */
			DUMBERSetRobotState(STATE_POWER_OFF);
			low_bat=0;
		}
		
		if ((battery_level==BAT_LOW_BAT)||(battery_level==BAT_EMPTY))
		{
			/* if battery is low, set lowbat to 1; */
			low_bat=1;
		}

		/* Test du bouton marche arret */
		if (IO_GET_PIN(PUSHBUTTON))
		{
			DUMBERSetRobotState(STATE_POWER_OFF);	
		}
		
		if (UARTGetBufferSize() != 0)
		{
			c= UARTGetchar();
			
			switch (c)
			{
			case '\b': /* backspace char = flush buffer */
				ptr_cmd = buffer_cmd;
				cmd_received = 0;
				break;
			case '\n': /* newline char  = end of command */
			case '\r':
				*ptr_cmd = 0;
				cmd_received = 1;
				break;
			default:
				*ptr_cmd = c;
				ptr_cmd ++;	
			}	
		} 
		
		if (cmd_received)
		{
			ptr_cmd = buffer_cmd;
			cmd_received=0;
			inactivity_counter =0;
			
			/* verification du checksum */
			if (DUMBERVerifyChecksum(ptr_cmd)!=0) /* checksum failed */
			{
				printf ("S\n");
			}
			else
			{
				system = DUMBERGetRobotState();
			
				switch (system)
				{
				case STATE_RUNNING:
					switch (*ptr_cmd)
					{
						case CMD_PING: /* ping command: check if robot is on */
							printf (OK_ANS);
							break;
						case CMD_SET_MOTORS: /* motor command: set motor speed and direction */
							if (sscanf ((char *)ptr_cmd, "m=%i,%i", &moteur_gauche, &moteur_droit)==2)
							{							
								if (moteur_gauche==1) MOTORWalk(MOTOR_LEFT, MAX_SPEED, MOTOR_FORWARD);
								else if (moteur_gauche==-1) MOTORWalk(MOTOR_LEFT, MAX_SPEED, MOTOR_REVERSE);
								else MOTORWalk(MOTOR_LEFT, MAX_SPEED, MOTOR_BREAK);
							
								if (moteur_droit==1) MOTORWalk(MOTOR_RIGHT, MAX_SPEED, MOTOR_FORWARD);
								else if (moteur_droit==-1) MOTORWalk(MOTOR_RIGHT, MAX_SPEED, MOTOR_REVERSE);
								else MOTORWalk(MOTOR_RIGHT, MAX_SPEED, MOTOR_BREAK);
							
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							} 
							break;
						case CMD_RESET_WATCHDOG: /* reset watchdog */
							if (TIMERAckWDT() == STATE_DISABLED)
							{
								printf (ERR_ANS);
							}
							else
							{
								printf ("O:%d\n",WDT_faults);
							}
							break;
						case CMD_GET_SENSOR: /* sensor cmd: return sensor state */
							printf ("O:%d\n",IO_GET_PIN(BALL_DETECT));
							break;
						case CMD_GET_ODO: /* odometers cmd: return odometers */
							printf ("O:%d,%d\n", odo_gauche, odo_droit);
							break;
						case CMD_GET_VBAT: /* voltage cmd: return voltage state */
							printf ("O:%d\n", DUMBERVbatLevel());
							break;
						case CMD_RESET: /* reset system */
							MOTORReset();
						
							odo_gauche=0;
							odo_droit=0;
						
							TIMERResetWDT();
							printf (OK_ANS);
							break;
						case CMD_GET_VERSION: /* return FW version */
							printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
							break;
						case CMD_SET_LEFT_NORMAL: /* set motor left speed normal*/
							if (sscanf ((char *)ptr_cmd, "Y=%u", (unsigned int*)&params.motorLeftNormal)==1)
							{
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							}
							break;
						case CMD_SET_LEFT_TURBO: /* set motor left speed turbo */
							if (sscanf ((char *)ptr_cmd, "y=%u", (unsigned int*)&params.motorLeftTurbo)==1)
							{
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							}
							break;
						case CMD_SET_RIGHT_NORMAL: /* set motor right speed normal*/
							if (sscanf ((char *)ptr_cmd, "Z=%u", (unsigned int*)&params.motorRightNormal)==1)
							{
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							}
							break;
						case CMD_SET_RIGHT_TURBO: /* set motor right speed turbo*/
							if (sscanf ((char *)ptr_cmd, "z=%u", (unsigned int*)&params.motorRightTurbo)==1)
							{
								printf (OK_ANS);	
							}
							else
							{
								printf (ERR_ANS);
							}
							break;	
						case CMD_RECORD_PARAMS: /* record params */
							printf (OK_ANS);
							E2PWrite (0, sizeof(struct ST_EEPROM), (unsigned char*) &params);
							break;
						case CMD_GET_PARAMS:
							printf ("O:%u,%u,%u,%u\n",params.motorLeftNormal,params.motorLeftTurbo,
													  params.motorRightNormal,params.motorRightTurbo);
							break;
						case CMD_START_WATCHDOG:    /* start watchdog */
						case CMD_UNSECURE_START:    /* start insecurely*/
							printf (ERR_ANS); /* Commande non autorisée */
							break;
						case CMD_MOVE:
							if (sscanf ((char *)ptr_cmd, "M=%i", &distance)==1)
							{							
								MOTORMove(distance);
							
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							} 
							break;
						case CMD_TURN:
							if (sscanf ((char *)ptr_cmd, "T=%i", &distance)==1)
							{							
								MOTORTurn(distance);
							
								printf (OK_ANS);
							}
							else
							{
								printf (ERR_ANS);
							} 
							break;
						case CMD_GET_BUSY:
							printf ("O:%d\n", MOTORGetState());
							break;
						default: /* unknown cmd */
							printf ("C:%c\n",*ptr_cmd);	
							break;
					}				
					break;
				case STATE_IDLE:
					switch (*ptr_cmd)
					{
						case CMD_PING: /* ping command: check if robot is on */
							printf (OK_ANS);
							break;
						case CMD_START_WATCHDOG: /* start watchdog */
							TIMERStartSystem(WITH_WDT);
							printf (OK_ANS);
							break;
						case CMD_UNSECURE_START: /* start insecurely*/
							TIMERStartSystem(WITHOUT_WDT);
							printf (OK_ANS);
							break;
						case CMD_RESET: /* reset system */
							MOTORReset();
						
							odo_gauche=0;
							odo_droit=0;
						
							TIMERResetWDT();
							printf (OK_ANS);
							break;
						case CMD_GET_VERSION: /* return FW version */
							printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
							break;
						case CMD_SET_MOTORS: 		/* motor command: set motor speed and dir */
						case CMD_RESET_WATCHDOG: 	/* reset watchdog */
						case CMD_GET_SENSOR: 		/* sensor cmd: return sensor state */	
						case CMD_GET_ODO: 			/* odometrie cmd: return odometrie */
						case CMD_GET_VBAT: 			/* voltage cmd: return voltage state */
						case CMD_SET_LEFT_NORMAL: 	/* set motor left speed normal*/
						case CMD_SET_LEFT_TURBO: 	/* set motor left speed turbo */
						case CMD_SET_RIGHT_NORMAL: 	/* set motor right speed normal*/
						case CMD_SET_RIGHT_TURBO: 	/* set motor right speed turbo*/
						case CMD_RECORD_PARAMS: 	/* record params */
						case CMD_GET_PARAMS:		/* get params */
						case CMD_MOVE:
						case CMD_TURN:
						case CMD_GET_BUSY:
							printf (ERR_ANS); /* Commande non autorisée */
							break;
						default: /* unknown cmd */
							printf ("C:%c\n",*ptr_cmd);	
							break;
					}					
					break;
				default:
					 /*		DUMBERGetRobotState() == STATE_DISABLED	=> Systeme inactif (du au WDT) 
					  ||	DUMBERGetRobotState() == STATE_LOW_BAT		=> Batterie trop faible   
					  */
					switch (*ptr_cmd)
					{
						case CMD_PING: /* ping command: check if robot is on */
							printf (OK_ANS);
							break;
						case CMD_RESET: /* reset system */
							MOTORReset();
						
							odo_gauche=0;
							odo_droit=0;
						
							TIMERResetWDT();
							printf (OK_ANS);
							break;
						case CMD_GET_VERSION: /* return FW version */
							printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
							break;
						case CMD_SET_MOTORS: 		/* motor command: set motor speed and dir */
						case CMD_START_WATCHDOG: 	/* start watchdog */
						case CMD_UNSECURE_START:    /* start insecurely */
						case CMD_RESET_WATCHDOG: 	/* reset watchdog */
						case CMD_GET_SENSOR: 		/* sensor cmd: return sensor state */	
						case CMD_GET_ODO: 			/* odometrie cmd: return odometrie */
						case CMD_GET_VBAT: 			/* voltage cmd: return voltage state */
						case CMD_SET_LEFT_NORMAL: 	/* set motor left speed normal*/
						case CMD_SET_LEFT_TURBO: 	/* set motor left speed turbo */
						case CMD_SET_RIGHT_NORMAL: 	/* set motor right speed normal*/
						case CMD_SET_RIGHT_TURBO: 	/* set motor right speed turbo*/
						case CMD_RECORD_PARAMS: 	/* record params */
						case CMD_GET_PARAMS:		/* get params */
						case CMD_MOVE:
						case CMD_TURN:
						case CMD_GET_BUSY:
							printf (ERR_ANS); /* Commande non autorisée */
							break;
						default: /* unknown cmd */
							printf ("C:%c\n",*ptr_cmd);	
							break;
					}				
				}
			}			
		}
	}
}

/**
 * \brief Initialization of micro peripherals
 *
 * Used for initialization of peripherals, like USART, Timers, I/O and EEPROM 
 */ 
void DUMBERInitPeriph(void)
{
	/* Recuperation des parametres en EEPROM */
	E2PInit();
	E2PRead(0, sizeof(struct ST_EEPROM), (unsigned char*)&params);

	if (params.eepromVer == 0xFF) /* la table est invalide: a initialiser */
	{
		params.eepromVer=1;
		params.motorLeftNormal=0x35;
		params.motorLeftTurbo=0x70;
		params.motorRightNormal=0x35;
		params.motorRightTurbo=0x70;

		E2PWrite (0, sizeof(struct ST_EEPROM), (unsigned char*) &params);
	}
	
	UARTInit();
	TIMERInit();
	IOInit();
	MOTORInit();
	DUMBERInitMisc();
	
	/* Start Xbee */
	IO_SET_PIN(XBEE);
}

/**
 * \brief Callback called every 100 ms
 *
 * This callback is called every 100 ms. It is mainly used for blinking activity light and for managing power off
 * There is neither input parameters nor output parameters.
 */
void DUMBERCallbackActivity(void)
{
static char compteur=0;
char etat;

	/* Appelée toute les 100ms environ */
		
	compteur ++;
	etat = DUMBERGetRobotState();

	switch (etat)
	{
	case STATE_IDLE:
		if (low_bat==0)
		{
			if (compteur <=1) IO_SET_PIN(LED);
			else if (compteur <10) IO_CLEAR_PIN(LED);
			else compteur =0;	
		}
		else
		{
			if (compteur>=1)
			{
				IO_TOGGLE_PIN(LED);
				compteur = 0;	
			}	
		}			
		break;
	case STATE_RUNNING:
		if (low_bat==0)
		{
			IO_SET_PIN(LED);
			compteur = 0;	
		}
		else
		{
			if (compteur>=1)
			{
				IO_TOGGLE_PIN(LED);
				compteur = 0;	
			}				
		}
		
		break;
	case STATE_DISABLED:
		if (compteur>=10)
		{
			IO_TOGGLE_PIN(LED);
			compteur = 0;	
		}
		break;
	}
	
	/* Inactivity counter */
	if (INACTIVITY_DELAY != -1)
	{
		inactivity_counter++;
	
		if (inactivity_counter>= INACTIVITY_DELAY)
		{
			DUMBERSetRobotState(STATE_POWER_OFF);		
		}
	}	
}

/**
 * \brief Miscellaneous initialization
 *
 * This function initialize various stuff like ADC (for battery voltage monitoring) or activity LED.
 * There is neither input parameters nor output parameters.
 */
void DUMBERInitMisc(void)
{
	/* Mise en route de l'ADC pour la surveillance de la batterie */
	ADMUX = (1<<REFS0) + (1<<ADLAR) + (6<<ADMUX); /* Conversion d'ADC6, ajustement à gauche et utilisation d'AVCC comme reference */
	ADCSRB = 0;
	ADCSRA = (1<<ADEN) + (1<<ADATE) + (7<<ADPS0);
	ADCSRA = ADCSRA | (1<<ADSC);
}

/**
 * \brief Get battery voltage value
 *
 * This function is used to get battery voltage and return a level depending on this value.
 * @return Battery level
 */
char DUMBERVbatLevel(void)
{
unsigned char vbat;
	/* Lire l'ADC */

	vbat = (unsigned char) ADCH;
	
	if (vbat == 0) /* premiere conversion -> ne pas en tenir compte */
	{
		return BAT_NORMAL;
	}
	else if (vbat <VBAT_SEUIL_STOP)
	{
		return BAT_EMPTY;
	}
	else if (vbat<VBAT_SEUIL_ALERTE)
	{
		return BAT_LOW_BAT;
	}
	else return BAT_NORMAL;
}

/**
 * \brief Return current state of the robot
 *
 * Get current robot state
 * @return Current robot state
 * @see STATE_RUNNING 
 * @see STATE_DISABLED 
 * @see STATE_IDLE 
 * @see STATE_LOW_BAT 
 * @see STATE_POWER_OFF
 */
char DUMBERGetRobotState(void)
{
	return ROBOT_State;
}

/**
 * \brief Set current state of the robot
 *
 * Set current robot state
 * @param[in] state New robot state
 * @see STATE_RUNNING 
 * @see STATE_DISABLED 
 * @see STATE_IDLE 
 * @see STATE_LOW_BAT 
 * @see STATE_POWER_OFF
 */
void DUMBERSetRobotState(char state)
{
	ROBOT_State = state;
	
	if (state == STATE_POWER_OFF)
	{
		IOSwitchBallDetectOff();
		MOTORReset();
		
		/* Switch off robot */
		IO_SET_PIN(SHUTDOWN);
	}	
}

char DUMBERVerifyChecksum(unsigned char *ptr)
{


	ptr_loc = ptr;
	checksum =0;
	length = strlen ((const char*)ptr);
	
	for(i=0; i<length-1; i++)
	{
		checksum = checksum ^ *ptr_loc;	
		ptr_loc++;		
	}
	
	if (checksum=='\n') checksum = checksum +1;
	checksum = checksum ^ *ptr_loc;
	
	return checksum;
}
