#include "Printer.h"
#include <utils.h>

#define NODEBUG_PRINT
#include <debug_print.h>

Printer::Printer(const char* id, const char* codePage){
    this->id = new char[strlen(id)+1];
    strcpy(this->id, id);
    this->codePage = new char[strlen(codePage)+1];
    strcpy(this->codePage, codePage);
    
    buff = new uint8_t[PRINTER_MAX_BUFF_SIZE];
    buffUsed = 0;

}

Printer::~Printer(){
    delete []id;
    delete []codePage;
    delete []buff;
    
    //todo: release TransTable
}

const char* Printer::getId(){
    return id;
}

const char* Printer::getCodePage(){
    return codePage;
}

void Printer::addMacro(const char* macro, const uint8_t* value, const uint16_t valueSize){
    DEBUG_PRINT("[Prn:addMcr] adding macro=%s offset=%d length=%d\n",macro, *value, valueSize);
    TransTable* p = new TransTable();
    
    p->mSize = strlen(macro);
    p->macro = new char[p->mSize+1];
    strcpy(p->macro, macro);

    p->cSize = valueSize;
    p->codes = new uint8_t[p->cSize];
    memcpy(p->codes, value, p->cSize);
    
    DEBUG_PRINT("[Prn:addMcr] codes=");
    DEBUG_ARRAY(value, valueSize);
    DEBUG_PRINT("\n");
    
    p->next = pc;
    pc = p;
}

void Printer::loadMacros(const PrnMacros* macros){
    DEBUG_PRINT("[Prn:loadMcr] loading %d macros from %X\n",macros->listSize, (unsigned int)macros);
    for(uint8_t i=0; i<macros->listSize; i++){
        addMacro(macros->list[i].macro, macros->map+macros->list[i].offset, macros->list[i].length);
    }
}

void Printer::clearBuffer(){
    buffUsed = 0;
}

void Printer::load2Buffer(const char* s){
    DEBUG_PRINT("[Prn:L2b] %s\n",s);
    buffUsed = strlen(s);
    memcpy(buff,s,buffUsed);
}

bool Printer::write(uint8_t* buff, uint16_t buffLen){
    DEBUG_PRINT("[Prn:write] buffLen=%d\n",buffLen);
    return true;
}

bool Printer::writeBuffer(){
    DEBUG_PRINT("[Prn:wrBuff] buffUsed=%d\n",buffUsed);
    return write(buff,buffUsed);
}


void Printer::replaceMacro(const char* s, uint8_t* value, uint16_t valueSize){
    DEBUG_PRINT("[Prn:replMcr] macro=%s\n",s);
    buffUsed = replaceBytes(buff, buffUsed, PRINTER_MAX_BUFF_SIZE, (uint8_t*)s, strlen(s), value, valueSize);
}

void Printer::replaceMacros(){
    DEBUG_PRINT("[Prn:replMacros]\n");

    TransTable* p = pc;
    while (p) {
        DEBUG_PRINT("[Prn:replMcr] Replacing macro=%s with codes=",p->macro);
        DEBUG_ARRAY(p->codes, p->cSize);
        DEBUG_PRINT("\n");
        // replace macro with codes
        buffUsed = replaceBytes(buff, buffUsed, PRINTER_MAX_BUFF_SIZE, (uint8_t*)p->macro, p->mSize, p->codes, p->cSize);
        p = p->next;
    }
    
}

void Printer::startStatusUpdate(uint32_t interval){
    statusTimer.start(interval);
}

void Printer::stopStatusUpdate(){
    statusTimer.stop();
}


void Printer::loop(){
    if (statusTimer.isRunning() && statusTimer.timeout()){
        updatePrinterStatus();
        statusTimer.restart();
    }
}

bool SerialPrinter::write(uint8_t* buff, uint16_t buffLen){
    DEBUG_PRINT("[SerialPrn:write] buffLen=%d data=\n",buffLen);
    if (_status & PRN_STAT_BIT_OFFLINE){
        DEBUG_PRINT("[SerialPrn:write] printer is offline\n");
        return false;
    }
    DEBUG_ARRAY(buff,buffLen);
    int w = ss->write(buff,buffLen); 
    DEBUG_PRINT("\n==data written=%d\n",w);
    return w == buffLen;
}


size_t SerialPrinter::print(const char* s){
    DEBUG_PRINT("[SerialPrn:print] %s\n",s);
    if (_status & PRN_STAT_BIT_OFFLINE){
        DEBUG_PRINT("[SerialPrn:print] printer is offline\n");
        return 0;
    }
    return ss->print(s);
}

size_t SerialPrinter::print(int i){
    DEBUG_PRINT("[SerialPrn:print] %d\n",i);
    if (_status & PRN_STAT_BIT_OFFLINE){
        DEBUG_PRINT("[SerialPrn:print] printer is offline\n");
        return 0;
    }

    return ss->print(i);
}

size_t SerialPrinter::println(const char* s){
    DEBUG_PRINT("[SerialPrn:println] %s\n",s);
    if (_status & PRN_STAT_BIT_OFFLINE){
        DEBUG_PRINT("[SerialPrn:println] printer is offline\n");
        return 0;
    }

    return ss->println(s);
}

size_t SerialPrinter::println(int i){
    DEBUG_PRINT("[SerialPrn:println] %d\n",i);
    if (_status & PRN_STAT_BIT_OFFLINE){
        DEBUG_PRINT("[SerialPrn:println] printer is offline\n");
        return 0;
    }

    return ss->println(i);
}

SerialPrinter::SerialPrinter(const char* id, const char* codePage, int rxPin, int txPin, unsigned int baudRate)
        :Printer(id, codePage){
    DEBUG_PRINT("[SerialPrn] rx=%d, tx=%d, baudrate=%d\n", rxPin, txPin, baudRate);
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->baudRate = baudRate;
    ss = new SoftwareSerial(rxPin, txPin);
    DEBUG_PRINT("[SerialPrn] created\n");
}

SerialPrinter::~SerialPrinter(){
    //delete ss;
}

void SerialPrinter::begin(){
    DEBUG_PRINT("[SerialPrn:begin] \n");
    //----- start printer
    ss->begin(baudRate);
}

int SerialPrinter::writeWaitRead(uint8_t* buff, size_t size, uint32_t wait){
    int _ps;
    Timer rxTimer = Timer(wait);
    rxTimer.start();

    ss->write(buff,size);

    while(!ss->available() && !rxTimer.timeout()){
        delay(0);
    };

    if (ss->available()){
        _ps = ss->read();
    } else {
        _ps = -1;
    }

    return _ps;

}
