#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <SoftwareSerial.h>

#define PRINTER_MAX_BUFF_SIZE   (uint16_t)1024

#define PRN_STAT_BIT_ERROR           0x01
#define PRN_STAT_BIT_OFFLINE         0x02
#define PRN_STAT_BIT_PAPER_OUT       0x04
#define PRN_STAT_BIT_PAPER_ENDING    0x08

struct PrnMacro {
    char* macro;
    uint16_t offset;
    uint16_t length;
};

struct PrnMacros {
    uint8_t*    map;
    PrnMacro*   list;
    uint8_t     listSize;
};

class Printer {
    public:
        struct TransTable {
            char* macro;
            uint16_t mSize;
            uint8_t* codes;
            uint16_t cSize;
            TransTable* next;
        };

    protected:
        char* id;
        TransTable* pc;
        uint8_t* buff;
        uint16_t buffUsed;
        char* codePage;
        int _status;

    public:
        Printer(const char* id, const char* codePage);
        virtual ~Printer();
        virtual const char* getId();
        virtual const char* getCodePage();
        
        virtual void begin() =0;

        virtual bool write(uint8_t* buff, uint16_t buffLen);
        
        virtual void print(char* s) =0;
        virtual void print(int i) =0;
        virtual void println(char* s) =0;
        virtual void println(int i) =0;
        
        void clearBuffer();
        void load2Buffer(const char* s);
        bool writeBuffer();

        void loadMacros(const PrnMacros* macros);
        void addMacro(const char* macro, const uint8_t* value, const uint16_t valueSize);
        void replaceMacros();
        
        void replaceMacro(const char* s, uint8_t* value, uint16_t valueSize);

        virtual int status(){return _status;};

};

class SerialPrinter : public Printer {
    protected:
        SoftwareSerial* ss;
        int rxPin, txPin;
        unsigned int baudRate;

    public:
        SerialPrinter(const char* id, const char* codePage, int rxPin, int txPin, unsigned int baudRate);
        virtual ~SerialPrinter();
        virtual void begin() override;
        virtual bool write(uint8_t* buff, uint16_t buffLen) override;
        virtual void print(char* s) override;
        virtual void print(int i) override;
        virtual void println(char* s) override;
        virtual void println(int i) override;        
    
};

#endif

