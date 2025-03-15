#include "DHT11.h"

// STM32->DHT11
void STM32_DHT11(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PE6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

// DHT11->STM32
void DHT11_STM32(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PE6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

// STM32��DHT11������ʼ�ź�
void STM32_DHT11_start(void)
{
    //�л�����Ϊ���ģʽ
    STM32_DHT11();
    //���18ms�ĵ͵�ƽ
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(18);
    //���������ߣ��ȴ�Ӧ���ź�
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
    delay_us(30);
}

// ���DHT11�Ƿ���STM32����Ӧ���źţ�������ȷ�źŷ���1�����򷵻�0
uint8_t DHT11_STM32_reply(void)
{
    // �ȷ�����ʼ�ź�
    STM32_DHT11_start();
    // �л�����Ϊ����ģʽ
    DHT11_STM32();
    // ����ǵ͵�ƽ�������83us�ĵ͵�ƽ��87us�ĸߵ�ƽ
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET)
    {
        uint8_t count = 0;
        // ����һ���͵�ƽ�ļ���ֵ�������ʱ֤���������
        while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET && count < 100)
        {
            count++;
            delay_us(1);
        }
        if (count >= 100)
        {
            return 0;
        }
        else
        {
            count = 0;
        }

        // ����һ���ߵ�ƽ�ļ���ֵ�������ʱ֤���������
        while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET && count < 100)
        {
            count++;
            delay_us(1);
        }
        if (count >= 100)
        {
            return 0;
        }

    }
    else
    {
        return 0;
    }
    return 1;
}

// STM32��DHT11������źŽ��н��룬������54us�ĵ͵�ƽ��23-27us�ĸߵ�ƽ����Ϊ0��68-74us�ĸߵ�ƽ����Ϊ1
uint8_t DHT11_STM32_decode(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        // �ȴ��͵�ƽ��ʼ������λǰ����
        while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET);

        // �����ߵ�ƽ����ʱ��
        uint32_t duration = 0;
        while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET)
        {
            duration++;
            delay_us(1);
            if (duration > 100) break;  // ��ʱ
        }

        // �жϱ���ֵ
        data <<= 1;
        if (duration > 30)  // �ߵ�ƽ����30usΪ"1"
        {
            data |= 0x01;
        }
    }
    return data;
}

// STM32����DHT11��Ӧ���źţ��������䴫�뵽�ṹ��ĸ������У�������У��ͼ��鴫���Ƿ���ȷ
uint8_t DHT11_STM32_data(DHT11_Data_TypeDef* DHT11_DATA)
{
    // Ӧ���ź���Ч
    if (DHT11_STM32_reply())
    {
        // ����������5�ֽ����ݣ�40λ��
        DHT11_DATA->humi_int  = DHT11_STM32_decode();  // ʪ������
        DHT11_DATA->humi_deci = DHT11_STM32_decode();  // ʪ��С��
        DHT11_DATA->temp_int  = DHT11_STM32_decode();  // �¶�����
        DHT11_DATA->temp_deci = DHT11_STM32_decode();  // �¶�С��
        DHT11_DATA->check_sum = DHT11_STM32_decode();  // У���

        // ����У��λУ������
        if (DHT11_DATA->humi_int + DHT11_DATA->humi_deci + DHT11_DATA->temp_int + DHT11_DATA->temp_deci != DHT11_DATA->check_sum)
        {
            return 0;
        }
        // �����źţ��ȴ�54us�ĵ͵�ƽ�����ߣ�ת��GPIO�˿�״̬
        while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET){}
        STM32_DHT11();
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
    }
    else
    {
        return 0;
    }
    return 1;
}
