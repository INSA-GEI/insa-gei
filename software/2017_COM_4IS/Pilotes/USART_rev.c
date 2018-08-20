//________________________________________________________________________
//

// Version USART spÃ©cifique au TP 4IS.
// MÃªmes fonctions, mais le .h est coupÃ© en deux.
// USART.h + USART_Conf_User.h dans lequel on met juste les defines qui vont
// bien
//________________________________________________________________________
//
//________________________________________________________________________
//

#include "stm32f10x.h"
#include "Clock.h"
#include "GPIO.h"
#include "NVIC_IT_Ext.h"
#include "USART_rev.h"
#include "USART_User_Conf.h"

//=======================================================================================
// 	Auteur T.R.
//  Juin 2016
//  Processeur : STM32F103RB
//  Logiciel dev : ÂµVision 4.70/5.23
//  Version 1, 29 Juin 2017
//  
//=======================================================================================





/*
**************** EMISSION : PRINCIPE fct Put_String *****************************************
Chaine à transmettre = azerty.
En dur : 0x61 0x7A 0x65 0x72 0x74 0x79 0x0 ; Le 0 traduit la fin de la chaîne. C'est universel.

La fonction Put_String se contente d'émettre  0x61 0x7A 0x65 0x72 0x74 0x79
Elle aura détecté le \0 et ne l'aura pas émis. Ou si \0 n'existe pas, alors
la transmission s'arrêtera lors d'un dépassement max (voir define)
AUCUN CARACTERE DE CONTROLE N EST EMIS EN PLUS.

Pour être dans les clous et envoyer selon l'OS un ou plusieurs caractères de contrôle:
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  (\n = LF = 0xA et \r = CR = 0xD)
ce sera à la couche service de dessus d'envoyer deux chaînes :
0x61 0x7A 0x65 0x72 0x74 0x79 0x0
Puis
0x0A 0x0D 0x0


**************** RECEPTION *****************************************
Tous les protocoles finissent par CR mais sait-on jamais... MAC OS en \n ?
La détection du dernier caractère sera donc au choix.
Exemple :
0x61 0x7A 0x65 0x72 0x74 0x79 \n \r  donnera 
0x61 0x7A 0x65 0x72 0x74 0x79 \n \0 en dur
Eventuellement en couche service on pourra ou non supprimer \n si le protocole place
en fin de chaîne \n \r


*/



//=======================================================================================
// Rappel sur les ressources sur le STM32F103RB 
//
// 3 USART possibles :
// USART_1_TX = TIM1_CH2 = PA9
// USART_1_RX = TIM1_CH3 = PA10
// USART_2_TX = TIM2_CH3 = PA2
// USART_2_RX = TIM2_CH4 = PA3
// USART_3_TX = PB10
// USART_3_RX = PB11
//=======================================================================================

/*			VALIDATION

*/




//-------------------------------------------------------------------------------------

//--------------------  Attributs privé  ----------------------------------------------
#define Null 0
// ====== Transmission ===================
char * Pteur_String_Emission[3];
char Transmission_En_Cours[3]; //vu
int Nb_Car_E[3];
int  Longueur_String_Emis[3]; //vu


// ====== Réception ===================
char Byte_Rec_USART[3];
char Flag_Byte_Recu[3]; //vu
//Faire de préférence 3 tableaux
char String_Rec_USART_1[Buf_Len_StrRec_USART1+1];
char String_Rec_USART_2[Buf_Len_StrRec_USART2+1];
char String_Rec_USART_3[Buf_Len_StrRec_USART3+1];

char Flag_Str_Recu[3]; // vu
int Longueur_String_recu[3]; // vu
int Cpt_Byte_Recus[3];

// ====== pointeur callback ===================
static void (*Ptr_IT_Fct_Rec_Byte[3])(void);
static void (*Ptr_IT_Fct_Rec_Str[3])(void);
static void (*Ptr_IT_Fct_Transm_Str[3])(void);
char IT_Rec_Byte_OK[3];
char IT_Rec_Str_OK[3];
char IT_Transm_Str_OK[3];

