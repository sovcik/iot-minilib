#ifndef __INDICATOR_H__
#define __INDICATOR_H__

#include <BinaryOut.h>

#define LED_DUO_OFF         0
#define LED_DUO_C1          1
#define LED_DUO_C2          2
#define LED_DUO_BOTH        3

#define LED_DUO_RED         1
#define LED_DUO_GREEN       2
#define LED_DUO_ORANGE      3

class Indicator {
    protected:
        int status;
        bool enabled;
    public:
        virtual void set(int s);
        virtual ~Indicator(){};
        int get() const {return status;}
        virtual void show(){};
        
        void enable();
        void disable();
        bool isEnabled() const {return enabled;}
};

class LED_MonoColor : public Indicator {
    protected:
        BinaryOut* c1;
    public:
        LED_MonoColor(BinaryOut* c1);
        virtual ~LED_MonoColor();
        virtual void set(int s) override;
};

class LED_DuoColor : public Indicator {
    protected:
        BinaryOut* c1;
        BinaryOut* c2;
    public:
        LED_DuoColor(BinaryOut* c1, BinaryOut* c2);
        virtual ~LED_DuoColor();
        virtual void set(int s) override;

};

#endif