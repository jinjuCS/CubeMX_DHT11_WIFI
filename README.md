# CubeMX_DHT11_WIFI
## 功能介绍
该项目基于STM32开发板，通过CubeMX初始化，基于调用HAL库，实现温湿度采集，并通过WiFi协议传输到PC端的串口上。
## 所需硬件
* 野火STM32F103VET6开发板
* DHT11温湿度传感器
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
