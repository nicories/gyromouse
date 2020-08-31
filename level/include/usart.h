#ifndef LEVEL_USART_H
#define LEVEL_USART_H
#include <stdint.h>
#include "STM32F042x.h"
#define USART_BAUD_RATE 115200
#define CLOCK 8000000

void usart_init();
void usart_putc(uint8_t c);
void usart_puts(const char *str);
void usart_putx(uint32_t val);

static volatile USART1_Type *const usart2 = (USART1_Type *) USART2_BASE;
#endif
