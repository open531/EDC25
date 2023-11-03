#include <Arduino.h>
#include <HardwareSerial.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

HardwareSerial MySerial1(PA10, PA9);

void u1_printf(char *fmt, ...);

void setup()
{
  MySerial1.begin(115200);
}

void loop()
{
  static int cnt = 1;
  u1_printf("test: %d %f\r\n", cnt++, 1.0 / cnt);
  delay(500);
}

void u1_printf(char *fmt, ...)
{
  uint16_t len;
  va_list ap;
  va_start(ap, fmt);
  uint8_t buf[200];
  vsprintf((char *)buf, fmt, ap);
  va_end(ap);
  len = strlen((char *)buf);
  for (uint16_t i = 0; i < len; i++)
  {
    MySerial1.write(buf[i]);
  }
}