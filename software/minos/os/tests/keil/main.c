/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: fichier vide pour projet
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm_regs.h"
#include "kernel.h"

#include "stm_clock.h"
#include "lcd.h"

#include <stdio.h>

/* Description des taches */
// Tache 10ms
TaskType Tache10ms_ID;
st_TaskInfo Tache10ms_info;
const char Tache10ms_nom[] = "Tache_10ms";
TASK(Tache10ms);

// Tache 500ms
TaskType Tache500ms_ID;
st_TaskInfo Tache500ms_info;
const char Tache500ms_nom[] = "Tache_500ms";
TASK(Tache500ms);

// Tache LCD
TaskType TacheLCD_ID;
st_TaskInfo TacheLCD_info;
const char TacheLCD_nom[] = "Tache_LCD";
TASK(TacheLCD);

// Event 10ms -> utilisé par Tache10ms 
st_EventInfo Event10ms_Info;
EventMaskType Event10ms_ID;

// Event 500ms -> utilisé par Tache500ms 
st_EventInfo Event500ms_Info;
EventMaskType Event500ms_ID;

// Event LCD -> utilisé par TacheLCD 
st_EventInfo EventLCD_Info;
EventMaskType EventLCD_ID;

// Resource 1 -> partagée par tache 1 et 2
ResourceTasksListType ResourceTasks;
st_ResourceInfo Resource1_Info;
ResourceType Resource1_ID;

void Init_Periph(void);

#define _PERIODE_TIM1_ 30456
#define _PERIODE_TIM2_ 720

#define _PRESCALER_TIM1_ 100
#define _PRESCALER_TIM2_ 25
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)
{
	//stm32_Init ();                                  // STM32 setup

	/* Init LCD */
	lcd_init();
	 
	Init_Periph();

	/* Code du projet a rajouter ici */
	InitOS();

	set_cursor(0,1);
	lcd_print("ca demarre");
	set_cursor(0,0);

	Tache10ms_info.taskname = (char*)Tache10ms_nom;
	Tache10ms_info.entrypoint = Tache10ms;
	Tache10ms_info.priority = 3;

	Tache10ms_ID = DeclareTask(&Tache10ms_info);

	Tache500ms_info.taskname = (char*)Tache500ms_nom;
	Tache500ms_info.entrypoint = Tache500ms;
	Tache500ms_info.priority = 4;

	Tache500ms_ID = DeclareTask(&Tache500ms_info);

	TacheLCD_info.taskname = (char*)TacheLCD_nom;
	TacheLCD_info.entrypoint = TacheLCD;
	TacheLCD_info.priority = 2;

	TacheLCD_ID = DeclareTask(&TacheLCD_info);

	Event10ms_Info.TaskID = Tache10ms_ID;
	Event10ms_ID = DeclareEvent(&Event10ms_Info);

	Event500ms_Info.TaskID = Tache500ms_ID;
	Event500ms_ID = DeclareEvent(&Event500ms_Info);

	EventLCD_Info.TaskID = TacheLCD_ID;
	EventLCD_ID = DeclareEvent(&EventLCD_Info);

	/*ClearResourceTasksList(ResourceTasks);
	AddTaskInResourceTasksList(ResourceTasks,Tache_1_ID);
	AddTaskInResourceTasksList(ResourceTasks,Tache_2_ID);
	Resource1_ID = DeclareResource(&Resource1_Info,ResourceTasks);*/

	StartOS(OSDEFAULTAPPMODE);

	while(1); /* Boucle infinie : un main ne doit jamais rendre la main (ah ah, jeu de mot) */

	return 0; 
}

TASK(Tache10ms)
{
static int compteur=0;

	while (1)
	{
		WaitEvent(Event10ms_ID);
		ClearEvent(Event10ms_ID);
		
		compteur++;
		
		if (compteur>=200)
		{
			compteur =0;
		
			GPIOB->ODR = GPIOB->ODR  ^ 0x100;
		}
	}
}

TASK(Tache500ms)
{
	while (1)
	{
		WaitEvent(Event500ms_ID);
		ClearEvent(Event500ms_ID);
	
		GPIOB->ODR = GPIOB->ODR  ^ 0x8000;
	}
}