// ====== Flag d'erreur ===================
char Flag_Overrun_T[3]; // vu
char Flag_Overrun_Str_R[3]; //vu

//-------------------------------------------------------------------------------------

//=======================================================================================
// Fonctions de Configuration USART
//=======================================================================================

void Reset_Var_USART(USART_TypeDef *USART)
{
	int i;
	if (USART==USART1) i=0;
	else if (USART==USART2) i=1;
	else i=2; // usart3

	// Emission :
  Transmission_En_Cours[i]=0;
	Nb_Car_E[i]=0;
	Longueur_String_Emis[i]=0;
		
	// Réception	
	Flag_Byte_Recu[i]=0;
	Byte_Rec_USART[i]=0;
		
	Cpt_Byte_Recus[i]=0;	
  Flag_Str_Recu[i]=0;	
  Longueur_String_recu[i]=0;
  Cpt_Byte_Recus[i]=0;

	// Flag Erreur
  Flag_Overrun_T[i]=0;
	Flag_Overrun_Str_R[i]=0;	
	
	// indic IT
	IT_Rec_Byte_OK[i]=0;
	IT_Rec_Str_OK[i]=0;
}


//_______________________________________________________________________________________
//
void Init_USART(USART_TypeDef *USART,int baudrate,  char Prio_USART)
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


	
//  activation horloge periph + détermination Freq_Periph. + Config IO+ PtrFct_Recep + NVIC_Enable IT :
if (USART==USART1)  
{
	(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_APB2ENR_USART1EN;
	Frequence_Ck_USART_Hz=CLOCK_GetPCLK2();
	// USART_1_TX = TIM1_CH2 = PA9
	// USART_1_RX = TIM1_CH3 = PA10
	GPIO_Configure(GPIOA, 9, OUTPUT, ALT_PPULL);
	GPIO_Configure(GPIOA, 10, INPUT, INPUT_FLOATING);
  NVIC_Enable_IT(37);	
	NVIC_Prio_IT(37, Prio_USART );
}

if (USART==USART2)
{
  (RCC->APB1ENR)=(RCC->APB1ENR) | RCC_APB1ENR_USART2EN;
	Frequence_Ck_USART_Hz=CLOCK_GetPCLK1();
	// USART_2_TX = TIM2_CH3 = PA2
	// USART_2_RX = TIM2_CH4 = PA3
	GPIO_Configure(GPIOA, 2, OUTPUT, ALT_PPULL);
	GPIO_Configure(GPIOA, 3, INPUT, INPUT_FLOATING);
	NVIC_Enable_IT(38);	
	NVIC_Prio_IT(38, Prio_USART );
}
if (USART==USART3)  
{
	(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_APB1ENR_USART3EN;
	Frequence_Ck_USART_Hz=CLOCK_GetPCLK1();
	// USART_3_TX = PB10
	// USART_3_RX = PB11
	GPIO_Configure(GPIOB, 10, OUTPUT, ALT_PPULL);
	GPIO_Configure(GPIOB, 11, INPUT, INPUT_FLOATING);
	NVIC_Enable_IT(39);
  NVIC_Prio_IT(39, Prio_USART );	
	
}


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
USART->CR1=(USART->CR1)|USART_CR1_RE; // Transmit Enable
USART->CR1=(USART->CR1)|USART_CR1_RXNEIE; // validation IT locale en réception

// reset des variables 
Reset_Var_USART(USART);
}

void Init_IT_Serial_Receive_Byte(USART_TypeDef *USART, void (*IT_function) (void))
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Ptr_IT_Fct_Rec_Byte[Num_USART]=IT_function;	
	IT_Rec_Byte_OK[Num_USART]=1;
}

