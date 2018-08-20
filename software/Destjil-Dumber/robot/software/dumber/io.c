/**
 * \file io.c
 * \brief Input/Output functions
 * \author Sebastien DI MERCURIO
 * 
 * io.c file contains functions related to GPIO pins
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

/**
 * \brief IO Initialization
 *
 * Initialize GPIO pins as input or output:
 * - Led on PC5, as output
 * - IR led on PB1, as output
 * - Ball detection on PD2, as input
 * - Shutdown on PD7, as output
 * - XBEE reset on PD4, as output
 * - Push button on PC4, as input
 */
void IOInit(void)
{
	/* Led initialization */
	DDRC = DDRC | (1<<PIN5);
	PORTC = PORTC & ~(1<<PIN5);	
	
	/* IR initialization */
	DDRB = DDRB | (1<<PIN1);
	PORTB = PORTB & ~(1<<PIN1);	
	
	/* Ball detection initialization */
	DDRD = DDRD & ~(1<<PIN2);
	
	/* Shutdown initialization */
	DDRD = DDRD | (1<<PIN7);
	PORTD = PORTD & ~(1<<PIN7);
	
	/* XBEE initialization */
	DDRD = DDRD | (1<<PIN4);
	PORTD = PORTD & ~(1<<PIN4);
	
	/* Push button initialization */
	DDRC = DDRC & ~(1<<PIN4);
	
	/* Motor left forward initialization */
	DDR_MOTOR_LEFT_FORWARD = DDR_MOTOR_LEFT_FORWARD | (1<<MOTOR_LEFT_FORWARD_PIN_NBR);
	PORT_MOTOR_LEFT_FORWARD = PORT_MOTOR_LEFT_FORWARD & ~(1<<MOTOR_LEFT_FORWARD_PIN_NBR);
	
	/* Motor left reverse initialization */
	DDR_MOTOR_LEFT_REVERSE = DDR_MOTOR_LEFT_REVERSE | (1<<MOTOR_LEFT_REVERSE_PIN_NBR);
	PORT_MOTOR_LEFT_REVERSE = PORT_MOTOR_LEFT_REVERSE & ~(1<<MOTOR_LEFT_REVERSE_PIN_NBR);
	
	/*  Motor right forward initialization */
	DDR_MOTOR_RIGHT_FORWARD = DDR_MOTOR_RIGHT_FORWARD | (1<<MOTOR_RIGHT_FORWARD_PIN_NBR);
	PORT_MOTOR_RIGHT_FORWARD = PORT_MOTOR_RIGHT_FORWARD & ~(1<<MOTOR_RIGHT_FORWARD_PIN_NBR);
	
	/* Motor right reverse initialization */
	DDR_MOTOR_RIGHT_REVERSE = DDR_MOTOR_RIGHT_REVERSE | (1<<MOTOR_RIGHT_REVERSE_PIN_NBR);
	PORT_MOTOR_RIGHT_REVERSE = PORT_MOTOR_RIGHT_REVERSE & ~(1<<MOTOR_RIGHT_REVERSE_PIN_NBR);
}

/**
 * \brief Enable ball detection
 *
 * Enable 36Khz modulation of IR light in order to enable ball detection
 * There is neither input parameters nor output parameters.
 */
void IOSwitchBallDetectOn(void)
{
	DDR_BALL_DETECT = DDR_BALL_DETECT | (1<<BALL_DETECT_PIN_NBR);
}

/**
 * \brief Disable ball detection
 *
 * Disable 36Khz modulation of IR light thus disabling ball detection
 * There is neither input parameters nor output parameters.
 */
void IOSwitchBallDetectOff(void)
{
	DDR_BALL_DETECT = DDR_BALL_DETECT & ~(1<<BALL_DETECT_PIN_NBR);	
}