#ifndef __ESP8266_H
#define __ESP8266_H

#include "usart.h"
#include <string.h>

// 初始化
void ESP8266_init(void);

// 发送命令
void ESP8266_sendCmd(uint8_t* cmd, uint16_t cmdLen);

// 读取响应
void ESP8266_readReply(uint8_t* buffer, uint16_t* buffLen);

#endif // !__ESP8266_H
