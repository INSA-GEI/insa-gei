/*******************************************************************************
* File Name          : misc.c
* Author             : S DI Mercurio
* Version            : V5.0.0
* Date               : 15/07/2011
* Description        : Ce fichier fourni tout les services autres que ceux des uart
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
#include "stm32f10x.h"
#include "configuration.h"
#include "mac.h"

/******************************************************************************
 * 
 * Global
 *
 ******************************************************************************/

TYPE_NOEUD TypeNoeud=INDEFINI;
TYPE_CARTE TypeCarte=CARTE_INCONNUE;

/*
 * void MACInit(void)
 *
 * Initialisation de la couche MAC (GPIO, UART, Timer, IT, ...)
 *
 * Parametres:
 *     Entr�e: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void MACInit(void)
{
	/* Init du micro et des peripheriques */
	Configuration();
	TurnOffFM();

	TIMEInit();

	if (GPIOC->IDR & (1<<10)) /* si PC10 == '1', la carte est une gateway, sinon une carte standard	*/
	{
		TypeCarte = GATEWAY;
	}
	else
	{
		TypeCarte = STANDARD;
	}

	if (TypeCarte == STANDARD) /* Init de l'ecran, si la carte est une carte standard */
	{
		lcd_init();	
		lcd_clear();
	}
}

/******************************************************************************
 * 
 * Module RT606
 *
 ******************************************************************************/

/* 
 * Definition des identifiants des differents mode de fonctionnement du RS606
 */
typedef enum 
{
	RT606_OFF, 
	RT606_TX, 
	RT606_RX, 
	RT606_TX_LOOPBACK
} RT606_MODE;


int etat_RT = RT606_OFF;

/*
 * void RS606SetMode(RS606_MODE mode)
 *
 * configure le module RT606 dans l'un de ses 4 modes possible
 *
 * Parametres:
 *     Entr�e: 
 *            mode (RT606_MODE): mode de fonctionnement du RT606
 *						RT606_OFF = Le module est inactif
 *					    RT606_TX  = Le module est en emission 
 *						RT606_RX  = le module est en reception 
 *						RT606_TX_LOOPBACK = le module est en emission et reception: les donn�es envoy�es sont reboucl�es en reception
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void RT606SetMode(RT606_MODE mode)
{
volatile int temp;

	if (mode != etat_RT) /* Inutile de modifier la config du module RT606 si c'est la meme que celle deja en cours */
	{
		/* Met les lignes PB9 (TX_CMD) PB8 (RX_CMD) en sortie  */
	
		/* remet PB8 et PB9 � 0 */
		GPIOB->BRR = GPIO_Pin_8 | GPIO_Pin_9;
	
		switch (mode)
		{
			case RT606_OFF:
				/* Rien a faire  */
				break;
			case RT606_TX:
				GPIOB->BSRR = GPIO_Pin_9; // met la ligne TX_CMD � 1 */ 
				break;
			case RT606_RX:
				GPIOB->BSRR = GPIO_Pin_8; // met la ligne RX_CMD � 1 */	
				break;
			case RT606_TX_LOOPBACK:
				GPIOB->BSRR = GPIO_Pin_8 | GPIO_Pin_9; /* Met les lignes TX_CMD et RX_CMD � 1 */
				break;
		}
	
		etat_RT = mode;
	}
}

/*
 * void TurnOnTXFM(void)
 *
 * configure le module RT606 en transmission (emission d'une porteuse)
 *
 * Parametres:
 *     Entr�e: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOnTXFM(void)
{
	RT606SetMode(RT606_TX);
}

/*
 * void TurnOnRXFM(void)
 *
 * configure le module RT606 en reception (attention au bruit !!)
 *
 * Parametres:
 *     Entr�e: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOnRXFM(void)
{
	RT606SetMode(RT606_RX);
}

/*
 * void TurnOffFM(void)
 *
 * Desactive le module RT606 (ni en reception, ni en transmission)
 *
 * Parametres:
 *     Entr�e: 
 *            rien
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TurnOffFM(void)
{
	RT606SetMode(RT606_OFF);
}

/*
 * CDSTATE_ENUM GetCDState(void)
 *
 * Lit l'etat de la porteuse FM
 *
 * Parametres:
 *     Entr�e: 
 *            rien
 *
 *     Sortie:
 *            Etat de la porteuse 
 *
 * Limitations:
 *     Aucune
 */
