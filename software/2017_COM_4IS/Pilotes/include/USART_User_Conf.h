#include "stm32f10x.h"

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 28/06
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 5.23
//  Fichier de configuration de l'USART
//=======================================================================================








#ifndef _USART_USER_CONF_H__
#define _USART_USER_CONF_H__

//=======================================================================================

//_________ USER DEFINE _________________________________________________________________

// Taille du buffer d'émission (réglable pour chaque USART)
#define Max_Car_In_String_Emission_1 500
#define Max_Car_In_String_Emission_2 500
#define Max_Car_In_String_Emission_3 500
// Taille du buffer de réception (réglable pour chaque USART)
#define Buf_Len_StrRec_USART1 15
#define Buf_Len_StrRec_USART2 15
#define Buf_Len_StrRec_USART3 15

// Caractère de terminaison, par défaut \r = 0xD :
#define Term_Car_USART1 0x0D
#define Term_Car_USART2 0x0D
#define Term_Car_USART3 0x0D

// Mode ECHO lors de la réception
//#define ECHO_USART1
#define ECHO_USART2
#define ECHO_USART3

// caractères particuliers (ne pas modifier !!) : utilisable dans le main
#define Null 0
#define ClearScreen 0xC
#define LF 0xA
#define CR 0xD

// débuggage COM USART
//#define Debugg_Bourrage // à commenter lorsque l'appli est débuggée.
// Si bourrage (trop d'octets reçus ou émis) => plantage de l'appli dans un while 1 pour analyse erreur

#endif
