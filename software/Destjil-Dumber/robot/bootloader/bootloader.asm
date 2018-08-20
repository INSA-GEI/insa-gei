;***************************************************************************
;*
;* Bootloader
;*
;*
;***************************************************************************
.include "m88Pdef.inc"

.org 		0xF00 		;Pour un bootloader de 256 mots

;***** Global constante
.equ		MYUBRR_H=$20
.equ		MYUBRR_L=$10
	
;***************************************************************************
;* 
;* RESET
;*
;* Blabla
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
RESET:
;***** Initialize stack pointer
;* Initialize stack pointer to highest address in internal SRAM
;* Comment out for devices without SRAM

	LDI		R16,high(RAMEND) ;High byte only required if 
	OUT		SPH,R16	         ;RAM is bigger than 256 Bytes
	LDI		R16,low(RAMEND)	 
	OUT		SPL,R16

;	RJMP 	main

;***************************************************************************
;* 
;* main
;*
;* Blabla
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
main:
	; Init de l'UART: 9600 8N1
	; Set baud rate
	LDI 	R16,MYUBRR_H
	LDI 	R17,MYUBRR_L
	STS 	UBRR0H, R16
	STS 	UBRR0L, R17
	; Enable receiver and transmitter
	LDI 	R16, (1<<RXEN0)|(1<<TXEN0)
	STS 	UCSR0B, R16
	; Set frame format: 8data, 1stop bit
	LDI 	R16, (3<<UCSZ00)
	STS 	UCSR0C, R16

	; Init du timer 1 (prescaler / 1024)
	LDI		R16, (1<<WGM12)|(1<<CS12)|(1<<CS10)
	STS		TCCR1B, R16

	; Verifie la presence d'un moniteur coté PC
	RCALL	probePresence	

	SBRC	R16,0
	RCALL	reprog
	EOR		R30,R30
	EOR		R31,R31
	IJMP

forever:
	SLEEP
	;RJMP	forever		;eternal loop

;***************************************************************************
;* 
;* probePresence
;*
;* Test la presence d'un logiciel client sur le PC  
;* Envoi du caractere 0xA5 et attente du caractere 0x8E en retour du PC
;* Re essai au bout de 100 ms, max 5 fois
;* retour: R16=1 si OK, 0 sinon
;*
;***************************************************************************

;***** Subroutine register variables
.equ	CONTACT_CHAR=$A5
;***** Code
probePresence:
	LDI		R17,CONTACT_CHAR
	RCALL	writeChar
	RET

;***************************************************************************
;* 
;* writeChar
;*
;* Envoi d'un caratere sur la LS
;* Entree: R16= caractere a envoyer
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
writeChar:
	LDS 	R17, UCSR0A
	SBRS 	R17, UDRE0
	RJMP 	writeChar
	; Put data (r16) into buffer, sends the data
	STS 	UDR0,R16
	RET

;***************************************************************************
;* 
;* reprog
;*
;* Execute la routine de reprogrammation
;* Entree: rien
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
reprog:

	RET
