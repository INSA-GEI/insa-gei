/**
 * \file motors.c
 * \brief Motor control functions
 * \author Sebastien DI MERCURIO
 * 
 * motors.c file contains functions related to motors
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motors.h"
#include "eeprom.h"
#include "io.h"

void MOTORStoreSpeed(char motor);
char MOTORSet(char moteur, signed char cmd);

unsigned int counter_left_wheel;
unsigned int counter_right_wheel;

unsigned int duration_left;
unsigned int duration_right;

unsigned int setpoint_left;
unsigned int setpoint_right;

char left_direction;
char right_direction;

unsigned int distance;

#define MAX_DURATION	2500
#define NOT_USED		65535

#ifndef PCINT10
	#define PCINT10	2
#endif /* ifndef PCINT10 */
/**
 * \brief Initialize motors
 *
 * Initialize some points related to motor and motor control.
 */
void MOTORInit(void)
{
	MOTORReset();
	
	/* Enable push pull on hall sensor 
	 * Hall sensor for motor 1: PB0 and PB2
	 * Hall sensor for motor 2: PC2 and PC3
	 * At startup, each pin is set as input => no need to change
	 */
	
	PORTC |= (1<<PIN2)+(1<<PIN3);
	PORTB |= (1<<PIN0)+(1<<PIN2);
	
	/* Now activating interrupt on external change for those pins */
	PCMSK0 |= (1<<PCINT0);
	PCMSK1 |= (1<<PCINT10);
	
	PCICR |= (1<<PCIE1) + (1<<PCIE0);
}

/**
 * \brief Reset motors
 *
 * Reset stuff related to motor and motor control.
 */
void MOTORReset(void)
{
	MOTORSet(MOTOR_LEFT, MOTOR_BREAK);
	MOTORSet(MOTOR_RIGHT, MOTOR_BREAK);
	
	counter_left_wheel =0;
	counter_right_wheel =0;
	setpoint_left=MAX_DURATION;
	setpoint_right=MAX_DURATION;
	duration_left = MAX_DURATION;
	duration_right = MAX_DURATION;
	distance = NOT_USED;
}
/**
 * \brief Change motor speed
 *
 * Change motor speed. Speed can take 5 different values.
 * @param[in] motor Identity of the motor to set. Can take values: MOTOR_LEFT, MOTEUR_DROIT
 * @param[in] cmd Speed for the motor. Can take values: MOTOR_STOP, MOTOR_FORWARD, MOTOR_REVERSE, MOTOR_BREAK
 * @return Status for the command: 1 = command successful, 0= speed error
 * @see MOTOR_LEFT
 * @see MOTOR_RIGHT
 * @see MOTOR_STOP
 * @see MOTOR_FORWARD 
 * @see MOTOR_REVERSE 
 * @see MOTOR_BREAK   
 */
char MOTORSet(char motor, signed char cmd)
{
char status = 1;

	if (motor== MOTOR_LEFT)
	{
		switch (cmd)
		{
			case MOTOR_STOP:
				IO_CLEAR_PIN(MOTOR_LEFT_FORWARD);
				IO_CLEAR_PIN(MOTOR_LEFT_REVERSE);
				break;
			case MOTOR_REVERSE:
				IO_CLEAR_PIN(MOTOR_LEFT_FORWARD);
				IO_SET_PIN(MOTOR_LEFT_REVERSE);
				break;			
			case MOTOR_FORWARD:
				IO_SET_PIN(MOTOR_LEFT_FORWARD);
				IO_CLEAR_PIN(MOTOR_LEFT_REVERSE);
				break;
			case MOTOR_BREAK:
				IO_SET_PIN(MOTOR_LEFT_FORWARD);
				IO_SET_PIN(MOTOR_LEFT_REVERSE);
				break;
			default:
				status = 0;
		}	
	}
	else
	{
		switch (cmd)
		{
			case MOTOR_STOP:
				IO_CLEAR_PIN(MOTOR_RIGHT_FORWARD);
				IO_CLEAR_PIN(MOTOR_RIGHT_REVERSE);
				break;
			case MOTOR_REVERSE:
				IO_CLEAR_PIN(MOTOR_RIGHT_FORWARD);
				IO_SET_PIN(MOTOR_RIGHT_REVERSE);
				break;			
			case MOTOR_FORWARD:
				IO_SET_PIN(MOTOR_RIGHT_FORWARD);
				IO_CLEAR_PIN(MOTOR_RIGHT_REVERSE);
				break;
			case MOTOR_BREAK:
				IO_SET_PIN(MOTOR_RIGHT_FORWARD);
				IO_SET_PIN(MOTOR_RIGHT_REVERSE);
				break;
			default:
				status = 0;
		}	
	}
	
	return status;
}

