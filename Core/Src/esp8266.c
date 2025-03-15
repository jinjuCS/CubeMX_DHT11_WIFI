#include "esp8266.h"

uint8_t reBuffer[1024];
uint16_t reBufferLen;

// 初始化
void ESP8266_init(void)
{
    // 初始化串口3
    MX_USART3_UART_Init();

    // 发送AT指令，复位ESP32
    uint8_t* cmd = "AT+RST\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 延时等待
    HAL_Delay(2000);
}

// 发送命令
void ESP8266_sendCmd(uint8_t* cmd, uint16_t cmdLen)
{
    // 清空响应缓冲区
    memset(reBuffer, 0, 1024);

    // 通过串口3发送命令
    HAL_UART_Transmit(&huart3, cmd, cmdLen, 1000);

    //读取响应
    uint32_t timeout = HAL_GetTick();
    do
    {
        ESP8266_readReply(reBuffer, &reBufferLen);
        if (HAL_GetTick() - timeout > 15000) {  // 15秒超时
            printf("Timeout! No OK received.\n");
            break;
        }
        if (strstr((char*)reBuffer, "ERROR") != NULL) {  // 检查错误响应
            printf("Command failed: %.*s\n", reBufferLen, reBuffer);
            break;
        }
    } while (strstr((char*)reBuffer, "OK") == NULL);
    
    // 打印输出
    printf("%.*s\n", reBufferLen, reBuffer);
}

// 读取响应
void ESP8266_readReply(uint8_t* buffer, uint16_t* buffLen)
{
    // 串口3接收变长数据
    HAL_UARTEx_ReceiveToIdle(&huart3, buffer, 1024, buffLen, 1000);
}
