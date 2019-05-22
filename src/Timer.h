#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_DEFAULT_TIME      1000 //if not specified, timer will be set to 1000ms

#include <Arduino.h>

class Timer {
    public:
        unsigned long time;
        unsigned long startedAt;
        bool running;

    public:

        Timer();                        // timer is created but not running
        Timer(unsigned long t);         // timer is created for specified interval but not running
        void set(unsigned long t);      // set timer interval
        void start();                   // start timer
        void start(unsigned long t);    // specify interval and start
        void stop();                    // stop timer
        bool isRunning() const;
        unsigned long getRunningTime() const;   // how long it has been running?
        unsigned long getRemainingTime() const; // remaining time to timeout
        void restart();   // restart timer - start if not running
        void reset();     // resets start time - no impact if not running
        bool timeout();   // true if running and time elapsed
};

#endif

