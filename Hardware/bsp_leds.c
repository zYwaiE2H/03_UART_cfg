#include "bsp_leds.h"

// LED设备数组定义 - 使用结构体配置
GPIO_LED_t leds[] = {
    {RCU_GPIOC, GPIOC, GPIO_PIN_6, GPIO_OTYPE_PP, 0},  // LED_SW: 三极管控制
    {RCU_GPIOD, GPIOD, GPIO_PIN_8, GPIO_OTYPE_PP, 1},  // LED1: 初始高电平熄灭
    {RCU_GPIOD, GPIOD, GPIO_PIN_9, GPIO_OTYPE_PP, 1},  // LED2
    {RCU_GPIOD, GPIOD, GPIO_PIN_10, GPIO_OTYPE_PP, 1}, // LED3
    {RCU_GPIOD, GPIOD, GPIO_PIN_11, GPIO_OTYPE_PP, 1}, // LED4
    {RCU_GPIOD, GPIOD, GPIO_PIN_12, GPIO_OTYPE_PP, 1}, // LED5
    {RCU_GPIOD, GPIOD, GPIO_PIN_13, GPIO_OTYPE_PP, 1}, // LED6
    {RCU_GPIOD, GPIOD, GPIO_PIN_14, GPIO_OTYPE_PP, 1}, // LED7
    {RCU_GPIOD, GPIOD, GPIO_PIN_15, GPIO_OTYPE_PP, 1}  // LED8
};

#define LEDS_COUNT (sizeof(leds) / sizeof(leds[0]))

// 初始化
void Leds_init()
{
  for (uint8_t i = 0; i < LEDS_COUNT; i++)
  {
    GPIO_output(leds[i].rcu, leds[i].port, leds[i].pin,
                leds[i].out_type, leds[i].init_status);
  }
}
// 点亮某个灯 - 写入结构体中定义的点亮电平
void Leds_turn_on(uint8_t index)
{
  if (index >= LEDS_COUNT || index == 0)
    return;
  gpio_bit_reset((uint32_t)leds[index].port, leds[index].pin);
}

// 熄灭某个灯 - 写入与点亮电平相反的值
void Leds_turn_off(uint8_t index)
{
  if (index >= LEDS_COUNT || index == 0)
    return;
  gpio_bit_set((uint32_t)leds[index].port, leds[index].pin);
}
// 点亮或熄灭某个灯，flag为真，灯亮；flag为假，灯灭
void Leds_turn(uint8_t index, uint8_t flag)
{
  flag ? Leds_turn_on(index) : Leds_turn_off(index);
}
// 点亮或熄灭所有灯  flag为1，灯亮；flag为0，灯灭
void Leds_turn_all(uint8_t flag)
{
  for (uint8_t i = 1; i < LEDS_COUNT; i++)
  {
    Leds_turn(i, flag);
  }
}