#include <Arduino.h>
#include "Beeper.h"

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

Beeper::Beeper(){
    DEBUG_PRINT("[Beeper] creating beeper\n");
    enabled = true;
    phase = 0;
    beepCount = 0;
    setDuration(500,200); // default beep
}

Beeper::~Beeper(){
    soundOff();
}

void Beeper::setDuration(uint16_t durationOn, uint16_t durationOff){
    beepDurationOn = durationOn;
    beepDurationOff = durationOff; 
}

void Beeper::beep(int8_t count){
    if (enabled) {
        DEBUG_PRINT("[Beeper:beep] BEEP count=%d\n",count);
        beepCount = count;
        phase = 0;
        phaseStart = 0;
        phaseDuration = 0;
        loop();
    } else {
        DEBUG_PRINT("[Beeper:beep] BEEP is OFF\n");
    }
}

void Beeper::doBeep(uint32_t waitTime){
    uint32_t w = millis();
    DEBUG_PRINT("[Beeper:doBeepN] ");
    do {
        delay(10);
        loop();
    } while (millis()-w < waitTime);
}

void Beeper::loop(){
    if (beepCount < 1)
        return;

    unsigned long now = millis();

    if (now-phaseStart < phaseDuration){
        DEBUG_PRINT("[Beeper:loop] now=%d ps=%d\n", now, phaseStart);
        return;
    } else {
        DEBUG_PRINT("b");
    }

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
        default:
            DEBUG_PRINT("[Beeper:loop] phase=%d\n",phase);
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

PinBeeper::PinBeeper(uint8_t pin) : Beeper(){
    DEBUG_PRINT("[PinBeeper] creating beeper on gpio=%d\n",pin);
    beeperPin = pin;
    pinMode(beeperPin, OUTPUT);
    soundOff();
}

void PinBeeper::soundOn(){
    DEBUG_PRINT("[PinBeeper:soundOn] pin=%d\n", beeperPin);
    pinMode(beeperPin, OUTPUT);
    digitalWrite(beeperPin,1);
}

void PinBeeper::soundOff(){
    DEBUG_PRINT("[PinBeeper:soundOff] pin=%d\n", beeperPin);
    pinMode(beeperPin, OUTPUT);
    digitalWrite(beeperPin,0);
}

PWMBeeper::PWMBeeper(uint8_t pin) : PinBeeper(pin){
    DEBUG_PRINT("[PWMBeeper] creating beeper on gpio=%d\n",pin);
    beeperPin = pin;
    pinMode(beeperPin, OUTPUT);
    soundOff();
    setPitch(NOTE_A1);
}

void PWMBeeper::setPitch(uint16_t pitch){
    DEBUG_PRINT("[PWMBeeper:setPitch] pitch=%d\n", pitch);
    beepPitch = pitch;
}

void PWMBeeper::soundOn(){
    DEBUG_PRINT("[PWMBeeper:soundOn] pin=%d\n", beeperPin);
    pinMode(beeperPin, OUTPUT);
    analogWrite(beeperPin, beepPitch);
}

void PWMBeeper::soundOff(){
    DEBUG_PRINT("[PWMBeeper:soundOff] pin=%d\n", beeperPin);
    pinMode(beeperPin, OUTPUT);
    analogWrite(beeperPin, 0);
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
