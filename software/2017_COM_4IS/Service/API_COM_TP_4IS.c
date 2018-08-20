#include "API_Com_TP_4IS.h"
#include "stm32f10x.h"
#include "Timer_1234.h"
#include "Timer_Systick.h"
#include "ADC_DMA.h"
#include "USART_rev.h"
#include "GPIO.h"

/* =============================================
				LES GPIO
 ==============================================*/
GPIO_TypeDef * Quel_GPIO(int Port);
void GPIO_Pin_Conf(int Port, int Broche, int Sens, int Techno)
{
	GPIO_TypeDef * GPIO;
  GPIO=Quel_GPIO(Port);
	GPIO_Configure(GPIO, Broche, Sens, Techno);	
}

void GPIO_Pin_Set(int Port,int Broche)
{
	GPIO_TypeDef * GPIO;
  GPIO=Quel_GPIO(Port);
	GPIO_Set(GPIO,Broche);
}
void GPIO_Pin_Clear(int Port,int Broche)
{
	GPIO_TypeDef * GPIO;
  GPIO=Quel_GPIO(Port);
	GPIO_Clear(GPIO,Broche);	
}

void GPIO_Pin_Toggle(int Port,int Broche)
{
	GPIO_TypeDef * GPIO;
  GPIO=Quel_GPIO(Port);
	GPIO_Toggle(GPIO,Broche);	
}

short GPIO_Pin_Read(int Port,int Broche)
{
	GPIO_TypeDef * GPIO;
  GPIO=Quel_GPIO(Port);
	return GPIO_Read(GPIO,Broche);
}



/* =============================================
				LES TIMERS
 ==============================================*/
 


void Timer_Global_Conf(int Timer, float Duree_us ) // OK simu
{
	switch(Timer)
	{
		case 1:
		{
			Timer_1234_Init(TIM1, Duree_us);
			break;
		}
		case 2:
		{
			Timer_1234_Init(TIM2, Duree_us);
			break;
		}
		case 3:
		{
			Timer_1234_Init(TIM3, Duree_us);
			break;
		}
		case 4:
		{
			Timer_1234_Init(TIM4, Duree_us);
			break;
		}
		case 5:
		{
			Systick_Period(Duree_us);
			SysTick_On;
			break;
		}
		default:
		{
			while(1); // mauvaise conf Timer lors de la conf globale !!!
		}
	}
}

void Timer_IT_Conf(int Timer, char Prio, void (*IT_function) (void)) // OK simu
{
	switch(Timer)
	{
		case 1:
		{
			Active_IT_Debordement_Timer( TIM1, Prio, IT_function);
			break;
		}
		case 2:
		{
			Active_IT_Debordement_Timer( TIM2, Prio, IT_function);
			break;
		}
		case 3:
		{
			Active_IT_Debordement_Timer( TIM3, Prio, IT_function);
			break;
		}
		case 4:
		{
			Active_IT_Debordement_Timer( TIM4, Prio, IT_function);
			break;
		}
		case 5:
		{
			Systick_Prio_IT(Prio,IT_function);
			SysTick_Enable_IT;
			break;
		}
		default:
		{
			while(1); // mauvaise conf Timer lors de la conf IT !!!
		}
	}
}


/* =============================================
				L'ADC
 ============================================= */

void ADC_Global_Conf(int Pin)
{
	switch(Pin)
	{
		case PA0:Port_IO_Init(GPIOA, 0, INPUT, ANALOG);break; 
		case PA1:Port_IO_Init(GPIOA, 1, INPUT, ANALOG);break;
		case PA2:Port_IO_Init(GPIOA, 2, INPUT, ANALOG);break;
		case PA3:Port_IO_Init(GPIOA, 3, INPUT, ANALOG);break;
		case PA4:Port_IO_Init(GPIOA, 4, INPUT, ANALOG);break;
		case PA5:Port_IO_Init(GPIOA, 5, INPUT, ANALOG);break;
		case PA6:Port_IO_Init(GPIOA, 6, INPUT, ANALOG);break;
		case PA7:Port_IO_Init(GPIOA, 7, INPUT, ANALOG);break;
		case PB0:Port_IO_Init(GPIOB, 0, INPUT, ANALOG);break;
		case PB1:Port_IO_Init(GPIOB, 1, INPUT, ANALOG);break;
		case PC0:Port_IO_Init(GPIOC, 0, INPUT, ANALOG);break;
		case PC1:Port_IO_Init(GPIOC, 1, INPUT, ANALOG);break;
		case PC2:Port_IO_Init(GPIOC, 2, INPUT, ANALOG);break;
		case PC3:Port_IO_Init(GPIOC, 3, INPUT, ANALOG);break;
		case PC4:Port_IO_Init(GPIOC, 4, INPUT, ANALOG);break;
		case PC5:Port_IO_Init(GPIOC, 5, INPUT, ANALOG);break;
	}

	Init_TimingADC_ActiveADC(ADC1, 10.0 );
}

int ADC_Get_Value(int Pin)
{
Single_Channel_ADC(ADC1, Pin);
Start_ADC_Conversion(ADC1);	
Wait_On_EOC_ADC (ADC1);
return Read_ADC(ADC1);
}


/* =============================================
				Liaison Serie USART
 ============================================= */
USART_TypeDef * Quel_USART(int Serial);

// Fonctions indicateurs
char Serial_Is_Str_Received(int Serial)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	return Is_Str_Received(USART);
}

char Serial_Is_Byte_Received(int Serial)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	return Is_Byte_Received(USART);
}

char Serial_Is_Overrun_Receive_Str(int Serial)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	return Is_Overrun_Receive_Str(USART);
}


void Serial_Clear_Overrun_Receive_Str(int Serial)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	Clear_Overrun_Receive_Str(USART);
}	

// conf et get set
void Serial_Global_Conf(int Serial,int baudrate, char Prio_USART)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	Init_USART(USART,baudrate, Prio_USART); 

}

char Serial_Get_Char(int Serial) // rem : n'interfère pas avec get_String
																 // le car réside dans une var statique RAM
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	return Get_Char(USART);
}

char * Serial_Get_String(int Serial)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
  return (Get_String(USART)); 	
}

void Serial_Put_Char(int Serial,char Car)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	Put_Char(USART,Car);
}

void Serial_Put_String(int Serial,char * pCar)
{
	USART_TypeDef *USART;
	USART=Quel_USART(Serial);
	Put_String(USART,pCar);
}

//============== private ====================================================

USART_TypeDef * Quel_USART(int Serial)
{
	switch(Serial)
	{
		case USART_1: return USART1;
		case USART_2: return USART2;
		case USART_3: return USART3;
	}
	return USART1; //default
	
}

//============== private ====================================================

GPIO_TypeDef * Quel_GPIO(int Port)
{
	switch(Port)
	{
		case Port_A: return GPIOA;
		case Port_B: return GPIOB;
		case Port_C: return GPIOC;
		case Port_D: return GPIOD;
		case Port_E: return GPIOE;
	}
	return GPIOE; // default
	
}