/**
 * \brief Speed counter update
 *
 * This function is called every 1/30Khz and is used to update speed counter for each motor
 */
void MOTORUpdateCounter( void )
{
	if (counter_left_wheel < MAX_DURATION) counter_left_wheel++;
	else duration_left = MAX_DURATION;
	
	if (counter_right_wheel < MAX_DURATION) counter_right_wheel++;
	else duration_right = MAX_DURATION;
}

/**
 * \brief Motor control function
 *
 * This function is called every ms and is used to control motor speed.
 */
void MOTORControlInterrupt(void)
{
	if (setpoint_left == MAX_DURATION) MOTORSet(MOTOR_LEFT, MOTOR_BREAK);
	else
	{
		if (left_direction == MOTOR_FORWARD)
		{
			if (duration_left > setpoint_left) MOTORSet(MOTOR_LEFT, MOTOR_FORWARD); /* motor speed is below setpoint => increase speed */
			else MOTORSet(MOTOR_LEFT, MOTOR_STOP); /* motor speed is above setpoint => decrease speed */
		}
		else
		{
			if (duration_left > setpoint_left) MOTORSet(MOTOR_LEFT, MOTOR_REVERSE); /* motor speed is below setpoint => increase speed */
			else MOTORSet(MOTOR_LEFT, MOTOR_STOP); /* motor speed is above setpoint => decrease speed */
		}
	}	
	
	if (setpoint_right == MAX_DURATION) MOTORSet(MOTOR_RIGHT, MOTOR_BREAK);
	else
	{
		if (right_direction == MOTOR_FORWARD)
		{
			if (duration_right > setpoint_right) MOTORSet(MOTOR_RIGHT, MOTOR_FORWARD); /* motor speed is below setpoint => increase speed */
			else MOTORSet(MOTOR_RIGHT, MOTOR_STOP); /* motor speed is above setpoint => decrease speed */
		}
		else
		{
			if (duration_right > setpoint_right) MOTORSet(MOTOR_RIGHT, MOTOR_REVERSE); /* motor speed is below setpoint => increase speed */
			else MOTORSet(MOTOR_RIGHT, MOTOR_STOP); /* motor speed is above setpoint => decrease speed */
		}
	}	
}

/**
 * \brief Interrupt function for left motor sensor
 *
 * This function is called each time the sensor of left motor produces a top
 */
ISR (PCINT1_vect)
{
	if (PINC & (1<<PIN2))
	{
		duration_left = counter_left_wheel;
		counter_left_wheel =0;
		
		/*if (duration_left < MAX_DURATION) duration_left = duration_left>>1;
		else duration_left = MAX_DURATION;*/
		if (duration_left > MAX_DURATION) duration_left = MAX_DURATION;
	}
}

/**
 * \brief Interrupt function for right motor sensor
 *
 * This function is called each time the sensor of right motor produces a top
 */
ISR (PCINT0_vect)
{
	if (PINB & (1<<PIN0))
	{
		duration_right = counter_right_wheel;
		counter_right_wheel=0;
		
		if (duration_right > MAX_DURATION) duration_right = MAX_DURATION;
		
		if (distance != NOT_USED)
		{
			distance --;
			
			if (distance ==0)
			{
				MOTORReset();				
			}
			
			/*if (distance < MIN_DISTANCE) 
			{
				setpoint_left = MIN_SPEED;
				setpoint_right = MIN_SPEED;				
			}*/
		}
	}
}

