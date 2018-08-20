//=======================================================================================
// 	Auteur: S DI MERCURIO
//  Septembre 2017 , 
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 5.23
//
//  Driver Usart pour l'API Xbee
//
//  RQ: Ne fonctionne que pour les cartes "Reseaux de Capteurs" (USART figé)
//=======================================================================================

#include "stm32f10x.h"
#include "Clock.h"
#include "GPIO.h"
#include "NVIC_IT_Ext.h"
#include "Xbee_usart.h"

#ifdef __TESTS__
#define PRIVATE
#else
#define PRIVATE static
#endif /*  __TESTS__ */

PRIVATE u8 xbee_usart_buffer_transmit[100+10];
PRIVATE u8 xbee_usart_ptr_transmit=0;
PRIVATE u8 xbee_usart_busy_transmit=0;
PRIVATE u8 xbee_usart_transmit_length=0;

PRIVATE void (*xbee_usart_callback)(char data);

/*
 * Fonction d'initialisation de l'USART2 (XBEE)
 */
void xbee_init_usart(int baudrate, char priority)
/*
	Active la bonne horloge
	Détermine le Baud rate par lecture de la freq USART (Clock de Pascal / Seb)
	configure les IO
	Initialise les pointeurs de fonctions utiles pour la réception
	Valide l'IT USART au niveau NVIC
	Positionne les flags au départ
*/

{
unsigned int Frequence_Ck_USART_Hz;	
int USART_Div;
int Mantisse,Fract;
USART_TypeDef *USART=USART2;
	
	(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_APB1ENR_USART2EN;
	Frequence_Ck_USART_Hz=CLOCK_GetPCLK1();
	// USART_2_TX = TIM2_CH3 = PA2
	// USART_2_RX = TIM2_CH4 = PA3
	GPIO_Configure(GPIOA, 2, OUTPUT, ALT_PPULL);
	GPIO_Configure(GPIOA, 3, INPUT, INPUT_FLOATING);
	NVIC_Enable_IT(38);	
	NVIC_Prio_IT(38, priority );

	// Détermination du Baud Rate:
	USART_Div=(Frequence_Ck_USART_Hz)/(baudrate); 
	//  USART_Div est en format 28.4
	Mantisse = USART_Div>>4;
	Fract=USART_Div&0x0000000F; 
	// on ne garde que les 4 bits fract de poids fort

	// configuration émission
	USART->CR1=(USART->CR1)|USART_CR1_UE; // UART On
	USART->CR2=(USART->CR1)&~(0x1<<12); // 8 bits de data !!!!!!!!!!!! to check
	USART->CR2=(USART->CR2)&~(0x3<<12); // 1 bit de stop
	USART->BRR=(Mantisse<<4)+Fract; // Baud Rate
	USART->CR1=(USART->CR1)|USART_CR1_TE; // Transmit Enable

	//Configuration réception
	USART->CR1=(USART->CR1)|USART_CR1_RE; // Receive Enable
	USART->CR1=(USART->CR1)|USART_CR1_RXNEIE; // validation IT locale en réception
	
	xbee_usart_callback=0;
}

/* 
 * Enregistre un callback pour remonter au services superieur les données recues 
 */
void xbee_set_callback(void (*func_ptr) (char data))
{
	xbee_usart_callback = func_ptr;
}

/*
 * Envoi un buffer de donnée sur l'USART2
 *
 * RQ: Retourne XBEE_USART_TX_SUCCESS si la liaison TX est libre
 *     Retourne XBEE_USART_TX_BUSY si la liaison TX est en cours d'utilisation
 */
int xbee_send_data_usart(char *buffer, int length)
{
int i;
	
	int status=XBEE_USART_SUCCESS;
	
	if (length <=0) return status; /* pas la peine de s'embeter, il n'y a rien a envoyer */
	
	if (xbee_is_busy_usart()==XBEE_USART_IDLE)
	{
		xbee_usart_busy_transmit = 1;
		
		/* Recopie du buffer en local */
		for (i=0; i<length; i++)
		{
			xbee_usart_buffer_transmit[i]=buffer[i];
		}
		
		xbee_usart_transmit_length=length;
		xbee_usart_ptr_transmit=0;
		
		USART2->CR1 |= USART_CR1_TXEIE;
	}
	else status = XBEE_USART_BUSY;
	
	return status;
}

/*
 * Verifie sur l'USART2 est libre pour transmettre
 *
 * RQ: Retourne XBEE_USART_IDLE si la liaison TX est libre
 *     Retourne XBEE_USART_BUSY si la liaison TX est en cours d'utilisation
 */
int xbee_is_busy_usart(void)
{
	int status = XBEE_USART_IDLE;
	
	if (xbee_usart_busy_transmit==1) status = XBEE_USART_BUSY;
	return status;
}

/*
 * Fonction d'interruption de l'USART2 (XBEE)
 */
void USART2_IRQHandler(void)
{
volatile int dummy;
volatile u8 data;
	
	// Partie transmission
	if ((((USART2->SR)&USART_SR_TXE)==USART_SR_TXE)&&(((USART2->CR1)&USART_CR1_TXEIE)==USART_CR1_TXEIE)) 
	{
		USART2->DR= xbee_usart_buffer_transmit[xbee_usart_ptr_transmit];
		xbee_usart_ptr_transmit++;
		
		if (xbee_usart_ptr_transmit>=xbee_usart_transmit_length)
		{
			/* Fin de la transmission, remise à zero du flag TXEIE, autorisant les IT TX */
			USART2->CR1 &= ~USART_CR1_TXEIE;
			xbee_usart_busy_transmit=0;
			xbee_usart_ptr_transmit=0;
			xbee_usart_transmit_length=0;
		}
	}
	
	// Partie reception
	if (((USART2->SR)&USART_SR_RXNE)==USART_SR_RXNE)
	{
		data = (u8)USART2->DR;
		
		if (xbee_usart_callback!=0)
		{
			xbee_usart_callback(data);
		}
		
		/* Des fois, l'USART se met en vrille, pour eviter ca, une bonne lecture des registre suffit normallement */
		dummy = USART1->SR;
		dummy = USART1->CR1;
		dummy = USART1->CR2;
		dummy = USART1->CR3;
		dummy = USART1->BRR;
		dummy = USART1->GTPR;
		dummy = USART1->SR;
	}
}
