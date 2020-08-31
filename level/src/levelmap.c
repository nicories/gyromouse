#include <stdint.h>
int16_t base_x = 0x0;
int16_t base_y = 0x0;
int16_t base_z = 0x0;

int8_t i;
int16_t last_x[4];
int16_t last_y[4];
int16_t last_z[4];

// 7 zust.
int8_t level_map(int16_t x) {
  return x / 0x249;
}
