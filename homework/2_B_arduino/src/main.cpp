#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial TTLSerial(PA9, PA10);
HardwareSerial ZigbeeSerial(PA2, PA3);

uint8_t a = 0;
uint8_t b = 0;
uint8_t c = 0;
uint8_t d = 0;
uint8_t u2_RX_Buf[11];

uint8_t isNear(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  if ((a == c && b - d <= 1 && b - d >= -1) ||
      (b == d && a - c <= 1 && a - c >= -1))
    return 1;
  else
    return 0;
}

void setup() {
  TTLSerial.begin(115200);
  ZigbeeSerial.begin(9600);
}

void loop() {
  while (!ZigbeeSerial.available())
    ;
  ZigbeeSerial.readBytes(u2_RX_Buf, sizeof(u2_RX_Buf));
  sscanf((char *)u2_RX_Buf, "%d %d %d %d", &a, &b, &c, &d);
  if (isNear(a, b, c, d)) {
    ZigbeeSerial.printf("true ");
  } else {
    ZigbeeSerial.printf("false");
  }
}
