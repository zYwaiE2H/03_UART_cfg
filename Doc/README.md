# GD32F407 UART 配置工程

## 项目简介

这是一个基于 GD32F407VE 微控制器的 UART/USART 通信示例工程，演示了中断驱动的串口通信、LED 控制和按键扫描功能。

**目标芯片：** GigaDevice GD32F407VE
**内核架构：** ARM Cortex-M4 with FPU
**核心外设：** USART0 (PA9=TX, PA10=RX, 115200 baud)
**LED 引脚：** PC6 (LED_SW), PD8~PD15 (LED1~LED8)
**按键引脚：** PC0~PC3 (KEY1~KEY4)

---

## 目录结构

```
├── User/                          # 应用层代码
│   ├── main.c                    # 主程序入口
│   ├── systick.c/h               # SysTick 定时器配置与延时函数
│   ├── gd32f4xx_it.c/h           # 中断服务例程（默认处理器）
│   └── gd32f4xx_libopt.h         # 库配置选项
│
├── Firmware/                      # 官方固件库
│   ├── CMSIS/                    # CMSIS 核心标准与设备头文件
│   │   ├── GD/GD32F4xx/Include/  # 设备定义头文件（gd32f4xx.h 等）
│   │   ├── GD/GD32F4xx/Source/   # 启动文件与系统初始化
│   │   └── core_cm4.h            # Cortex-M4 CMSIS 核心定义
│   └── GD32F4xx_standard_peripheral/
│       ├── Include/              # 外设驱动头文件（GPIO、RCU、USART 等）
│       └── Source/               # 外设驱动实现文件
│
├── Project/                       # Keil uVision 工程文件
│   ├── GD32F407.uvprojx          # 主工程文件
│   ├── GD32F407.uvoptx           # 工程选项
│   └── Objects/                  # IDE 编译输出（如使用 IDE 构建）
│
├── build/                         # VSCode/EIDE 构建输出
│   └── Target 1/
│       ├── .obj/                 # 目标文件（.o）和依赖文件（.d）
│       ├── compile_commands.json # 编译数据库，供 clangd 等工具使用
│       ├── GD32F407.elf          # ELF 格式输出（含调试信息）
│       ├── GD32F407.hex          # Intel HEX 文件（用于烧录）
│       └── GD32F407.axf          # 其他格式输出
│
├── Hardware/                      # 板级支持包（BSP）
│   ├── bsp_leds.c/h              # LED 驱动（数组配置方式）
│   ├── bsp_keys.c/h              # 按键扫描驱动（带回调钩子）
│   ├── bsp_battery_leds.c/h      # 电池电量 LED 指示（预留）
│   └── gpio_cfg.h                # GPIO 内联辅助函数
│
├── Library/                       # 外设驱动库
│   └── usart/
│       ├── USART0.c/h            # USART0 驱动（中断+回调）
│
├── Doc/                          # 文档目录
│   ├── readme.txt               # 本文档的旧版本
│   └── README.md                # 本文档
│
├── .eide/                        # CL.EIDE 扩展配置
│   ├── eide.yml                  # 工程配置（工具链、文件、内存布局）
│   └── env.ini                   # 环境配置
│
├── .vscode/                      # VSCode 设置
│   └── settings.json             # C/C++ 扩展配置
│
├── GD32F407.code-workspace       # VSCode 工作区文件
└── .gitignore                    # Git 忽略规则

```

---

## 硬件连接

### USART0 连接
| 引脚 | 功能 | 复用功能 |
|------|------|----------|
| PA9  | USART0_TX | AF7 |
| PA10 | USART0_RX | AF7 |

### LED 连接
本工程使用以下 GPIO 引脚控制 LED：

| 引脚 | 标识 | 说明 |
|------|------|------|
| PC6  | LED_SW | 三极管控制的电源指示灯 |
| PD8  | LED1 | 高电平熄灭（初始状态） |
| PD9  | LED2 | 高电平熄灭 |
| PD10 | LED3 | 高电平熄灭 |
| PD11 | LED4 | 高电平熄灭 |
| PD12 | LED5 | 高电平熄灭 |
| PD13 | LED6 | 高电平熄灭 |
| PD14 | LED7 | 高电平熄灭 |
| PD15 | LED8 | 高电平熄灭 |

### 按键连接
| 引脚 | 标识 | 配置 |
|------|------|------|
| PC0  | KEY1 | 上拉输入 |
| PC1  | KEY2 | 上拉输入 |
| PC2  | KEY3 | 上拉输入 |
| PC3  | KEY4 | 上拉输入 |

按键按下时为低电平（DOWN），释放时为高电平（UP）。

---

## 开发环境

### 支持的开发环境

1. **Keil MDK uVision**（推荐）
   - 打开 `Project/GD32F407.uvprojx`
   - 版本要求：MDK v5.21 或更高，包含 ARM Compiler 6

2. **VSCode + CL.EIDE 扩展**
   - 安装 VSCode 的 CL.EIDE 扩展
   - 打开 `GD32F407.code-workspace`
   - 推荐搭配 Cortex-Debug 扩展进行调试

