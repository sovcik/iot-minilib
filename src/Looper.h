#ifndef __LOOPER_H__
#define __LOOPER_H__

class Looper {
    public:
        virtual void loop(){};
};

struct LooperEntry {
    Looper *looper;
    LooperEntry *next;
};

class LooperHandler {
    protected:
        LooperEntry *_loopers;

    public:
        LooperHandler();
        ~LooperHandler();
        void loop();
        void registerLooper(Looper *looper);

};

#endif