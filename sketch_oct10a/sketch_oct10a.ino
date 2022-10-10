#include <avr/sleep.h>
#include <avr/power.h>
#include "functions.h"


extern state current_state;
extern bool alarm;
bool timer_int = false;


ISR(WDT_vect) { timer_int = true; }
void isr() {}

void goSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

void setup() {
  delay(3000);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);  
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  power_adc_disable();
  power_spi_disable();
  //power_timer0_disable();
  power_timer3_disable();
  power_twi_disable(); 

  cli();
  attachInterrupt(digitalPinToInterrupt(S1), isr, RISING);
  attachInterrupt(digitalPinToInterrupt(S2), isr, RISING);
  attachInterrupt(digitalPinToInterrupt(S3), isr, RISING);
  attachInterrupt(digitalPinToInterrupt(S4), isr, RISING);
  sei();

  turnOn();
}

void loop() {
  goSleep();
  isLongPress();
  if (timer_int) {
    switch (current_state) {
      case WAIT60: wait60(); break;
      case WAIT90: wait90(); break;
      case WAIT120: wait120(); break;
      default: break;
    }
    timer_int = false;
  } else if (current_state && alarm) {
      turnOn();
  }
}
