#ifndef __BSP_KEYS_H__
#define __BSP_KEYS_H__

#include "gpio_cfg.h"

#define UP SET
#define DOWN RESET

void Keys_init(); // 初始化
// 按键扫描，回调函数会接收到按键编号（1~4）
void Keys_scan();

void Key_DownHook(uint8_t index);
void Key_UpHook(uint8_t index);

#endif