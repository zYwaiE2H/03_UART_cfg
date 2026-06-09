#ifndef __BSP_BATTERY_LEDS_H__
#define __BSP_BATTERY_LEDS_H__

#include "gd32f4xx.h"
#include "systick.h"
#include "bsp_leds.h" // LED灯

// LED 数量定义
#define BATTERY_LED_COUNT 8
// 默认电量阈值 (0-4格电量对应LED显示)
#define BATTERY_POWER_MAX 8

// 初始化
void Battery_leds_init(void);
// 开始充电
void Battery_leds_start(uint8_t power); // power: 0-4 (对应电池电量格数)
// 结束充电
void Battery_leds_stop(void);
// 更新电量
void Battery_leds_update(uint8_t power);
// LED状态循环处理 (需要在主循环中定期调用)
void Battery_leds_loop(void);

#endif