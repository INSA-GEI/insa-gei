

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 5.23
//  API pour les TP de Soft embarqué COM, 4IS
//  Nécessite :
//  => Drivers_STM32F103_107_Juin2017a.lib (=lib perso. Usart n'est pas utilisé)
//  => clock.c indispensable pour faire tourner les lib perso
//  => USART_rev.c même version que USART (inclus dans la lib) mais dont le .h a été partitionné
//             un nouveau .h, USART_User_Conf.h qui permet de paramétrer USART_rev.c.
//=======================================================================================


#ifndef _API_Com_TP_4IS_H__
#define _API_Com_TP_4IS_H__

/* =============================================
				GPIO
 ============================================= */
// les GPIO
#define Port_A 		0
#define Port_B 		1
#define Port_C 		2
#define Port_D 		3
#define Port_E		4
   

// Sens
#define INPUT 				'i'
#define OUTPUT 				'o'

// Techno pour pin en entrée (INPUT)
#define ANALOG 				0
#define INPUT_FLOATING 		1
#define INPUT_PULL_DOWN_UP 	2

// Techno pour pin en sortie (OUTPUT)
#define OUTPUT_PPULL 		0
#define OUTPUT_OPDRAIN 		1
#define ALT_PPULL 			2
#define ALT_OPDRAIN 		3

/*
Fonction permettant de configurer une Pin de manière individuelle.
Les paramètres possibles sont donnés ci-dessus.
Exemple : GPIO_Pin_Conf(Port_B, 4, INPUT, INPUT_FLOATING);

*/
void GPIO_Pin_Conf(int Port, int Broche, int Sens, int Techno);

/*
Fonction d'utilisation d'une PIN
Ex : GPIO_Pin_Set(Port_B,4);
La fonction GPIO_Pin_Read renvoie 0 si la PIN est au niveau logique 0
Sinon, c'est que l'entrée est au niveau logique 1
*/
void GPIO_Pin_Set(int Port,int Broche);
void GPIO_Pin_Clear(int Port,int Broche);
void GPIO_Pin_Toggle(int Port,int Broche);
short GPIO_Pin_Read(int Port,int Broche);




/* =============================================
				LES TIMERS
 ============================================= */
#define Timer_1 				1
#define Timer_2 				2
#define Timer_3 				3
#define Timer_4 				4
#define Timer_Systick		5

/*
La configuration globale d'un Timer (paramètres ci dessus) permet
de remettre à 0 le timer après le temps passé en paramètre (en µs).
Dans le détail, un Timer est un compteur cadencé à 72MHz. La fonction calcule
automatiquement le nombre à compter pour atteindre la durée voulue.
Par exemple 1s de durée implique que le Timer compte jusqu'à 71 999 999.
NB : Le Systick ne permet d'atteindre un peu plus d'1 seconde
     les Timers 1 à 4, permettent d'atteindre 59 sec environ
*/
void Timer_Global_Conf(int Timer, float Duree_us );
//_____________________________________________________________________________

/*
La fonction permet de déclencher une interruption lorsque la durée est écoulée.
Le paramètre Prio correspond à la priorité d'interruption. Elle va de 0 à 15.
0 est la priorité la plus élevée
Le second paramètre est un pointeur de fonction. Il suffit de lui donner le nom
de la fonction à éxécuter.
Par exemple :
Timer_IT_Conf(Timer_1, 3, IT_Tim);
permet de lancer la fonction void IT_TIM(void) lors des débordements du Timer 1.
La priorité d'interruption est 3.

*/
void Timer_IT_Conf(int Timer, char Prio, void (*IT_function) (void));
//_____________________________________________________________________________






/* =============================================
				L'ADC
 ============================================= */
#define		PA0 0
#define		PA1 1
#define		PA2 2
#define		PA3 3
#define		PA4 4
#define		PA5 5
#define		PA6 6
#define		PA7 7
#define		PB0 8
#define		PB1 9
#define		PC0 10
#define		PC1 11
#define		PC2 12
#define		PC3 13
#define		PC4 14
#define		PC5 15

