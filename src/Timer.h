#ifndef __TIMER_H__
#define __TIMER_H__

class Timer {
    public:
        unsigned long time;
        unsigned long startedAt;
        bool running;

    public:

        Timer();
        Timer(unsigned long t);
        void set(unsigned long t);
        void start();
        void start(unsigned long t);
        void stop();
        bool isRunning() const;
        unsigned long getRunningTime() const;
        unsigned long getRemainingTime() const;
        void restart();
        bool timeout();
};

#endif

