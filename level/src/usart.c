#include "usart.h"
#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "rcc.h"

#define ISR_TXE (1 << 7) /* Transmit data register empty */
#define ISR_TC (1 << 6)  /* Transmission complete */


/* void usart_putx(uint32_t val) {} */

void usart_init() {

  // enable usart2 clock
  rcc->APB1ENR_b.USART2EN = 1;

  usart2->CR1_b.RE = 1;
  usart2->CR1_b.TE = 1;
  usart2->CR1_b.RXNEIE = 1;
  usart2->CR1_b.UE = 1;

  // enable gpioa
  rcc->AHBENR_b.IOPAEN = 1;

  // set Pin 2 and 3 to alternate function
  gpioa->MODER_b.MODER2 = 2;
  gpioa->MODER_b.MODER3 = 2;

  // PIN2,3 -> AF1 (usart)
  gpioa->AFRL_b.AFRL2 = GPIO_ALTERNATE_FUNCTION_1;
  gpioa->AFRL_b.AFRL3 = GPIO_ALTERNATE_FUNCTION_1;

  // Baudrate: frequency / rate
  usart2->BRR = (uint32_t) (CLOCK / USART_BAUD_RATE);
}
void usart_putc(uint8_t c) {
  while (!(usart2->ISR & ISR_TXE))
    ;
  usart2->TDR = c;
  while (!(usart2->ISR & ISR_TC))
    ;
}

void usart_putx4(uint8_t x) {
  if (x >= 10) {
    usart_putc(x + 55);
  } else {
    usart_putc(x + 48);
  }
}
void usart_putx(uint32_t val) {
  // split into 4 Bytes
  uint8_t *c = (uint8_t *)&val;
  uint8_t v1;
  uint8_t v2;
  for (int i = 3; i >= 0; i--) {
    v1 = c[i] >> 4;
    v2 = c[i] & 0b00001111;
    usart_putx4(v1);
    usart_putx4(v2);
  }
}

void usart_puts(const char *str) {
  while (*str != '\0') {
    usart_putc(*str);
    str++;
  }
}
