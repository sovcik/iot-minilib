#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned long t){
    time = t;
    running = false;
}


void Timer::start(unsigned long t){
    time = t;
    start();
}

void Timer::start(){
    startedAt = millis();
    running = true;
}

unsigned long Timer::getRunningTime() const {
    if (running)
        return (millis()-startedAt);
    else
        return 0;
        
}

unsigned long Timer::getRemainingTime() const {
    if (running)
        return time-(millis()-startedAt);
    else
        return 0;
        
}

