

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stm32f10x.h"                  // Device header

void SendUInt32(uint32_t data);
void SendInt32(int32_t data);
void SendString(char* data);
void SendFloat(float data);
void SendChar(char data);
void SendUInt16 (uint16_t data);
void SendUInt8 (uint8_t data);

#endif /* __DEBUG_H__ */
