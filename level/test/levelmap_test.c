#include "../include/levelmap.h"
#include <stdio.h>
#include <assert.h>

int main() {
  assert(level_map(0xf800) == -3);
  assert(level_map(0x0000) == 0);
  assert(level_map(0x0800) == 3);
}
