#include "esp8266.h"

uint8_t reBuffer[1024];
uint16_t reBufferLen;

// ��ʼ��
void ESP8266_init(void)
{
    // ��ʼ������3
    MX_USART3_UART_Init();

    // ����ATָ���λESP32
    uint8_t* cmd = "AT+RST\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // ��ʱ�ȴ�
    HAL_Delay(2000);
}

// ��������
void ESP8266_sendCmd(uint8_t* cmd, uint16_t cmdLen)
{
    // �����Ӧ������
    memset(reBuffer, 0, 1024);

    // ͨ������3��������
    HAL_UART_Transmit(&huart3, cmd, cmdLen, 1000);

    //��ȡ��Ӧ
    uint32_t timeout = HAL_GetTick();
    do
    {
        ESP8266_readReply(reBuffer, &reBufferLen);
        if (HAL_GetTick() - timeout > 15000) {  // 15�볬ʱ
            printf("Timeout! No OK received.\n");
            break;
        }
        if (strstr((char*)reBuffer, "ERROR") != NULL) {  // ��������Ӧ
            printf("Command failed: %.*s\n", reBufferLen, reBuffer);
            break;
        }
    } while (strstr((char*)reBuffer, "OK") == NULL);
    
    // ��ӡ���
    printf("%.*s\n", reBufferLen, reBuffer);
}

// ��ȡ��Ӧ
void ESP8266_readReply(uint8_t* buffer, uint16_t* buffLen)
{
    // ����3���ձ䳤����
    HAL_UARTEx_ReceiveToIdle(&huart3, buffer, 1024, buffLen, 1000);
}
