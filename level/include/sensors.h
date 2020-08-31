#include <stdint.h>
typedef struct _Sensors_Accelerometer {
  int32_t x;
  int32_t y;
  int32_t z;
} Sensors_Accelerometer;

typedef struct _Sensors_Gyroscope {
  int32_t x;
  int32_t y;
  int32_t z;
} Sensors_Gyroscope;

typedef struct _Sensors {
  Sensors_Accelerometer accelerometer;
  Sensors_Gyroscope gyroscope;
} Sensors;
