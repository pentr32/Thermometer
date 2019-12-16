#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_TOL 2

#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

void UartPutchar(char data);
int UartGetchar(void);

static FILE the_stdio = FDEV_SETUP_STREAM(UartPutchar, UartGetchar, _FDEV_SETUP_RW);

void UartInit(void)
{
	stdout = &the_stdio;
	stdin = &the_stdio;
	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#endif

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// 8-bit, 1 stop bit, no parity, asynchronous UART
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) |
	(0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) |
	(0 << UMSEL00);
}

void UartPutchar(char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int UartGetchar(void)
{
	char data;
	
	while (!(UCSR0A & (1 << RXC0)));
	
	data = UDR0;
	
	if (data == '\n') {
		return -1;
	}
	UartPutchar(data);

	return data;
}