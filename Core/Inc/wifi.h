#ifndef __WIFI_H
#define __WIFI_H

#include "esp8266.h"

// ����ö�����ͣ�STAģʽ��APģʽ
typedef enum
{
    STA = 1,
    AP = 2
} WIFI_MODE;

// ��ʼ������STAģʽ��APģʽ��
void WIFI_init(WIFI_MODE mode);

// STA����ģʽ
void WIFI_STA_MODE(void);

// AP����ģʽ
void WIFI_AP_MODE(void);

// ����TCP������
void WIFI_TCP_serverStart(void);

// �������ݣ���������ID��
void WIFI_ID_sendData(uint8_t ID, uint8_t* data, uint16_t len);

// �������ݣ���������ID���Զ�IP�ͽӿںţ�
// void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID, uint8_t* ip, uint16_t* port);
void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID);

#endif // !__WIFI_H