void Init_IT_Serial_Receive_Str(USART_TypeDef *USART, void (*IT_function) (void))
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Ptr_IT_Fct_Rec_Str[Num_USART]=IT_function;	
	IT_Rec_Str_OK[Num_USART]=1;
}

void Init_IT_Serial_Transmit_Str(USART_TypeDef *USART, void (*IT_function) (void))
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Ptr_IT_Fct_Transm_Str[Num_USART]=IT_function;	
	IT_Transm_Str_OK[Num_USART]=1;
}


























//=======================================================================================
// Fonctions de Gestion des flags
//=======================================================================================

char Is_Byte_Received(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	return Flag_Byte_Recu[Num_USART];
}



char Is_Str_Received(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	return Flag_Str_Recu[Num_USART];	
}


char Is_Transmission_Busy(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	return Transmission_En_Cours[Num_USART];	
}


char Is_Overrun_Receive_Str(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	return Flag_Overrun_Str_R[Num_USART];		
}


void Clear_Overrun_Receive_Str(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Flag_Overrun_Str_R[Num_USART]=0;	
}


char Is_Overrun_Transmit(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	return Flag_Overrun_T[Num_USART];	
}


void Clear_Overrun_Transmit(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Flag_Overrun_T[Num_USART]=0;	
}













 









void Plantage_USART(void);

//=======================================================================================
// Fonctions de Transmission USART
//=======================================================================================

void Put_Char(USART_TypeDef *USART,char Car)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	// test overrun
	while(Transmission_En_Cours[Num_USART]==1) 
	{
	}
	Transmission_En_Cours[Num_USART]=1;
	while (((USART->SR)& USART_SR_TXE)==0) // attendre que le buffer soit vide
	{
	}
	USART->DR = Car;
	Transmission_En_Cours[Num_USART]=0;
}

//_______________________________________________________________________________________
//

void Put_String(USART_TypeDef *USART,char * pCar)
{
	int Num_USART_str;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART_str=1;
	else if (USART==USART3) Num_USART_str=2;
	else Num_USART_str=0;

	if (*pCar!=0) // émission que si la chaîne n'est pas nulle
	{
		
		// overrun ?
		while(Transmission_En_Cours[Num_USART_str]==1) 
		{
		}
		Transmission_En_Cours[Num_USART_str]=1;
		Longueur_String_Emis[Num_USART_str]=0;
		while (((USART->SR)& USART_SR_TXE)==0)
		{}// attendre que le buffer soit vide
			
		Pteur_String_Emission[Num_USART_str]=pCar;
		//amorçage IT par écriture du premier caractère :

			
		USART->DR=*Pteur_String_Emission[Num_USART_str];
		Pteur_String_Emission[Num_USART_str]++;
		Nb_Car_E[Num_USART_str]=1;
		// configuration interruption local en transmission
		USART->CR1=(USART->CR1)|USART_CR1_TXEIE; // IT si TXE (Transmit data reg Empty) vaut 1
	}
}


int Get_Len_Transm_String(USART_TypeDef *USART)
{
	int Longueur;
	if (USART==USART1) Longueur=Longueur_String_Emis[0];
	else if  (USART==USART2) Longueur=Longueur_String_Emis[1];
	else if (USART==USART3) Longueur=Longueur_String_Emis[2];
	else Longueur = -1;
	return Longueur;
}































//=======================================================================================
// Fonctions de Réception USART
//=======================================================================================



char Get_Char(USART_TypeDef *USART)
{
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) Num_USART=1;
	else if (USART==USART3) Num_USART=2;
	else Num_USART=0;
	Flag_Byte_Recu[Num_USART]=0;
	return Byte_Rec_USART[Num_USART];	
}

