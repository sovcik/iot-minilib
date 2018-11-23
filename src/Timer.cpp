#include <Arduino.h>
#include "Timer.h"

Timer::Timer(){
    time = TIMER_DEFAULT_TIME;
    running = false;
}

Timer::Timer(unsigned long t){
    time = t;
    running = false;
}

void Timer::set(unsigned long t){
    time = t;
}

void Timer::start(unsigned long t){
    time = t;
    start();
}

void Timer::start(){
    startedAt = millis();
    running = true;
}

void Timer::stop(){
    running = false;
}

bool Timer::isRunning() const {
    return running;
}

void Timer::restart(){
    start();
}

bool Timer::timeout(){
    return (!running || millis()-startedAt > time);
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

