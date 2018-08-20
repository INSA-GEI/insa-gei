//=======================================================================================
// 	Auteur: S DI MERCURIO
//  Septembre 2017 , 
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 5.23
//
//  Interface publique de la bibliotheque d'acces pour l'API Xbee
//=======================================================================================

#include "stm32f10x.h"                  // Device header
#include "fm_cmd.h"
#include "fm_usart.h"

#include "test_fm.h"

#include <stdlib.h>
#include <string.h>

volatile int test_fm_temp;
char test_fm_buffer[120];
int test_fm_buffer_length=0;

extern char fm_string[30];
extern char fm_string_ptr;
extern int fm_string_received;
extern long long fm_timeout;

void TESTS_FM_Callback (char data)
{
		test_fm_temp = data;
}

/* Fonctions internes sous tests */
void fm_callback_receive(char data);
void fm_wait_x_ms(int delay);

int TEST_fm_callback_receive(void)
{
int results = 0;
	
	/* Test de reception */
	fm_string_ptr=0;
	fm_string_received=0;
	
	fm_callback_receive('O');
	fm_callback_receive('K');
	fm_callback_receive(0x0D); /* Fin de la chaine de caractere */
	
	if ((fm_string_ptr == 0) &&
		  (fm_string_received==1) &&
	    (fm_string[0]=='O') &&
	    (fm_string[1]=='K') &&
	    (fm_string[2]==0x0D) &&
	    (fm_string[3]==0x0)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Reception d'une chaine vide ("") */
	
	fm_string_ptr=0;
	fm_string_received=0;
	
	fm_callback_receive(0x0D); /* Fin de la chaine de caractere */
	
	if ((fm_string_ptr == 0) &&
		  (fm_string_received==1) &&
	    (fm_string[0]==0x0D) &&
	    (fm_string[1]==0x0)) results=(results<<1)+1;
	else results=(results<<1);

	return results;
}

int TEST_fm_get_data(void)
{
int results = 0;
char buffer[20];
int status;
	
	/* Test de reception d'une chaine */
	
	fm_string_ptr=0;
	fm_string_received=0;
	fm_timeout=FM_TIMEOUT_1S;
	
	fm_callback_receive('H');
	fm_callback_receive('e');
	fm_callback_receive('l');
	fm_callback_receive('l');
	fm_callback_receive('o');
	fm_callback_receive(0x0D); /* Fin de la chaine de caractere */
	
	status = fm_get_data(buffer);
	
	if ((fm_string_ptr == 0) &&
		  (fm_string_received==0) &&
	    (strcmp(buffer, "Hello\r")==0) &&
	    (status==FM_SUCCESS)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Pas de reception (timeout) */
	fm_string_ptr=0;
	fm_string_received=0;
	fm_timeout=FM_TIMEOUT_1S;
	
	status = fm_get_data(buffer);
	if ((fm_string_ptr == 0) &&
		  (fm_string_received==0) &&
	    (status==FM_TIMEOUT_EXPIRED)) results=(results<<1)+1;
	else results=(results<<1);

	return results;
}

int TEST_fm_init(void)
{
int results = 0;
	
	if ((fm_init() == FM_SUCCESS) &&
     (fm_timeout== FM_TIMEOUT_1S)) results=(results<<1)+1;
	else results=(results<<1);
	
	return results;
}

int TEST_fm_send_data(void)
{
	int results = 0;
	
	/* Test l'envoi de hello */
	if ((fm_send_data("Hello\n\r") == FM_SUCCESS)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test l'envoi d'un chaine de caractere vide => doit repondre success*/
	if ((fm_send_data("") == FM_SUCCESS)) results=(results<<1)+1;
	else results=(results<<1);
	
	/* Test l'envoi d'un chaine 'Salut' => doit repondre busy (hello n'a pas encore été transmit) */
	if ((fm_send_data("Salut\n\r") == FM_BUSY)) results=(results<<1)+1;
	else results=(results<<1);
	
	return results;
}

int TEST_FM_cmd(void)
{
int results = 0;

	if (TEST_fm_init() == 0x1) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_fm_send_data() == 0x7) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_fm_callback_receive() == 0x7) results=(results<<1)+1;
	else results=(results<<1);

	if (TEST_fm_get_data() == 0x3) results=(results<<1)+1;
	else results=(results<<1);
	
	return results;
}

int TEST_FM_usart_init(void)
{
int results = 0;
	
	fm_init_usart(9600, 3);
	
	fm_set_receive_callback(TESTS_FM_Callback);
	
	fm_set_mode(FM_TX);
	
	test_fm_buffer[0]='A';
	test_fm_buffer[1]='B';
	test_fm_buffer[2]='C';
	test_fm_buffer[3]='D';
	test_fm_buffer[4]='E';
	test_fm_buffer[5]='F';
	
	test_fm_buffer_length=6;
	
	fm_send_data_usart(test_fm_buffer, test_fm_buffer_length);
	
	while (1)
	{
	}
	
	return results;
}

int TEST_FM_usart_wait(void)
{
	/* Attente de 100ms */
	fm_wait_x_ms(100);
	
	/* attente de 0ms */
	fm_wait_x_ms(0);
	
	return 0x1;
}

int TEST_FM_usart_set_mode(void)
{
	fm_init_usart(9600, 3);
	
	/* Mode Off */
	fm_set_mode(FM_OFF);
	
	/* Mode FM_TX */
	fm_set_mode(FM_TX);
	
	/* Mode FM_RX */
	fm_set_mode(FM_RX);
	
	/* Mode FM_TX_LOOPBACK */
	fm_set_mode(FM_TX_LOOPBACK);
	
	return 0x1;
}

int TEST_FM_usart(void)
{
int results = 0;
	
	if (TEST_FM_usart_wait() == 0x1) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_FM_usart_set_mode() == 0x1) results=(results<<1)+1;
	else results=(results<<1);
	
	if (TEST_FM_usart_init() == 0x3) results=(results<<1)+1;
	else results=(results<<1);
	
	return results;
}




