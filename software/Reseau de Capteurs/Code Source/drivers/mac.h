/*******************************************************************************
* File Name          : mac.h
* Author             : S DI Mercurio
* Version            : V5.0.0
* Date               : 15/07/2011
* Description        : Ce fichier d'en-tete fourni toutes les definitions necessaires
*                      pour acceder au niveau MAC du projet reseau de capteur
*                     
********************************************************************************
* Thoses drivers are free software; you can redistribute them and/or
* modify them under the terms of the GNU General Public
* License as published by the Free Software Foundation;
* either version 2, or (at your option) any later version.
*
* Thoses drivers are distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*******************************************************************************/

#ifndef __MAC_H__
#define __MAC_H__

#include "stm32f10x.h"

#include "lcd.h"

/******************************************************************************
 * 
 * Global
 *
 ******************************************************************************/

#ifndef FALSE
	#define FALSE 0
	#define TRUE 	1
#endif /* FALSE */
/*
 * Definition des identifiants des differents UART
 * A utiliser avec les fonctions DataReceived et SendData
 */
typedef enum 
{
	XBEE,
	GSM,
	FM
} ID_UART;
 
/*
 * Definition des identifiants des differentes configuration de la carte
 */
typedef enum 
{
	INDEFINI,
	EXTREMITE,
	PASSERELLE_FM_XBEE,
  PASSERELLE_XBEE_XBEE,
	PASSERELLE_XBEE_GSM
} TYPE_NOEUD;

/*
 * Identifiant de la configuration de la carte
 * A fixer au debut du programme: vaut "INDEFINI" sinon
 */
extern TYPE_NOEUD TypeNoeud;

/*
 * Definition du type de carte sur laquelle s'execute le code
 */
typedef enum
{
	CARTE_INCONNUE,
	GATEWAY,
	STANDARD
} TYPE_CARTE;

/*
 * Identifiant du type de carte
 * Initialisé lors de l'initialisation de la carte
 */
extern TYPE_CARTE TypeCarte;

/*
 * void MACInit(void)
 *
 * Initialisation de la couche MAC (GPIO, UART, Timer, IT, ...)
 *
 * Parametres:
 *     Entrée: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void MACInit(void);


/******************************************************************************
 * 
 * Module RT606
 *
 ******************************************************************************/

/*
 * Definition des etats possible pour la variable CDState
 */
typedef enum 
{
	DATA_AVAILABLE,
	NOISE
} CDSTATE_ENUM;

/*
 * void TurnOnTXFM(void)
 *
 * configure le module RT606 en transmission (emission d'une porteuse)
 *
 * Parametres:
 *     Entrée: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOnTXFM(void);

/*
 * void TurnOnRXFM(void)
 *
 * configure le module RT606 en reception (attention au bruit !!)
 *
 * Parametres:
 *     Entrée: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOnRXFM(void);

/*
 * void TurnOffFM(void)
 *
 * Desactive le module RT606 (ni en reception, ni en transmission)
 *
 * Parametres:
 *     Entrée: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOffFM(void);

/*
 * CDSTATE_ENUM GetCDState(void)
 *
 * Lit l'etat de la porteuse FM
 *
 * Parametres:
 *     Entrée: 
 *            rien
 *
 *     Sortie:
 *            Etat de la porteuse 
 *
 * Limitations:
 *     Aucune
 */
CDSTATE_ENUM GetCDState(void);

/******************************************************************************
 * 
 * Module GPIO
 *
 ******************************************************************************/

/* 
 * Definition des identifiants des GPIOs et boutons
 *
 * Si d'autres boutons ou GPIO sont necessaires, les rajouter dans la liste
 */
enum 
{
	BUTTON_TAMP=0,	  /* Fixé à zero pour pouvoir s'en servir d'index dans le tableau GPIODebounceBuffer */
	BUTTON_WKUP
};

/*
 * Definition des etats de sortie pour GPIOButton
 */
enum
{
	BUTTON_PRESSED,
	BUTTON_JUST_PRESSED,
	BUTTON_RELEASED,
	BUTTON_JUST_RELEASED
};

/*
 * int GPIOButton (int button)
 *
 * Retourne l'etat d'un bouton (avec antirebond)
 * 
 * Parametres:
 *     Entrée: 
 *            button (int): identifiant du bouton
 *                	BUTTON_TAMP => pour lire l'etat du bouton TAMP
 *					BUTTON_WKUP => Pour lire l'etat du bouton WKUP 
 *     Sortie:
 *            int: etat du bouton (PRESSED, JUST_PRESSED, RELEASED, JUST_RELEASED) ou -1 si l'identifiant du bouton est invalide
 *
 * Limitations:
 *     Pour que l'antirebond fonctionne correctement, cette fonction ne doit pas etre appelée à une
 *     periode inferieur à 50ms.
 */
