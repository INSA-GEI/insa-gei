/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * MinOS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * MinOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with MinOS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm32f10x_type.h"
#include "os_config.h"
#include "kernel.h"
#include "interrupts.h"


u8 InterruptCounter;

/* Interrupt services declaration */

/*
 * void EnableAllInterrupts(void)
 * 
 * Reactive les interruptions
 */
void EnableAllInterrupts(void)
{
	/* 
	 * TODO: Je ne comprends pas trop la spec au sujet de cette fonction. Il ne semble
	 * pas y avoir de compteur d'activation/desactivation: du coup, je vois mal comment marche cette fonction 
	 * et a quoi elle sert
	 *
	 * Je rebranche sur ResumeAllInterrupts
	 */
	
	ResumeAllInterrupts();
}

/*
 * void DisableAllInterrupts(void)
 * 
 * Desactive les interruptions
 */
void DisableAllInterrupts(void)
{
	/* 
	 * TODO: Je ne comprends pas trop la spec au sujet de cette fonction. Il ne semble
	 * pas y avoir de compteur d'activation/desactivation: du coup, je vois mal comment marche cette fonction 
	 * et a quoi elle sert
	 *
	 * Je rebranche sur SuspendAllInterrupts
	 */
	
	SuspendAllInterrupts();	
}

/*
 * void ResumeAllInterrupts(void)
 * 
 * Reactive les interruptions en fonction du nombre desactivations qui ont deja eu lieu
 */
void ResumeAllInterrupts(void)
{
	if (InterruptCounter != 0)
	{
		InterruptCounter--;

		if (InterruptCounter == 0) EnableAllInterrupts_port();
	} 
}

/*
 * void SuspendAllInterrupts(void)
 * 
 * Desactive les interruptions et memorise le nombre de desactivation qui ont deja eu lieu
 */
void SuspendAllInterrupts(void)
{
	InterruptCounter ++;

	DisableAllInterrupts_port();
}

/*
 * void ResumeOSInterrupts(void)
 * 
 * Reactive les interruptions desactivées par l'OS
 */
void ResumeOSInterrupts(void)
{
	/* 
	 * TODO: Je ne comprends pas trop la spec au sujet de cette fonction. Il ne semble
	 * pas y avoir de compteur d'activation/desactivation: du coup, je vois mal comment marche cette fonction 
	 * et a quoi elle sert
	 *
	 * Je rebranche sur ResumeAllInterrupts
	 */
	
	ResumeAllInterrupts();
}

/*
 * void SuspendOSInterrupts(void)
 * 
 * Desactive les interruptions (demandé par l'OS)
 */
void SuspendOSInterrupts(void)
{
	/* 
	 * TODO: Je ne comprends pas trop la spec au sujet de cette fonction. Il ne semble
	 * pas y avoir de compteur d'activation/desactivation: du coup, je vois mal comment marche cette fonction 
	 * et a quoi elle sert
	 *
	 * Je rebranche sur SuspendAllInterrupts
	 */
	
	SuspendAllInterrupts();	
}

/*
 * void Interrupt_Init(void)
 * 
 * Ininitialise le module Interrupt du noyau
 */
void Interrupt_Init(void)
{
	InterruptCounter=0; 	// Compteur d'imbrication des activations/desactivation d'interruption à 0 (It autorisées) 

	SuspendAllInterrupts(); // Desactives les IT (le temps pour l'OS de s'initialiser
}