/**
 * \brief Start a motor
 *
 * Use to set speed and direction for a motor
 */
void MOTORWalk( char motor, int duration, char direction )
{
	if (motor == MOTOR_LEFT)
	{
		if ((direction == MOTOR_FORWARD) || (direction == MOTOR_REVERSE)) setpoint_left = duration;
		else setpoint_left = MAX_DURATION;
		
		left_direction = direction;
		counter_left_wheel=0;
	}
	else
	{
		if ((direction == MOTOR_FORWARD) || (direction == MOTOR_REVERSE)) setpoint_right = duration;
		else setpoint_right = MAX_DURATION;
		
		right_direction = direction;
		counter_right_wheel=0;
	}
	
	distance = NOT_USED;
	
	MOTORSet(motor, direction);
}
	
/**
 * \brief Let robot move in straight line
 *
 * This function set a distance and robot move in straight line this distance.
 * If distance is below 0, the move is backward
 */
void MOTORMove(int dist)
{
char dir;
unsigned int setpoint;

	dir = MOTOR_FORWARD;
	
	/*Rapport de réduction des roues 1/87
	 *On a deux point par tour du rotor, donc 174 par tour de roue
	 *Rayon des roues = 35mm
	 *Circonférence = 22cm*/
	
	//A cause d'un problème sur la division dans la librairie minilib (boucle infinie qui remplie la pile)
	//On considère que 174/22 ~ 8
	
	if (dist ==0) MOTORReset();
	else if (dist >0) 
	{
		distance = dist;
		dir = MOTOR_FORWARD;
	}	
	else 
	{
		distance = -dist;
		dir = MOTOR_REVERSE;
	}	
	
	if (distance>MIN_DISTANCE) setpoint = MAX_SPEED;	
	else setpoint = MIN_SPEED;
	
	setpoint_left = setpoint;
	left_direction = dir;
	setpoint_right = setpoint;
	right_direction = dir;
		
	if (counter_left_wheel == MAX_DURATION) counter_left_wheel=0;
	if (counter_right_wheel == MAX_DURATION) counter_right_wheel=0;
	
	MOTORSet(MOTOR_LEFT, dir);
	MOTORSet(MOTOR_RIGHT, dir);
}

/**
 * \brief Let robot turn 
 *
 * This function set a distance and robot turns this distance.
 * If distance is below 0, the turn is counterclockwise
 */
void MOTORTurn(int angle)
{
char dir_left;
char dir_right;
unsigned int setpoint;
	
	dir_left = MOTOR_FORWARD;
	dir_right = MOTOR_REVERSE;
		
	if (angle ==0) MOTORReset();
	else if (angle >0) 
	{
		distance = angle;
		dir_left = MOTOR_FORWARD;
		dir_right = MOTOR_REVERSE;
	}	
	else 
	{
		distance = -angle;
		dir_left = MOTOR_REVERSE;
		dir_right = MOTOR_FORWARD;
	}	
	
	if (distance>MIN_DISTANCE) setpoint = MAX_SPEED;	
	else setpoint = MIN_SPEED;
	
	setpoint_left = setpoint;
	left_direction = dir_left;
	setpoint_right = setpoint;
	right_direction = dir_right;
		
	if (counter_left_wheel == MAX_DURATION) counter_left_wheel=0;
	if (counter_right_wheel == MAX_DURATION) counter_right_wheel=0;
	
	MOTORSet(MOTOR_LEFT, dir_left);
	MOTORSet(MOTOR_RIGHT, dir_right);
}

/**
 * \brief Get movement state
 *
 * If robot is processing a move or turn function, this function return 1, ortherwise return 0;
 */
char MOTORGetState( void )
{
	if (distance!=NOT_USED) return 1;
	else return 0;
}

