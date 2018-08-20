
#include "clock.h"
#include "API_COM_TP_4IS.h"
#include "stdio.h"
#include "USART_User_Conf.h"

#include "Xbee_cmd.h"
// TESTS
#include "test_xbee.h"
#include "test_fm.h"
// TESTS

#define Test_String // si défini, test d'E/R string.
										// sinon test E/R char


char Invite[50];
char Invite2[50];
char * Reponse;
char Caract;


void IT_Tim(void);

int Valeur;
float ValeurVolts;

// TESTS
volatile int Test_Results;
int address;
volatile char buffer[100];

// TESTS

int main (void)
{

CLOCK_Configure();
Serial_Global_Conf(USART_1,9600, 0); 
//Serial_Global_Conf(USART_2,9600, 0); 
//Serial_Global_Conf(USART_3,9600, 0); 
	
Serial_Put_String(USART_1,"Bonjour n 1");	
//Serial_Put_String(USART_2,"Bonjour n 2");	
//Serial_Put_String(USART_3,"Bonjour n 3");	

Timer_Global_Conf(Timer_1, 2000000.0 );
Timer_IT_Conf(Timer_1, 3, IT_Tim);
	
ADC_Global_Conf(PA1);	
	
#ifdef __TESTS__ 
//Test_Results=TEST_Xbee_cmd();
//Test_Results=TEST_FM_usart();
	Test_Results=TEST_FM_cmd();
	
	while (1);
//	xbee_init(0x0D, 0xACC0, 0x20);
//	
//	Test_Results=xbee_send_data (0x10, "Hello");
//	
//	xbee_set_timeout(XBEE_NO_TIMEOUT);
//	Test_Results=xbee_get_data(&address, (char*)buffer);

// TESTS
#endif /* __TESTS__ */

while (1)
{
	#ifndef Test_String
  if (Serial_Is_Byte_Received(USART_1)==1)
	{
		Caract=Serial_Get_Char(USART_1);
		Serial_Put_Char(USART_1,Caract);
	}
	#endif
  Valeur=ADC_Get_Value(PA1);
  ValeurVolts	= (float)Valeur*3.3/4096.0;
}
 
//	return 0;
}







void IT_Tim(void)
{

	Serial_Put_String(USART_1,Invite);
	Serial_Put_String(USART_1,Invite2);
	
	#ifdef Test_String
	if (Serial_Is_Str_Received(USART_1)==1)
	{
		Reponse=Serial_Get_String(USART_1);
		Serial_Put_String(USART_1,Reponse);
	}
	sprintf(Invite," Valeur ADC PA1 : %i \n\r",Valeur); 
	sprintf(Invite2," Valeur ADC PA1 [V] : %f \n\r",ValeurVolts); 
	#else
	
	#endif
	
}
