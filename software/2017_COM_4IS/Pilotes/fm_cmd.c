/*!
 * \file
 * \brief 	Bibliotheque de fonctions pour initialiser, envoyer et recevoir de données vers/de un module FM (RT606).
 * \author 	S. DI MERCURIO
 * \date   	septembre 2017
 * \version 1.0
 * \warning Necessite le package \ref fm_usart
 */
 
#include "stm32f10x.h"                  // Device header
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fm_usart.h"
#include "fm_cmd.h"

#ifdef __TESTS__
#define PRIVATE
#else
#define PRIVATE static
#endif /*  __TESTS__ */

PRIVATE long long fm_timeout = FM_NO_TIMEOUT; /* Par defaut, les fonctions de reception sont bloquantes infinies */ 

#define FM_STRING_LENGTH 30
PRIVATE char fm_string[FM_STRING_LENGTH];
PRIVATE char fm_string_ptr=0;
PRIVATE int fm_string_received=0;

PRIVATE void fm_callback_receive(char data);

#ifdef __DEBUG__
static void fm_debug_receive(char *str);
static void fm_debug_send(char *str);
#endif /* __DEBUG__ */

int fm_init(void)
{
	/* Initialise l'USART3 */
	fm_init_usart(9600, 10);
	
	/* Enregistre le callback pour la reception */
  fm_set_receive_callback(fm_callback_receive);
	
	/* Regle le timeout à 1S */
	fm_set_timeout(FM_TIMEOUT_1S);
	
	return FM_SUCCESS;
}

/**
 * Envoi un chaine de caractere
 *
 */
int fm_send_data(char *str)
{
int status = FM_SUCCESS;
	
	if (fm_send_data_usart(str, strlen(str)) == FM_USART_SUCCESS) 
	{	
#ifdef __DEBUG__
		fm_debug_send(str);
#endif /* __DEBUG__ */
		
		status=FM_SUCCESS;
	}
	else status=FM_BUSY;
	
	return status;
}
	
/**
 * Recupere la chaine recue
 *
 * \warning Le ptr str doit pointer sur un buffer déjà alloué, suffisament grand (max 100 caracteres)
 */
int fm_get_data(char *str)
{
volatile long long cnt=0;
int finish=0;
int status = FM_SUCCESS;
int i;
	
	while (finish!=1)
	{
		if (fm_timeout!= FM_NO_TIMEOUT)
		{
			cnt++;
			
			if (cnt >= fm_timeout){
				status=FM_TIMEOUT_EXPIRED;
				finish=1;
			}
		}
		
		if (fm_string_received == 1)
		{
			/* recopie de la chaine */
			if (str!=0)
			{
				for (i=0; i<strlen(fm_string); i++)
				{
					str[i] = fm_string[i];
				}
			}
			else return FM_INVALID_BUFFER;
			
			fm_string_received=0;
			finish=1;
			
#ifdef __DEBUG__
			fm_debug_receive(str);
#endif /* __DEBUG__ */
		}
	}
	
	return status;
}

void fm_set_timeout(long long timeout)
{
	fm_timeout = timeout;
}

/**
 * Verifie l'état de la porteuse
 * @return Etat de la porteuse
 * \see FM_CARRIER
 * \remark L'etat de la porteuse n'est pas quelque chose de très fiable. Perferez une detecttion de donnée valide par pattern
 */ 
int fm_get_carrier(void)
{
	if (fm_get_carrier_low_level()==1) return FM_CARRIER_DETECTED;
	else return FM_NO_CARRIER;
}

//==============================================================================================================
//                                    Private Functions
//                                     Ne pas modifier
//==============================================================================================================

PRIVATE void fm_callback_receive(char data)
{
  fm_string[fm_string_ptr]=(char)data;
	fm_string_ptr++;
	
	if (data==0x0D) /* Fin de chaine */
	{
		/* Met un zero à la fin de la chaine */
		fm_string[fm_string_ptr]=0x00;
		fm_string_ptr=0;
		fm_string_received=1;
	}
	
	if (fm_string_ptr>=FM_STRING_LENGTH) /* Depassement de la taille du buffer: on fini avec un 0 et on envoi*/
	{
		fm_string[FM_STRING_LENGTH-1]=0;
		fm_string_ptr=0;
		fm_string_received=1;
	}
}

#ifdef __DEBUG__
static void fm_debug_receive(char *str)
{
char *string_out;
int i;

	string_out = malloc(200);
	
	if (string_out!=0)
	{
		sprintf(string_out, "<= FM data received:\n\r\tLength = %i\n\r\tData = %s\n\r", strlen(str), str);

		for (i=0; i<strlen(string_out); i++)
	  {
			ITM_SendChar(string_out[i]);
		}
	
		free (string_out);
	}
}

static void fm_debug_send(char *str)
{
	char *string_out;
	int i;
	
	string_out = malloc (strlen(str) + strlen ("=> FM data send:\n\r\tLength = %i\n\r\tData = %s\n\r")+20);
	
	if (string_out !=0)
	{
		sprintf (string_out, "=> FM data send:\n\r\tLength = %i\n\r\tData = %s\n\r", strlen(str), str);
	
		for (i=0; i<strlen(string_out); i++)
		{
			ITM_SendChar(string_out[i]);
		}
	
		free (string_out);
  }
}
#endif /* __DEBUG__ */
