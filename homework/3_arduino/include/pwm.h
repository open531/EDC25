#ifndef PWM_H
#define PWM_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>

#define ENA PA8
#define ENC_A PA6
#define ENC_B PA7

extern int count;
extern float serr, err, derr, dderr;
extern float Kp, Ki, Kd;
extern float rpm;
extern float goal;
extern int pwm;

extern HardwareSerial SerialUART1;
extern HardwareTimer TimerTIM1;

int PID(float goal, float now);
void Code0();
void Code1();
void TimerIsr();

#endif // PWM_H
