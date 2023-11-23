#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial TTLSerial(PA9, PA10);
HardwareSerial ZigbeeSerial(PA2, PA3);

uint8_t a = 0;
uint8_t b = 0;
uint8_t c = 0;
uint8_t d = 0;
uint8_t u2_RX_Buf[5];

int randNext(int left, int right) {
  static unsigned int seed = 0;
  seed++;
  srand((unsigned)millis() + seed * seed);
  return rand() % (right - left + 1) + left;
}

void setup() {
  TTLSerial.begin(115200);
  ZigbeeSerial.begin(9600);
}

void loop() {
  a = randNext(10, 99);
  b = randNext(10, 99);
  c = randNext(10, 99);
  d = randNext(10, 99);
  ZigbeeSerial.printf("%d %d %d %d\r\n", a, b, c, d);
  TTLSerial.printf("%d %d %d %d\r\n", a, b, c, d);
  while (!ZigbeeSerial.available())
    ;
  ZigbeeSerial.readBytes(u2_RX_Buf, sizeof(u2_RX_Buf));
  TTLSerial.write(u2_RX_Buf, sizeof(u2_RX_Buf));
  delay(2000);
}
