
#include "stm32f10x.h"                  // Device header
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>

void SendString(char* data)
{
	int i=0;
	
	while (data[i]!=0)
	{
		ITM_SendChar ((uint32_t)data[i]);
		i++;
	}
	
	ITM_SendChar ((uint32_t)'\n');
	ITM_SendChar ((uint32_t)'\r');
}

void SendFloat(float data)
{
	char str[50];
	
	sprintf (str, "%f", data);
  SendString(str);
}

void SendChar(char data)
{
	ITM_SendChar ((uint32_t)data);
	SendString ("\n\r");
}

void SendUInt32(uint32_t data)
{ 
	char str[50];
	
	sprintf (str, "%u", data);
	SendString(str);
}

void SendInt32(int32_t data)
{ 
	char str[50];
	
	sprintf (str, "%d", data);
	SendString(str);
}

void SendUInt16 (uint16_t data)
{
	char str[50];
	
	sprintf (str, "%u", data);
	SendString(str);
}

void SendUInt8 (uint8_t data)
{
	char str[50];
	
	sprintf (str, "%u", data);
	SendString(str);
}

