#include "utils.h"
#include <Arduino.h>
#include <EEPROM.h>

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

#ifdef ESP8266
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1)
// All version before core 2.4.2
extern "C" {
#include <cont.h>
extern cont_t g_cont;
}

uint32_t free_stack(){
    register uint32_t *sp asm("a1");
    return 4 * (sp - g_cont.stack);
}

uint32_t unmodified_stack(){
    return cont_get_free_stack(&g_cont);
}
#else
// All version from core 2.4.2
// https://github.com/esp8266/Arduino/pull/5018
// https://github.com/esp8266/Arduino/pull/4553
 extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}

#endif

uint32_t free_stack(){
    register uint32_t *sp asm("a1");
    return 4 * (sp - g_pcont->stack);
}

uint32_t unmodified_stack(){
    return cont_get_free_stack(g_pcont);
}

#else
uint32_t free_stack(){
    return -1;
}

uint32_t unmodified_stack(){
    return -1;
}

#endif

unsigned long mem_crc(void* obj, unsigned int size) {
  
    unsigned char* a;
    a = (unsigned char*)obj;

    const unsigned long crc_table[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };

    unsigned long crc = ~0L;

    for (unsigned int index = 0 ; index < size  ; ++index) {
        crc = crc_table[(crc ^ *(a+index)) & 0x0f] ^ (crc >> 4);
        crc = crc_table[(crc ^ (*(a+index) >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }
    return crc;
}

unsigned long getRandomLong() {
    unsigned long a;
  
    // simply getting random number between 2^24 and 2^32 did not work...
    // so let's do it byte-by-byte
    a = random(0xFF);     // first byte
    for(int i=0;i<3;i++){ // and next 3 bytes
        a <<= 8;
        a |= random(0xFF);
    }
    return a;
}

void getRandomHex(char* s, int length){
    char c[] = "0123456789ABDCEF";
    for(int i=0;i<length;i++){
        s[i]=c[random(15)];
    }
}

void splitUrl(const char* srcUrl, char* host, unsigned int &port, char* uri, int &useSSL){
    char curl[300];
    char* _host;

    DEBUG_PRINT("[utils:splitUrl] url=%s\n",srcUrl);
    
    strcpy(&curl[0],srcUrl);
    _host = &curl[0];
    if (strncmp(_host,(const char*)"wss://",6) == 0){
        DEBUG_PRINT("[utils:splitUrl] WSS url detected\n");
        port = 443;
        _host = _host+6;
        useSSL = 1;
    }
    if (strncmp(_host,(const char*)"ws://",5) == 0){
        DEBUG_PRINT("[utils:splitUrl] WS url detected\n");
        port = 80;
        _host = _host+5;
    }

    *uri = '/';
    *(uri+1) = 0;
    
    char* uriAdr = strchr(_host,'/');
    if (uriAdr){
        DEBUG_PRINT("[utils:splitUrl] URI specified=%s\n",uriAdr);
        *uriAdr = 0;
        uriAdr++;
        strcpy(uri+1,uriAdr);
    }
    
    char* portAdr = strchr(_host,':');
    if (portAdr) {
        DEBUG_PRINT("[utils:splitUrl] Port in url detected=%s\n",portAdr);
        *portAdr = 0;   // replace : with zero char, so host part is terminated
        portAdr++;
        port = strtoul(portAdr,NULL,10);
        if (port == 0){
            port = 80;
            DEBUG_PRINT("[utils:splitUrl] Wrong port number. Defaulting to %d",port);
        }
    }

    strcpy(host,_host);
    DEBUG_PRINT("[utils:splitUrl] Analyzed URL ssl=%d host=%s port=%d uri=%s\n",useSSL,host,port,uri);    
}

void mac2hex(char* dst, unsigned char* mac){
    char hex[] = "0123456789ABCDEF";
    for(int i=0;i<6;i++){
        *(dst+(i*2)) = hex[*(mac+5-i)>>4];
        *(dst+(i*2)+1) = hex[*(mac+5-i)&0x0F];
    }
    *(dst+12) = 0;
    DEBUG_PRINT("[utils:MAC2HEX] hex=%s\n",dst);
}

int saveToEEPROM(unsigned int address, unsigned char* data, unsigned int size){
    unsigned long crc;
    int succ = 0;
    crc = mem_crc(data, size);
    for (unsigned int i=0;i<size;i++)
        EEPROM.write(address+i,*(data+i));
    EEPROM.put(address+size, crc);
    EEPROM.commit(); // required for ESP

    unsigned char* tmp = new unsigned char[size];
    succ = loadFromEEPROM(address,tmp, size);
    delete []tmp;
    if (succ)
        DEBUG_PRINT("[utils:s2E] Data written to EEPROM\n");
    else   
        DEBUG_PRINT("[utils:s2E] EEPROM write failed.\n");
    
    return succ;   
}

int loadFromEEPROM(unsigned int address, unsigned char* dst, unsigned int size){
    unsigned char* tmp = new unsigned char[size];
    
    unsigned long crcE;
    unsigned long crcM;
    
    // load stored record
    DEBUG_PRINT("[utils:lfE] Config start:%d size:%d\n",address, size);
    for (unsigned int i=0;i<size;i++)
        *(tmp+i) = EEPROM.read(address+i);

    DEBUG_PRINT("[utils:lfE] Config loaded\n");
    
    // read stored CRC
    EEPROM.get(address+size, crcE);
    DEBUG_PRINT("[utils:lfE] Stored CRC=%X\n",crcE);
  
    // calculate CRC for stored settings
    crcM = mem_crc(tmp, size);
    DEBUG_PRINT("[utils:lfE] Calculated CRC=%X\n",crcM);

    if (crcM == crcE)
        memcpy(dst,tmp,size);

    delete []tmp;
  
    return (crcM == crcE);      
    
}

uint16_t replaceBytes(uint8_t* buff, const uint16_t buffUsed, const uint16_t buffSize, const uint8_t* find, const uint16_t findSize, const uint8_t* replace, const uint16_t replaceSize){
    uint16_t bu = buffUsed;
    uint16_t i = 0;
    uint8_t* bp = buff;
    uint16_t j;

    DEBUG_PRINT("[utils:replB] start buffused=%d findSize=%d replSize=%d\n", bu, findSize, replaceSize);
    //DEBUG_PRINT("[before]%s\n\n",buff);
    
    while (i+findSize <= bu                                                      // if $find can fit into remaining buffer
           && (bu + replaceSize - findSize) < buffSize){                        // and if buffer will not overflow after replace
        j=0;
        while(j < findSize && *(bp+j)==*(find+j)) j++;                          // check if $find can be found on current position
        if (j == findSize) {                                                    // found -> replace
            //DEBUG_PRINT("[utils:replB] found at %d\n",(bp-buff));
            memmove(bp+replaceSize,bp+findSize,bu-(i+findSize));                 // move the rest of buffer to create space for $replace
            //DEBUG_PRINT("[after move]%s\n\n",buff);
            memcpy(bp,replace,replaceSize);                                     // copy $replace to created space
            //DEBUG_PRINT("[after replace]%s\n\n",buff);
            bu += (replaceSize - findSize);                                     // adjust used buffer size
            i += replaceSize;                                                   // set new buffer position to be after $replace
            bp += replaceSize;
        } else {                                                                // if not found
            i++;                                                                // move one position in buffer
            bp++;
        }
    }
    //DEBUG_PRINT("[utils:replB] after replacing buff used=%d\n",bu);
    //DEBUG_PRINT("[after]%s\n\n",buff);
    return bu;                                                                  // return used buffer size
}

void printArray(uint8_t* a, uint16_t size){
    for (uint16_t i =0; i<size;i++)DEBUG_PRINT("%X ",*(a+i));
}

