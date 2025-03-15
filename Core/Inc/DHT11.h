#ifndef __DHT11_H
#define __DHT11_H

#include "gpio.h"
#include "tim.h"

/************************** DHT11 �������Ͷ���********************************/
typedef struct
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
} DHT11_Data_TypeDef;

// STM32->DHT11
void STM32_DHT11(void);

// DHT11->STM32
void DHT11_STM32(void);

// STM32��DHT11������ʼ�ź�
void STM32_DHT11_start(void);

// ���DHT11�Ƿ���STM32����Ӧ���źţ�������ȷ�źŷ���1�����򷵻�0
uint8_t DHT11_STM32_reply(void);

// STM32��DHT11������źŽ��н��룬������54us�ĵ͵�ƽ��23-27us�ĸߵ�ƽ����Ϊ0��68-74us�ĸߵ�ƽ����Ϊ1
uint8_t DHT11_STM32_decode(void);

// STM32����DHT11��Ӧ���źţ��������䴫�뵽�ṹ��ĸ������У�������У��ͼ��鴫���Ƿ���ȷ
uint8_t DHT11_STM32_data(DHT11_Data_TypeDef* DHT11_DATA);

#endif // !__DHT11_H
