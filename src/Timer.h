#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

class Timer {
    public:
        uint32_t time;
        uint32_t startedAt;
        bool running;

    public:

        Timer();

        /**
         * Create timer & specify interval. Do not start.
         */
        Timer(uint32_t t);

        /** 
         * Set timer interal.
         */
        void set(uint32_t t){time = t;};

        /**
         * Start timer from zero
         */
        inline void start(){restart();};


        /**
         * Set timer interval and start timer;
         */
        void start(uint32_t t){set(t);restart();};

        /**
         * Stop timer
         */
        void stop(){running = false;}; 

        /**
         * Resume timer
         */
        inline void resume(){running=true;};

        /**
         * Is timer running?
         */
        inline bool isRunning() const {return running;};

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
        void restart(){reset();resume();}; 

        /**
         * Reset timer. No impact if timer is not running.
         */
        void reset(){startedAt = millis();}; 

        /**
         * Time elapsed?
         */
        inline bool timeout(){return (running && millis()-startedAt > time);}; 

        /**
         * Trigger timeout
         */
        void triggerTimeout(){startedAt = millis()-time-100;};  //
};

#endif

