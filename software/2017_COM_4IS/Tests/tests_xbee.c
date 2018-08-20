//=======================================================================================
// 	Auteur: S DI MERCURIO
//  Septembre 2017 , 
//  Processeur : STM32F103RB
//  Logiciel dev : ÂµVision 5.23
//
//  Interface publique de la bibliotheque d'acces pour l'API Xbee
//=======================================================================================

#include "stm32f10x.h"                  // Device header
#include "xbee_cmd.h"
#include "xbee_usart.h"

#include "test_xbee.h"

#include <stdlib.h>

volatile int test_xbee_temp;
u8 test_xbee_buffer[120];
u8 test_xbee_buffer_length=0;

extern int xbee_frame_received;
extern char xbee_frame_buffer[120];
extern int xbee_frame_counter;

#define XBEE_MODE_AT	0
#define XBEE_MODE_API	1
extern int xbee_mode;

extern char xbee_string[30];
extern char xbee_string_ptr;
extern int xbee_string_received;

/* Variables necessaires pour la fonction XBEE_Rx_Callback */
extern int xbee_api_state_machine;
extern int xbee_api_length;
extern int xbee_api_escape_flag;

typedef struct{
	int address;
	int apiCmd;
	int rssi;
	char data[100];
	int data_length;
	int options;
	int status;
} ST_XBEE_FRAME;

void TESTS_XBEE_Callback (char data)
{
		test_xbee_temp = data;
}

/* Fonctions internes sous tests */
int xbee_unpack_frame(ST_XBEE_FRAME *frame);
void xbee_callback_receive(char data);
int xbee_escape_chars(char* buffer_out, char* buffer_in, int length_in);

int TEST_xbee_unpack_frame(void)
{
int results = 0;
int status =0;
ST_XBEE_FRAME	frame;
	
	// Tests de xbee_unpack_frame */
	xbee_frame_buffer[0]=0x7E;
	xbee_frame_buffer[1]=0;
	xbee_frame_buffer[2]=10;
	xbee_frame_buffer[3]=0x81; /* Trame de reception */
	xbee_frame_buffer[4]=0xBE; /* Source Addr = 0xBEEF */
	xbee_frame_buffer[5]=0xEF;
	xbee_frame_buffer[6]=0x35; /* RSSi de -53dbm */
	xbee_frame_buffer[7]=1;    /* Adresse broadcast */
	xbee_frame_buffer[8]=0x48; //H
	xbee_frame_buffer[9]=0x65; //e
	xbee_frame_buffer[10]=0x6C; //l
	xbee_frame_buffer[11]=0x6C; //l
	xbee_frame_buffer[12]=0x6F; //0
	xbee_frame_buffer[13]=0xB0;
	
	/* test avec un checksum invalid */
	status = xbee_unpack_frame(&frame);
	if (status == XBEE_INVALID_FRAME) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test avec un bon checksum */
	xbee_frame_buffer[13]=0xA7;
	status = xbee_unpack_frame(&frame);
	if (status == XBEE_SUCCESS) 
	{
		if ((frame.address == 0xBEEF) &&
			  (frame.apiCmd == 0x81 )&&
		    (frame.data_length == 5) &&
		    (frame.data[0] == 'H') &&
		    (frame.data[4] == 'o') &&
		    (frame.data[5] == 0x0) &&
		    (frame.options == 0x1) &&
		    (frame.rssi == 0x35)) results=(results<<1)+1;
		else results=(results<<1);
	}
	
	/* Test avec une PDU vide (pas de data) */
	xbee_frame_buffer[2]=5;
	xbee_frame_buffer[3]=0x81; /* Trame de reception */
	xbee_frame_buffer[4]=0xBE; /* Source Addr = 0xBEEF */
	xbee_frame_buffer[5]=0xEF;
	xbee_frame_buffer[6]=0x35; /* RSSi de -53dbm */
	xbee_frame_buffer[7]=1;    /* Adresse broadcast */
	xbee_frame_buffer[8]=0x9B;
	
	status = xbee_unpack_frame(&frame);
	if (status == XBEE_SUCCESS) 
	{
		if ((frame.address == 0xBEEF) &&
			  (frame.apiCmd == 0x81 )&&
		    (frame.data_length == 0) &&
		    (frame.data[0] == 0x0) &&
		    (frame.options == 0x1) &&
		    (frame.rssi == 0x35)) results=(results<<1)+1;
		else results=(results<<1);
	}
	
	/* Test avec une Trame d'acquitement de transmission (ack = ok)*/
	xbee_frame_buffer[2]=3;
	xbee_frame_buffer[3]=0x89; /* Trame d'acquitement */
	xbee_frame_buffer[4]=1; /* identifiant de trame */
	xbee_frame_buffer[5]=0; /* Success */
	xbee_frame_buffer[6]=0x75; /* Checksum */
	
	status = xbee_unpack_frame(&frame);
	if (status == XBEE_SUCCESS) 
	{
		if ((frame.status == 0) &&
			  (frame.apiCmd == 0x89 )) results=(results<<1)+1;
		else results=(results<<1);
	}
	
	/* Test avec une Trame d'acquitement de transmission (ack = non recu)*/
	xbee_frame_buffer[2]=3;
	xbee_frame_buffer[3]=0x89; /* Trame d'acquitement */
	xbee_frame_buffer[4]=1; /* identifiant de trame */
	xbee_frame_buffer[5]=1; /* Ack non recu */
	xbee_frame_buffer[6]=0x74; /* Checksum */
	
	status = xbee_unpack_frame(&frame);
	if (status == XBEE_SUCCESS) 
	{
		if ((frame.status == 1) &&
			  (frame.apiCmd == 0x89 )) results=(results<<1)+1;
		else results=(results<<1);
	}
	
	return results;
}