int GPIOButton (int button);

/*
 * int GPIOGetState (int port)
 *
 * Retourne l'etat courant d'un GPIO (sans antirebond)
 *
 * Parametres:
 *     Entrée: 
 *            port (int): identifiant du port
 *                	BUTTON_TAMP => pour lire l'etat du bouton TAMP
 *					BUTTON_WKUP => Pour lire l'etat du bouton WKUP 
 *     Sortie:
 *            int: etat du port (0 ou different de 0) ou -1 si l'identifiant du bouton est invalide 
 *
 * Limitations:
 *     Aucune
 */
int GPIOGetState (int port);

/******************************************************************************
 * 
 * Module Timer
 *
 ******************************************************************************/

/* 
 * Creation du type TIME_CALLBACK pour typer le parametre de la fonction TIMEInit 
 */
typedef void (*TIME_CALLBACK) (void);

/*
 * Variables exportées (publiques)
 */
extern int TIMEEnabled; /* Active ou desactive le timer periodique (10 ms)
                             0 (defaut): Le timer periodique est désactivé
							 1 : Le timer fonctionne */

/*
 * void TIMEInit (TIME_CALLBACK callback)
 *
 * Initilise le timer periodique et permet de definir une fonction callback appelée toute les 10ms
 * A la fin de cette fonction, le timer est initialisé mais inactif. Pour le demarrer, il faut mettre
 * la variable TIMEEnabled à 1
 *
 * Parametres:
 *     Entrée: 
 *            callback (TIME_CALLBACK): fonction a appeler toutes les 10ms ou NULL 
 *     Sortie:
 *            Aucun
 *
 * Limitations:
 *     Attention: le callback est appelé sous interruption ! Ne faire que des traitements courts avec.
 */
void TIMEInit (void);

/*
 * void TIMESetCallback(TIME_CALLBACK callback)
 *
 * Accroche un callback à l'IT 10 ms du timer
 *
 * Parametres:
 *     Entrée: 
 *            callback (TIME_CALLBACK): un pointeur sur une fonction de type (void) (*TIME_CALLBACK)(void)
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TIMESetCallback(TIME_CALLBACK callback);

/*
 * int TIME10msExpired(void)
 *
 * Indique si une periode de 10ms s'est ecoulée sur le timer periodique
 *
 * Parametres:
 *     Entrée: 
 *            Aucun
 *     Sortie:
 *            int : vaut 1 si 10ms s'est ecoulée, 0 sinon 
 *
 * Limitations:
 *     Aucune
 */
int TIME10msExpired(void);

/*
 * void TIMEWait1ms(void)
 *
 * Cree un delai de x ms.
 *
 * Parametres:
 *     Entrée: 
 *            ms (int): durée en ms.
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Attention: fonction bloquante ! Ne pas utiliser sous interruption.
 */
void TIMEWaitxms(int ms);

/******************************************************************************
 * 
 * Module USART
 *
 ******************************************************************************/

/*
 * void SendData(unsigned char d, ID_UART uart) 
 *
 * Envoi un caractere sur l'UART indiquée
 *
 * Parametres:
 *     Entrée:
 *	          d (unsigned char): caractere a envoyer	 
 *            uart (ID_UART): identifiant de la liaison serie	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Attention, fonction bloquante le temps de l'envoi du caractere
 */
void SendData(unsigned char d, ID_UART uart);

/*
 * void ActiverITRXXBEE (void)
 *
 * Active les interruptions en reception pour le XBEE
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void ActiverITRXXBEE (void);

/*
 * void DesactiverITRXXBEE (void)
 *
 * Desactive les interruptions en reception pour le XBEE
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void DesactiverITRXXBEE (void);

/*
 * void ActiverITRXFM (void)
 *
 * Active les interruptions en reception pour le RT606
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void ActiverITRXFM (void); 

/*
 * void DesactiverITRXFM (void)
 *
 * Desactive les interruptions en reception pour le RT606
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void DesactiverITRXFM (void);

/*
 * void ActiverITRXGSM (void)
 *
 * Active les interruptions en reception pour le GSM
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void ActiverITRXGSM (void);

/*
 * void DesactiverITRXGSM (void)
 *
 * Desactive les interruptions en reception pour le GSM
 *
 * Parametres:
 *     Entrée: 
 *            rien	
 *     Sortie:
 *            rien
 *
 * Limitations:
 *     Aucune
 */
void DesactiverITRXGSM (void);

#endif /* __MAC_H__ */				
