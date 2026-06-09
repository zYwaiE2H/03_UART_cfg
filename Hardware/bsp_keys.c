#include "bsp_keys.h"

typedef struct
{
  uint32_t port;
  uint32_t pin;
  uint8_t index;
} KeyConfig;

static KeyConfig keys[] = {
    {GPIOC, GPIO_PIN_0, 1},
    {GPIOC, GPIO_PIN_1, 2},
    {GPIOC, GPIO_PIN_2, 3},
    {GPIOC, GPIO_PIN_3, 4},
};

uint8_t last_states[] = {UP, UP, UP, UP};

// 初始化
void Keys_init()
{
  GPIO_input(RCU_GPIOC, GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PUPD_PULLUP);
}
__attribute__((weak)) void Key_DownHook(uint8_t index)
{
}
__attribute__((weak)) void Key_UpHook(uint8_t index) {}

// index 取值范围 1~4 代表 key1~key4
// 按键扫描
void Keys_scan()
{
  for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
  {
    FlagStatus cur_state = gpio_input_bit_get(keys[i].port, keys[i].pin);
    if (last_states[i] == cur_state)
      continue;
    if (cur_state == DOWN)
    {
      last_states[i] = DOWN;
      Key_DownHook(keys[i].index);
    }
    else if (cur_state == UP)
    {
      last_states[i] = UP;
      Key_UpHook(keys[i].index);
    }
  }
}
