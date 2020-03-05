#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>

uint32_t free_stack();
uint32_t unmodified_stack();
unsigned long mem_crc(void* obj, size_t size);
unsigned long getRandomLong();
void getRandomHex(char* s, size_t length);
void splitUrl(const char* srcUrl, char* host, unsigned int &port, char* uri, int &useSSL);
void mac2hex(char* dst, unsigned char* mac);
int saveToEEPROM(uint8_t address, uint8_t* data, size_t size);
int loadFromEEPROM(uint8_t address, uint8_t* dst, size_t size);
uint16_t replaceBytes(uint8_t* buff, const size_t buffUsed, const size_t buffSize, const uint8_t* find, const size_t findSize, const uint8_t* replace, const size_t replaceSize);
void printArray(uint8_t* a, size_t size);

#endif
