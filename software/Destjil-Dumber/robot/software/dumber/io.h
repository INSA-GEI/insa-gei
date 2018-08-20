/**
 * \file io.h
 * \brief Header for input/output functions
 * \author Sebastien DI MERCURIO
 * 
 * io.h file is header for input/output functions
 */


#ifndef IO_H_
#define IO_H_

#define DDR_LED		DDRC
#define PORT_LED	PORTC
#define PIN_LED		PINC
#define LED_PIN_NBR	5

#define DDR_IR		DDRB
#define PORT_IR		PORTB
#define PIN_IR		PINB
#define IR_PIN_NBR	1

#define DDR_BALL_DETECT		DDRD
#define PORT_BALL_DETECT	PORTD
#define PIN_BALL_DETECT		PIND
#define BALL_DETECT_PIN_NBR	2

#define DDR_SHUTDOWN		DDRD
#define PORT_SHUTDOWN		PORTD
#define PIN_SHUTDOWN		PIND
#define SHUTDOWN_PIN_NBR	7

#define DDR_XBEE		DDRD
#define PORT_XBEE		PORTD
#define PIN_XBEE		PIND
#define XBEE_PIN_NBR	4

#define DDR_PUSHBUTTON		DDRC
#define PORT_PUSHBUTTON		PORTC
#define PIN_PUSHBUTTON		PINC
#define PUSHBUTTON_PIN_NBR	4

#define DDR_MOTOR_LEFT_REVERSE		DDRD
#define PORT_MOTOR_LEFT_REVERSE		PORTD
#define PIN_MOTOR_LEFT_REVERSE		PIND
#define MOTOR_LEFT_REVERSE_PIN_NBR	3

#define DDR_MOTOR_LEFT_FORWARD		DDRB
#define PORT_MOTOR_LEFT_FORWARD		PORTB
#define PIN_MOTOR_LEFT_FORWARD		PINB
#define MOTOR_LEFT_FORWARD_PIN_NBR	3

#define DDR_MOTOR_RIGHT_FORWARD		DDRD
#define PORT_MOTOR_RIGHT_FORWARD	PORTD
#define PIN_MOTOR_RIGHT_FORWARD		PIND
#define MOTOR_RIGHT_FORWARD_PIN_NBR	5

#define DDR_MOTOR_RIGHT_REVERSE		DDRD
#define PORT_MOTOR_RIGHT_REVERSE	PORTD
#define PIN_MOTOR_RIGHT_REVERSE		PIND
#define MOTOR_RIGHT_REVERSE_PIN_NBR	6

/**
 * \brief Switch on pin
 *
 * This macro switch a pin on. Pin can be either: LED, IR, SHUTDOWN, XBEE
 */
#define IO_SET_PIN(pin)		PORT_##pin |= (1<<pin##_PIN_NBR)

/**
 * \brief Switch off pin
 *
 * This macro switch a pin off. Pin can be either: LED, IR, SHUTDOWN, XBEE
 */
#define IO_CLEAR_PIN(pin)	PORT_##pin &= ~(1<<pin##_PIN_NBR)

/**
 * \brief Toggle a pin
 *
 * This macro toggle a pin on. Pin can be either: LED, IR, SHUTDOWN, XBEE
 */
#define IO_TOGGLE_PIN(pin)	PORT_##pin ^= (1<<pin##_PIN_NBR)

/**
 * \brief Read pin
 *
 * This macro read an input pin. Pin can be either: PUSHBUTTON, BALL_DETECT
 */
#define IO_GET_PIN(pin)		((PIN_##pin & (1<<pin##_PIN_NBR))>>pin##_PIN_NBR)

void IOInit(void);

void IOSwitchBallDetectOn(void);
void IOSwitchBallDetectOff(void);

#endif /* IO_H_ */