#include "led.h"
#include <stdbool.h>
#include "gpio.h"
#include <stdint.h>
#include "rcc.h"
void led_init() {
  rcc->AHBENR_b.IOPBEN = 1;
  rcc->AHBENR_b.IOPFEN = 1;

  // led line
  gpiob->MODER_b.MODER0 = 1;
  gpiob->MODER_b.MODER1 = 1;
  gpiof->MODER_b.MODER1 = 1;
  gpioa->MODER_b.MODER8 = 1;
  gpiob->MODER_b.MODER5 = 1;
  gpiob->MODER_b.MODER4 = 1;
  gpioa->MODER_b.MODER1 = 1;
  gpioa->MODER_b.MODER0 = 1;

  // led LD3
  gpiob->MODER_b.MODER3 = 1;
}
void led_nucleo_on() {
  gpiob->ODR_b.ODR3 = 1;
}
void led_nucleo_off() {
  gpiob->ODR_b.ODR3 = 0;
}
void led_nucleo_toggle() {
  gpiob->ODR_b.ODR3 ^= 1;
}
void led_line_state(bool state[8]) {
  for (int i = 0; i < 8; i++) {
    if (state[i])
      led_line_on(i);
    else
      led_line_off(i);
  }
}
void led_line_toggle_all() {
  for (int i = 0; i < 8; i++)
    led_line_toggle(i);
}
void led_line_toggle(uint8_t n) {
  switch (n) {
  case 0:
    led_line_0 ^= 1;
    break;
  case 1:
    led_line_1 ^= 1;
    break;
  case 2:
    led_line_2 ^= 1;
    break;
  case 3:
    led_line_3 ^= 1;
    break;
  case 4:
    led_line_4 ^= 1;
    break;
  case 5:
    led_line_5 ^= 1;
    break;
  case 6:
    led_line_6 ^= 1;
    break;
  case 7:
    led_line_7 ^= 1;
    break;
  default:
    // nope
    break;
  }
}
void led_line_off_all() {
  led_line_0 = 0;
  led_line_1 = 0;
  led_line_2 = 0;
  led_line_3 = 0;
  led_line_4 = 0;
  led_line_5 = 0;
  led_line_6 = 0;
  led_line_7 = 0;
}
void led_line_on(uint8_t n) {

  switch (n) {
  case 0:
    led_line_0 = 1;
    break;
  case 1:
    led_line_1 = 1;
    break;
  case 2:
    led_line_2 = 1;
    break;
  case 3:
    led_line_3 = 1;
    break;
  case 4:
    led_line_4 = 1;
    break;
  case 5:
    led_line_5 = 1;
    break;
  case 6:
    led_line_6 = 1;
    break;
  case 7:
    led_line_7 = 1;
    break;
  default:
    // nope
    break;
  }
}
void led_line_off(uint8_t n) {

  switch (n) {
  case 0:
    led_line_0 = 0;
    break;
  case 1:
    led_line_1 = 0;
    break;
  case 2:
    led_line_2 = 0;
    break;
  case 3:
    led_line_3 = 0;
    break;
  case 4:
    led_line_4 = 0;
    break;
  case 5:
    led_line_5 = 0;
    break;
  case 6:
    led_line_6 = 0;
    break;
  case 7:
    led_line_7 = 0;
    break;
  default:
    // nope
    break;
  }
}
