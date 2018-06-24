#include "BinaryOut.h"
#include "ShiftInOut.h"

BinaryOut::BinaryOut(){
    phase = 2;
}


void BinaryOut::pulseEnable(uint32_t onDuration, uint32_t offDuration){
    phaseDuration = 0;
    phaseStartMilis = 0;
    phaseOnDuration = onDuration;
    phaseOffDuration = offDuration;
}


void BinaryOut::pulse(){
    if (millis() - phaseStartMilis > phaseDuration){
        phaseStartMilis = millis();
        phase++;
        if (phase > 1) {
            phase = 0;
            phaseDuration = phaseOffDuration;
            off();
        } else {
            phaseDuration = phaseOnDuration;
            on();
        }
    }

}

BOut_Pin::BOut_Pin(uint8_t pin){
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void BOut_Pin::on(){
    digitalWrite(pin, 1);
}

void BOut_Pin::off(){
    digitalWrite(pin, 0);
}

BOut_ShReg::BOut_ShReg(ShiftOutRegister* reg, uint8_t bit){
    this->sho_reg = reg;
    this->bit = bit;
}

void BOut_ShReg::on(){
    sho_reg->setBit(bit);
}

void BOut_ShReg::off(){
    sho_reg->clrBit(bit);
}

