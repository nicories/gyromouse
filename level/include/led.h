#ifndef LEVEL_LED_H
#define LEVEL_LED_H

#include <stdint.h>
#include "gpio.h"
#include <stdbool.h>

#define led_line_0 gpiob->ODR_b.ODR0
#define led_line_1 gpiob->ODR_b.ODR1
#define led_line_2 gpiof->ODR_b.ODR1

#define led_line_3 gpioa->ODR_b.ODR8
#define led_line_4 gpiob->ODR_b.ODR5
#define led_line_5 gpiob->ODR_b.ODR4
#define led_line_6 gpioa->ODR_b.ODR1
#define led_line_7 gpioa->ODR_b.ODR0
void led_init();
void led_line_toggle(uint8_t n);
void led_line_toggle_all();
void led_line_on(uint8_t n);
void led_line_off(uint8_t n);
void led_line_state(bool state[8]);
void led_nucleo_on();
void led_nucleo_off();
void led_nucleo_toggle();

void led_line_off_all();
#endif
