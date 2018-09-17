#ifndef __SHIFTOUTSPI_H__
#define __SHIFTOUTSPI_H__

#include "ShiftInOut.h"

class SHOR_SPI : public ShiftOutRegister {
    public:
        SHOR_SPI(uint8_t latchPin);
        virtual void begin() override;
        virtual void write() override;
};


#endif