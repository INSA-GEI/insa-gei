/*!
 * \brief 	Xbee Commandes
 * \details Bibliotheque pour piloter les XBEE en mode API
 * \author 	S. DI MERCURIO
 * \date   	2017
 * \version 1.0
 * \warning Necessite le package xbee_usart
 */

#include "stm32f10x.h"

#include "Xbee_usart.h"
#include "Xbee_cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __TESTS__
#define PRIVATE
#else
#define PRIVATE static
#endif /*  __TESTS__ */

#define XBEE_MODE_AT	0
#define XBEE_MODE_API	1

PRIVATE const char *XBEE_ENTER_AT="+++";
PRIVATE const char *XBEE_AT_OK="OK";

PRIVATE long long xbee_timeout = XBEE_NO_TIMEOUT; /* par defaut, les fonctions de reception sont bloquantes infinies */ 

PRIVATE int xbee_mode= XBEE_MODE_AT;

#define XBEE_STRING_LENGTH 30
PRIVATE char xbee_string[XBEE_STRING_LENGTH];
PRIVATE char xbee_string_ptr=0;
PRIVATE int xbee_string_received=0;

#define XBEE_FRAME_LENGTH 120
PRIVATE int xbee_frame_received=0;
PRIVATE char xbee_frame_buffer[XBEE_FRAME_LENGTH];
PRIVATE int xbee_frame_counter=1;

/* Variable necessaire pour la fonction xbee_callback_receive */
PRIVATE int xbee_api_state_machine=0;
PRIVATE int xbee_api_length=0;
PRIVATE int xbee_api_escape_flag=0;

typedef struct{
	int address;
	int apiCmd;
	int rssi;
	char data[100];
	int data_length;
	int options;
	int status;
} ST_XBEE_FRAME;


PRIVATE void xbee_callback_receive(char data);
PRIVATE int xbee_get_string(char *str);
PRIVATE int xbee_get_frame(ST_XBEE_FRAME *frame);
PRIVATE char xbee_compute_checksum(char *buffer, int length);
PRIVATE int xbee_unpack_frame(ST_XBEE_FRAME *frame);
PRIVATE int xbee_escape_chars(char* buffer_out, char* buffer_in, int length_in);

#ifdef __DEBUG__
static void xbee_debug_receive(void);
static void xbee_debug_send(int address, char *str);
#endif /* __DEBUG__ */

int xbee_init(int channel, int pan_id, int address)
{
	/* Verification des parametres d'entrée */
	if ((channel <0x0B) || (channel >0x1A)) return XBEE_INVALID_CHANNEL;
	if ((pan_id <0) || (pan_id >0xFFFF)) return XBEE_INVALID_PANID;
	if ((address <0) || (address >0xFFFF)) return XBEE_INVALID_ADDRESS;
	
	/* Initialise l'USART2 */
	xbee_init_usart(9600, 3);
	
	/* Enregistre le callback pour la reception */
	xbee_set_callback(xbee_callback_receive);
	
#ifndef __TESTS__ /* pour les tests, pas d'init XBEE */
	/* Regle le timeout en reception à 3s, le temps de passer les commandes AT */
	xbee_set_timeout(3*XBEE_TIMEOUT_1S);
	
	/* Initialisation du XBEE en mode API 2, avec le channel, le pan_id et l'adresse de la carte passés en parametre*/
	/* Entre en mode commande AT */
	xbee_mode= XBEE_MODE_AT;
	xbee_send_data_usart((char*)XBEE_ENTER_AT, strlen(XBEE_ENTER_AT));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string,XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
	
	/* Regle le canal d'emission (ATCH) */
	sprintf(xbee_string, "ATCH=%02X\r",channel);
	xbee_send_data_usart(xbee_string, strlen (xbee_string));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string, XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
	
	/* Regle le PAN ID (ATID) */
	sprintf(xbee_string, "ATID=%04X\r",pan_id);
	xbee_send_data_usart(xbee_string, strlen (xbee_string));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string,XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
	
	/* Regle l'adresse locale (ATMY) */
	sprintf(xbee_string, "ATMY=%04X\r",address);
	xbee_send_data_usart(xbee_string, strlen (xbee_string));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string,XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
	
	/* Passe en mode APi 2) */
	sprintf(xbee_string, "ATAP=2\r");
	xbee_send_data_usart(xbee_string, strlen (xbee_string));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string,XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
	
	/* Fin du mode AT */
	sprintf(xbee_string, "ATCN\r");
	xbee_send_data_usart(xbee_string, strlen (xbee_string));
	
	/* Attend la reception du XBEE (OK)*/
	if (xbee_get_string(xbee_string)!=XBEE_SUCCESS)
	{
		return XBEE_INIT_FAILURE;
	}
	else
	{
		if (strcmp(xbee_string,XBEE_AT_OK)!=0)
		{
			return XBEE_INIT_FAILURE;
		}
	}
#endif /* __TESTS__ */
	
	xbee_mode= XBEE_MODE_API;
	xbee_timeout=XBEE_TIMEOUT_1S;
	
	return XBEE_SUCCESS;
}

