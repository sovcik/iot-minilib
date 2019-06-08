#include "ShiftOutSPI.h"
#include <SPI.h>

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

SHOR_SPI::SHOR_SPI(uint8_t latchPin) : ShiftOutRegister(latchPin) {}

void SHOR_SPI::begin(){

    pinMode(shoLatchPin, OUTPUT);

    #ifdef ESP8266
    SPI.begin();
    #endif

    #ifdef ESP32
    SPI.begin();
    #endif

    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setFrequency(1000000);

    write();

    DEBUG_PRINT("[shr_spi:begin] started\n");
}

void SHOR_SPI::write() {
    uint16_t d = shoData;
    DEBUG_PRINT("[shr_spi:write] data=%X\n",d);
    digitalWrite(shoLatchPin, LOW);
    //SPI.write16(shoData);
    SPI.write16(d);
    digitalWrite(shoLatchPin, HIGH);
    shoChanged = 0;
}