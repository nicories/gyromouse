#include <stdint.h>
#include "rcc.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"

void i2c_init() {
  // enable i2c-1 clock
  rcc->APB1ENR |= (uint32_t) (1 << 21);
  // configure filter before enable!?

  // Pin 9 & 10 to alternative function
  gpioa->MODER |= (uint32_t)((1 << 19) | (1 << 21));

  // alternate function 4 -> Pin9 SCL, Pin10 SDA
  gpioa->AFRH_b.AFRH9 = (uint32_t) 4;
  gpioa->AFRH_b.AFRH10 = (uint32_t) 4;

  // pullup
  gpioa->PUPDR_b.PUPDR9 = 1;
  gpioa->PUPDR_b.PUPDR10 = 1;

  // high speed
  gpioa->OSPEEDR_b.OSPEEDR9 = 3;
  gpioa->OSPEEDR_b.OSPEEDR10 = 3;

  // open drain
  gpioa->OTYPER_b.OT9 = 1;
  gpioa->OTYPER_b.OT10 = 1;

  // 400khz, from cubemx
  i2c->TIMINGR = (uint32_t) 0x20B;

  // enable i2c
  i2c->CR1_b.PE = 1;

  // auto send stop condition after nbytes
  i2c->CR2_b.AUTOEND = 1;

}

int32_t i2c_write8(uint8_t slave_addr, uint8_t data) {
  // wait for busy
  while (i2c->ISR_b.BUSY);
  i2c->CR2_b.SADD1 = slave_addr;
  i2c->CR2_b.RD_WRN = 0;
  i2c->CR2_b.NBYTES = 1;
  i2c->TXDR = data;
  i2c->CR2_b.START = 1;
  /* while (i2c->ISR_b.BUSY); */
  while (!i2c->ISR_b.STOPF);

  return 1;
}
int32_t i2c_read8(uint8_t slave_addr) {
  while (i2c->ISR_b.BUSY);
  i2c->CR2_b.SADD1 = slave_addr;
  i2c->CR2_b.RD_WRN = 1;
  i2c->CR2_b.NBYTES = 1;
  i2c->CR2_b.START = 1;
  while (!i2c->ISR_b.RXNE);
  uint32_t r = i2c->RXDR_b.RXDATA;

  while (!i2c->ISR_b.STOPF);
  return r;
}
int32_t i2c_read_reg8(uint8_t slave_addr, uint8_t reg_addr) {
  i2c_write8(slave_addr, reg_addr);
  return i2c_read8(slave_addr);
}

int32_t i2c_write_reg8(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {

  while (i2c->ISR_b.BUSY);
  i2c->CR2_b.SADD1 = slave_addr;
  i2c->CR2_b.RD_WRN = 0;
  i2c->CR2_b.NBYTES = 2;
  i2c->CR2_b.START = 1;

  i2c->TXDR = reg_addr;
  while (!i2c->ISR_b.TXE);

  i2c->TXDR = data;

  while (!i2c->ISR_b.STOPF);

  return 1;
}
