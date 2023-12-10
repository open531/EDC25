#include "canmvk210.h"

CanMVK210::CanMVK210(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
}