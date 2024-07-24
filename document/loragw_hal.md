**loragw_hal.c **

**1、load_firmware**

```c
int load_firmware(uint8_t target, uint8_t *firmware, uint16_t size)
```

​			该函数用于将固件加载到目标设备或 MCU（微控制器）中。首先，根据 *target* 的值，选择正确的寄存器地址和配置。

​					*·（1）target为MCU_ARB：将 reg_rst 设置为 LGW_MCU_RST1 ,用于控制MCU的复位操作。将 reg_sel 设置为LGW_MCU_SELECT_MUX_1 ，用于选择MCU 访问模式或多路复用器配置。*

​					*·（2）target为MCU_AGC：将 reg_rst 设置为 LGW_MCU_RST1 ,用于控制MCU的复位操作。将 reg_sel 设置为LGW_MCU_SELECT_MUX_1 ，用于选择MCU 访问模式或多路复用器配置。*

​			然后，配置 MCU 的复位寄存器和多路复用器选择寄存器的寄存器地址。其次，调用 lgw_reg_wb 将 firmware 写入 LGW_MCU_PROM_DATA （存放固件数据的寄存器） ，再调用 lgw_reg_rb 从 LGW_MCU_PROM_DATA 中读取数据并存储至 fw_check 中，比较 firmware 和  fw_check 数据是否相等，如果不相等，说明数据在写入或读取过程中出现了问题。最后，恢复 MCU 对程序内存的控制。

**2、lgw_start**

```c
int lgw_start(void)
```

​			这是一个网关初始化函数，用于设置和启动 LoRa 集中器（concentrator），并确保 LoRa 集中器在运行之前经过正确的初始化和配置，以确保其能够正常工作。首先，通过调用 lgw_connect 函数连接 sx1301 ，再通过调用 lgw_soft_reset 函数重置所有寄存器，并关闭所有无线电模块。接下来配置时钟、无线电模块和GPIO*（如果启用了LBT，也要进行配置和启动）*。然后，选择校准命令并加载校准固件，检查校准固件的版本并启动校准过程。其次，设置频率和偏移，再加载固件。最后，使 MCU 退出重置状态，并启动 GPS 事件捕获。

**3、lgw_receive**

```c
int lgw_receive(uint8_t max_pkt, struct lgw_pkt_rx_s *pkt_data)
```

​			这是一个用于从网关集中器的 FIFO 缓冲区中读取数据包的函数。首先，需要检查 LoRa 集中器是否启动。然后，初始化接收缓冲区，使用循环从接收 FIFO 中读取数据包并进行数据包的有效性检查。其次，从接收缓冲区中读取数据包负载及其元数据（如链路和频率信息），并将负载数据复制到 lgw_pkt_rx_s 结构体中。再根据 FIFO 中的状态信息设置数据包状态并计算原始时间戳和修正后的时间戳。最后，更新接收 FIFO 的状态，表明数据包已经被处理。

**4、lgw_send**

```c
int lgw_send(struct lgw_pkt_tx_s pkt_data)
```

​			这是用来向网关集中器发送数据包的函数。它会处理 LoRa 或 FSK 数据包的发送过程，进行各种配置和检查，确保数据包能够正确发送。首先，需要检查 LoRa 集中器是否启动。然后，根据不同的调制方式（LoRa 或 FSK）设置相关的配置。接下来设置 TX 数据缓冲区，包含必要的元数据和用户数据。最后，执行 LBT（Listen Before Talk）检查，确认频道是否空闲。再根据不同的触发模式（立即、时间戳、GPS）设置触发发送。

**5、lgw_status**

```c
int lgw_status(uint8_t select, uint8_t *code)
```

​			该函数用于监控和管理网关的运行状态，确保数据的发送和接收过程正常运行。它通过传入的 select 进行查看状态：

​						*select = TX_STATUS：这表明我们要检查发射器（TX）的状态，调用 lgw_reg_r 函数读取 LGW_TX_STATUS 寄存器的值。根据这个值来判断 TX 的状态。*

​						*select = RX_STATUS：这表明我们要检查接收器（RX）的状态，接收器状态设置为RX_STATUS_UNKNOWN。*

​			该函数最后都是通过 code 参数返回具体的状态。