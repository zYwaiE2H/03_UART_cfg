#ifndef __BSP_LEDS_H__
#define __BSP_LEDS_H__

#include "gpio_cfg.h"

// LED结构体定义
typedef struct
{
    rcu_periph_enum rcu; // rcu类型
    uint32_t port;       // GPIO端口
    uint16_t pin;        // GPIO引脚
    uint8_t out_type;    // 输出模式
    uint8_t init_status; // 初始状态
} GPIO_LED_t;

// LED设备数组声明 (外部可见)
extern GPIO_LED_t leds[];

// 初始化
void Leds_init();
// 点亮某个灯
void Leds_turn_on(uint8_t index);
// 熄灭某个灯
void Leds_turn_off(uint8_t index);
// 点亮或熄灭某个灯，flag为真，灯亮；flag为假，灯灭
void Leds_turn(uint8_t index, uint8_t flag);
// 点亮或熄灭所有灯  flag为1，灯亮；flag为0，灯灭
void Leds_turn_all(uint8_t flag);

#endif