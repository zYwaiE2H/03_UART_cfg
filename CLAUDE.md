# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an embedded firmware project for the **GigaDevice GD32F407VE** microcontroller (ARM Cortex-M4). The project implements UART/USART communication with interrupt-driven reception, along with LED control and key scanning functionality.

**Core Purpose:** This is a UART configuration and testing project demonstrating:
- Interrupt-driven serial communication with callback pattern
- Board Support Package (BSP) abstraction for LEDs and keys
- GPIO configuration helpers
- SysTick-based timing and delays

**Target MCU:** GD32F407VE (512KB Flash, 128KB RAM)

## Architecture

### Layered Structure

```
┌─────────────────────────────────────────────┐
│         Application Layer (User/)           │
│    main.c, callbacks, business logic       │
├─────────────────────────────────────────────┤
│        Peripheral Driver Layer (Library/)  │
│    Hardware abstraction: USART0 driver     │
├─────────────────────────────────────────────┤
│        BSP Layer (Hardware/)               │
│    LED/Key board support with config arrays│
├─────────────────────────────────────────────┤
│       HAL Layer (Hardware/gpio_cfg.h)      │
│    Inline GPIO helpers (output, input, AF) │
├─────────────────────────────────────────────┤
│   StdPeriph Lib (Firmware/GD32F4xx_*)     │
│    Official GD32 peripheral drivers        │
└─────────────────────────────────────────────┘
```

### Key Design Patterns

1. **Callback Pattern for USART**: Define `USART0_on_recv(uint8_t *data, uint32_t len)` in `main.c` to receive data. The ISR collects data into a ring buffer and calls this hook on idle line.

2. **Weak Hook Functions**: Key scanning uses `__attribute__((weak))` for `Key_DownHook` and `Key_UpHook`. Override these in `main.c` to handle key events without modifying BSP code.

3. **Configuration via Defines**: `USART0.h` uses compile-time switches:
   - `USART0_PRINTF` (1=enables `fputc` for `printf`)
   - `USART0_RECV_CALLBACK` (1=enables callback invocation)

4. **Array-Based BSP**: LEDs and keys use configuration arrays (`GPIO_LED_t leds[]`, `KeyConfig keys[]`) making it easy to modify or expand.

### Important File Locations

| Component | Path |
|-----------|------|
| Main application | [User/main.c](User/main.c) |
| USART driver | [Library/usart/USART0.c](Library/usart/USART0.c) |
| USART config | [Library/usart/USART0.h](Library/usart/USART0.h) |
| BSP LEDs | [Hardware/bsp_leds.c](Hardware/bsp_leds.c) |
| BSP Keys | [Hardware/bsp_keys.c](Hardware/bsp_keys.c) |
| GPIO helpers | [Hardware/gpio_cfg.h](Hardware/gpio_cfg.h) |
| SysTick | [User/systick.c](User/systick.c) |
| ISR handlers | [User/gd32f4xx_it.c](User/gd32f4xx_it.c) |
| Build config | [.eide/eide.yml](.eide/eide.yml) |

## Development Commands

### Build

**VSCode + CL.EIDE:**
- Build: `Ctrl+Shift+B` (Build Task)
- Output: `build/Target 1/` (`.hex`, `.elf`, `.axf`)

**Keil uVision:**
- Build: `F7` or Build → Build Target
- Output: `Project/Objects/` or `build/Target 1/`

### Flash & Debug

**OpenOCD (default):**
```bash
openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg \
  -c "program build/Target 1/GD32F407.elf verify reset exit"
```

**J-Link:**
```bash
JLinkExe -device GD32F407VE -if SWD -speed 4000 -CommanderScript jlink.cmd
```

**VSCode Debug:**
- Press `F5` (requires `launch.json` with Cortex-Debug configuration)
- Default config uses OpenOCD

### Single Test/Example Modification

The current main loop only calls `Keys_scan()` with a delay. To test different features:

1. **Test USART send**: Uncomment send lines in `main.c`:
   ```c
   USART0_send_byte('b');
   USART0_send_data(cmd, 3);
   printf(" Test string\n");
   ```

2. **Test LEDs**: Call `Leds_turn_on(1)`, `Leds_turn_all(1)` etc. in the main loop.

3. **Add custom logic**: Implement `USART0_on_recv()` and `Key_DownHook()` in `main.c`.

## Toolchain Configuration

- **Compiler:** ARM Compiler 6 (ARMCLANG), C99, `-O1`
- **CPU:** Cortex-M4 with FPU (single precision, hard ABI)
- **Defines:** `USE_STDPERIPH_DRIVER`, `GD32F407`, `__MICROLIB`
- **Memory:** Flash `0x08000000` (512KB), RAM `0x20000000` (128KB)
- **Include paths:** `User/`, `Firmware/CMSIS/GD/GD32F4xx/Include/`, `Firmware/GD32F4xx_standard_peripheral/Include/`, `Hardware/`, `Library/`

## Adding New Features

### Adding a New Module
1. Create `.c` and `.h` files in appropriate directory:
   - `Library/` for new peripheral drivers
   - `Hardware/` for board-specific code (sensors, external ICs)
   - `User/` for application logic
2. Add source files to [.eide/eide.yml](.eide/eide.yml) under `virtualFolder.files`
3. For Keil: Right-click group → Add Existing Files to Group

### Adding a New USART
1. Create a driver following `Library/usart/USART0.c` pattern
2. Define pins, AF number, baud rate, IRQ priority in the header
3. Add to `eide.yml` and enable interrupts in NVIC

### Modifying Clock Configuration
Edit [Firmware/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c](Firmware/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c) `SystemInit()` and `Firmware/CMSIS/GD/GD32F4xx/Include/system_gd32f4xx.h` (HSE_VALUE, etc.)

## Common Tasks

| Task | How-to |
|------|--------|
| Change USART baud rate | Edit `USART0_BAUDRATE` in [Library/usart/USART0.h](Library/usart/USART0.h) |
| Change USART pins | Edit `USART0_TX_*` and `USART0_RX_*` macros in [Library/usart/USART0.h](Library/usart/USART0.h) |
| Change LED/Key pins | Edit arrays in [Hardware/bsp_leds.c](Hardware/bsp_leds.c) or [Hardware/bsp_keys.c](Hardware/bsp_keys.c) |
| Adjust interrupt priority | Edit `USART0_PRIORITY` macro (format: `preempt_priority, subpriority`) |
| Enable/disable printf over USART | Set `USART0_PRINTF 0` or `1` in [Library/usart/USART0.h](Library/usart/USART0.h) |
| Change SysTick frequency | Modify `systick_config()` in [User/systick.c](User/systick.c) |

## Caveats

- `delay_1ms()` and `delay_1us()` are blocking; unsuitable for real-time requirements
- USART RX buffer size is fixed at 128 bytes (`RX0_LENTH` in [Library/usart/USART0.c](Library/usart/USART0.c))
- The `gpio_cfg.h` helper functions are `static inline`, requiring inclusion in each translation unit that uses them

## External Resources

- **GD32 documentation:** https://www.gd32mcu.com/
- **GD32F4xx Reference Manual:** Available from GigaDevice
- **CMSIS specification:** https://arm-software.github.io/CMSIS/
- **Keil MDK:** https://www.keil.com/support/man/

## Notes for Future Development

- The project follows a "library-less" approach for user code (no HAL abstraction beyond StdPeriph)
- Keep BSP code in `Hardware/` and drivers in `Library/` for separation of concerns
- The standard peripheral library is included but not all drivers are used (see `eide.yml` for active files)