TASK(TacheLCD)
{
static int compteur;

	while (1)
	{
		WaitEvent(EventLCD_ID);
		ClearEvent(EventLCD_ID);
	
		GPIOB->ODR = GPIOB->ODR  ^ 0x1000;

		compteur ++;
		if (compteur>=10000) compteur =0;

		printf ("\n         \nCpt=%i",compteur);
	}
}

void BackgroundTask(void)
{
volatile int j;

	while (1)
	{
		j=j+1;
	}
}

void Init_Periph(void)
{
	/* Init de l'horloge */
	//Init_Clock_System();

	/* Init Timer 1 */
	RCC->APB2ENR |= RCC_TIM1EN; /* Mise en route de l'horloge du timer 1 */

	TIM1->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM1->PSC = _PRESCALER_TIM1_;
	TIM1->ARR = _PERIODE_TIM1_; /* Periode de PWM -> 423ms */

	TIM1->CR1 |= TIM_CEN; 
	TIM1->DIER |= TIM_UIE;
	 
	/* Init Timer 2 */
	RCC->APB1ENR |= RCC_TIM2EN; /* Mise en route de l'horloge du timer 2 */

	TIM2->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM2->PSC = _PRESCALER_TIM2_;
	TIM2->ARR = _PERIODE_TIM2_; /* Periode de PWM -> 10ms */

	TIM2->CR1 |= TIM_CEN; 
	TIM2->DIER |= TIM_UIE;

	/* Init GPIOB */
	RCC->APB2ENR |= RCC_IOPBEN;

	GPIOB->ODR = /*GPIO_PIN_8 + GPIO_PIN_2 + GPIO_PIN_3*/ 0;	
	
	/*GPIOB->CRH &= ~((3<<GPIO_MODE_8_SHIFT) + (3<<GPIO_CNF_8_SHIFT) +
	               (3<<GPIO_MODE_12_SHIFT) + (3<<GPIO_CNF_12_SHIFT) +
	               (3<<GPIO_MODE_15_SHIFT) + (3<<GPIO_CNF_15_SHIFT)); */
	
	GPIOB->CRH = (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_8_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_8_SHIFT) +
	             (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_9_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_9_SHIFT) +
	             (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_10_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_10_SHIFT) +
				 (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_11_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_11_SHIFT) +
				 (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_12_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_12_SHIFT) +
				 (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_13_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_13_SHIFT) +
				 (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_14_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_14_SHIFT) +
				 (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_15_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_15_SHIFT);
			 
	/* Init NVIC */
	NVIC_SET_PRIO_SYSTEM(SVCALL, 14);
	NVIC_SET_PRIO_SYSTEM(PENDSV, 14);
	NVIC_SET_PRIO_SYSTEM(SYSTICK, 15);

	NVIC_SET_PRIO_PERIPH(TIM1_UP,10);
	NVIC_SET_PRIO_PERIPH(TIM2,12);

	NVIC_SET_PRIO_SYSTEM(MEM_FAULT, 1);
	NVIC_SET_PRIO_SYSTEM(BUS_FAULT, 0);
	NVIC_SET_PRIO_SYSTEM(USAGE_FAULT, 2);

	NVIC_ENABLE_PERIPH_IT(TIM1_UP);
	NVIC_ENABLE_PERIPH_IT(TIM2);

	NVIC_ENABLE_SYSTEM_IT(MEM_FAULT);
	NVIC_ENABLE_SYSTEM_IT(BUS_FAULT);
	NVIC_ENABLE_SYSTEM_IT(USAGE_FAULT);	
}

void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = TIM1->SR & ~(TIM_UIF);

	SetEvent(TacheLCD_ID, EventLCD_ID);
}

void TIM2_IRQHandler(void)
{
static int compteur_1;
	
	TIM2->SR = TIM2->SR & ~(TIM_UIF);
	
	SetEvent(Tache10ms_ID, Event10ms_ID);

	compteur_1++;
	
	if (compteur_1>=500) 
	{
		compteur_1 = 0;

		SetEvent(Tache500ms_ID, Event500ms_ID);
	}
}
