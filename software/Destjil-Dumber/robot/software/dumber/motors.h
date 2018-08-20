/**
 * \file motors.h
 * \brief Header for motors functions
 * \author Sebastien DI MERCURIO
 * 
 * motors.h file is header for motors functions
 */


#ifndef MOTORS_H_
#define MOTORS_H_

#define MOTOR_STOP			0
#define MOTOR_FORWARD		1
#define MOTOR_REVERSE		2
#define MOTOR_BREAK			3

#define MOTOR_LEFT			1
#define MOTOR_RIGHT			2

#define MIN_DISTANCE	50
#define MAX_SPEED		250
#define MIN_SPEED		400

void MOTORInit(void);
void MOTORControlInterrupt(void);
void MOTORWalk(char motor, int duration, char direction);
void MOTORReset(void);
void MOTORMove(int dist);
void MOTORTurn(int angle);
char MOTORGetState(void);
void MOTORUpdateCounter(void);

#endif /* MOTORS_H_ */