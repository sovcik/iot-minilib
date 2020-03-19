#ifndef __LOOPER_H__
#define __LOOPER_H__

/**
 * Generic class consolidating loop() methods for other classes.
 * It is assumed that such objects will be registered and called via LooperHandler
 */
class Looper {
    public:
        virtual void loop(){};
};

struct LooperEntry {
    Looper *looper;
    LooperEntry *next;
};

/**
 * Generic looper.
 * Calling its loop() will executes loop() methods for all registered loopers.
 */
class LooperHandler {
    protected:
        LooperEntry *_loopers;

    public:
        /**
         * Generic looper.
         * Calling its loop() will executes loop() methods for all registered loopers.
        */
        LooperHandler();
        ~LooperHandler();

        /**
         * Execute loop() methods of all Loopers registered in this handler.
         */
        void loop();

        /**
         * Register Looper object (descendant of Looper class) to be executed by this LooperHandler
         * 
         * @looper - pointer to Looper object
         */
        void registerLooper(Looper *looper);

};

#endif