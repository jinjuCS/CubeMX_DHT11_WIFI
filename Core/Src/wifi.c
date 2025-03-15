#include "wifi.h"

// 初始化（以STA模式或AP模式）
void WIFI_init(WIFI_MODE mode)
{
    // esp8266初始化
    ESP8266_init();

    // 设置工作模式及IP设置
    if (mode == STA)
    {
        WIFI_STA_MODE();
    }
    else if (mode == AP)
    {
        WIFI_AP_MODE();
    }

    printf("WIFI模块初始化成功\n");
}

// STA工作模式
void WIFI_STA_MODE(void)
{
    // 配置WIFI模式
    printf("WIFI模式设置为STA模式\n");
    uint8_t* cmd = "AT+CWMODE=1\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 连接到指定AP
    printf("连接AP\n");
    cmd = "AT+CWJAP=\"RedmiK50\",\"123456789\"\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 查询本机IP
    printf("本机IP为\n");
    cmd = "AT+CIPSTA?\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// AP工作模式
void WIFI_AP_MODE(void)
{
    // 配置WIFI模式
    printf("WIFI模式设置为AP模式\n");
    uint8_t* cmd = "AT+CWMODE=2\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 设置AP参数
    printf("设置AP\n");
    cmd = "AT+CWSAP=\"ESP8266\",\"1234567890\",5,3\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 设置本机IP
    printf("设置本机IP\n");
    cmd = "AT+CIPAP=\"192.168.5.1\"\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// 启动TCP服务器
void WIFI_TCP_serverStart(void)
{
    // 设置使能多连接
    printf("设置使能多连接\n");
    uint8_t* cmd = "AT+CIPMUX=1\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 开启TCP服务器
    printf("开启TCP服务器\n");
    cmd = "AT+CIPSERVER=1,8080\r\n";
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 设置IPD数据格式（提示对端IP和端?）
    // printf("设置IPD数据格式\n");
    // cmd = "AT+CIPDINFO=1\r\n";
    // ESP8266_sendCmd(cmd, strlen((char*)cmd));
}

// 发送数据（包含连接ID）
void WIFI_ID_sendData(uint8_t ID, uint8_t* data, uint16_t len)
{
    // 设置发送数据的连接ID和数据长度
    printf("发送数据\n");
    uint8_t cmd[50] = {0};
    sprintf((char*)cmd, "AT+CIPSEND=%d,%d\r\n", ID, len);
    ESP8266_sendCmd(cmd, strlen((char*)cmd));

    // 进入数据模式，通过串口发送数据
    HAL_UART_Transmit(&huart3, data, len, 1000);
}

// 用临时缓冲区接收IPD数据
uint8_t tempBuffer[1024] = {0};
uint16_t tempLen;

// 接收数据（包含连接ID，对端IP和接口号）
// void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID, uint8_t* ip, uint16_t* port)
void WIFI_ID_readData(uint8_t* buffer, uint16_t* len, uint8_t* ID)
{
    // 使用串口接收数据
    HAL_UARTEx_ReceiveToIdle(&huart3, tempBuffer, 1024, &tempLen, 1000);

    // 从接收到的IPD数据中解析数据

    // 判断是否一+IPD为头，进行过滤
    // while (strstr((char*)tempBuffer, "+IPD") != NULL)
    // {
    //     // 利用sscanf和正则表达式提取所有的信息
    //     sscanf((char*)tempBuffer, "%*[\r\n]+IPD,%hhu,%hu,\"%[^\"]\",%hu", ID, len, ip, port);

    //     // 将数据复制到传入缓冲区参数中
    //     char* pData = strstr((char*)tempBuffer, ":") + 1;
    //     memcpy(buffer, pData, *len);
    // }
    while (strstr((char*)tempBuffer, "+IPD") != NULL)
    {
        // 利用sscanf和正则表达式提取所有的信息
        sscanf((char*)tempBuffer, "%*[\r\n]+IPD,%hhu,%hu", ID, len);

        // 将数据复制到传入缓冲区参数中
        char* pData = strstr((char*)tempBuffer, ":") + 1;
        memcpy(buffer, pData, *len);
    }
}
