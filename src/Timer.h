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
         * 
         * @t - timer interval in ms
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
         * 
         * @t - new timer interval
         */
        void start(uint32_t t){set(t);restart();};

        /**
         * Stop timer
         */
        void stop(){running = false;}; 

        /**
         * Resume timer
         * Timer status will be set to "running" after calling this method.
         */
        inline void resume(){running=true;};

        /**
         * Is timer running?
         */
        inline bool isRunning() const {return running;};

        /**
         * Elapsed time since start.
         * Returns zero if not running.
         */
        unsigned long getRunningTime() const;

        /**
         * Remaining time till timeout
         * Returns zero if not running.
         */
        unsigned long getRemainingTime() const;

        /**
         * Reset & start timer
         */
        void restart(){reset();resume();}; 

        /**
         * Reset timer to start again from now.
         * No impact if timer is not running.
         */
        void reset(){startedAt = millis();}; 

        /**
         * Time elapsed?
         */
        inline bool timeout(){return (running && millis()-startedAt > time);}; 

        /**
         * Trigger timer timeout. 
         * Next call of method timeout() will return true.
         */
        void triggerTimeout(){startedAt = millis()-time-100;};  //
};

#endif

