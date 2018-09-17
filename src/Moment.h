#ifndef __MOMENT_H__
#define __MOMENT_H__

#include <Arduino.h>

struct MomentElements {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millis;
    int8_t tzSig;
    int8_t tzOffH;
    int8_t tzOffM;
};

class Moment {
    protected:
        MomentElements me;

    public:
        int fromISO(const char *isoTime);
        const char* toUTC();

};


#endif