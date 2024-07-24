**loragw_reg.c**

**1、reg_w_align32**

```c
int reg_w_align32(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, struct lgw_reg_s r, int32_t reg_value)
```

​			该函数可以处理不同的寄存器写入需求：

​					*（1）直接写入：处理寄存器的大小为 8 位，且没有偏移的情况。这种情况下，可以直接调用 lgw_spi_w 函数执行 SPI 写操作。*

​					*（2）单字节读-修改-写入：处理寄存器的大小为 1 到 8 位，并且寄存器中存在偏移量的情况。先读取当前寄存器的值，创建一个掩码和新的数据，合并旧数据和新数据，然后写回寄存器*。

​					*（3）多字节写入：处理寄存器大小在 1 到 32 位之间的情况，且没有偏移。将 转换为大端格式（从最低有效字节到最高有效字节），存储在 buf 中，调用 lgw_spi_wb 函数以批量写入寄存器的值。*

​			如果寄存器的大小和偏移量不被支持，则返回错误。

**2、reg_r_align32 **

```c
int reg_r_align32(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, struct lgw_reg_s r, int32_t *reg_value)
```

​			该函数负责从寄存器读取数据并处理不同的寄存器配置。

​			  	*（1）处理小于等于8位的寄存器数据：调用 lgw_spi_r 读取一个字节的数据，根据寄存器的偏移量 r.offs 和长度 r.leng 进行数据对齐和掩码操作。如果寄存器是有符号的，则需要进行符号扩展。如果寄存器是无符号的，直接将数据右移对齐后存储。*

​		    	*（2）处理大于8位且不超过32位的寄存器数据：计算寄存器的数据需要多少字节，然后调用 lgw_spi_rb 函数读取寄存器的多个字节数据。将字节数组转换为32位数据，再根据寄存器的符号位进行符号扩展或直接返回数据。*

**3、lgw_reg_w**

```c
int lgw_reg_w(uint16_t register_id, int32_t reg_value)
```

​			该函数主要目的是向指定的寄存器写入一个值。它首先检查 SPI 是否初始化以及连接状态，（如果是特殊寄存器，如 LGW_PAGE_REG ，需要切换寄存器页面和触发软件复位），接下来从 loregs 数组中获取指定寄存器的信息，再检查寄存器是否为只读以及是否需要切换页面。最后，调用 reg_w_align32 函数执行 SPI 写操作。

**4、lgw_reg_r**

```c
int lgw_reg_r(uint16_t register_id, int32_t *reg_value)
```

​			该函数主要用于从指定寄存器读取数据。它首先检查 SPI 是否初始化，然后从寄存器数组 loregs 中获取寄存器的结构体，根据寄存器的需要选择是否切换页面。最后，调用  reg_r_align32 函数从寄存器中读取数据，并存储到 reg_value 中。

**5、lgw_reg_wb**

```c
int lgw_reg_wb(uint16_t register_id, uint8_t *data, uint16_t size)
```

​			该函数负责执行对寄存器的批量写入操作。它首先检查 SPI 是否初始化，接下来从 loregs 数组中获取指定寄存器的信息，再检查寄存器是否为只读以及是否需要切换页面。最后，调用 lgw_spi_wb 函数执行实际的 SPI 写入操作。

**6、lgw_reg_rb **

```c
int lgw_reg_rb(uint16_t register_id, uint8_t *data, uint16_t size)
```

​			该函数负责从指定寄存器读取指定长度的数据并存储至 data 中。它首先检查 SPI 是否初始化，接下来从 loregs 数组中获取指定寄存器的信息，再检查寄存器是否为只读以及是否需要切换页面。最后，调用 lgw_spi_rb 函数执行实际的 SPI 读操作。

**7、lgw_connect**

```c
int lgw_connect(bool spi_only, uint32_t tx_notch_freq)
```

​			该函数用于建立与 SX1301 之间的 SPI 连接。它首先调用 lgw_spi_open 函数打开 SPI 连接，然后如果 spi_only 为 false，则检测是否存在 FPGA。

​					*如果读取 FPGA 版本失败或版本不支持，假设没有 FPGA 或版本不支持。将 SPI 多路复用模式设置为 LGW_SPI_MUX_MODE0 。如果检测到 FPGA ，设置 SPI 多路复用模式为 LGW_SPI_MUX_MODE1，进行 FPGA 软复位，然后配置FPGA。*

​			接下来，读取 SX1301 的版本寄存器，并与预期的版本进行比较。最后，初始化寄存器页面。

**8、page_switch**

```c
int page_switch(uint8_t target)
```

​			该函数用于设置 SPI 设备的寄存器页。它首先使用 PAGE_MASK 过滤掉 target 中不需要的位，只保留寄存器页相关的位，并将其存储至  lgw_regpage 中。然后，调用 lgw_spi_w 函数向指定的寄存器地址（PAGE_ADDR）中写入 lgw_regpage。

**9、lgw_soft_reset**

```c
int lgw_soft_reset(void)
```

​			该函数用于执行对 LoRa 集中器的软重置操作。它通过调用 lgw_spi_w 函数向指定的寄存器写入值，以触发软重置。