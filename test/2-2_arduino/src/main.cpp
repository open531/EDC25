#include <Arduino.h>
#include <HardwareSerial.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

HardwareSerial MySerial1(PA10, PA9);
HardwareSerial MySerial2(PA3, PA2);

uint8_t MySerial2_Buf[8];

void u1_printf(char *fmt, ...);
void u2_printf(char *fmt, ...);

void setup()
{
  MySerial1.begin(115200);
  MySerial2.begin(9600);
}

void loop()
{
  while (MySerial2.readBytes(MySerial2_Buf, sizeof(MySerial2_Buf)) < sizeof(MySerial2_Buf))
    ;
  u1_printf("received:");
  MySerial1.setTimeout(HAL_MAX_DELAY);
  MySerial1.write(MySerial2_Buf, sizeof(MySerial2_Buf));
  u2_printf("received:");
  MySerial2.setTimeout(HAL_MAX_DELAY);
  MySerial2.write(MySerial2_Buf, sizeof(MySerial2_Buf));
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
  MySerial1.write(buf, len);
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
  MySerial2.write(buf, len);
}