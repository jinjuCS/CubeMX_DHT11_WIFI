#ifndef __ESP8266_H
#define __ESP8266_H

#include "usart.h"
#include <string.h>

// ��ʼ��
void ESP8266_init(void);

// ��������
void ESP8266_sendCmd(uint8_t* cmd, uint16_t cmdLen);

// ��ȡ��Ӧ
void ESP8266_readReply(uint8_t* buffer, uint16_t* buffLen);

#endif // !__ESP8266_H