char * Get_String(USART_TypeDef *USART)
{
	char *Ptr_Char;
	int Num_USART;
	//USART_Utilisee=USART;
	if (USART==USART2) 
	{
		Num_USART=1;
		Ptr_Char= String_Rec_USART_2;
	}
	else if (USART==USART3)
	{
		Num_USART=2;
		Ptr_Char= String_Rec_USART_3;
	}
	else // usart 1
	{
		Num_USART=0;
		Ptr_Char= String_Rec_USART_1;
	}
	
	Flag_Str_Recu[Num_USART]=0;
	return Ptr_Char;	
}


int Get_Len_Receiv_String(USART_TypeDef *USART)
{
	int Longueur;
	if (USART==USART1) Longueur=Longueur_String_recu[0];
	else if  (USART==USART2) Longueur=Longueur_String_recu[1];
	else if (USART==USART3) Longueur=Longueur_String_recu[2];
	else Longueur = -1;
	return Longueur;
}































//=======================================================================================
// Les fonctions d'IT de l'USART
//=======================================================================================


void USART1_IRQHandler(void)
/*


*/ 

{
  char Octet;
	// Put_Str (put char sans IT...)
  // si transmission => test à la fois du flag TXE et de sa valisation TXEIE (le flag peut être à 1 et enable à 0 !)

	
/*
	**************************************************************************************************
	**  	EMISSION STRING		
	**************************************************************************************************
*/	
	
	// si TXE et sa validation 
	if ((((USART1->SR)&USART_SR_TXE)==USART_SR_TXE)&&(((USART1->CR1)&USART_CR1_TXEIE)==USART_CR1_TXEIE)) 
	{
	  // // **********  TEST FIN TRANSMISSION  ****************
		if ((Nb_Car_E[0]==Max_Car_In_String_Emission_1)||(*Pteur_String_Emission[0]==Null))
		{
			// fin de transmission : on verrouille la longueur du string reçu
			Longueur_String_Emis[0]=Nb_Car_E[0];
			USART1->CR1=(USART1->CR1)&~USART_CR1_TXEIE; 
			Transmission_En_Cours[0]=0;
			// Appel fct si configuré
			if (IT_Transm_Str_OK[0]==1)
			{
				Ptr_IT_Fct_Transm_Str[0]();
			}
		}
		else 
		{
		// en cours de transmission
		USART1->DR=*Pteur_String_Emission[0];
		Pteur_String_Emission[0]++;
		Nb_Car_E[0]++;
		}

	}
	
/*
	**************************************************************************************************
	**  	RECEPTION		
	**************************************************************************************************
*/	

	// Accumulation dans String_Rec_USART[i] jusqu'à détection term Car ou longueur excessive
	// récupération systématique byte (pour getchar)
	if (((USART1->SR)&USART_SR_RXNE)==USART_SR_RXNE)
		
	
	{
			Octet=USART1->DR;
		  // test overrun
			if (Flag_Str_Recu[0]==1)
			{
				Flag_Overrun_Str_R[0]=1;
				Plantage_USART();
			}
			// Flag str recu = 0	
			Flag_Str_Recu[0]=0;
		
			// **********   TEST FIN RECEPTION  ****************
			if ((Octet==Term_Car_USART1) ||(Cpt_Byte_Recus[0]==Buf_Len_StrRec_USART1))  
				{
				// arrêt de l'accumulation par un remplissage 00 en dernier caract
				String_Rec_USART_1[Cpt_Byte_Recus[0]]=0;
				// Récupération de la longueur de la chaîne
				Longueur_String_recu[0]=Cpt_Byte_Recus[0];
				// remise à 0 cpteur Byte pour prochaine chaîne
				Cpt_Byte_Recus[0]=0;
				Flag_Str_Recu[0]=1;
				// levée callback si validée	
				if 	(IT_Rec_Str_OK[0]==1)
					{
						Ptr_IT_Fct_Rec_Str[0]();
					}
			
				}
			
			else
				{
					// on accumule
					String_Rec_USART_1[Cpt_Byte_Recus[0]]=Octet;
					Cpt_Byte_Recus[0]++;
				}
		
		// Récup Octet
		Flag_Byte_Recu[0]=1;
		Byte_Rec_USART[0]=Octet;
	
		#ifdef ECHO_USART1
		USART1->DR=Octet;
		#endif
	
		// levée callback si validée	
		if 	(IT_Rec_Byte_OK[0]==1)
			{
			Ptr_IT_Fct_Rec_Byte[0]();
			}

				
	}
			
	
	
}

