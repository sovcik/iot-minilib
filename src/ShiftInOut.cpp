#include "ShiftInOut.h"

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

ShiftOutRegister::ShiftOutRegister(uint8_t latchPin){
    shoData = 0;
    shoChanged = 0;

    shoLatchPin = latchPin;

}

void ShiftOutRegister::begin() {

    pinMode(shoLatchPin, OUTPUT);
    write();
}

void ShiftOutRegister::clrBit(uint8_t bit){
    uint16_t mask = ~(1<<bit);
    DEBUG_PRINT("[shr:clrbit] bit=%d mask=%X data=%X", bit, mask, shoData);
    shoData &= mask;
    DEBUG_PRINT(" out=%X\n",shoData);
    shoChanged = 1;
    
}

void ShiftOutRegister::setBit(uint8_t bit){
    DEBUG_PRINT("[shr:setbit] bit=%d data=%X", bit, shoData);
    shoData |= 1<<bit;
    DEBUG_PRINT(" out=%X\n",shoData);
    shoChanged = 1;
}

uint16_t ShiftOutRegister::getData() const {
    return shoData;
}

SHOR_SHO::SHOR_SHO(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin) : ShiftOutRegister(latchPin) {
    
    shoDataPin = dataPin;
    shoClockPin = clockPin;

}

void SHOR_SHO::begin(){
    pinMode(shoLatchPin, OUTPUT);
    pinMode(shoDataPin, OUTPUT);
    pinMode(shoClockPin, OUTPUT);
    write();
}

void SHOR_SHO::write(){
    DEBUG_PRINT("[shr_sho:write] data=%X\n",shoData);
    digitalWrite(shoLatchPin, LOW);
    shiftOut(shoDataPin, shoClockPin, MSBFIRST, (shoData>>8));
    shiftOut(shoDataPin, shoClockPin, MSBFIRST, shoData);
    digitalWrite(shoLatchPin, HIGH);
    shoChanged = 0;
}
