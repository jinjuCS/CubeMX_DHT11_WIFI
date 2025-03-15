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

// STM32向DHT11发送起始信号
void STM32_DHT11_start(void)
{
    //切换引脚为输出模式
    STM32_DHT11();
    //输出18ms的低电平
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(18);
    //将总线拉高，等待应答信号
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
    delay_us(30);
}

// 检查DHT11是否向STM32发送应答信号，发送正确信号返回1，否则返回0
uint8_t DHT11_STM32_reply(void)
{
    // 先发送起始信号
    STM32_DHT11_start();
    // 切换引脚为输入模式
    DHT11_STM32();
    // 如果是低电平，则检验83us的低电平和87us的高电平
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET)
    {
        uint8_t count = 0;
        // 设置一个低电平的极大值，如果超时证明传输出错
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

        // 设置一个高电平的极大值，如果超时证明传输出错
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

// STM32对DHT11输入的信号进行解码，先输入54us的低电平，23-27us的高电平解码为0，68-74us的高电平解码为1
uint8_t DHT11_STM32_decode(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        // 等待低电平开始（数据位前导）
        while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET);

        // 测量高电平持续时间
        uint32_t duration = 0;
        while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET)
        {
            duration++;
            delay_us(1);
            if (duration > 100) break;  // 超时
        }

        // 判断比特值
        data <<= 1;
        if (duration > 30)  // 高电平超过30us为"1"
        {
            data |= 0x01;
        }
    }
    return data;
}

// STM32接收DHT11的应答信号，将数据其传入到结构体的各变量中，并根据校验和检验传输是否正确
uint8_t DHT11_STM32_data(DHT11_Data_TypeDef* DHT11_DATA)
{
    // 应答信号有效
    if (DHT11_STM32_reply())
    {
        // 接收完整的5字节数据（40位）
        DHT11_DATA->humi_int  = DHT11_STM32_decode();  // 湿度整数
        DHT11_DATA->humi_deci = DHT11_STM32_decode();  // 湿度小数
        DHT11_DATA->temp_int  = DHT11_STM32_decode();  // 温度整数
        DHT11_DATA->temp_deci = DHT11_STM32_decode();  // 温度小数
        DHT11_DATA->check_sum = DHT11_STM32_decode();  // 校验和

        // 根据校验位校验数据
        if (DHT11_DATA->humi_int + DHT11_DATA->humi_deci + DHT11_DATA->temp_int + DHT11_DATA->temp_deci != DHT11_DATA->check_sum)
        {
            return 0;
        }
        // 结束信号，等待54us的低电平后拉高，转换GPIO端口状态
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