// USART2_IRQHandler est desormais implementé dans Xbee_usart.c

//void USART2_IRQHandler(void)
//{

//  char Octet;

///*
//	**************************************************************************************************
//	**  	EMISSION STRING		
//	**************************************************************************************************
//*/	

//	if ((((USART2->SR)&USART_SR_TXE)==USART_SR_TXE)&&(((USART2->CR1)&USART_CR1_TXEIE)==USART_CR1_TXEIE)) 
//	{
//	  // // **********  TEST FIN TRANSMISSION  ****************
//		if ((Nb_Car_E[1]==Max_Car_In_String_Emission_2)||(*Pteur_String_Emission[1]==Null))
//		{
//			// fin de transmission : on verrouille la longueur du string reçu
//			Longueur_String_Emis[1]=Nb_Car_E[1];
//			USART2->CR1=(USART2->CR1)&~USART_CR1_TXEIE; 
//			Transmission_En_Cours[1]=0;
//			// Appel fct si configuré
//			if (IT_Transm_Str_OK[1]==1)
//			{
//				Ptr_IT_Fct_Transm_Str[1]();
//			}
//		}
//		else 
//		{
//		// en cours de transmission
//		USART2->DR=*Pteur_String_Emission[1];
//		Pteur_String_Emission[1]++;
//		Nb_Car_E[1]++;
//		}

//	}
//	
///*
//	**************************************************************************************************
//	**  	RECEPTION		
//	**************************************************************************************************
//*/	

//	// Accumulation dans String_Rec_USART[i] jusqu'à détection term Car ou longueur excessive
//	// récupération systématique byte (pour getchar)
//	if (((USART2->SR)&USART_SR_RXNE)==USART_SR_RXNE)
//		
//	
//	{
//			Octet=USART2->DR;
//		  // test overrun
//			if (Flag_Str_Recu[1]==1)
//			{
//				Flag_Overrun_Str_R[1]=1;
//				Plantage_USART();
//			}
//			// Flag str recu = 0	
//			Flag_Str_Recu[1]=0;
//		
//			// **********   TEST FIN RECEPTION  ****************
//			if ((Octet==Term_Car_USART2) ||(Cpt_Byte_Recus[1]==Buf_Len_StrRec_USART2))  
//				{
//				// arrêt de l'accumulation par un remplissage 00 en dernier caract
//				String_Rec_USART_2[Cpt_Byte_Recus[1]]=0;
//				// Récupération de la longueur de la chaîne
//				Longueur_String_recu[1]=Cpt_Byte_Recus[1];
//				// remise à 0 cpteur Byte pour prochaine chaîne
//				Cpt_Byte_Recus[1]=0;
//				Flag_Str_Recu[1]=1;
//				// levée callback si validée	
//				if 	(IT_Rec_Str_OK[1]==1)
//					{
//						Ptr_IT_Fct_Rec_Str[1]();
//					}
//			
//				}
//			
//			else
//				{
//					// on accumule
//					String_Rec_USART_2[Cpt_Byte_Recus[1]]=Octet;
//					Cpt_Byte_Recus[1]++;
//				}
//		
//		// Récup Octet
//		Flag_Byte_Recu[1]=1;
//		Byte_Rec_USART[1]=Octet;
//	
//		#ifdef ECHO_USART2
//		USART2->DR=Octet;
//		#endif
//	
//		// levée callback si validée	
//		if 	(IT_Rec_Byte_OK[1]==1)
//			{
//			Ptr_IT_Fct_Rec_Byte[1]();
//			}