### 工具链

- **编译器：** ARM Compiler 6 (ARMCLANG)
  - 路径：`d:\Keil\ARM\ARMCLANG\bin\armclang.exe`（默认配置）
  - 版本要求：v6.21 或更高

- **汇编器：** armasm.exe

- **调试/烧录：**
  - **OpenOCD**（默认）：支持 CMSIS-DAP、J-Link 等调试器
  - **J-Link**：Segger J-Link 调试器

---

## 编译与烧录

### 使用 Keil uVision

1. **编译工程**
   - 打开 `Project/GD32F407.uvprojx`
   - 按 `F7` 键或点击 Build → Build Target
   - 输出文件位于 `Project/Objects/` 或 `build/Target 1/`

2. **调试运行**
   - 连接调试器（如 J-Link、DAP-Link）
   - 按 `Ctrl+F5` 或点击 Debug → Start Debug Session
   - 按 `F5` 运行程序

### 使用 VSCode (CL.EIDE)

1. **编译**
   - 在 VSCode 中打开项目
   - 按 `Ctrl+Shift+B` 执行构建任务
   - 输出文件位于 `build/Target 1/`

2. **调试**
   - 按 `F5` 启动调试（需配置 `launch.json`）
   - 默认配置由 CL.EIDE 自动生成

### 使用 OpenOCD 烧录

```bash
# 基本命令
openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg \
  -c "program build/Target 1/GD32F407.elf verify reset exit"

# 或使用 HEX 文件
openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg \
  -c "program build/Target 1/GD32F407.hex verify reset exit"
```

### 使用 JLink 烧录

创建 `jlink.cmd` 文件：
```
executable.elf build/Target 1/GD32F407.elf
load
r
g
q
```
然后执行：
```bash
JLinkExe -device GD32F407VE -if SWD -speed 4000 -CommanderScript jlink.cmd
```

---

## 编译配置

### 预定义宏

工程中预定义了以下宏：

| 宏名 | 说明 |
|------|------|
| `USE_STDPERIPH_DRIVER` | 启用标准外设库 |
| `GD32F407` | 目标芯片型号 |
| `__MICROLIB` | 使用 microLIB 轻量级 C 库 |

### 包含路径

```
User/
Firmware/CMSIS/GD/GD32F4xx/Include
Firmware/GD32F4xx_standard_peripheral/Include
Firmware/CMSIS
Hardware
Library
Library/usart
```

### 编译选项（ARMCLANG）

- **CPU 类型：** `Cortex-M4`
- **浮点单元：** 支持单精度（`fpv4-sp-d16`）
- **浮点 ABI：** `hard`
- **C 语言标准：** `C99`
- **优化级别：** `-O1`
- **字节序：** 小端（`-mlittle-endian`）
- **调试信息：** 启用（-g）

### 内存布局

| 区域 | 起始地址 | 大小 | 说明 |
|------|----------|------|------|
| Flash (ROM) | 0x0800 0000 | 512KB | 程序代码与常量数据 |
| RAM (IRAM) | 0x2000 0000 | 128KB | 数据段、堆栈、堆 |
| IRAM2 | 0x1000 0000 | 64KB | 备用 RAM（可选） |

---

## 核心代码说明

### main.c - 主程序

```c
int main(void)
{
    systick_config();   // 初始化 SysTick 定时器 (1ms tick)
    USART0_init();      // 初始化 USART0 (PA9/PA10, 115200, 中断+空闲)
    Leds_init();        // 初始化所有 LED
    Keys_init();        // 初始化按键（上拉输入）

    while (1)
    {
        Keys_scan();            // 扫描按键状态
        delay_1ms(10);          // 延时 10ms（简单轮询）
    }
}
```

**关键回调函数（需在 main.c 中实现）：**
```c
// 收到串口数据时自动调用（见 Library/usart/USART0.c）
void USART0_on_recv(uint8_t *data, uint32_t len)
{
    printf("RX_Buffer: %s, len %d", data, len);
}

// 按键按下时调用
void Key_DownHook(uint8_t index)
{
    printf("idx: %d\n", index);
    // index: 1=KEY1, 2=KEY2, 3=KEY3, 4=KEY4
}
```

### systick.c - 延时函数

- `systick_config()`: 配置 SysTick 定时器为 1MHz（每 1us 中断一次）
- `delay_1ms(uint32_t count)`: 阻塞延时指定毫秒数
- `delay_1us(uint32_t count)`: 阻塞延时指定微秒数

> **注意：** `delay_*` 函数是阻塞式的，不适合实时系统或需要多任务场景。

### Library/usart/USART0.c - USART 驱动

**特性：**
- 中断驱动接收：数据通过 `USART0_IRQHandler()` 自动存入环形缓冲区
- 空闲行检测：收到完整帧后自动调用 `USART0_on_recv()`
- 阻塞发送：`USART0_send_byte()`, `USART0_send_data()`, `USART0_send_string()`
- printf 重定向：定义 `USART0_PRINTF 1` 后可使用 `printf()`

