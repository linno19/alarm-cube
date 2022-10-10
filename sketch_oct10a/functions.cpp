#include "functions.h"

state current_state = WAIT60;
bool alarm = false;
volatile int passes = 0;

void isLongPress() {
  unsigned long current_time = millis();
  unsigned long target_time = current_time + LONG_PRESS; 
  while (current_time < target_time) {
    if (digitalRead(S1) || digitalRead(S2) || digitalRead(S3) || digitalRead(S4)) {
      current_time = millis();
    }
    else
      return; 
  }
  current_state ? turnOff() : turnOn();
}

void turnOn() {
  current_state = WAIT60;
  alarm = false;
  passes = 0;
  SREG &= 0x7;                      // Satus Register, 7th bit to disable all interrupts
  MCUSR &= ~(1<<WDRF);              // MCU Status Register, reset the WD Reset Flag
  WDTCSR |= (1<<WDCE) | (1<<WDE);   // WD Timer Control Register, enable prescaler editing
  WDTCSR = 1<<WDP0 | 1<<WDP3;       // Set prescaler to 8 sec
  WDTCSR |= 1<<WDIE;                // Enable WD Interrupt
  SREG |= 0x80;                     // Enable all interrupts
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
    delay(100);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    delay(100);
  }
}

void turnOff() {
  wdt_disable();  
  for (int i = 0; i < 5; i++) {
    digitalWrite(RED, HIGH);
    delay(100);
    digitalWrite(RED, LOW);
    delay(100);
  }
  current_state = OFF;
}

void wait60() {
  passes++;
  if (!alarm && (passes >= TIME1)) {
    alarm = true;
  }
  if (alarm) {
    digitalWrite(GREEN, HIGH);
    delay(1000);
    digitalWrite(GREEN, LOW);
    if (passes >= TIME2) {
      current_state = WAIT90;
    } 
  }
}

void wait90() {
  digitalWrite(BLUE, HIGH);
  delay(1000);
  digitalWrite(BLUE, LOW);
  if (++passes >= TIME3) {
    current_state = WAIT120;
  }
}

void wait120() {
  if (++passes >= TURNOFF) {
    turnOff();
    return;
  } 
  digitalWrite(RED, HIGH);
  delay(1000);
  digitalWrite(RED, LOW);
}
