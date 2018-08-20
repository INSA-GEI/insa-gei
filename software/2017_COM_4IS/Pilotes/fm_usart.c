/*!
 * \file
 * \brief 	Pilote de liaison serie pour le module FM (RT606).
 * \author 	S. DI MERCURIO
 * \date   	septembre 2017
 * \version 1.0
 */
 
#include "stm32f10x.h"                  // Device header

#include "Clock.h"
#include "GPIO.h"
#include "NVIC_IT_Ext.h"
#include "fm_usart.h"

#ifdef __TESTS__
#define PRIVATE
#else
#define PRIVATE static
#endif /*  __TESTS__ */

PRIVATE u8 fm_usart_buffer_transmit[100];
PRIVATE u8 fm_usart_ptr_transmit=0;
PRIVATE u8 fm_usart_busy_transmit=0;
PRIVATE u8 fm_usart_transmit_length=0;

PRIVATE void (*fm_usart_receive_callback)(char data);
PRIVATE void (*fm_usart_transmit_empty_callback)(void);

PRIVATE void fm_wait_1ms(void);
PRIVATE void fm_wait_x_ms(int delay);
	
/*
 * Fonction d'initialisation de l'USART3 (FM)
 */
void fm_init_usart(int baudrate, char priority)
{
/*
	Active la bonne horloge
	Détèrmine le Baud rate par lecture de la freq USART (Clock de Pascal / Seb)
	configure les IO
	Initialise les pointeurs de fonctions utiles pour la reception
	Valide l'IT USART au niveau NVIC
	Positionne les flags au départ
*/

unsigned int Frequence_Ck_USART_Hz;	
int USART_Div;
int Mantisse,Fract;
USART_TypeDef *USART=USART3;
	
	(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_APB1ENR_USART3EN;
	Frequence_Ck_USART_Hz=CLOCK_GetPCLK1();
	// USART_3_TX = PB10
	// USART_3_RX = PB11
	GPIO_Configure(GPIOB, 10, OUTPUT, ALT_PPULL);
	GPIO_Configure(GPIOB, 11, INPUT, INPUT_FLOATING);
	NVIC_Enable_IT(39);
  NVIC_Prio_IT(39, priority );	
	
	// Détérmination du Baud Rate:
	USART_Div=(Frequence_Ck_USART_Hz)/(baudrate); 
	//  USART_Div est en format 28.4
	Mantisse = USART_Div>>4;
	Fract=USART_Div&0x0000000F; 
	// on ne garde que les 4 bits fract de poids fort

	// configuration transmission
	USART->CR1=(USART->CR1)|USART_CR1_UE; // UART On
	USART->CR2=(USART->CR1)&~(0x1<<12); // 8 bits de data !!!!!!!!!!!! to check
	USART->CR2=(USART->CR2)&~(0x3<<12); // 1 bit de stop
	USART->BRR=(Mantisse<<4)+Fract; // Baud Rate
	USART->CR1=(USART->CR1)|USART_CR1_TE; // Transmit Enable

	fm_usart_receive_callback=0;
	fm_usart_transmit_empty_callback=0;
	
	/* Configuration des lignes TX_CMD, RX_CMD et CARRIER_DETECT */
	GPIO_Configure(GPIOB, 7, INPUT,  INPUT_PULL_DOWN_UP); // CARRIER_DETECT PB7
	GPIO_Configure(GPIOB, 8, OUTPUT, OUTPUT_PPULL); // RX_CMD PB8
	GPIO_Configure(GPIOB, 9, OUTPUT, OUTPUT_PPULL); // TX_CMD PB9
	
	/* Passage du module en reception */
	fm_set_mode(FM_RX);
	
	/* Attente de 4 ms pour etre sur que les pll et autres machins choses soient bien activés dans le module FM */
	fm_wait_x_ms(4);
	
	// Configuration reception
	USART->CR1=(USART->CR1)|USART_CR1_RE; // Transmit Enable
	USART->CR1=(USART->CR1)|USART_CR1_RXNEIE; // validation IT locale en reception
}

/**
 *
 * Configure le module FM dans l'un de ses 4 modes possible
 *
 * Parametres:
 *     Entrée: 
 *           	FM_OFF = Le module est inactif
 *					  FM_TX  = Le module est en emission 
 *						FM_RX  = le module est en reception 
 *						FM_TX_LOOPBACK = le module est en emission et reception: les données envoyées sont rebouclées en reception
 *
 *     Sortie:
 *            rien 
 *
 * Limitations:
 *     Aucune
 */
void fm_set_mode(int mode)
{
volatile int temp;

	/* remet PB8 et PB9 à 0 */
	GPIOB->BRR = GPIO_BRR_BR8 | GPIO_BRR_BR9;

	switch (mode)
	{
		case FM_OFF:
			/* Rien a faire  */
			break;
		case FM_TX:
			GPIOB->BSRR = GPIO_BSRR_BS9; // met la ligne TX_CMD à 1 */ 
			break;
		case FM_RX:
			GPIOB->BSRR = GPIO_BSRR_BS8; // met la ligne RX_CMD à 1 */	
			break;
		case FM_TX_LOOPBACK:
			GPIOB->BSRR = GPIO_BSRR_BS8 | GPIO_BSRR_BS9; /* Met les lignes TX_CMD et RX_CMD à 1 */
			break;
	}
}

/* 
 * Enregistre un callback pour remonter au service superieur les données recues 
 */
void fm_set_receive_callback(void (*func_ptr) (char data))
{
	fm_usart_receive_callback = func_ptr;
}

/* 
 * Enregistre un callback pour indiquer au service supeireur que la transmission est completement finie 
 */
void fm_set_transmit_empty_callback(void (*func_ptr) (void))
{
	fm_usart_transmit_empty_callback = func_ptr;
}

/*
 * Envoi un buffer de donnée sur l'USART3
 *
 * RQ: Retourne FM_USART_TX_SUCCESS si la liaison TX est libre
 *     Retourne FM_USART_TX_BUSY si la liaison TX est en cours d'utilisation
 */
int fm_send_data_usart(char *buffer, int length)
{
int i;
	
	int status=FM_USART_SUCCESS;
	
	if (length <=0) return status; /* pas la peine de s'embeter, il n'y a rien a envoyer */
	
	if (fm_is_busy_usart()==FM_USART_IDLE)
	{
		fm_usart_busy_transmit = 1;
		
		/* Recopie du buffer en local */
		for (i=0; i<length; i++)
		{
			fm_usart_buffer_transmit[i]=buffer[i];
		}
		
		fm_usart_transmit_length=length;
		fm_usart_ptr_transmit=0;
		
		/* Passage du module en transmission */
		fm_set_mode(FM_TX);
		
		fm_wait_x_ms(2); /* Attente de 2 ms, pour etre sur que le module s'est correctement configuré en emission (normallement 0,5ms) */
		
		//USART3->DR = buffer[0];					/* On lance la transmission */
		//fm_usart_ptr_transmit++;
		USART3->CR1 |= USART_CR1_TXEIE; 	/* Pour le module FM, on prefere utiliser le flag Tc (transmit complete) 
		                                  qui indique plus finement que la transmission est veritablement finie */
	}
	else status = FM_USART_BUSY;
	
	return status;
}

/*
 * Verifie sur l'USART3 est libre pour transmettre
 *
 * RQ: Retourne FM_USART_IDLE si la liaison TX est libre
 *     Retourne FM_USART_BUSY si la liaison TX est en cours d'utilisation
 */
int fm_is_busy_usart(void)
{
	int status = FM_USART_IDLE;
	
	if (fm_usart_busy_transmit==1) status = FM_USART_BUSY;
	return status;
}

/*
 * Recupere l'etat de la porteuse
 *
 * RQ: Retourne 0 si aucune porteuse n'est trouvée
 *     Retourne 1 si une porteuse est trouvée
 */
int fm_get_carrier_low_level(void)
{
int state;
	
	if (GPIOB->IDR & GPIO_IDR_IDR7)
		state = 0; /* Pas de CD -> Bruit */
	else
		state = 1; /* CD detectée -> Des données valables sont receptionables */
	
	return state;
}

//==============================================================================================================
//                                    Private Functions
//                                     Ne pas modifier
//==============================================================================================================

/*
 * Fonction d'interruption de l'USART3 (FM)
 */
void USART3_IRQHandler(void)
{
volatile int dummy;
volatile u8 data;
	
	// Partie transmission
	if ((((USART3->SR)&USART_SR_TXE)==USART_SR_TXE)&&(((USART3->CR1)&USART_CR1_TXEIE)==USART_CR1_TXEIE)) 
	{
		USART3->DR= fm_usart_buffer_transmit[fm_usart_ptr_transmit];
		fm_usart_ptr_transmit++;
		
		if (fm_usart_ptr_transmit>=fm_usart_transmit_length)
		{
			/* Fin de la transmission, remise à zero du flag TXEIE, autorisant les IT TX */
			USART3->CR1 &= ~USART_CR1_TXEIE;
			fm_usart_busy_transmit=0;
			fm_usart_ptr_transmit=0;
			fm_usart_transmit_length=0;
			
			/* Activation de l'IT TC */
			USART3->CR1 |= USART_CR1_TCIE;
		}
	}
	
	if ((((USART3->SR)&USART_SR_TC)==USART_SR_TC)&&(((USART3->CR1)&USART_CR1_TCIE)==USART_CR1_TCIE))  
	{
		/* On supprime l'IT TC */
		USART3->CR1 &= ~USART_CR1_TCIE;
		
		/* On rebascule en reception */
		fm_wait_x_ms(4); /* Attente de 4 ms -> on s'assure que le dernier caractere a bien été envoyé, modulé, transmit par le module FM (normallement 3 ms)*/
		
		fm_set_mode(FM_RX); /* On repasse en reception */
		
		if (fm_usart_transmit_empty_callback !=0)
		{
			fm_usart_transmit_empty_callback();
		}
	}
	
	// Partie reception
	if (((USART3->SR)&USART_SR_RXNE)==USART_SR_RXNE)
	{
		data = (u8)USART3->DR;
		
		if (fm_usart_receive_callback!=0)
		{
			fm_usart_receive_callback(data);
		}
		
		/* Des fois, l'USART se met en vrille, pour eviter ca, une bonne lecture des registres suffit normallement */
		dummy = USART1->SR;
		dummy = USART1->CR1;
		dummy = USART1->CR2;
		dummy = USART1->CR3;
		dummy = USART1->BRR;
		dummy = USART1->GTPR;
		dummy = USART1->SR;
	}
}

/**
 * Attente de 1 ms.
 */
PRIVATE void fm_wait_1ms(void)
{
#define FM_1MS_DURATION 6024
	
	volatile int i;
	
	for (i=0; i<FM_1MS_DURATION; i++); /* Attente de 1ms */
}

/**
 * Attente de x ms.
 * @param delay Nombre de ms a attendre
 */
PRIVATE void fm_wait_x_ms(int delay)
{
	volatile int i;
	
	for (i=0; i<delay; i++) /* Attente de delay ms */
	{
		fm_wait_1ms();
	}		
}

