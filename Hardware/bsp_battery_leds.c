#include "bsp_battery_leds.h"

// 内部状态变量
static uint8_t state = 0;     // 0:停止 1:充电中 2:停止闪烁中
static uint8_t cur_power = 0; // 当前电量值 (0-4)
static uint8_t flag = 0;      // 动画循环计数

// 初始化
void Battery_leds_init(void)
{
    Leds_init();
}

// 开始充电
void Battery_leds_start(uint8_t power)
{
    // 限制电量范围在 0-BATTERY_POWER_MAX
    if (power > BATTERY_POWER_MAX)
    {
        power = BATTERY_POWER_MAX;
    }

    state = 1;
    flag = power;
    cur_power = power;
}

// 结束充电
void Battery_leds_stop(void)
{
    state = 2;
}

// 更新电量
void Battery_leds_update(uint8_t power)
{
    // 限制电量范围
    if (power > BATTERY_POWER_MAX)
    {
        power = BATTERY_POWER_MAX;
    }

    state = 1;         // 充电状态
    flag = power;      // 重置动画条件
    cur_power = power; // 更新电量
}

// LED状态循环处理 (需要在主循环中定期调用)
void Battery_leds_loop(void)
{
    if (state == 0) // 停止状态
    {
        Leds_turn_all(0);
    }
    else if (state == 1) // 充电中 - 流动效果
    {
        for (uint8_t i = 1; i < BATTERY_LED_COUNT + 1; i++)
        {
            // flag >= i 表示该灯应点亮
            // flag 递增实现流动效果
            Leds_turn(i, flag >= i);
        }

        flag++;
        if (flag > BATTERY_LED_COUNT)
            flag = cur_power;
    }
    else if (state == 2) // 充电停止 - 闪烁3次提示最终电量
    {
        // 闪烁 3 次
        for (uint8_t i = 0; i < 3; i++)
        {
            // 显示当前电量的灯 (前 cur_power 个灯亮)
            for (size_t j = 0; j < BATTERY_LED_COUNT; j++)
            {
                Leds_turn(j, cur_power >= j);
            }

            delay_1ms(200);   // 延时 200ms
            Leds_turn_all(0); // 全部灯灭
            delay_1ms(200);   // 延时 200ms
        }
        state = 0; // 闪烁完成，回到停止状态
    }
}
