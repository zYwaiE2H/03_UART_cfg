#include "USART0.h"

void USART0_init()
{
  // PA9: TX 复用 AF7
  GPIO_output_af(USART0_TX_RCU, USART0_TX_PORT, USART0_TX_PIN, GPIO_OTYPE_PP, USART0_TX_AF);
  // PA10: RX 复用 AF7
  GPIO_output_af(USART0_RX_RCU, USART0_RX_PORT, USART0_RX_PIN, GPIO_OTYPE_PP, USART0_TX_AF);

  // =========== 串口配置
  // 时钟使能
  rcu_periph_clock_enable(RCU_USART0);
  // 重置串口(可选)
  usart_deinit(USART0);
  // 设置波特率(必须要配置)
  usart_baudrate_set(USART0, USART0_BAUDRATE);
  // 允许串口发送
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
  // 允许接收
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);
  // ================中断相关===================
  nvic_irq_enable(USART0_IRQn, USART0_PRIORITY);
  // 接收缓冲区不为空：(收到数据触发)
  usart_interrupt_enable(USART0, USART_INT_RBNE);
  // 空闲标志位：(发送完成触发)
  usart_interrupt_enable(USART0, USART_INT_IDLE);
  // 使能串口
  usart_enable(USART0);
}

// 发送1个byte数据
void USART0_send_byte(uint8_t byte)
{
  while (!usart_flag_get(USART0, USART_FLAG_TBE))
    ;
  usart_data_transmit(USART0, byte);
}

// 发送多个byte数据
void USART0_send_data(uint8_t *data, uint32_t len)
{
  if (!data)
    return;
  // 指针不为NULL 同时 长度不为0，进入循环
  for (uint32_t i = 0; i < len; i++)
  {
    USART0_send_byte(data[i]);
  }
}

// 发送字符串 (结尾标记\0)
void USART0_send_string(char *data)
{
  if (!data)
    return;
  for (uint32_t i = 0; data[i] != 0; i++)
  {
    USART0_send_byte(data[i]);
  }
}

#if USART0_PRINTF
int fputc(int ch, FILE *f)
{
  USART0_send_byte(ch);
  return ch;
}
#endif

#define RX0_LENTH 128                     // 数组总长度
static uint8_t RX0_Buffer[RX0_LENTH + 1]; // +1, 多留一个字符，放字符串结束符
static uint32_t rx0_cnt = 0;              // 放了几个元素的个数，下标
// 中断处理函数
void USART0_IRQHandler()
{
  // 接收
  if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
  {
    uint8_t recv = usart_data_receive(USART0);
    if (rx0_cnt >= RX0_LENTH)
      rx0_cnt = 0;
    RX0_Buffer[rx0_cnt++] = recv;
  }
  // 空闲
  if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))
  {
    usart_data_receive(USART0);
    RX0_Buffer[rx0_cnt] = '\0';
#if USART0_RECV_CALLBACK
    // 收到串口0数据，回调函数
    USART0_on_recv(RX0_Buffer, rx0_cnt);
#endif

    rx0_cnt = 0;
  }
}
