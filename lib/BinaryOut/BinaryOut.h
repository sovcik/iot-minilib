#ifndef __BINARYOUT_H__
#define __BINARYOUT_H__

#include <Arduino.h>
#include "ShiftInOut.h"

class BinaryOut {
    protected:
        uint8_t phase;
        uint32_t phaseDuration;
        uint32_t phaseStartMilis;
        uint32_t phaseOnDuration;
        uint32_t phaseOffDuration;

    public:
        BinaryOut();
        virtual ~BinaryOut(){};
        virtual void on()=0;
        virtual void off()=0;
        virtual void pulse();
        virtual void pulseEnable(uint32_t onDuration, uint32_t offDuration);
};

class BOut_Pin : public BinaryOut {
    protected:
        uint8_t pin;
    public:
        BOut_Pin(uint8_t pin);
        virtual void on() override;
        virtual void off() override;
};


class BOut_ShReg : public BinaryOut {
    protected:
        ShiftOutRegister* sho_reg;
        uint8_t bit;
    public:
        BOut_ShReg(ShiftOutRegister* reg, uint8_t bit);
        virtual void on() override;
        virtual void off() override;
};

#endif
