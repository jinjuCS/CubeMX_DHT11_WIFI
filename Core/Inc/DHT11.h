#ifndef __DHT11_H
#define __DHT11_H

#include "gpio.h"
#include "tim.h"

/************************** DHT11 数据类型定义********************************/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
} DHT11_Data_TypeDef;

// STM32->DHT11
void STM32_DHT11(void);

// DHT11->STM32
void DHT11_STM32(void);

// STM32向DHT11发送起始信号
void STM32_DHT11_start(void);

// 检查DHT11是否向STM32发送应答信号，发送正确信号返回1，否则返回0
uint8_t DHT11_STM32_reply(void);

// STM32对DHT11输入的信号进行解码，先输入54us的低电平，23-27us的高电平解码为0，68-74us的高电平解码为1
uint8_t DHT11_STM32_decode(void);

// STM32接收DHT11的应答信号，将数据其传入到结构体的各变量中，并根据校验和检验传输是否正确
uint8_t DHT11_STM32_data(DHT11_Data_TypeDef* DHT11_DATA);

#endif // !__DHT11_H
