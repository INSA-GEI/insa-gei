#include "stm32f10x.h"
#include "USART_User_Conf.h"

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 28/06
//  Processeur : STM32F103RB
//  Logiciel dev : �Vision 5.23
// 
//=======================================================================================


#ifndef _USART_rev_H__
#define _USART_rev_H__


/* 
 ======================================================================================
|
|								GENERALITE
|   Compl�ter le fichier USART_User_Conf.h pour param�trer la COM
|   taille des buffer, caraxct�res sp�ciaux...
|
 ======================================================================================
 
La lib fournit les fonctionnalit�s de bas niveau de l'UART mais aussi des fonctions
de plus haut niveau qui permettent la manipulation de cha�ne de caract�re.

* les 3 USART sont programmables
* les IO sont configur�es dans la fonction d'initialisation de l'USART

=> USART_1_TX = TIM1_CH2 = PA9
=> USART_1_RX = TIM1_CH3 = PA10
=> USART_2_TX = TIM2_CH3 = PA2
=> USART_2_RX = TIM2_CH4 = PA3
=> USART_3_TX = PB10
=> USART_3_RX = PB11
 
Le d�bit de chacune des transmission est param�trable.
Par contre les autres configurations sont fig�es :
- pas de parit�
- 8 bits
- pas de contr�le de flux mat�riel
- 1 bit de stop
 
Lors de l'�mission d'une cha�ne de caract�res, il est pr�vu un nombre limite que le 
user peut param�trer et ce pour chaque USART.
M�me chose pour les cha�nes de r�ception. Cela limite la taille des buffers en RAM.

Remarque importante : Afin d'�conomiser un maximum de tps CPU, les fonctions
d'�mission et de r�ception fonctionnent sous interruption (pour chaque caract�re).
Les IT sont non bloquantes. Le temps maxi requis pour chacune 
des IT est estm� � moins de 3us.
Les 3 USARTs peuvent donc cohabiter sans aucun soucis en full duplex.
C'est au user dans son code principal � bien utiliser les fonctions
de polling ( notamment Is_Str_Received(USART)  )


 
*/


//            !!! Si un bourrage s'est produit en r�ception 
//            on tombe dans une boucle sans fin. Utile pour le d�bug.





/* 
 ======================================================================================
|
|								L'EMISSION
|
 ======================================================================================
 Deux fonctions permettent l'�mission :
 => PutChar
 => Put_String
 
 PutChar n'utilise aucune interruption. La fonction contr�le avant l'�mission qu'auncune
 autre est en cours. Auquel cas il y a attente.
 Il est donc possible d'envoyer une cha�ne avec une simple boucle sans se pr�occuper du
 bourrage. Dans ce cas l'�mission occupe � 100% le CPU
 
 PRINCIPE DE LA FONCTION Put_String
 A l'appel, le premier caract�re est �mis (si non null) � la suite de quoi on autorise les
 interruptions en fin d'�mission de caract�re.
 Ainsi, le second caract�re et ceux qui suivent sont �mis sous interruption 
 (lib�ration CPU).
 La fin d'�mission est effective lorsque le caract�re nul est trouv� (non �mis)
 ou lorsque la cha�ne d�passe la longeur maximal (voir define).
 
***********************************************
 Plus en d�tail : Put_String
***********************************************


Couche applicative :	Suite des codes ASCII + 0
														||
														||   Put_String
														\/
Liaison physique :			suite des codes ASCII

Exemple :
Chaine � transmettre = azerty.
En dur : 0x61 0x7A 0x65 0x72 0x74 0x79 0x0 ; Le 0 traduit la fin de la cha�ne. C'est universel.

La fonction Put_String se contente d'�mettre  0x61 0x7A 0x65 0x72 0x74 0x79
Elle aura d�tect� le \0 et ne l'aura pas �mis. 

Pour �tre dans les clous et envoyer selon l'OS un ou plusieurs caract�res de contr�le:
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  (\n = LF = 0xA et \r = CR = 0xD)
ce sera � la couche service de dessus d'envoyer deux cha�nes :
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

Deux fonctions permettent la r�ception :
CHAR GET_CHAR(USART_TYPEDEF *USART);
CHAR * GET_STRING(USART_TYPEDEF *USART);

Les deux exploitent l'interruption enfouie USART, en concurrence avec la transmission.

On peut travailler en polling en utilisant les fonctions :
Is_Byte_Received
Is_Str_Received

La fonction Is_Overrun_Receive_Str permet de savoir si on a loup� une ou plusieurs
cha�ne. Si Debugg_Bourrage est d�fini, alors la fonction ne sert � rien. Si un bourrage
est d�tect�, on part directement dans un while(1) qui plante le pgm (utile � la mise au pt).




Tous les protocoles finissent par CR mais sait-on jamais... MAC OS en \n ?
La d�tection du dernier caract�re sera donc au choix.
Exemple :
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  donnera 
0x61 0x7A 0x65 0x72 0x74 0x79 \n \0 en dur
Eventuellement en couche service on pourra ou non supprimer \n si le protocole place
en fin de cha�ne \n \r

OPTION EN RECEPTION:
le mode ECHO permet de renvoyer tous les caract�res trait�s.

***********************************************
 Plus en d�tail : Scrutation vs Interruption
***********************************************

Couche applicative :	Suite des codes ASCII + 0
														/\
														||   Sur d�tection du caract�re de terminaison
														||
Liaison physique :			suite des codes ASCII + Term_Car_USARTx (� choisir, par d�faut \r ='Enter'=0x0D)


Les deux fonctions :
void Init_IT_Serial_Receive_Byte(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Receive_Str(USART_TypeDef *USART, void (*IT_function) (void));
Les callback sont lanc�s respectivement � l'arriv�e d'un byte ou d'un string.

ATTENTION :
Si les callback sont utilis�s, il est primoridal de ne pas y rester pour ne pas bloquer l'IT USART enfouie.
A plus forte raison, il est INTERDIT dans un callback d'appeler une fct em/rec USART car l'IT USART s'interrompt => plantage.
CONSEIL => utiliser cette fonctionnalit� que pour r�cup�rer un flag ou une donn�e � la vol�e,y rester le moins possible.
Mieux => ne pas utiliser les callback mais utiliser un polling r�gulier sur l'appli globale, genre OS.



 */