CDSTATE_ENUM GetCDState(void)
{
CDSTATE_ENUM state=NOISE;

	if (etat_RT == RT606_OFF)
	{
		/* qq soit l'etat de CD, c'est du bruit*/
		state = NOISE;
	}

	if (etat_RT == RT606_TX)
	{
		/* qq soit l'etat de CD, c'est OK pour emettre */
		state = DATA_AVAILABLE;
	}

	if (etat_RT == RT606_RX)
	{
		if (GPIOB->IDR & GPIO_Pin_7)
		{
			/* Pas de CD -> Bruit */
			state = NOISE;
		}
		else
		{
			/* CD detect�e -> Des donn�es valables sont receptionables */
			state = DATA_AVAILABLE;
		}
	}

	return state;
}

/******************************************************************************
 * 
 * Module GPIO
 *
 ******************************************************************************/

/*
 * Variables pour gerer l'antirebond (fonction GPIOButton)
 */

int GPIODebounceBuffer[2][2]; /* Tableau pour l'antirebond, � 2 lignes (pour les deux boutons)
                                 et 2 colonnes (pour les deux etats compar�s) */
int GPIOButonLastState[2]= {BUTTON_RELEASED,BUTTON_RELEASED};    /* Tableau pour memoriser l'etat precedent des boutons et detecter les
                                 transitions appuy�->relach� et relach�->appuy� */
/*
 * int GPIOGetState (int port)
 *
 * Retourne l'etat courant d'un GPIO (sans antirebond)
 *
 * Parametres:
 *     Entr�e: 
 *            port (int): identifiant du port
 *                	BUTTON_TAMP => pour lire l'etat du bouton TAMP
 *					BUTTON_WKUP => Pour lire l'etat du bouton WKUP 
 *     Sortie:
 *            int: etat du port (0 ou different de 0) ou -1 si l'identifiant du bouton est invalide 
 *
 * Limitations:
 *     Aucune
 */
int GPIOGetState (int port)
{
int state =0;

	switch (port)
	{
	case BUTTON_TAMP:
		state = (GPIOC->IDR & GPIO_Pin_13);
		break;
	case BUTTON_WKUP:
		state = (GPIOA->IDR & GPIO_Pin_0);
		break;
	default:
		state = -1;	
	}	

	return state;
}

/*
 * int GPIOButton (int button)
 *
 * Retourne l'etat d'un bouton (avec antirebond)
 * 
 * Parametres:
 *     Entr�e: 
 *            button (int): identifiant du bouton
 *                	BUTTON_TAMP => pour lire l'etat du bouton TAMP
 *					BUTTON_WKUP => Pour lire l'etat du bouton WKUP 
 *     Sortie:
 *            int: etat du bouton (PRESSED, JUST_PRESSED, RELEASED, JUST_RELEASED) ou -1 si l'identifiant du bouton est invalide
 *
 * Limitations:
 *     Pour que l'antirebond fonctionne correctement, cette fonction ne doit pas etre appel�e � une
 *     periode inferieur � 50ms.
 */
