#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>

/**
 * Return free stack size.
 * 
 * May not work on other than ESP8266 boards.
 */
uint32_t free_stack();

/**
 * Return unmodified/untouched stack size.
 * 
 * May not work on other than ESP8266 boards.
 */
uint32_t unmodified_stack();

/**
 * Calculate CRC32 for provided data
 * 
 * @buff - pointer to data buffer
 * @size - buffer size - bytes from buffer to be included in calculation
 */
uint32_t mem_crc(void* buff, size_t size);

uint32_t getRandomLong();
void getRandomHex(char* s, size_t length);
void splitUrl(const char* srcUrl, char* host, unsigned int &port, char* uri, int &useSSL);
void mac2hex(char* dst, unsigned char* mac);
int saveToEEPROM(uint8_t address, uint8_t* data, size_t size);
int loadFromEEPROM(uint8_t address, uint8_t* dst, size_t size);

/**
 * Find and replace in binary buffer.
 * Looks for a specified chunk of bytes in provided buffer and replaces them with another chunk of bytes.
 * 
 * @buff - buffer containing original data
 * @buffUsed - size of buffer used by data
 * @buffSize - total buffer size - it has be sufficient if data to be replaced is shorted than replacement
 * @find - pointer to data to be searched for in @buff
 * @findSize - size of data to be found
 * @replace - pointer to data which will replace found data
 * @replaceSize - size of replacement
 */
uint16_t replaceBytes(uint8_t* buff, const size_t buffUsed, const size_t buffSize, const uint8_t* find, const size_t findSize, const uint8_t* replace, const size_t replaceSize);

/**
 * Print buffer content in as hexadecimal to DEBUG_PORT
 * 
 * @buff - buffer to print
 * @size - buffer size
 */
void printArray(uint8_t* buff, size_t size);

#endif