**配置项（USART0.h）：**
```c
#define USART0_PRINTF 1           // 启用 printf 重定向
#define USART0_RECV_CALLBACK 1    // 启用接收回调
#define USART0_BAUDRATE 115200UL  // 波特率
#define USART0_PRIORITY 2, 2      // NVIC 优先级（抢占，子优先级）
```

### gd32f4xx_it.c - 中断处理

提供所有 Cortex-M4 异常处理程序的默认实现（无限循环）。`SysTick_Handler()` 调用 `delay_decrement()` 支持延时函数。`USART0_IRQHandler()` 实现在 `Library/usart/USART0.c` 中。

---

## 添加新功能

### 添加新源文件到工程

1. 将源文件（`.c`）和头文件（`.h`）放入合适目录（建议 `User/`、`Hardware/` 或 `Library/`）
2. **VSCode/EIDE:** 编辑 `.eide/eide.yml`，在 `virtualFolder.files` 中添加文件路径
3. **Keil uVision:** 右键点击对应组 → Add Existing Files to Group

### 使用新的外设

标准外设驱动已包含在 `Firmware/GD32F4xx_standard_peripheral/` 中。

**示例：使用 USART2**

```c
#include "gd32f4xx_usart.h"

void usart2_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART2);

    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);

    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);

    usart_enable(USART2);
}
```

### 修改时钟配置

时钟初始化位于 `Firmware/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c` 中的 `SystemInit()` 函数。修改 `Firmware/CMSIS/GD/GD32F4xx/Include/system_gd32f4xx.h` 中的晶振频率定义（如 `HSE_VALUE`）可调整系统时钟。

---

## 故障排除

### 编译错误

| 问题 | 可能原因 | 解决方案 |
|------|----------|----------|
| `fatal error: gd32f4xx.h: No such file or directory` | 包含路径未设置 | 检查 `.eide/eide.yml` 或 Keil 工程选项中的 Include Paths |
| `undefined reference to 'Systick_Config'` | 源文件未添加到工程 | 确认 `User/systick.c` 已包含在工程中 |
| 无法找到 `armclang.exe` | 工具链路径错误 | 检查 Keil 安装路径，或修改 EIDE 配置 |
| `USART0_IRQHandler` 重定义 | 重复实现了 USART 驱动 | 确保只有一个 `USART0_IRQHandler` 定义 |

### 运行问题

| 问题 | 可能原因 | 解决方案 |
|------|----------|----------|
| 芯片不响应调试器 | 调试器未连接或接口错误 | 检查 SWD 连接（SWDIO, SWCLK, GND, VCC），确认 OpenOCD/JLink 配置 |
| 串口无输出/乱码 | 波特率不匹配或接线错误 | 确认终端波特率 115200, 8N1, 检查 PA9/PA10 连接 |
| 按键无响应 | 扫描频率过高或上拉配置错误 | 确保 `Keys_scan()` 被调用，检查按键电路（按下应拉低） |
| 程序意外复位 | HardFault 或其他异常 | 查看 `gd32f4xx_it.c` 中的处理器，添加调试语句或检查内存访问 |

### 调试建议

- 使用串口输出调试信息（`printf()` 或 `USART0_send_string()`）
- 在异常处理器中设置断点，查明触发异常的原因
- 检查堆栈使用，避免栈溢出
- 使用示波器检查 USART 信号

---

## 常用操作指南

| 任务 | 操作 |
|------|------|
| 修改 USART0 波特率 | 编辑 `Library/usart/USART0.h` 中的 `USART0_BAUDRATE` |
| 修改 USART0 引脚 | 编辑 `USART0_TX_PORT/PIN/AF` 等宏 |
| 修改按键映射 | 编辑 `Hardware/bsp_keys.c` 的 `keys[]` 数组 |
| 修改 LED 配置 | 编辑 `Hardware/bsp_leds.c` 的 `leds[]` 数组 |
| 调整 USART 中断优先级 | 编辑 `Library/usart/USART0.h` 中的 `USART0_PRIORITY`（格式：抢占,子优先级） |
| 启用/禁用 printf 输出 | 设置 `Library/usart/USART0.h` 的 `USART0_PRINTF 0/1` |
| 调整接收缓冲区大小 | 编辑 `Library/usart/USART0.c` 的 `RX0_LENTH`（默认 128） |

---

## 参考资源

- **GigaDevice 官网：** https://www.gd32mcu.com/
- **GD32F4xx 数据手册与参考手册：** 从官网下载
- **Keil MDK 文档：** https://www.keil.com/support/man/
- **CMSIS 规范：** https://arm-software.github.io/CMSIS/
- **GD32 标准外设库文档：** `Firmware/GD32F4xx_standard_peripheral/Doc/`（如有）

---

## 许可证

本项目使用 GigaDevice 提供的标准外设库，遵循其原始许可证（BSD 风格）。详见各源文件中的版权声明。

---

## 更新日志

- **2024-01-15** - 初始版本，基于 GD32F4xx V3.2.0 固件库
- **2025-06-09** - 更新文档，准确描述 UART 工程结构

---

**最后更新：** 2025年6月9日
**维护者：** 项目所有者
