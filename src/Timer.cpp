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

inline void Timer::set(unsigned long t){
    time = t;
}

inline void Timer::start(unsigned long t){
    time = t;
    start();
}

inline void Timer::start(){
    startedAt = millis();
    running = true;
}

inline void Timer::stop(){
    running = false;
}

inline bool Timer::isRunning() const {
    return running;
}

inline void Timer::restart(){
    start();
}

inline void Timer::reset(){
    startedAt = millis();
}

inline bool Timer::timeout(){
    return (running && millis()-startedAt > time);
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

