#include "stm32f10x.h"

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 28/06
//  Processeur : STM32F103RB
//  Logiciel dev : �Vision 5.23
//  Fichier de configuration de l'USART
//=======================================================================================








#ifndef _USART_USER_CONF_H__
#define _USART_USER_CONF_H__

//=======================================================================================

//_________ USER DEFINE _________________________________________________________________

// Taille du buffer d'�mission (r�glable pour chaque USART)
#define Max_Car_In_String_Emission_1 500
#define Max_Car_In_String_Emission_2 500
#define Max_Car_In_String_Emission_3 500
// Taille du buffer de r�ception (r�glable pour chaque USART)
#define Buf_Len_StrRec_USART1 15
#define Buf_Len_StrRec_USART2 15
#define Buf_Len_StrRec_USART3 15

// Caract�re de terminaison, par d�faut \r = 0xD :
#define Term_Car_USART1 0x0D
#define Term_Car_USART2 0x0D
#define Term_Car_USART3 0x0D

// Mode ECHO lors de la r�ception
//#define ECHO_USART1
#define ECHO_USART2
#define ECHO_USART3

// caract�res particuliers (ne pas modifier !!) : utilisable dans le main
#define Null 0
#define ClearScreen 0xC
#define LF 0xA
#define CR 0xD

// d�buggage COM USART
//#define Debugg_Bourrage // � commenter lorsque l'appli est d�bugg�e.
// Si bourrage (trop d'octets re�us ou �mis) => plantage de l'appli dans un while 1 pour analyse erreur

#endif
