#ifndef LEVEL_GPIO_H
#define LEVEL_GPIO_H

/*
  MODER
  | 00 | Input (reset)          |
  | 01 | General purpose output |
  | 10 | Alternate Function     |
  | 11 | Analog                 |

  OTYPER
  | 0 | push-pull  |
  | 1 | open-drain |

  OSPEEDR
  | x0 | low speed    |
  | 01 | medium speed |
  | 11 | high speed   |
  AFR{L,h}
  |   0 | AF0 |
  | ... | ... |
  |   7 | AF7 |
  ODR (output data)
  | 0 | 0 |
  | 1 | 1 |
 */

#define GPIO_ALTERNATE_FUNCTION_1 1
#define GPIO_ALTERNATE_FUNCTION_2 2
#define GPIO_ALTERNATE_FUNCTION_3 3
#define GPIO_ALTERNATE_FUNCTION_4 4

#include "STM32F042x.h"

static volatile GPIOA_Type *const gpioa = GPIOA;
static volatile GPIOF_Type *const gpiob = GPIOB;
static volatile GPIOF_Type *const gpiof = GPIOF;

#endif
