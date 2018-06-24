#ifndef __SHIFTINOUT_H__
#define __SHIFTINOUT_H__

#include <Arduino.h>

class ShiftOutRegister {
    protected:
        uint16_t shoData;

        uint8_t shoChanged;
        
        uint8_t shoLatchPin;

    public:
        ShiftOutRegister(uint8_t latchPin);
        virtual ~ShiftOutRegister(){};
        virtual void begin();
        virtual void write() =0;
        virtual void setBit(uint8_t bit);
        virtual void clrBit(uint8_t bit);
        uint16_t getData() const;

};

class SHOR_SHO :public ShiftOutRegister {
    protected:
        uint8_t shoDataPin;
        uint8_t shoClockPin;

    public:
        SHOR_SHO(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);
        virtual void begin() override;
        virtual void write() override;
};


#endif