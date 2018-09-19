#ifndef __BEEPER_H__
#define __BEEPER_H__

#include <Arduino.h>
#include "tones.h"
#include <BinaryOut.h>
#include <Looper.h>

class Beeper : public Looper {
    protected:
        bool enabled;
        
        unsigned int beepPitch;
        unsigned int beepDurationOn;    // duration of beeping phase
        unsigned int beepDurationOff;   // duration of silence between beeps
        unsigned long phaseStart;       // time when specific beeping phase started
        unsigned long phaseDuration;
        byte phase;
        byte beepCount;
    
    public:
        Beeper();
        virtual ~Beeper();
        void setBeep(unsigned int pitch, unsigned int durationOn, unsigned int durationOff);
        void beep(byte count);
        virtual void soundOn();
        virtual void soundOff();
        void enable();
        void disable();
        bool isEnabled();
        void doBeep(unsigned long);     // blocking beeping
        void loop() override;          // non-blocking beeping
    
};

class PinBeeper : public Beeper {
    protected:
        byte beeperPin;

    public:
        PinBeeper(byte pin);
        virtual void soundOn() override;
        virtual void soundOff() override;
};

class SHO_Beeper : public BOut_ShReg, public Beeper {
    public:
        SHO_Beeper(ShiftOutRegister* reg, uint8_t bit);
        virtual void soundOn() override;
        virtual void soundOff() override;

};

#endif
