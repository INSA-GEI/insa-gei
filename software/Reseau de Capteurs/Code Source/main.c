/*
 * Reseau de capteur version 6
 *
 *   Avant de vous exciter � taper du code partout, remplissez les champs suivants:
 *
 *	 Nom du bin�me: 
 *   Groupe :
 *   Quelle note esperez vous (honnetement) avoir ?:          
 *
 * Les clefs de compilation suivantes peuvent etre utilis�es:
 *         _LCD_DEBUG_: Permet de desactiver l'ecran LCD et les temporisations bloquantes en simul�
 */

#include "mac.h" 	  /* <-- avec ca, vous avez accez au niveau materiel  */

#include <stdio.h>    /* <-- avec ca, vous avez acces � sprintf et sscanf */
#include <string.h>	  /* <-- et avec ca, vous avez acces � toutes les fonctions de gestion des chaines */

#include "Retarget.h"
/*********************** LISEZ CA AVANT DE PASSER A LA SUITE *************************
 *
 * le scanf n'existe pas: vous l'avez imagin�, oubliez le !
 * printf ecrit sur l'ecran LCD
 * si vous devez faire du formatage de chaine, utilisez la forme sprintf
 * si vous devez faire de l'analyse de chaine, utilisez la forme sscanf (ou autre chose, genre strtok)
 * si vous ne savez pas utiliser sprintf et/ou sscanf et/ou strtok et/ou autres fonctions de string.h, demandez � Google !
 *
 * Bonne chance
 *
 *************************************************************************************/

/*
 * int main (void)
 *
 * Point d'entr�e du programme
 *
 * Parametres:
 *     Entr�e: 
 *            aucun	
 *     Sortie:
 *            aucun
 *
 * Limitations:
 *     La fonction ne doit pas se terminer -> Boucle infinie
 */
int main (void)
{
uint16_t chenillard_led=0x100;
uint8_t compteur=0;
GPIO_InitTypeDef GPIO_Configuration;
	
	/* Initialisation de la couche materielle */
	MACInit(); 

	if (TypeCarte == STANDARD)
	{
		/* Affichage d'un petit message sympatique sur l'ecran */
		set_cursor(0,0);
		printf("Allez, au boulot");	 /* Rappel au passage: le printf ecrit sur l'ecran LCD */
	}

	fprintf (&__debug, "Un coucou du debug \n");
	ActiverITRXGSM();
	/* Boucle principale, infinie */
	
	GPIO_Configuration.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Configuration.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Configuration.GPIO_Pin = GPIO_Pin_8 + GPIO_Pin_9 + GPIO_Pin_10 + GPIO_Pin_11 + GPIO_Pin_12 + GPIO_Pin_13 + GPIO_Pin_14 + GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_Configuration);
	
	TIMEEnabled=1;
	while (1)
	{
		/* 
		 * Ce que vous devez/pouvez faire ici 
		 *
		 * 1 - Lire la doc dans le repertoire "doc" du projet
		 * 2 - Lire ou relire le support de TP qui vous a �t� donn�
		 * 3 - Demander conseil � Mr Auriol
		 * 4 - Reflechir
		 */
		if (TIME10msExpired()==TRUE)
		{
			compteur++;
			
			if (compteur>=10)
			{
				compteur=0;
				
				GPIO_Write(GPIOB, chenillard_led);
				chenillard_led= chenillard_led<<1;
				if (chenillard_led ==0) chenillard_led=0x100;
			}
		}
	}
}

/*
 * void DataReceived (unsigned char d, ID_UART uart)
 *
 * Fonction appel�e par les interruptions UART � chaque octet recu
 *
 * Parametres:
 *     Entr�e: 
 *            d (unsigned char): octet recu
 *            uart (ID_UART) : Identifiant de l'UART ayant recu l'octet	
 *     Sortie:
 *            aucun
 *
 * Limitations:
 *     Attention ! Vous etes sous interruption ici ... Pas de boucle infinie, et n'y restez pas des heures.
 */
void DataReceived (unsigned char d, ID_UART uart)
{
	switch (uart)
	{
	case XBEE:
		/*
		 * TODO: rajoutez le traitement a faire lors de la reception via XBEE 
		 */
		break;
	case FM:
		/*
		 * TODO: rajoutez le traitement a faire lors de la reception via FM 
		 */
		break;
	case GSM:
		/*
		 * TODO: rajoutez le traitement a faire lors de la reception via GSM 
		 */
		d=d+1;
		SendData(d, GSM);
		break;
	}
}							
