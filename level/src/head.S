.global _start
Vectors:
.word 0x20001800
/* initial value of the sp */
.word _start + 1
.org 0x7C // TIM2 Interrupt
.word _tim2_isr + 1 // LSB auf 1
.org 0xB0 // USART2 Interrupt
.word _usart + 1 // LSB auf 1
