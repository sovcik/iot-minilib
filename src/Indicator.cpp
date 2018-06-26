#include "Indicator.h"

#define NODEBUG_PRINT
#include <debug_print.h>

void Indicator::set(int s){
    DEBUG_PRINT("[Indicator:set] status=%d val=%d\n",status,s);
    status = s;
}

void Indicator::enable(){
    DEBUG_PRINT("[Indicator:enable] enabled\n");
    enabled = true;
}

void Indicator::disable(){
    DEBUG_PRINT("[Indicator:disable] disabled\n");
    enabled = false;
}

LED_MonoColor::LED_MonoColor(BinaryOut* c1){
    this->c1 = c1;
}

LED_MonoColor::~LED_MonoColor(){
    if (c1) delete c1;
}

void LED_MonoColor::set(int s){
    if (s) 
        c1->on();
    else
        c1->off();
}

LED_DuoColor::LED_DuoColor(BinaryOut* c1, BinaryOut* c2){
    this->c1 = c1;
    this->c2 = c2;
}

LED_DuoColor::~LED_DuoColor(){
    if (c1) delete c1;
    if (c2) delete c2;
}

void LED_DuoColor::set(int s){
    DEBUG_PRINT("[LDuo:set] status=%d\n",s);
    switch(s){
        case LED_DUO_OFF:
            c1->off();
            c2->off();
            break;
        case LED_DUO_C1:
            c1->on();
            c2->off();
            break;
        case LED_DUO_C2:
            c1->off();
            c2->on();
            break;
        case LED_DUO_BOTH:
            c1->on();
            c2->on();
    }
}

