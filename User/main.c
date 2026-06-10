#include <stdio.h>
#include "gd32f4xx.h"
#include "systick.h"
#include "gpio_cfg.h"
#include "bsp_keys.h"
#include "USART0.h"

void USART0_on_recv(uint8_t *data, uint32_t len)
{
  printf("RX_Buffer: %s, len %d", data, len);
}

void Key_DownHook(uint8_t idx)
{
  printf("idx: %d\n", idx);
}

int main(void)
{
  // 系统滴答定时器初始化
  systick_config();
  // 串口配置
  USART0_init();
  Keys_init();

  USART0_send_byte('b');
  // USART0_send_byte('b');
  // USART0_send_byte('c');

  // uint8_t cmd[4] = {'x', 'y', '\n', 'z'};
  // USART0_send_data(cmd, 3);

  // USART0_send_string("xy\nwz");

  // printf("abcdefg\n");
  while (1)
  {
    Keys_scan();
    delay_1ms(10);
  }
}
