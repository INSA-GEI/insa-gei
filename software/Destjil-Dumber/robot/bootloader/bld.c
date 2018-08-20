#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

register unsigned char status asm("r26");
void (*func)(void);

unsigned char buffer_cmd[32];
unsigned char page_buffer[SPM_PAGESIZE];

void writeChar(unsigned char data);
unsigned char readChar(void);
void checkPresence(void);
void reprog(void);
void programPage (uint8_t page, uint8_t *buf);

#define OK_ANS		'O'
#define RESEND_ANS	'R'

#define FILL_PAGE_CMD	'F'
#define FILL_NEXT_CMD	'N'
#define WRITE_PAGE_CMD  'W'
#define READ_CMD		'r'
#define READ_NEXT_CMD	'n'
#define INFOS_CMD		'I'

#define TIMEOUT_100		0x100
#define TIMEOUT_50		0x80

void main (void)
{
	func =0;

	/* Init de l'UART 9600 8N1 */
	UBRR0 = 0x2010;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	
	/* Init du timer 1 (prescaler / 1024) */
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);

	checkPresence();
	if (status == 0) reprog();

	func();
}

void writeChar(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char readChar(void)
{ 
unsigned char temp;

	status =1;

	/* Wait for data to be received */
	while ( !((temp=UCSR0A) & (1<<RXC0))  &&
	        !(TIFR1 & (1<<OCF1A)));

	if (temp & (1<<RXC0))
	{
		/* Get and return received data from buffer */
		status = 0;
	}

	return UDR0;
}

void checkPresence(void)
{
	/* Test la presence du moniteur PC */
unsigned char compteur;
unsigned char temp;

	compteur =6;
	OCR1A = TIMEOUT_100;

	do
	{
		compteur --;

		writeChar(0xA5);
		temp = readChar();

		if (temp!=0x84) status =1;
	}
	while ((compteur!=0) && (status != 0)); 
}

void reprog(void)
{
unsigned char compteur;
unsigned char temp;
unsigned char longueur_cmd;
unsigned char *ptr;
unsigned char *ptr_buffer_page;
unsigned char crc;
unsigned char index;

unsigned char page;

	OCR1A = TIMEOUT_50;
	
	ptr = buffer_cmd;
	ptr_buffer_page = page_buffer;
	
	compteur =0;

	do
	{	temp = readChar();
		if (compteur==0) longueur_cmd=temp;
		
		*ptr++ = temp;
	}
	while ((compteur<longueur_cmd) && (status!=1));
	
	if (status!=1)
	{
		ptr = buffer_cmd;
		crc=0;

		for (index=0; index<longueur_cmd; index++)
		{
			crc ^= *ptr++;
		}

		if (crc) status =1;
	}

	if (status!=1)
	{
		ptr=buffer_cmd+2;

		switch (buffer_cmd[1])
		{
		case FILL_PAGE_CMD:
			page = *ptr++;
		case FILL_NEXT_CMD:
			for (index=0; index<16; index++)
			{
				*ptr_buffer_page++=*ptr++;	
			}
			writeChar(OK_ANS);
			break;
		case WRITE_PAGE_CMD:
			programPage(page,page_buffer);
			ptr_buffer_page=page_buffer;
			
			for (index=0; index<SPM_PAGESIZE; index++)
			{
				*ptr_buffer_page++=0xFF;
			}
			break;
		case READ_CMD:
			ptr_buffer_page = (unsigned char*)(*ptr++ * SPM_PAGESIZE);
		case READ_NEXT_CMD:
			writeChar(OK_ANS);

			for (index=0; index<16; index++)
			{
				writeChar(*ptr_buffer_page++);	
			}
			break;
		case INFOS_CMD:
			writeChar(OK_ANS);
			writeChar(SPM_PAGESIZE);
			break;
		}
	}
	else
	{
		writeChar(RESEND_ANS);
	}
	
	compteur =0;
	ptr=buffer_cmd;	
}
    
void programPage (uint8_t page, uint8_t *buf)
{
    uint16_t i;
    /*uint8_t sreg;*/
	uint16_t address;

	address = (uint16_t)(page)*SPM_PAGESIZE;
    eeprom_busy_wait ();

    boot_page_erase (address);
    boot_spm_busy_wait ();      // Wait until the memory is erased.

    for (i=0; i<SPM_PAGESIZE; i+=2)
    {
        // Set up little-endian word.

        uint16_t w = *buf++;
        w += (*buf++) << 8;
    
        boot_page_fill (address + i, w);
    }

    boot_page_write (address);     // Store buffer in flash page.
    boot_spm_busy_wait();       // Wait until the memory is written.

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.

    boot_rww_enable ();
}
