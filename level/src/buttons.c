#include "gpio.h"
#include "buttons.h"
void buttons_init() {
  //button 1
  gpioa->MODER_b.MODER12 = 0;
  gpioa->PUPDR_b.PUPDR12 = 1;
  // button 2
  gpioa->MODER_b.MODER11 = 0;
  gpioa->PUPDR_b.PUPDR11 = 1;

}