/*
Fonction qui configure l'ADC sur la Pin souhaitée (16 possibles, voir ci-dessus).
La fonction est à lancer qu'une seule fois. Elle configure automatiquement la Pin
choisie en INPUT, ANALOG.
Si l'utilisateur veut pouvoir traiter une autre entrée, il suffit simplement de configurer
la Pin voulue en INPUT, ANALOG. Pas besoin de relancer la fonction de config générale.
NB : le temps de prélèvement de la tension est fixée à 10µs.
*/
void ADC_Global_Conf(int Pin);
//_____________________________________________________________________________

/*
C'est une fonction permettant :
- de lancer une conversion sur la Pin voulue
- d'attendre la fin de conversion
- de récupérer le résultat de conversion (entier de 0 à 4095) puisque l'ADC est un 12 bits.
*/
int ADC_Get_Value(int Pin);
//_____________________________________________________________________________






/* =============================================
				Liaison Serie USART

 Rappel sur les ressources sur le STM32F103RB 

 3 USART possibles :
 USART_1_TX = (TIM1_CH2) = PA9
 USART_1_RX = (TIM1_CH3) = PA10
 USART_2_TX = (TIM2_CH3) = PA2
 USART_2_RX = (TIM2_CH4) = PA3
 USART_3_TX = PB10
 USART_3_RX = PB11

 ============================================= */
#define USART_1 0
#define USART_2 1
#define USART_3 2

/*
La fonction configure l'USART voulue (USART_1 à USART_3)
Les Pin associées sont automatiquement configurées pour être reliées à l'USART voulue
Comme toutes les transmissions et réception se font sous interruption, il faut
donner une priorité. On conseille de la fixer à 0 de manière à la rendre la plus
prioritaire. Cela permet d'éviter de manquer un caractère (sauf s'il existe des 
contraintes tps réel très forte par ailleurs).
NB : les interruptions sont très courtes et non bloquantes (<3µs)
On peut régler le débit en Baud
NB : l'USART est obligatoirement en mode
- 8 bits
- sans contrôle de flux
- 1 bit de stop
- pas de parité.

Le fichier USART_User_Conf.h contient les paramètres que l'utilisateur peut ajuster en fonction
de l'application. A lire (et modifier si besoin).

NB: le fichier USART_rev.h contient en commentaire toutes les explications sur le fonctionnement
du module USART. 

*/
void Serial_Global_Conf(int Serial,int baudrate, char Prio_USART);
//_____________________________________________________________________________

/*
Fonctions de test des USART. Elles permettent notamment de savoir si un caractère vient d'être reçu
ou si une châine de caractère (String) vient d'être reçue. Ce dernier évènement survient si un caractère
spécial de fin de chaîne est reçu. Il est paramétrable pour chaque USART (voir  USART_User_Conf.h).
Par défaut le caractère de fin de chaîne est \r (= touche entrée)
Enfin on peut éventuellement contrôler que les transmissions / réceptions se déroulent bien avec
les deux dernières fonctions overrun.
*/
char Serial_Is_Str_Received(int Serial); 
char Serial_Is_Byte_Received(int Serial);
char Serial_Is_Overrun_Receive_Str(int Serial);
void Serial_Clear_Overrun_Receive_Str(int Serial);
//_____________________________________________________________________________


/*
Fonctions d'écriture et de lecture d'un caractère ou d'une châine.
Pour les chaînes de caractère, voici comment se passe une émission :

Couche applicative :	Suite des codes ASCII + 0
														||
														||   Serial_Put_String
														\/
Liaison physique :			suite des codes ASCII
(le 0, caractère nul est enlevé) Aucun caractère de chaîne n'est émis

La réception :
Couche applicative :	Suite des codes ASCII + 0
														/\
														||   Sur détection du caractère de terminaison
														||
Liaison physique :			suite des codes ASCII + Term_Car_USARTx (à choisir, par défaut \r ='Enter'=0x0D)

*/
char Serial_Get_Char(int Serial);
char * Serial_Get_String(int Serial); 
void Serial_Put_Char(int Serial,char Car); 
void Serial_Put_String(int Serial,char * pCar);
//_____________________________________________________________________________

#endif
