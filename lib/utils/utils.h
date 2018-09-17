#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>

uint32_t free_stack();
uint32_t unmodified_stack();
unsigned long mem_crc(void* obj, unsigned int size);
unsigned long getRandomLong();
void getRandomHex(char* s, int length);
void splitUrl(const char* srcUrl, char* host, unsigned int &port, char* uri, int &useSSL);
void mac2hex(char* dst, unsigned char* mac);
int saveToEEPROM(unsigned int address, unsigned char* data, unsigned int size);
int loadFromEEPROM(unsigned int address, unsigned char* dst, unsigned int size);
uint16_t replaceBytes(uint8_t* buff, const uint16_t buffUsed, const uint16_t buffSize, const uint8_t* find, const uint16_t findSize, const uint8_t* replace, const uint16_t replaceSize);
void printArray(uint8_t* a, uint16_t size);

#endif