//=======================================================================================
//                      Configuration  
//=======================================================================================

void Init_USART(USART_TypeDef *USART,int baudrate, char Prio_USART); // OK
/*___________________________________________________________________________________________

Permet de pr�parer n'importe laquelle des 3 USART avec un d�bit donn� (8bits, 1 stop, pas de parit�)
Utilise une IT enfouie qui permet la transmission en IT et la r�ception en IT.
___________________________________________________________________________________________*/


void Init_IT_Serial_Receive_Byte(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Receive_Str(USART_TypeDef *USART, void (*IT_function) (void));
void Init_IT_Serial_Transmit_Str(USART_TypeDef *USART, void (*IT_function) (void));
/*___________________________________________________________________________________________

Ces trois fonctions permettent de renvoyer au user une IT pour indiquer l'�v�nement de r�ception 
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
G�r� automatiquement :
- mis � un automatiquement � l'appel des deux fonctions put
- mis � 0 automatiquement lors de la fin des processus put

Gestion du flag Flag_Overrun_Transmit :
- mis � un automatiquement � l'appel des deux fonctions put, si une transmission est en cours
- mis � 0 par la fonction RESET_VAR_USART, plus pertinent, par la fonction VOID CLEAR_OVERRUN_TRANSMIT
___________________________________________________________________________________________*/

char Is_Str_Received(USART_TypeDef *USART); // OK
char Is_Byte_Received(USART_TypeDef *USART); //OK
char Is_Overrun_Receive_Str(USART_TypeDef *USART);
void Clear_Overrun_Receive_Str(USART_TypeDef *USART);

/*___________________________________________________________________________________________

Gestion des Flag de r�ception :
Ils sont remis � 0 soit au reset g�n�ral, soit lors du get
le flag_Byte_Received passe � 1 � chaque r�ception de byte.
Le flag_Str_Received passe � 1 en r�ception dernier caract�re.


Le flag FLAG_OVERRUN_RECEIVE_STR
Mis � 1 en fin de r�ception string si le flag Str received �tait encore � 1.
Remis � 0 au reset g�n�ral et par la fonction CLEAR_OVERRUN_RECEIVE_STR

___________________________________________________________________________________________*/


//=======================================================================================
// 							  Utilisation, rem : // OK signifie valid� en reel serial 1
//=======================================================================================

char Get_Char(USART_TypeDef *USART); //OK
char * Get_String(USART_TypeDef *USART); // OK
int Get_Len_Receiv_String(USART_TypeDef *USART); //OK


void Put_Char(USART_TypeDef *USART,char Car); // OK
void Put_String(USART_TypeDef *USART,char * pCar); // OK
int Get_Len_Transm_String(USART_TypeDef *USART); //OK

/*
Les fonctions d'�mission
=======================================================================================
La fonction PUT_STRING s'arr�te sous deux conditions et ajoute un caract�re de fin (voir define .h):
- rencontre du caract�re null
ou
- atteinte du nombre max de caract�res �mis (voir define).
Elle utilise l'IT USART enfouie
La fonction  Put_Char n'utilise aucune IT. L'�mission est directe.


INT GET_LEN_TRANSM_STRING(USART_TYPEDEF *USART);
Elle permet de r�cup�rer si besoin la longueur du caract�re �mis (caract�re null non compris).
!ATTENTION ! Cette longueur est mise � jour lorsque la transmission est finie. Il faut donc
ins�rer une attente fin de transmission :
	while(Flag_Transmission_Busy(USART1)==1);
	LEM=Get_Len_Transm_String(USART1);	

Les fonctions de r�ception
=======================================================================================
Deux fonctions :
CHAR GET_CHAR(USART_TYPEDEF *USART);
CHAR * GET_STRING(USART_TYPEDEF *USART);

Les deux exploitent l'interruption enfouie USART, en concurrence avec la transmission.
Les drapeaux FLAG_BYTE_RECEIVED et FLAG_STR_RECEIVED passent � 1 pour indiquer que les donn�es
sont pr�tes � �tre lues.
Si les donn�es sont �cras�es le flag FLAG_OVERRUN_RECEIVE passe � 1.

*/





#endif