int TEST_xbee_callback_receive(void)
{
int results = 0;
	
	/* Test du mode AT: Reception d'une chaine ("OK") */
	xbee_mode=XBEE_MODE_AT;
	
	xbee_string_ptr=0;
	xbee_string_received=0;
	
	xbee_callback_receive('O');
	xbee_callback_receive('K');
	xbee_callback_receive(0x0D); /* Fin de la chaine de caractere */
	
	if ((xbee_string_ptr == 0) &&
		  (xbee_string_received==1) &&
	    (xbee_string[0]=='O') &&
	    (xbee_string[1]=='K') &&
	    (xbee_string[2]==0x0)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test du mode AT: Reception d'une chaine vide ("") */
	xbee_mode=XBEE_MODE_AT;
	
	xbee_string_ptr=0;
	xbee_string_received=0;
	
	xbee_callback_receive(0x0D); /* Fin de la chaine de caractere */
	
	if ((xbee_string_ptr == 0) &&
		  (xbee_string_received==1) &&
	    (xbee_string[0]==0x0)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test du mode API: Reception d'une trame (TX status) */
	xbee_mode=XBEE_MODE_API;
	
	xbee_frame_received=0;
	xbee_frame_buffer[0]=0;
	xbee_api_state_machine=0;
	xbee_api_length=0;
	xbee_api_escape_flag=0;
	
	xbee_callback_receive(0x7E); /* Debut de trame */
	xbee_callback_receive(0); 
	xbee_callback_receive(3); /* Longeur de PDU */
	xbee_callback_receive(0x89); /* TX Status */
	xbee_callback_receive(1); /* Frame ID */
	xbee_callback_receive(0); /* Status = ACK */
	xbee_callback_receive(0x75); /* Status = ACK */
	
	if ((xbee_frame_received==1) &&
	    (xbee_frame_buffer[0]==0x7E) &&
	    (xbee_frame_buffer[1]==0) &&
			(xbee_frame_buffer[2]==3) &&
			(xbee_frame_buffer[3]==0x89) &&
			(xbee_frame_buffer[4]==1) &&
			(xbee_frame_buffer[5]==0) &&
			(xbee_frame_buffer[6]==0x75)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test du mode API: Reception d'une trame (RX Packet) */
	xbee_mode=XBEE_MODE_API;
	
	xbee_frame_received=0;
	xbee_frame_buffer[0]=0;
	xbee_api_state_machine=0;
	xbee_api_length=0;
	xbee_api_escape_flag=0;
	
	xbee_callback_receive(0x7E); /* Debut de trame */
	xbee_callback_receive(0); 
	xbee_callback_receive(7); /* Longeur de PDU */
	xbee_callback_receive(0x81); /* TX Status */
	xbee_callback_receive(0xBE); /* @ H */
	xbee_callback_receive(0xEF); /* @ L */
	xbee_callback_receive(0x35); /* RSSI = -53dbm */
	xbee_callback_receive(1); /* @ broadcast */
	xbee_callback_receive('O'); /* PDU */
	xbee_callback_receive('K'); /* PDU */
	xbee_callback_receive(0x1); /* Checksum */
	
	if ((xbee_frame_received==1) &&
	    (xbee_frame_buffer[0]==0x7E) &&
	    (xbee_frame_buffer[1]==0) &&
			(xbee_frame_buffer[2]==7) &&
			(xbee_frame_buffer[3]==0x81) &&
			(xbee_frame_buffer[4]==0xBE) &&
			(xbee_frame_buffer[5]==0xEF) &&
	    (xbee_frame_buffer[6]==0x35) &&
			(xbee_frame_buffer[7]==1) &&
			(xbee_frame_buffer[8]=='O') &&
	    (xbee_frame_buffer[9]=='K') &&
			(xbee_frame_buffer[10]==0x1)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test du mode API: Reception d'une trame avec caracteres a echapper dans la PDU (RX Packet) */
	xbee_mode=XBEE_MODE_API;
	
	xbee_frame_received=0;
	xbee_frame_buffer[0]=0;
	xbee_api_state_machine=0;
	xbee_api_length=0;
	xbee_api_escape_flag=0;
	
	xbee_callback_receive(0x7E); /* Debut de trame */
	xbee_callback_receive(0); 
	xbee_callback_receive(7); /* Longeur de PDU */
	xbee_callback_receive(0x81); /* TX Status */
	xbee_callback_receive(0xBE); /* @ H */
	xbee_callback_receive(0xEF); /* @ L */
	xbee_callback_receive(0x7D); /* ESC */
	xbee_callback_receive(0x31); /* RSSI = -17dbm */
	xbee_callback_receive(1); /* @ broadcast */
	xbee_callback_receive(0x7D); /* ESC */
	xbee_callback_receive(0x5D); /* ESC echappé */
	xbee_callback_receive('K'); /* PDU */
	xbee_callback_receive(0x1); /* Checksum */
	
	if ((xbee_frame_received==1) &&
	    (xbee_frame_buffer[0]==0x7E) &&
	    (xbee_frame_buffer[1]==0) &&
			(xbee_frame_buffer[2]==7) &&
			(xbee_frame_buffer[3]==0x81) &&
			(xbee_frame_buffer[4]==0xBE) &&
			(xbee_frame_buffer[5]==0xEF) &&
	    (xbee_frame_buffer[6]==0x11) &&
			(xbee_frame_buffer[7]==1) &&
			(xbee_frame_buffer[8]==0x7D) &&
	    (xbee_frame_buffer[9]=='K') &&
			(xbee_frame_buffer[10]==0x1)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test du mode API: Reception d'une trame dont la longueur est invalide (>127) */
	xbee_mode=XBEE_MODE_API;
	
	xbee_frame_received=0;
	xbee_frame_buffer[0]=0;
	xbee_api_state_machine=0;
	xbee_api_length=0;
	xbee_api_escape_flag=0;
	
	xbee_callback_receive(0x7E); /* Debut de trame */
	xbee_callback_receive(0); 
	xbee_callback_receive(200); /* Longeur de PDU */
	xbee_callback_receive(0x81); /* TX Status */
	xbee_callback_receive(0xBE); /* @ H */
	xbee_callback_receive(0xEF); /* @ L */
	xbee_callback_receive(0x7D); /* ESC */
	xbee_callback_receive(0x31); /* RSSI = -17dbm */
	xbee_callback_receive(1); /* @ broadcast */
	xbee_callback_receive(0x7D); /* ESC */
	xbee_callback_receive(0x5D); /* ESC echappé */
	xbee_callback_receive('K'); /* PDU */
	xbee_callback_receive(0x1); /* Checksum */
	
	if ((xbee_frame_received==1) &&
	    (xbee_frame_buffer[0]==0x7E) &&
	    (xbee_frame_buffer[1]==0) &&
			(xbee_frame_buffer[2]==7) &&
			(xbee_frame_buffer[3]==0x81) &&
			(xbee_frame_buffer[4]==0xBE) &&
			(xbee_frame_buffer[5]==0xEF) &&
	    (xbee_frame_buffer[6]==0x11) &&
			(xbee_frame_buffer[7]==1) &&
			(xbee_frame_buffer[8]==0x7D) &&
	    (xbee_frame_buffer[9]=='K') &&
			(xbee_frame_buffer[10]==0x1)) results=(results<<1)+1;
	else results=(results<<1);
	return results;
}

int TEST_xbee_escape_chars(void)
{
int results=0;
char test_xbee_buffer[20];
char *buf;
	
	/* Buffer avec caracteres a echapper */
	test_xbee_buffer[0]=0x7E;
	test_xbee_buffer[1]=0;
	test_xbee_buffer[2]=6; /* Longeur de PDU */
	test_xbee_buffer[3]=0x81; /* TX Status */
	test_xbee_buffer[4]=0xBE; /* @ H */
	test_xbee_buffer[5]=0xEF; /* @ L */
	test_xbee_buffer[6]=0x11; /* RSSI = -17dbm */
  test_xbee_buffer[7]=1; /* @ broadcast */
	test_xbee_buffer[8]=0x7D; /* ESC echappé */
	test_xbee_buffer[9]='K'; /* PDU */
	test_xbee_buffer[10]=0x1;
	
	buf = malloc (15); /* normallement juste 13 */
	if (buf == 0) return -1; /* impossible de faire le test */
	
	if ((xbee_escape_chars(buf, test_xbee_buffer, 11) ==13)	&&
		(buf[0] == 0x7E) &&
		(buf[1] == 0) &&
		(buf[2] == 6) &&
		(buf[3] == 0x81) &&
		(buf[4] == 0xBE) &&
		(buf[5] == 0xEF) &&
		(buf[6] == 0x7D) &&
		(buf[7] == 0x31) &&
		(buf[8] == 1) &&
		(buf[9] == 0x7D) &&
		(buf[10] == 0x5D) &&
		(buf[11] == 'K') &&
		(buf[12] == 0x1))	results=(results<<1)+1;
	else results=(results<<1);
	
	free (buf);
	
	/* Buffer sans caracteres a echapper */
	test_xbee_buffer[0]=0x7E;
	test_xbee_buffer[1]=0;
	test_xbee_buffer[2]=6; /* Longeur de PDU */
	test_xbee_buffer[3]=0x81; /* TX Status */
	test_xbee_buffer[4]=0xBE; /* @ H */
	test_xbee_buffer[5]=0xEF; /* @ L */
	test_xbee_buffer[6]=0x12; /* RSSI = -17dbm */
  test_xbee_buffer[7]=1; /* @ broadcast */
	test_xbee_buffer[8]=0x70; /* ESC echappé */
	test_xbee_buffer[9]='K'; /* PDU */
	test_xbee_buffer[10]=0x1;
	
	buf = malloc (15); /* normallement juste 13 */
	if (buf == 0) return -1; /* impossible de faire le test */
	
	if ((xbee_escape_chars(buf, test_xbee_buffer, 11) ==11)	&&
		(buf[0] == 0x7E) &&
		(buf[1] == 0) &&
		(buf[2] == 6) &&
		(buf[3] == 0x81) &&
		(buf[4] == 0xBE) &&
		(buf[5] == 0xEF) &&
		(buf[6] == 0x12) &&
		(buf[7] == 1) &&
		(buf[8] == 0x70) &&
		(buf[9] == 'K') &&
		(buf[10] == 0x1))	results=(results<<1)+1;
	else results=(results<<1);
	
	free (buf);
	
	return results;
}

int TEST_xbee_init(void)
{
int results = 0;
	
	if (xbee_init(0xFFF, 0x3233, 0xACC0) == XBEE_INVALID_CHANNEL) results=(results<<1)+1;
	else results=(results<<1);
	
	if (xbee_init(0x0D, 0x30000, 0xACC0) == XBEE_INVALID_PANID) results=(results<<1)+1;
	else results=(results<<1);
	
	if (xbee_init(0x0D, 0x3233, 0x30000) == XBEE_INVALID_ADDRESS) results=(results<<1)+1;
	else results=(results<<1);
	
	
	/* les tests suivants ont été validés (retrait du flag _TESTS_) */
//	/* Attente sans envoi de reponse */
//	status= xbee_init(0x0D, 0x3233, 0xACC0);
//	if ( status == XBEE_INIT_FAILURE) results=(results<<1)+1;
//	
//	/* Envoi de error */
//	status= xbee_init(0x0D, 0x3233, 0xACC0);
//	if (status == XBEE_INIT_FAILURE) results=(results<<1)+1;
//	
//	/* Envoi de ok */
//	status= xbee_init(0x0D, 0x3233, 0xACC0);
//	if (status == XBEE_SUCCESS) results=(results<<1)+1;
	
	return results;
}

int TEST_Xbee_cmd(void)
{
int results = 0;

	if (TEST_xbee_init() == 0x7) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_xbee_unpack_frame() == 0x1F) results=(results<<1)+1;
	else results=(results<<1);

	if (TEST_xbee_callback_receive() == 0x7) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_xbee_escape_chars() == 0x1) results=(results<<1)+1;
	else results=(results<<1);
	return results;
}

int TEST_Xbee_usart(void)
{
int results = 0;
	
	xbee_init_usart(9600, 3);
	
	xbee_set_callback(TESTS_XBEE_Callback);
	
//	test_xbee_buffer[0]=0x7E;
//	test_xbee_buffer[1]=0;
//	test_xbee_buffer[2]=10;
//	test_xbee_buffer[3]=1;
//	test_xbee_buffer[4]=1;
//	test_xbee_buffer[5]=0;
//	test_xbee_buffer[6]=5;
//	test_xbee_buffer[7]=0;
//	test_xbee_buffer[8]=0x48; //H
//	test_xbee_buffer[9]=0x65; //e
//	test_xbee_buffer[10]=0x6C; //l
//	test_xbee_buffer[11]=0x6C; //l
//	test_xbee_buffer[12]=0x6F; //0
//	test_xbee_buffer[13]=0xB;  // checksum
	
	test_xbee_buffer[0]='A';
	test_xbee_buffer[1]='B';
	test_xbee_buffer[2]='C';
	test_xbee_buffer[3]='D';
	test_xbee_buffer[4]='E';
	test_xbee_buffer[5]='F';
	
	test_xbee_buffer_length=0;
	
	xbee_send_data_usart((char*)test_xbee_buffer, test_xbee_buffer_length);
	
	while (1)
	{
	}
	
	return results;
}
