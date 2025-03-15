#include "wifi.h"

// ��ʼ������STAģʽ��APģʽ��
void WIFI_init(WIFI_MODE mode)
{
    // esp8266��ʼ��
    ESP8266_init();

    // ���ù���ģʽ��IP����
    if (mode == STA)
    {
        WIFI_STA_MODE();
    }
    else if (mode == AP)
    {
        WIFI_AP_MODE();
    }

    printf("WIFIģ���ʼ���ɹ�\n");
}

// STA����ģʽ
void WIFI_STA_MODE(void)
{
    // ����WIFIģʽ
    printf("WIFIģʽ����ΪSTAģʽ\n");
    uint8_t* cmd = "AT+CWMODE=1\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ���ӵ�ָ��AP
    printf("����AP\n");
    cmd = "AT+CWJAP=\"RedmiK50\",\"123456789\"\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ��ѯ����IP
    printf("����IPΪ\n");
    cmd = "AT+CIPSTA?\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// AP����ģʽ
void WIFI_AP_MODE(void)
{
    // ����WIFIģʽ
    printf("WIFIģʽ����ΪAPģʽ\n");
    uint8_t* cmd = "AT+CWMODE=2\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ����AP����
    printf("����AP\n");
    cmd = "AT+CWSAP=\"ESP8266\",\"1234567890\",5,3\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ���ñ���IP
    printf("���ñ���IP\n");
    cmd = "AT+CIPAP=\"192.168.5.1\"\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// ����TCP������
void WIFI_TCP_serverStart(void)
{
    // ����ʹ�ܶ�����
    printf("����ʹ�ܶ�����\n");
    uint8_t* cmd = "AT+CIPMUX=1\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ����TCP������
    printf("����TCP������\n");
    cmd = "AT+CIPSERVER=1,8080\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ����IPD���ݸ�ʽ����ʾ�Զ�IP�Ͷ�?��
    // printf("����IPD���ݸ�ʽ\n");
    // cmd = "AT+CIPDINFO=1\r\n";
    // ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// �������ݣ���������ID��
void WIFI_ID_sendData(uint8_t ID, uint8_t* data, uint16_t len)
{
    // ���÷������ݵ�����ID�����ݳ���
    printf("��������\n");
    uint8_t cmd[50] = {0};
    sprintf((char*)cmd, "AT+CIPSEND=%d,%d\r\n", ID, len);
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ��������ģʽ��ͨ�����ڷ�������
    HAL_UART_Transmit(&huart3, data, len, 1000);
}

// ����ʱ����������IPD����
uint8_t tempBuffer[1024] = {0};
uint16_t tempLen;

// �������ݣ���������ID���Զ�IP�ͽӿںţ�
// void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID, uint8_t* ip, uint16_t* port)
void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID)
{
    // ʹ�ô��ڽ�������
    HAL_UARTEx_ReceiveToIdle(&huart3, tempBuffer, 1024, &tempLen, 1000);

    // �ӽ��յ���IPD�����н�������

    // �ж��Ƿ�һ+IPDΪͷ�����й���
    // while (strstr((char*)tempBuffer, "+IPD") != NULL)
    // {
    //     // ����sscanf��������ʽ��ȡ���е���Ϣ
    //     sscanf((char*)tempBuffer, "%*[\r\n]+IPD,%hhu,%hu,\"%[^\"]\",%hu", ID, len, ip, port);

    //     // �����ݸ��Ƶ����뻺����������
    //     char* pData = strstr((char*)tempBuffer, ":") + 1;
    //     memcpy(buffer, pData, *len);
    // }
    while (strstr((char*)tempBuffer, "+IPD") != NULL)
    {
        // ����sscanf��������ʽ��ȡ���е���Ϣ
        sscanf((char*)tempBuffer, "%*[\r\n]+IPD,%hhu,%hu", ID, len);

        // �����ݸ��Ƶ����뻺����������
        char* pData = strstr((char*)tempBuffer, ":") + 1;
        memcpy(buffer, pData, *len);
    }
}
