#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MAJOR_VER         1
#define MINOR_VER         2

#define CMD_MAX_LENGTH    20

#define AC_CHANNEL_MAX    4
#define DC3V_CHANNEL_MAX  3
#define DC5V_CHANNEL_MAX  1

// Pin are named after RP2040 one, not XIAO one
#define DATAOUT           3 //MOSI  Arduino/XIAO equivalent: 10
#define SPICLOCK          2 //SCK   Arduino/XIAO equivalent: 8

#define CS_RELAY          26 //CS_RELAY  Arduino/XIAO equivalent: 0
#define CS_PWR_DC         27 //CS_PWR_DC Arduino/XIAO equivalent: 1
#define LATCH             28 //          Arduino/XIAO equivalent: 2
#define RESET_IO          29 //          Arduino/XIAO equivalent: 3
#define PWM_VAR_PWR       6 //       Arduino/XIAO equivalent: 4

#endif /* __CONFIG_H__ */