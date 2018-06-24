#include <Arduino.h>
#include "Beeper.h"

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

#define BEEPER_PIN_WRITE(...)   analogWrite( __VA_ARGS__ )

Beeper::Beeper(){
    DEBUG_PRINT("[Beeper] creating beeper\n");
    enabled = false;
    phase = 0;
    setBeep(NOTE_C1,500,200); // default beep
}

Beeper::~Beeper(){
    beepCount = 0;
    soundOff();
}

void Beeper::setBeep(unsigned int pitch, unsigned int durationOn, unsigned int durationOff){
    beepPitch = pitch;
    beepDurationOn = durationOn;
    beepDurationOff = durationOff; 
}

void Beeper::beep(byte count){
    if (enabled) {
        beepCount = count;
        phase = 0;
        phaseStart = 0;
        phaseDuration = 0;
        doBeep();
    } else {
        DEBUG_PRINT("[Beeper:beep] BEEP is OFF");
    }
}

void Beeper::doBeep(unsigned long waitTime){
    unsigned long w = millis();
    DEBUG_PRINT("[Beeper:doBeepN] ");
    do {
        delay(10);
        doBeep();
    } while (millis()-w < waitTime);
}

void Beeper::doBeep(){
    if (beepCount < 1)
        return;

    unsigned long now = millis();

    if (now-phaseStart < phaseDuration)
        return;

    switch (phase){
        case 0:
            phaseDuration = beepDurationOn;
            phaseStart = now;
            phase = 1;
            soundOn();
            break;
        case 1:
            phaseDuration = beepDurationOff;
            phaseStart = now;
            phase = 0;
            beepCount--;
            soundOff();
            break;
    }
  
}

void Beeper::soundOn(){
    DEBUG_PRINT("[Beeper:soundOn]\n");
    return;
}


void Beeper::soundOff(){
    DEBUG_PRINT("[Beeper:soundOff]\n");
    return;
}

void Beeper::enable(){
    enabled = true;
}

void Beeper::disable(){
    enabled = false;
}

bool Beeper::isEnabled(){
    return enabled;
}

PinBeeper::PinBeeper(byte pin){
    DEBUG_PRINT("[PinBeeper] creating beeper on gpio=%d\n",pin);
    beeperPin = pin;
    enabled = true;
    phase = 0;
    pinMode(beeperPin, OUTPUT);
    BEEPER_PIN_WRITE(beeperPin, 0);
    setBeep(NOTE_C1,500,200); // default beep
}


void PinBeeper::soundOn(){
    DEBUG_PRINT("[PinBeeper:soudOn]\n");
    BEEPER_PIN_WRITE(beeperPin, beepPitch);
}

void PinBeeper::soundOff(){
    DEBUG_PRINT("[PinBeeper:soudOff]\n");
    BEEPER_PIN_WRITE(beeperPin, 0);
}

SHO_Beeper::SHO_Beeper(ShiftOutRegister* reg, uint8_t bit) : BOut_ShReg(reg, bit){
    DEBUG_PRINT("[SHO_Beeper] creating beeper on bit=%d\n",bit);
}

void SHO_Beeper::soundOn(){
    DEBUG_PRINT("[SHO_Beeper:soudOn]\n");
    sho_reg->setBit(this->bit);
}

void SHO_Beeper::soundOff(){
    DEBUG_PRINT("[SHO_Beeper:soudOff]\n");
    sho_reg->clrBit(this->bit);
}