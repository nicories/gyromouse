#include "buttons.h"
#include "gpio.h"
#include "i2c.h"
#include "led.h"
#include "mpu.h"
#include "rcc.h"
#include "usart.h"
#include <stdbool.h>
#include "sensors.h"
#include "levelmap.h"


static volatile TIM2_Type *const tim2 = (TIM2_Type *)TIM2_BASE;
static volatile NVIC_Type *const nvic = (NVIC_Type *)NVIC_BASE;
typedef struct {
  bool is_idle;
  bool odd;
  uint16_t max;
  uint16_t current;

} idle;
void read_sensors();
idle idle_status = {.is_idle = false, .odd = true, .max = 0x10, .current = 0};
bool connect_to_mpu() {
  usart_puts("Connecting to MPU...\r\n");
  while (i2c_read_reg8(IMU_I2C_SLAVE_ADDR, MPUREG_WHOAMI) !=
         IMU_I2C_SLAVE_ADDR) {
    usart_puts("Could not connect to MPU...\r\n");
  }
  usart_puts("Connected to MPU!!\r\n");
  // wake up
  i2c_write_reg8(IMU_I2C_SLAVE_ADDR, 107, 0x0);
  // set 16g
  i2c_write_reg8(IMU_I2C_SLAVE_ADDR, 28, (1 << 4) | (1 << 3));
}
int _start() {

  usart_init();
  i2c_init();
  led_init();
  // TIMER
  rcc->APB1ENR_b.TIM2EN = 1;
  // prescale -> 1kHz
  tim2->PSC_b.PSC = 8000;
  // 10Hz
  tim2->ARR = 100;
  // enable interrupt generation
  tim2->DIER_b.UIE = 1;
  tim2->CR1_b.ARPE = 1;
  // enable tim2
  tim2->CR1_b.CEN = 1;

  connect_to_mpu();
  Sensors sensors;
  int16_t i_x, last_x = 3, unchanged_x = 0, unchanged_x_max = 500;
  read_sensors(&sensors);
  last_x = sensors.accelerometer.x / 0x249;

  bool idle_state[8] = {true, false, true, false, true, false, true, false};
  bool idle = false;
  for (;;) {

    read_sensors(&sensors);

    i_x = level_map(sensors.accelerometer.x);
    if (last_x == i_x) {
      if (unchanged_x < unchanged_x_max) {
        unchanged_x++;
      }
    } else {
      unchanged_x = 0;
    }
    last_x = i_x;

    if (gpioa->IDR_b.IDR11)
      usart_putc('l');
    else
      usart_putc('L');
    if (gpioa->IDR_b.IDR12)
      usart_putc('r');
    else
      usart_putc('R');
    usart_putx(sensors.accelerometer.x);
    usart_putx(sensors.accelerometer.y);
    usart_putx(sensors.accelerometer.z);
    usart_putc(';');

    if (!idle) {
      if (unchanged_x == unchanged_x_max) {
        // first time idle setup
        idle = true;
        led_line_state(idle_state);
        // enable interrupt
        nvic->ISER_b.SETENA |= 1 << 15;
      } else {
        // animation
        led_line_off_all();
        switch (i_x) {
        case -3:
          led_line_on(0);
          led_line_on(1);
          break;
        case -2:
          led_line_on(1);
          led_line_on(2);
          break;
        case -1:
          led_line_on(2);
          led_line_on(3);
          break;
        case 0:
          led_nucleo_on();
          led_line_on(3);
          led_line_on(4);
          break;
        case 1:
          led_line_on(4);
          led_line_on(5);
          break;
        case 2:
          led_line_on(5);
          led_line_on(6);
          break;
        case 3:
          led_line_on(6);
          led_line_on(7);
          break;
        }
      }
    } else {
      // idle -> only read sensors
      if (unchanged_x == 0) {
        idle = false;
        nvic->ICER_b.CLRENA |= 1 << 15;
      }
    }
  }
  return 0;
}

int _tim2_isr() {
  led_line_toggle_all();
  led_nucleo_toggle();
  tim2->SR_b.UIF = 0;
  return 0;
}
/* int _usart() { */
/*   uint8_t data = usart2->RDR_b.RDR; */
/*   tim2->ARR = 5000; */

/*   return 0; */
/* } */

void read_sensors(Sensors *sensors) {

  int iterations = 1;
  uint16_t x, y, z;
  uint32_t x_sum = 0, y_sum = 0, z_sum = 0;

  // average
  for (int i = 0; i < iterations; i++) {
    x = i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_X_HIGH);
    x <<= 8;
    x |= i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_X_LOW);
    x_sum += x;

    y = i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_Y_HIGH);
    y <<= 8;
    y |= i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_Y_LOW);
    y_sum += y;

    z = i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_Z_HIGH);
    z <<= 8;
    z |= i2c_read_reg8(IMU_I2C_SLAVE_ADDR, ACCEL_Z_LOW);
    z_sum += z;
  }
  sensors->accelerometer.x = (x_sum / iterations);
  sensors->accelerometer.y = y_sum / iterations;
  sensors->accelerometer.z = z_sum / iterations;
}
