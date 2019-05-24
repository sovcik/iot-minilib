#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

class Timer {
    public:
        unsigned long time;
        unsigned long startedAt;
        bool running;

    public:

        /**
         * Create timer & specify interval. Do not start.
         */
        Timer(unsigned long t);         // timer is created for specified interval but not running

        /** 
         * Set timer interal.
         */
        void set(unsigned long t){time = t;};

        /**
         * Start timer
         */
        void start(){running = true;};

        /**
         * Set interval and start timer
         */
        void start(unsigned long t); 

        /**
         * Stop timer
         */
        void stop(){running = false;}; 

        /**
         * Is timer running?
         */
        bool isRunning() const {return running;};

        /**
         * Elapsed time since start
         */
        unsigned long getRunningTime() const;

        /**
         * Remaining time till timeout
         */
        unsigned long getRemainingTime() const;

        /**
         * Reset & start timer
         */
        void restart(){reset();start();}; 

        /**
         * Reset timer. No impact if timer is not running.
         */
        void reset(){startedAt = millis();}; 

        /**
         * Time elapsed?
         */
        bool timeout(){return (running && millis()-startedAt > time);}; 

        /**
         * Trigger timeout
         */
        void timeoutNow(){startedAt = millis()-time-100;};  //
};

#endif