int xbee_send_data(int addr_destination, char *str)
{
int status = XBEE_SUCCESS;
char *buffer, *buffer_esc;

int i;
int buffer_length;

ST_XBEE_FRAME frame;
	
	#ifdef __DEBUG__
	xbee_debug_send(addr_destination, str);
	#endif /* __DEBUG__ */
	
	/* Allocation d'un buffer suffisant */
	buffer = malloc(strlen(str)+9);
	
	if (buffer == 0x0) return XBEE_SEND_FAILED;
	if ((addr_destination>0xFFFF) || (addr_destination<0)) return XBEE_INVALID_ADDRESS;
	
	/* Remplissage du buffer */
	buffer[0] = 0x7E;
	buffer[1] = 0;
	buffer[2] = strlen(str)+5;
	buffer[3] = 0x01;
	buffer[4] = xbee_frame_counter;
	
	xbee_frame_counter++;
	if (xbee_frame_counter>255) xbee_frame_counter=1;

	buffer[5] = addr_destination>>8;
	buffer[6] = addr_destination&0xFF;

	buffer[7] = 0x0;
	
	for (i=0; i<strlen(str); i++)
	{
		buffer[8+i]=str[i];
	}
	
	buffer[strlen(str)+9-1]=xbee_compute_checksum(buffer, strlen(str)+9-1);
	
	/* Allocation d'un buffer pour la reception de la chaine echappÃ©e */
	buffer_esc = malloc((strlen(str)+9)*2); /* (strlen(str)+9)*2 dans le cas ou tout les caracteres seraient Ã  echapper */
	if (buffer_esc==0) return 0;
	
	buffer_length= xbee_escape_chars(buffer_esc, buffer, strlen(str)+9);
	
	if (buffer_length == 0) return XBEE_SEND_FAILED;	
	
	if (buffer != buffer_esc) free(buffer);
	xbee_send_data_usart(buffer_esc, buffer_length);
	
	/* Attente de la reponse */
	status= xbee_get_frame(&frame);
	
	if (status==XBEE_SUCCESS) /* si une reponse Ã  Ã©tÃ© recue */
	{
		/* Verifions que ce soit soit ACk, soit NACK */
		if (frame.status != 0) /* -> No ACK */
			status = XBEE_SEND_FAILED; 
	}
	
	free (buffer_esc); /* On libere le buffer avant de partir */
	
	return status;
}
	
int xbee_get_data(int *addr_source, char *str)
{
int status = XBEE_SUCCESS;
ST_XBEE_FRAME frame;
int i;
	
	/* Recuperation de la trame */
	status=xbee_get_frame(&frame);
	
	if (status == XBEE_SUCCESS)
	{
		*addr_source=frame.address;
	
		for (i=0; i<frame.data_length; i++)
		{
			str[i] = frame.data[i]; 
		}
	}
	
	return status;
}
	
void xbee_set_timeout(long long timeout)
{
	xbee_timeout = timeout;
}

//==============================================================================================================
//                                    Private Functions
//                                     Ne pas modifier
//==============================================================================================================

PRIVATE int xbee_get_string(char *str)
{
volatile long long cnt=0;
int finish=0;
int status = XBEE_SUCCESS;
	
	while (finish!=1)
	{
		if (xbee_timeout!= XBEE_NO_TIMEOUT)
		{
			cnt++;
			
			if (cnt >= xbee_timeout){
				status=XBEE_TIMEOUT_EXPIRED;
				finish=1;
			}
		}
		
		if (xbee_string_received == 1)
		{
			xbee_string_received=0;
			str = xbee_string;
			finish=1;
			
#ifdef __DEBUG__
			xbee_debug_receive();
#endif /* __DEBUG__ */
		}
	}
	
	return status;
}

