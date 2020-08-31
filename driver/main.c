#include <errno.h> /* ERROR Number Definitions           */
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "../level/include/sensors.h"
#define USART_BAUD_RATE B115200

void emit(int fd, int type, int code, int val);
int tty_setup();
int uinput_setup();

void cleanup(int uinput, int tty);
uint8_t hex_char_to_int(char c) {
  uint8_t r;
    if (c > '9')
      r = c - 55; 
    else
      r = c - 48;
    return r;
}
int main(void) {

  int uinput = uinput_setup();
  int tty = tty_setup();

  if (tty == -1) {
    printf("Could not connect to any tty.\n");
    cleanup(uinput, tty);
    return 1;
  }

  printf("\nStarting Main Loop..\n");
  for (;;) {

    uint8_t read_buffer[300]; /* Buffer to store the data received */
    int bytes_read = read(tty, &read_buffer, 300); /* Read the data */

    int i;
    for (i = 3 * 8 + 1; i <= 30 * 8; i++) {
      if (read_buffer[i] == ';') {
        break;
      }
    }
    for (int j = 27; j >=0; j--) {
      printf("%c", read_buffer[i-j]);
    }
    printf("\n");
    uint8_t x0 = hex_char_to_int(read_buffer[i - 20]);
    uint8_t x1 = hex_char_to_int(read_buffer[i - 19]);
    uint8_t x2 = hex_char_to_int(read_buffer[i - 18]);
    uint8_t x3 = hex_char_to_int(read_buffer[i - 17]);
    int16_t x =  x0 * 16 * 16 * 16 + x1 * 16 * 16 + x2 * 16 + x3;
    uint8_t y0 = hex_char_to_int(read_buffer[i - 12]);
    uint8_t y1 = hex_char_to_int(read_buffer[i - 11]);
    uint8_t y2 = hex_char_to_int(read_buffer[i - 10]);
    uint8_t y3 = hex_char_to_int(read_buffer[i - 9]);
    int16_t y =  y0 * 16 * 16 * 16 + y1 * 16 * 16 + y2 * 16 + y3;

    int16_t x_rel = y / 20;
    int16_t y_rel = x / -20;
    if (abs(x_rel) < 5)
      x_rel = 0;
    if (abs(y_rel) < 5)
      y_rel = 0;

    emit(uinput, EV_REL, REL_X, x_rel);
    emit(uinput, EV_REL, REL_Y, y_rel);
    emit(uinput, EV_SYN, SYN_REPORT, 0);

    usleep(15000);
    fflush(stdout);
  }

  cleanup(uinput, tty);
  return 0;
}

void emit(int fd, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

int tty_setup() {
  printf("\nTTY SETUP:\n");
  char *valid_ttys[] = {"/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2",
                        "/dev/ttyACM3"};
  int tty = -1;
  int i = 0;
  while (tty == -1 && i < sizeof(valid_ttys) / sizeof(valid_ttys[0])) {
    printf("Trying %s...\n", valid_ttys[i]);
    tty = open(valid_ttys[i],
               O_RDWR|
    O_NOCTTY); /* No terminal will control the process   */
    i++;
  }

  if (tty == -1) /* Error Checking */ {
    return tty;
  } else {
    printf("Connected to %s.\n", valid_ttys[i - 1]);
  }

  /*---------- Setting the Attributes of the serial port using termios structure
   * --------- */

  printf("Setting attributes..\n");
  struct termios SerialPortSettings; /* Create the structure */

  /* Setting the Baud rate */
  cfsetispeed(&SerialPortSettings, B115200);
  cfsetospeed(&SerialPortSettings, B115200);
  /* SerialPortSettings.c_cflag &= ~PARENB; /\* no parity *\/ */
  /* SerialPortSettings.c_cflag &= ~CSTOPB; /\* 1 stop bit *\/ */
  /* SerialPortSettings.c_cflag &= ~CSIZE; */
  /* SerialPortSettings.c_cflag |= CS8 | CLOCAL; /\* 8 bits *\/ */
  /* SerialPortSettings.c_lflag = ICANON;        /\* canonical mode *\/ */
  /* SerialPortSettings.c_oflag &= ~OPOST;       /\* raw output *\/ */

  /* 8N1 Mode */
  SerialPortSettings.c_cflag &=
      ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
  SerialPortSettings.c_cflag &=
      ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
  SerialPortSettings.c_cflag &=
      ~CSIZE; /* Clears the mask for setting the data size             */
  SerialPortSettings.c_cflag |= CS8; /* Set the data bits = 8 */

  SerialPortSettings.c_cflag &= ~CRTSCTS; /* No Hardware flow Control */
  SerialPortSettings.c_cflag |=
      CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines */

  SerialPortSettings.c_iflag &=
      ~(IXON | IXOFF |
        IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
  SerialPortSettings.c_iflag &=
      ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode */

  SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

  /* Setting Time outs */
  SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
  SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */
  /* ioctl(tty, TCSETS, &SerialPortSettings); */
  /* ioctl(tty, TCGETS, &SerialPortSettings); */
  tcsetattr(tty, TCSANOW, &SerialPortSettings);

  tcflush(tty, TCIFLUSH); /* Discards old data in the rx buffer            */

  printf("tty is set up.\n");
  return tty;
}

int uinput_setup() {
  printf("\nUINPUT SETUP:\n");
  struct uinput_setup usetup;

  int uinput = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if (uinput == -1) {
    printf("Error opening uinput.\n");
  } else {
    printf("Connected to uinput.\n");
  }

  /*
   * The ioctls below will enable the device that is about to be
   * created, to pass key events, in this case the space key.
   */
  ioctl(uinput, UI_SET_EVBIT, EV_KEY);
  ioctl(uinput, UI_SET_KEYBIT, BTN_LEFT);
  /* ioctl(uinput, UI_SET_KEYBIT, BTN_RIGHT); */

  ioctl(uinput, UI_SET_EVBIT, EV_REL);
  ioctl(uinput, UI_SET_RELBIT, REL_X);
  ioctl(uinput, UI_SET_RELBIT, REL_Y);

  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;  /* sample vendor */
  usetup.id.product = 0x5678; /* sample product */
  strcpy(usetup.name, "Example device");

  ioctl(uinput, UI_DEV_SETUP, &usetup);
  ioctl(uinput, UI_DEV_CREATE);

  printf("uinput is setup.\n");

  return uinput;
}

void cleanup(int uinput, int tty) {

  ioctl(uinput, UI_DEV_DESTROY);
  close(uinput);
  printf("Closed uinput\n");

  close(tty);
  printf("Closed tty\n");
}
