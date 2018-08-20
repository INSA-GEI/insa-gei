#include "stm32f10x.h"
#include "USART_User_Conf.h"

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 28/06
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 5.23
// 
//=======================================================================================


#ifndef _USART_rev_H__
#define _USART_rev_H__


/* 
 ======================================================================================
|
|								GENERALITE
|   Compléter le fichier USART_User_Conf.h pour paramétrer la COM
|   taille des buffer, caraxctères spéciaux...
|
 ======================================================================================
 
La lib fournit les fonctionnalités de bas niveau de l'UART mais aussi des fonctions
de plus haut niveau qui permettent la manipulation de chaîne de caractère.

* les 3 USART sont programmables
* les IO sont configurées dans la fonction d'initialisation de l'USART

=> USART_1_TX = TIM1_CH2 = PA9
=> USART_1_RX = TIM1_CH3 = PA10
=> USART_2_TX = TIM2_CH3 = PA2
=> USART_2_RX = TIM2_CH4 = PA3
=> USART_3_TX = PB10
=> USART_3_RX = PB11
 
Le débit de chacune des transmission est paramétrable.
Par contre les autres configurations sont figées :
- pas de parité
- 8 bits
- pas de contrôle de flux matériel
- 1 bit de stop
 
Lors de l'émission d'une chaîne de caractères, il est prévu un nombre limite que le 
user peut paramétrer et ce pour chaque USART.
Même chose pour les chaînes de réception. Cela limite la taille des buffers en RAM.

Remarque importante : Afin d'économiser un maximum de tps CPU, les fonctions
d'émission et de réception fonctionnent sous interruption (pour chaque caractère).
Les IT sont non bloquantes. Le temps maxi requis pour chacune 
des IT est estmé à moins de 3us.
Les 3 USARTs peuvent donc cohabiter sans aucun soucis en full duplex.
C'est au user dans son code principal à bien utiliser les fonctions
de polling ( notamment Is_Str_Received(USART)  )


 
*/


//            !!! Si un bourrage s'est produit en réception 
//            on tombe dans une boucle sans fin. Utile pour le débug.





/* 
 ======================================================================================
|
|								L'EMISSION
|
 ======================================================================================
 Deux fonctions permettent l'émission :
 => PutChar
 => Put_String
 
 PutChar n'utilise aucune interruption. La fonction contrôle avant l'émission qu'auncune
 autre est en cours. Auquel cas il y a attente.
 Il est donc possible d'envoyer une chaîne avec une simple boucle sans se préoccuper du
 bourrage. Dans ce cas l'émission occupe à 100% le CPU
 
 PRINCIPE DE LA FONCTION Put_String
 A l'appel, le premier caractère est émis (si non null) à la suite de quoi on autorise les
 interruptions en fin d'émission de caractère.
 Ainsi, le second caractère et ceux qui suivent sont émis sous interruption 
 (libération CPU).
 La fin d'émission est effective lorsque le caractère nul est trouvé (non émis)
 ou lorsque la chaîne dépasse la longeur maximal (voir define).
 
***********************************************
 Plus en détail : Put_String
***********************************************


Couche applicative :	Suite des codes ASCII + 0
														||
														||   Put_String
														\/
Liaison physique :			suite des codes ASCII

Exemple :
Chaine à transmettre = azerty.
En dur : 0x61 0x7A 0x65 0x72 0x74 0x79 0x0 ; Le 0 traduit la fin de la chaîne. C'est universel.

La fonction Put_String se contente d'émettre  0x61 0x7A 0x65 0x72 0x74 0x79
Elle aura détecté le \0 et ne l'aura pas émis. 

Pour être dans les clous et envoyer selon l'OS un ou plusieurs caractères de contrôle:
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  (\n = LF = 0xA et \r = CR = 0xD)
ce sera à la couche service de dessus d'envoyer deux chaînes :
0x61 0x7A 0x65 0x72 0x74 0x79 0x0
Puis
0x0A 0x0D 0x0

 
*/


/* 
 ======================================================================================
|
|								LA RECEPTION
|
 ======================================================================================

Deux fonctions permettent la réception :
CHAR GET_CHAR(USART_TYPEDEF *USART);
CHAR * GET_STRING(USART_TYPEDEF *USART);

Les deux exploitent l'interruption enfouie USART, en concurrence avec la transmission.

On peut travailler en polling en utilisant les fonctions :
Is_Byte_Received
Is_Str_Received

La fonction Is_Overrun_Receive_Str permet de savoir si on a loupé une ou plusieurs
chaîne. Si Debugg_Bourrage est défini, alors la fonction ne sert à rien. Si un bourrage
est détecté, on part directement dans un while(1) qui plante le pgm (utile à la mise au pt).




Tous les protocoles finissent par CR mais sait-on jamais... MAC OS en \n ?
La détection du dernier caractère sera donc au choix.
Exemple :
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  donnera 
0x61 0x7A 0x65 0x72 0x74 0x79 \n \0 en dur
Eventuellement en couche service on pourra ou non supprimer \n si le protocole place
en fin de chaîne \n \r

OPTION EN RECEPTION:
le mode ECHO permet de renvoyer tous les caractères traités.

***********************************************
 Plus en détail : Scrutation vs Interruption
***********************************************

Couche applicative :	Suite des codes ASCII + 0
														/\
														||   Sur détection du caractère de terminaison
														||
Liaison physique :			suite des codes ASCII + Term_Car_USARTx (à choisir, par défaut \r ='Enter'=0x0D)


Les deux fonctions :
void Init_IT_Serial_Receive_Byte(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Receive_Str(USART_TypeDef *USART, void (*IT_function) (void));
Les callback sont lancés respectivement à l'arrivée d'un byte ou d'un string.

ATTENTION :
Si les callback sont utilisés, il est primoridal de ne pas y rester pour ne pas bloquer l'IT USART enfouie.
A plus forte raison, il est INTERDIT dans un callback d'appeler une fct em/rec USART car l'IT USART s'interrompt => plantage.
CONSEIL => utiliser cette fonctionnalité que pour récupérer un flag ou une donnée à la volée,y rester le moins possible.
Mieux => ne pas utiliser les callback mais utiliser un polling régulier sur l'appli globale, genre OS.



 */