PRIVATE int xbee_unpack_frame(ST_XBEE_FRAME *frame)
{
int status = XBEE_SUCCESS;
int length;
int checksum;
int i;
	
	/* Recupere la longueur de la trame (tel qu'ecrite dans la trame)*/
	length = (int)((int)(xbee_frame_buffer[1])<<8) + (int)xbee_frame_buffer[2];
	
	/* Calcul le checksum */
	checksum = xbee_compute_checksum(xbee_frame_buffer, length+1+3);
	if (checksum != 0x00 ) 
	{
		return XBEE_INVALID_FRAME;
	} 
	
	frame->apiCmd = xbee_frame_buffer[3];
	
	if (frame->apiCmd ==0x89) /* transmit frame status */
	{
		frame->status = xbee_frame_buffer[5];
		
		if ((frame->status<0) && (frame->status>4)) return XBEE_INVALID_FRAME;
	}
	else if (frame->apiCmd ==0x81) /* 16-bit address receive frame */
	{
		frame->address = (int)((int)(xbee_frame_buffer[4])<<8) + (int)xbee_frame_buffer[5];
		
		frame->rssi = (int)xbee_frame_buffer[6];
		frame->options = (int)xbee_frame_buffer[7];
		
		if ((frame->options<0) && (frame->options>2)) return XBEE_INVALID_FRAME;
		
		/* Recopie la pdu */
		for (i=0; i<(length-5); i++)
		{
			frame->data[i] = xbee_frame_buffer[8+i];
		}
		
		frame->data[i] = 0x0;
		frame->data_length=i;
	}
	else status = XBEE_INVALID_FRAME;
	
	return status;
}

PRIVATE int xbee_get_frame(ST_XBEE_FRAME *frame)
{
volatile long long cnt=0;
int finish=0;
int status = XBEE_SUCCESS;
	
	xbee_mode = XBEE_MODE_API;
	
	while (finish!=1)
	{
		if (xbee_timeout!= XBEE_NO_TIMEOUT)
		{
			cnt++;
			
			if (cnt >= xbee_timeout){
				status=XBEE_TIMEOUT_EXPIRED;
				finish=1;
			}
		}
		
		if (xbee_frame_received == 1)
		{
			xbee_frame_received=0;
			status = xbee_unpack_frame(frame);
			finish=1;
			
			#ifdef __DEBUG__
			xbee_debug_receive();
			#endif /*__DEBUG__ */
		}
	}
	
	return status;
}

PRIVATE void xbee_callback_receive(char data)
{
static int cnt;
	
	if (xbee_mode==XBEE_MODE_AT) /* Reception de chaine de caractere se terminant par 0x0D */
	{
		xbee_string[xbee_string_ptr]=(char)data;
		xbee_string_ptr++;
		if (data==0x0D) /* Fin de chaine */
		{
			/* Met un zero à la fin de la chaine */
			xbee_string[xbee_string_ptr-1]=0x00;
			xbee_string_ptr=0;
			xbee_string_received=1;
		}
		
		if (xbee_string_ptr>=XBEE_STRING_LENGTH) /* Depassement de la taille du buffer: on fini avec un 0 et on envoi*/
		{
			xbee_string[XBEE_STRING_LENGTH-1]=0;
			xbee_string_ptr=0;
			xbee_string_received=1;
		}
	}
	else /* On est en mode API */
	{
			if (xbee_api_state_machine !=0 ) /* on n'est pas en train d'attendre le start of frame 0x7E */
			{
				/* Tous les carcateres qui suivent le SOF sont potentiellement echappés: a tester */
				if ((data == 0x7D) && (xbee_api_escape_flag==0)) /* escape char */
				{
					xbee_api_escape_flag=1; /* on indique que le prochain caractere devra etre echappé */
					return; /* fin des traitements, attente du caractere suivant */
				}
				else
				{
					if (xbee_api_escape_flag==1) /* caractere à des-echapper */
					{
						data= data ^ 0x20;
						xbee_api_escape_flag=0;
					}
				}
			}
			
			switch (xbee_api_state_machine)
			{
				case 0: /* attente du start of frame (0x7E, '~') */
					cnt=0;
					if (data==0x7E)
					{
						xbee_frame_buffer[cnt]=data;
						xbee_api_state_machine=1;
						cnt++;
						xbee_api_escape_flag=0;
					}
					break;
				case 1:
					xbee_frame_buffer[cnt]=data;
				  cnt ++;
				
				  if (cnt==3)
					{
						xbee_api_length = (int)((int)(xbee_frame_buffer[1])<<8) + (int)xbee_frame_buffer[2];
						if ((xbee_api_length <0) || (xbee_api_length>126)) /* longueur invalide */
						{
							xbee_api_length=0;
							cnt=0;
							xbee_api_state_machine=0;
						}
						else
						{ 
							xbee_api_escape_flag=0;
							xbee_api_state_machine=2;
						}
					}
					break;
				case 2:
					xbee_frame_buffer[cnt]=data;
					cnt++;
					
					if (cnt== (xbee_api_length+3)) xbee_api_state_machine=3;
					break;
				case 3:
					xbee_frame_buffer[cnt]=data; /* Ecriture du checksum */
				
				  xbee_frame_received=1;
					xbee_api_escape_flag=0;
					xbee_api_state_machine=0;
					xbee_api_length=0;
					break;
				default:
					xbee_api_state_machine =0;
					xbee_frame_received=0;
					cnt=0;
				  xbee_api_escape_flag=0;
			}
	}
}

