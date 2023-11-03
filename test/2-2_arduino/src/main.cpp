#include <Arduino.h>
#include <HardwareSerial.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

HardwareSerial MySerial1(PA10, PA9);
HardwareSerial MySerial2(PA3, PA2);

void u1_printf(char *fmt, ...);
void u2_printf(char *fmt, ...);

void setup()
{
  MySerial1.begin(115200);
  MySerial2.begin(9600);
}

void loop()
{
  while (MySerial2.read() != HAL_OK)
    ;
  u1_printf("received:");
  while (MySerial1.available())
  {
    u1_printf("%c", MySerial1.read());
  }
  u2_printf("received:");
  while (MySerial2.available())
  {
    u2_printf("%c", MySerial2.read());
  }
  delay(100);
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

void u2_printf(char *fmt, ...)
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
    MySerial2.write(buf[i]);
  }
}