#ifndef __WIFI_H
#define __WIFI_H

#include "esp8266.h"

// 定义枚举类型，STA模式和AP模式
typedef enum
{
    STA = 1,
    AP = 2
} WIFI_MODE;

// 初始化（以STA模式或AP模式）
void WIFI_init(WIFI_MODE mode);

// STA工作模式
void WIFI_STA_MODE(void);

// AP工作模式
void WIFI_AP_MODE(void);

// 启动TCP服务器
void WIFI_TCP_serverStart(void);

// 发送数据（包含连接ID）
void WIFI_ID_sendData(uint8_t ID, uint8_t* data, uint16_t len);

// 接收数据（包含连接ID，对端IP和接口号）
// void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID, uint8_t* ip, uint16_t* port);
void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID);

#endif // !__WIFI_H