//=======================================================================================
//                      Configuration  
//=======================================================================================

void Init_USART(USART_TypeDef *USART,int baudrate, char Prio_USART); // OK
/*___________________________________________________________________________________________

Permet de préparer n'importe laquelle des 3 USART avec un débit donné (8bits, 1 stop, pas de parité)
Utilise une IT enfouie qui permet la transmission en IT et la réception en IT.
___________________________________________________________________________________________*/


void Init_IT_Serial_Receive_Byte(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Receive_Str(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Transmit_Str(USART_TypeDef *USART, void (*IT_function) (void));
/*___________________________________________________________________________________________

Ces trois fonctions permettent de renvoyer au user une IT pour indiquer l'événement de réception 
Byte ou String.
___________________________________________________________________________________________*/






//=======================================================================================
// 							  flag 
//=======================================================================================
char Is_Transmission_Busy(USART_TypeDef *USART); // OK
char Is_Overrun_Transmit(USART_TypeDef *USART);
void Clear_Overrun_Transmit(USART_TypeDef *USART);
/*___________________________________________________________________________________________

Gestion du flag Flag_Transmission_Busy :
Géré automatiquement :
- mis à un automatiquement à l'appel des deux fonctions put
- mis à 0 automatiquement lors de la fin des processus put

Gestion du flag Flag_Overrun_Transmit :
- mis à un automatiquement à l'appel des deux fonctions put, si une transmission est en cours
- mis à 0 par la fonction RESET_VAR_USART, plus pertinent, par la fonction VOID CLEAR_OVERRUN_TRANSMIT
___________________________________________________________________________________________*/

char Is_Str_Received(USART_TypeDef *USART); // OK
char Is_Byte_Received(USART_TypeDef *USART); //OK
char Is_Overrun_Receive_Str(USART_TypeDef *USART);
void Clear_Overrun_Receive_Str(USART_TypeDef *USART);

/*___________________________________________________________________________________________

Gestion des Flag de réception :
Ils sont remis à 0 soit au reset général, soit lors du get
le flag_Byte_Received passe à 1 à chaque réception de byte.
Le flag_Str_Received passe à 1 en réception dernier caractère.


Le flag FLAG_OVERRUN_RECEIVE_STR
Mis à 1 en fin de réception string si le flag Str received était encore à 1.
Remis à 0 au reset général et par la fonction CLEAR_OVERRUN_RECEIVE_STR

___________________________________________________________________________________________*/


//=======================================================================================
// 							  Utilisation, rem : // OK signifie validé en reel serial 1
//=======================================================================================

char Get_Char(USART_TypeDef *USART); //OK
char * Get_String(USART_TypeDef *USART); // OK
int Get_Len_Receiv_String(USART_TypeDef *USART); //OK


void Put_Char(USART_TypeDef *USART,char Car); // OK
void Put_String(USART_TypeDef *USART,char * pCar); // OK
int Get_Len_Transm_String(USART_TypeDef *USART); //OK

/*
Les fonctions d'émission
=======================================================================================
La fonction PUT_STRING s'arrête sous deux conditions et ajoute un caractère de fin (voir define .h):
- rencontre du caractère null
ou
- atteinte du nombre max de caractères émis (voir define).
Elle utilise l'IT USART enfouie
La fonction  Put_Char n'utilise aucune IT. L'émission est directe.


INT GET_LEN_TRANSM_STRING(USART_TYPEDEF *USART);
Elle permet de récupérer si besoin la longueur du caractère émis (caractère null non compris).
!ATTENTION ! Cette longueur est mise à jour lorsque la transmission est finie. Il faut donc
insérer une attente fin de transmission :
	while(Flag_Transmission_Busy(USART1)==1);
	LEM=Get_Len_Transm_String(USART1);	

Les fonctions de réception
=======================================================================================
Deux fonctions :
CHAR GET_CHAR(USART_TYPEDEF *USART);
CHAR * GET_STRING(USART_TYPEDEF *USART);

Les deux exploitent l'interruption enfouie USART, en concurrence avec la transmission.
Les drapeaux FLAG_BYTE_RECEIVED et FLAG_STR_RECEIVED passent à 1 pour indiquer que les données
sont prêtes à être lues.
Si les données sont écrasées le flag FLAG_OVERRUN_RECEIVE passe à 1.

*/





#endif
