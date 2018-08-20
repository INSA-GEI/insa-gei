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

.include "definitions.inc"

					.syntax unified		/* Required in order to be able to use Thumb-2 instructions
										   Otherwise, produce only Thumb-1 code (16 bits instructions) */			
					.cpu cortex-m3			 
					.fpu softvfp
					.thumb
					.file	"call_wrapper.S"

					.section ".text"
					.align	2

/* 
 * Call wrapper
 */
					.global	OSCallWrapper_0
					.thumb
					.thumb_func	
					.type	OSCallWrapper_0, %function
OSCallWrapper_0: 

					.global	OSCallWrapper_1
					.thumb
					.thumb_func	
					.type	OSCallWrapper_1, %function
OSCallWrapper_1:

					.global	OSCallWrapper_2
					.thumb
					.thumb_func	
					.type	OSCallWrapper_2, %function
OSCallWrapper_2:

					.global	OSCallWrapper_3
					.thumb
					.thumb_func	
					.type	OSCallWrapper_3, %function
OSCallWrapper_3:    PUSH	{R5-R7, LR}             
					
					MRS		R5, IPSR  		/* R5 contains IPSR register, Reserved bits read as 0 */
					MOVW	R6, #0x01FF
					AND		R5, R5, R6
					CMP		R5, #0
					BEQ		Wrapper_NotIT	/* If (IPSR & 0x1FF) == 0, we are not under IT */
					
					BL		OS_Call			/* If under IT, call OS service */		

					BL		OS_Generate_PendSV	/* Then program a pending SV interrupt */

					POP		{R5-R7, LR}
					BX		LR

Wrapper_NotIT:		POP 	{R5-R7, LR}
					SVC		1 				/* Call OS service (RQ: the immediate value is meaningless)*/
					BX		LR				/* get back to main appli */
                
					.end