PRIVATE char xbee_compute_checksum(char *buffer, int length)
{
int i;
unsigned char checksum=0;

	for (i =3; i<length; i++)
	{
		checksum =checksum + (unsigned char)buffer[i];
	}
	
	return (0xFF-checksum);
}

PRIVATE int xbee_escape_chars(char* buffer_out, char* buffer_in, int length_in)
{
int new_length =length_in;
int i, i_out;
int length_out;
	
	/* Premiere passe: calcul de la longueur du nouveau buffer */
	for (i = 1; i< length_in-1; i++)
	{
		if ((buffer_in[i]== 0x7E) ||
			  (buffer_in[i]== 0x7D) ||
		    (buffer_in[i]== 0x11) ||
		    (buffer_in[i]== 0x13)) new_length++;
	}
	
	if (length_in == new_length) /* Pas de caractere Ã  echapper */
	{
		length_out = length_in;
	}
	else /* il y a des caracteres Ã  echapper */
	{
		length_out = new_length;
	}
	
	buffer_out[0] = 0x7E;
	
	i_out=1;
	
	for (i = 1; i< length_in; i++)
	{
		buffer_out[i_out] = buffer_in[i];
		
		if ((buffer_out[i_out]== 0x7E) ||
				(buffer_out[i_out]== 0x7D) ||
				(buffer_out[i_out]== 0x11) ||
				(buffer_out[i_out]== 0x13))
		{
			buffer_out[i_out+1]=buffer_out[i_out]^0x20;
			buffer_out[i_out]=0x7D;
			
			i_out++;
		}
		
		i_out++;
	}
	
	return length_out;
}

#ifdef __DEBUG__
static void xbee_debug_receive(void)
{
char *string_out;
int i;
ST_XBEE_FRAME frame; 
	
	string_out = malloc(200);
	
	if ((string_out!=0) && (xbee_mode == XBEE_MODE_API))
	{
		if (xbee_unpack_frame(&frame)==XBEE_SUCCESS)
		{
			if (frame.apiCmd == 0x89) // TX ACK
			{
				if (frame.status == 0)
					sprintf(string_out, "<= Xbee Transmit Status (0x89)\n\r\tStatus = Success\n\r");
				else
					sprintf(string_out, "<= Xbee Transmit Status (0x89)\n\r\tStatus = Fail (No Ack)\n\r"); 
			}
			else
				sprintf (string_out, "<= Xbee Receive Packet (0x81)\n\r\tAddress = 0x%04X\n\r\tRssi = 0x%02X\n\r\tLength = %i\n\r\tData = %s\n\r", frame.address, frame.rssi, frame.data_length, frame.data);
		}
		
		for (i=0; i<strlen(string_out); i++)
		{
			ITM_SendChar(string_out[i]);
		}
	
		free (string_out);
	}
}

static void xbee_debug_send(int address, char *str)
{
	char *string_out;
	int i;
	
	string_out = malloc (strlen(str) + strlen ("=> Xbee Transmit Request (0x01)\n\r\tAddress = 0x\n\r\tLength = \n\r\tData = \n\r")+20);
	
	if (string_out != 0)
	{
		sprintf (string_out, "=> Xbee Transmit Request (0x01)\n\r\tAddress = 0x%04X\n\r\tLength = %i\n\r\tData = %s\n\r",address, strlen(str), str);
	
		for (i=0; i<strlen(string_out); i++)
		{
			ITM_SendChar(string_out[i]);
		}
	
		free (string_out);
	}
}
#endif /* __DEBUG__ */
