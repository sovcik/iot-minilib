#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned long t){
    time = t;
    running = false;
}

unsigned long Timer::getRunningTime() const {
    return running?(millis()-startedAt):0;
}

unsigned long Timer::getRemainingTime() const {
    return running?time-(millis()-startedAt):0;        
}

