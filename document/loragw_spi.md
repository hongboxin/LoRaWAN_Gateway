**loragw_spi.native.c**

**1、lgw_spi_open**

```c
int lgw_spi_open(void **spi_target_ptr)
```

​			该函数用于打开指定的 SPI 设备，并配置相关信息。它首先分配内存来存储  SPI  设备描述符的指针，再调用 open 函数打开 SPI 设备文件描述符。接下来调用 ioctl 函数对 SPI 进行配置：

​						*设置 SPI 模式（Mode0）、设置 SPI 时钟速度、设置 SPI 时序、设置每字节位数*

​			最后，返回 SPI 设备描述符。

**2、lgw_spi_w**

```c
int lgw_spi_w(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t data)
```

​			该函数用于将一个字节的数据写入指定的 SPI 设备寄存器。它首先需要获取 SPI 设备文件描述符，然后准备要发送的帧，最后调用 ioctl 函数发送 SPI 消息（要设置发送数据缓冲区）。

**3、lgw_spi_r**

```c
int lgw_spi_r(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data)
```

​			该函数用于从 SPI 设备中读取一个字节的数据。它首先需要获取 SPI 设备文件描述符，然后准备要发送的帧，最后调用 ioctl 函数发送 SPI 消息（要设置接收数据缓冲区）。

**4、lgw_spi_wb**

```c
int lgw_spi_wb(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
```

​			该函数用于将数据分块写入 SPI 设备。它首先需要获取 SPI 设备文件描述符，然后准备要发送的帧，最后调用 ioctl 函数发送 SPI 消息（要设置发送数据缓冲区）。最后，通过累加传输的字节数来检查是否传输了所有数据字节。

**5、lgw_spi_rb**

```c
int lgw_spi_rb(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
```

​			该函数用于在 SPI 接口上执行突发读取操作。它首先需要获取 SPI 设备文件描述符，然后根据 spi_mux_mode 准备命令字节，最后设置传输，并循环处理数据块。
