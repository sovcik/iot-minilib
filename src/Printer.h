#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <SoftwareSerial.h>
#include <Looper.h>
#include <Timer.h>

#define PRINTER_MAX_BUFF_SIZE   (uint16_t)1024

#define PRN_STAT_BIT_OFFLINE            0x01   // 
#define PRN_STAT_BIT_PAPER_OUT          0x02   // paper out
#define PRN_STAT_BIT_PAPER_ENDING       0x04
#define PRN_STAT_BIT_SHAFT_UNSET        0x08
#define PRN_STAT_BIT_COVER_OPEN         0x10
#define PRN_STAT_BIT_CUTTER_ERROR       0x20
#define PRN_STAT_OTHER_ERROR            0x40
#define PRN_STAT_UNRECOVERABLE_ERROR    0x80

#define PRINTER_STATUS_FREQUENCY    10000
#define PRINTER_STATUS_TIMEOUT      1000

enum Printer_Status {
    PRN_OFF,
    PRN_READY,
    PRN_ERROR,
    PRN_OFFLINE
};

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

class Printer : public Looper {
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
        
        virtual size_t print(char* s) =0;
        virtual size_t print(int i) =0;
        virtual size_t println(char* s) =0;
        virtual size_t println(int i) =0;
        
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

        Timer *statusUpdate;

        int writeWaitRead(uint8_t* buff, size_t size, uint32_t wait=100);
        virtual void updatePrinterStatus()=0;

    public:
        SerialPrinter(const char* id, const char* codePage, int rxPin, int txPin, unsigned int baudRate);
        virtual ~SerialPrinter();
        virtual void begin() override;
        virtual bool write(uint8_t* buff, uint16_t buffLen) override;
        virtual size_t print(char* s) override;
        virtual size_t print(int i) override;
        virtual size_t println(char* s) override;
        virtual size_t println(int i) override;

        // from Looper class
        virtual void loop() override;
    
};

#endif