int GPIOButton (int button)
{
	/* Le dernier echantillon devient le plus ancien, et l'etat actuel du bouton est echantillonn� */
	GPIODebounceBuffer[button][0] = GPIODebounceBuffer[button][1];
	GPIODebounceBuffer[button][1] = GPIOGetState(button);

	/* Si l'identifiant du bouton est invalide, renvoi -1 */
	if (GPIODebounceBuffer[button][1]==-1) return -1;
	 
	if (GPIODebounceBuffer[button][0] == GPIODebounceBuffer[button][1])	/* Le bouton est stable */
	{
		if (GPIOButonLastState[button] == GPIODebounceBuffer[button][1]) /* le bouton n'a pas chang� d'etat depuis la derniere fois */
		{
			if (GPIODebounceBuffer[button][1] == 0) return BUTTON_PRESSED;
			else return BUTTON_RELEASED;
		}
		else /* Le bouton vient de changer d'etat */
		{
			GPIOButonLastState[button] = GPIODebounceBuffer[button][1]; /* Au memorise l'etat courant pour detecter un changement d'etat
			                                                               la prochaine fois */
			
			/* Transition bouton relach�-> appuy� */ 
			if (GPIODebounceBuffer[button][1]==0) return BUTTON_JUST_PRESSED;
			else return BUTTON_JUST_RELEASED;
		} 				
	}
	else 
	{
		if (GPIOButonLastState[button] == 0) return BUTTON_PRESSED; /* Le bouton n'est pas stable, on renvoi l'etat stable precedent */
		else return BUTTON_RELEASED;
	}

/* Suppression du warning qui indique que la fonction se termine sans return. ARMCC n'est pas tr�s fut� sur ce coup l� ... */
#pragma diag_remark	940
}

/******************************************************************************
 * 
 * Module Timer
 *
 ******************************************************************************/

TIME_CALLBACK TIMECallback = 0;
int TIMEEnabled;
int TIME10msevent;

/*
 * void TIMEInit (TIME_CALLBACK callback)
 *
 * Initialise le timer periodique et permet de definir une fonction callback appel�e toute les 10ms
 * A la fin de cette fonction, le timer est initialis� mais inactif. Pour le demarrer, il faut mettre
 * la variable TIMEEnabled � 1
 *
 * Parametres:
 *     Entr�e: 
 *            callback (TIME_CALLBACK): fonction a appeler toutes les 10ms ou NULL 
 *     Sortie:
 *            Aucun
 *
 * Limitations:
 *     Aucune
 */
void TIMEInit (void)
{
	TIMEEnabled =0;
	TIME10msevent=0;
}

/*
 * void TIMESetCallback(TIME_CALLBACK callback)
 *
 * Accroche un callback � l'IT 10 ms du timer
 *
 * Parametres:
 *     Entr�e: 
 *            callback (TIME_CALLBACK): un pointeur sur une fonction de type (void) (*TIME_CALLBACK)(void)
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void TIMESetCallback(TIME_CALLBACK callback)
{
	TIMECallback = callback;
}

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
int TIME10msExpired(void)
{ 
int status = FALSE;

	if (TIME10msevent == 1)
	{
		TIME10msevent =0;
		status = TRUE;
	}

	return status;
}

/*
 * void TIMEWait1ms(void)
 *
 * Cree un delai d'1 ms.
 *
 * Parametres:
 *     Entr�e: 
 *            Aucun
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Attention: fonction bloquante ! Ne pas utiliser sous interruption.
 */
void TIMEWait1ms(void)
{
#define _MAX_1MS_ 9000L

#ifndef _LCD_DEBUG_
volatile unsigned int i;
			   
	for (i=_MAX_1MS_; i!=0; i--); 
#endif /* _LCD_DEBUG_ */
}

/*
 * void TIMEWaitxms(void)
 *
 * Cree un delai de x ms.
 *
 * Parametres:
 *     Entr�e: 
 *            ms (int): dur�e en ms.
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Attention: fonction bloquante ! Ne pas utiliser sous interruption.
 */
void TIMEWaitxms(int ms)
{
volatile int cpt;

	for (cpt=0; cpt<ms; cpt++)
	{
		TIMEWait1ms();	
	}
}

/*
 * void SysTick_Handler(void)
 *
 * routine d'interruption du timer
 *
 * Parametres:
 *     Entr�e: 
 *            Aucun
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Aucune a par que c'est une routine d'interruption
 */
void SysTick_Handler(void)
{
	if (TIMEEnabled)
	{
		TIME10msevent=1;

		if (TIMECallback != 0x0)
		{
			TIMECallback();
		}
	}
}
