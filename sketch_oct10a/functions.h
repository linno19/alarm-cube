#pragma once
#include <Arduino.h>
#include <avr/wdt.h>

#define S1 1
#define S2 2
#define S3 3
#define S4 0

#define RED 9
#define GREEN 6
#define BLUE 5

#define LONG_PRESS 3000
#define TIME1 3 //675
#define TIME2 5 //900
#define TIME3 7
#define TURNOFF 10

enum state {OFF, WAIT60, WAIT90, WAIT120};

void isLongPress();
void turnOn();
void turnOff();
void wait60();
void wait90();
void wait120();