//				
//	}

//}


//void USART3_IRQHandler(void)
//{

//  char Octet;

///*
//	**************************************************************************************************
//	**  	EMISSION STRING		
//	**************************************************************************************************
//*/	

//	if ((((USART3->SR)&USART_SR_TXE)==USART_SR_TXE)&&(((USART3->CR1)&USART_CR1_TXEIE)==USART_CR1_TXEIE)) 
//	{
//	  // // **********  TEST FIN TRANSMISSION  ****************
//		if ((Nb_Car_E[2]==Max_Car_In_String_Emission_3)||(*Pteur_String_Emission[2]==Null))
//		{
//			// fin de transmission : on verrouille la longueur du string reçu
//			Longueur_String_Emis[2]=Nb_Car_E[2];
//			USART3->CR1=(USART3->CR1)&~USART_CR1_TXEIE; 
//			Transmission_En_Cours[2]=0;
//			// Appel fct si configuré
//			if (IT_Transm_Str_OK[2]==1)
//			{
//				Ptr_IT_Fct_Transm_Str[2]();
//			}
//		}
//		else 
//		{
//		// en cours de transmission
//		USART3->DR=*Pteur_String_Emission[2];
//		Pteur_String_Emission[2]++;
//		Nb_Car_E[2]++;
//		}

//	}
//	
///*
//	**************************************************************************************************
//	**  	RECEPTION		
//	**************************************************************************************************
//*/	

//	// Accumulation dans String_Rec_USART[i] jusqu'à détection term Car ou longueur excessive
//	// récupération systématique byte (pour getchar)
//	if (((USART3->SR)&USART_SR_RXNE)==USART_SR_RXNE)
//		
//	
//	{
//			Octet=USART3->DR;
//		  // test overrun
//			if (Flag_Str_Recu[2]==1)
//			{
//				Flag_Overrun_Str_R[2]=1;
//				Plantage_USART();
//			}
//			// Flag str recu = 0	
//			Flag_Str_Recu[2]=0;
//		
//			// **********   TEST FIN RECEPTION  ****************
//			if ((Octet==Term_Car_USART3) ||(Cpt_Byte_Recus[2]==Buf_Len_StrRec_USART3))  
//				{
//				// arrêt de l'accumulation par un remplissage 00 en dernier caract
//				String_Rec_USART_3[Cpt_Byte_Recus[2]]=0;
//				// Récupération de la longueur de la chaîne
//				Longueur_String_recu[2]=Cpt_Byte_Recus[2];
//				// remise à 0 cpteur Byte pour prochaine chaîne
//				Cpt_Byte_Recus[2]=0;
//				Flag_Str_Recu[2]=1;
//				// levée callback si validée	
//				if 	(IT_Rec_Str_OK[2]==1)
//					{
//						Ptr_IT_Fct_Rec_Str[2]();
//					}
//			
//				}
//			
//			else
//				{
//					// on accumule
//					String_Rec_USART_3[Cpt_Byte_Recus[2]]=Octet;
//					Cpt_Byte_Recus[2]++;
//				}
//		
//		// Récup Octet
//		Flag_Byte_Recu[2]=1;
//		Byte_Rec_USART[2]=Octet;
//	
//		#ifdef ECHO_USART3
//		USART3->DR=Octet;
//		#endif
//	
//		// levée callback si validée	
//		if 	(IT_Rec_Byte_OK[2]==1)
//			{
//			Ptr_IT_Fct_Rec_Byte[2]();
//			}

//				
//	}

//}


void Plantage_USART(void)
{
	// si vous êtes dans cette fonction, c'est qu'il s'est produit
	// un bourrage réception.
	// observer les flags
	
  //Flag_Overrun_Str_R
	
	// 0 =usart1, 1 = USART2, 2=USART3
	

#ifdef Debugg_Bourrage	
	while(1);
#endif
}
