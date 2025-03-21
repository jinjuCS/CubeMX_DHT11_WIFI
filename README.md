# CubeMX_DHT11_WIFI
## 功能介绍
该项目基于STM32开发板，通过CubeMX初始化，采用调用HAL库的方式，实现温湿度采集，并通过WiFi协议传输到PC端。
## 所需硬件
* 野火STM32F103VET6开发板，作为MCU主控芯片，同时还要利用片上外设ESP8266
* DHT11温湿度传感器，采集环境中的温湿度信息
* 杜邦线，用于连接DHT11传感器与开发板，以及ESP8266的使能
## 所需软件
* STM32CubeMX，实现基于HAL库的初始化
* Keil uVision5，提供编译和烧录环境
* VScode，安装EIDE插件，提供便捷的代码编写环境
## 主要目录结构
<pre>
工程目录  
├── Core  
│   ├── Inc  
│   │   ├── dht11.h      # DHT11驱动  
│   │   ├── wifi.h       # WiFi通信接口  
│   │   └── esp8266.h    # ESP8266底层AT指令  
│   ├── Src  
│   │   ├── main.c       # 整合后的主逻辑  
│   │   ├── dht11.c      # DHT11数据采集  
│   │   ├── wifi.c       # WiFi功能实现  
│   │   ├── esp8266.c    # ESP8266通信  
│   │   ├── usart.c      # 串口配置  
│   │   └── tim.c        # 定时器延时  
└── ...  
</pre>
## 实现逻辑
### STM32与DHT11传感器
* 查阅DHT11传感器的数据手册，主要查阅其接发数据的信号形式、发送数据的格式、数据的起始信号和终止信号
* HAL库中没有直接定义好的微秒级延迟函数，而DHT11传输数据的过程中有许多微秒级的控制。所以需要自己开启一个片上定时器，通过设置分频信号，比较计数寄存器的值来实现微秒级的延迟函数
* 当STM32以规定的形式向DHT11传递起始信号后，等待DHT11传递应答信号
* 两边确认完毕后，DHT11通过特定的电平转换格式向STM32发送40位的信号，STM32经过解码，还原回每个信号代表的高低电平
### STM32与ESP8266
* STM32开发板本身没有WiFi模块，所以需要借用板子上的ESP8266模块与外界进行Wifi通讯
* ESP8266需要先进行初始化配置，烧录AT固件，使用AT指令搭建底层无线局域网络（STA模式或者AP模式），并把ESP8266配置TCP服务器
* STM32将采集到的温湿度数据通过串口发送给ESP8266，ESP8266通过解析数据中的信息，将最终数据传递给与其在同一个无线局域网络内的TCP客户端PC上
