#ifndef NODEBUG_PRINT
#ifdef DEBUG_ESP_PORT
#define DEBUG_PRINT(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#define DEBUG_ARRAY(ARR,ARR_L) for (uint16_t _aidx =0; _aidx<ARR_L;_aidx++) {DEBUG_ESP_PORT.printf("%02X ",*(ARR+_aidx)); if (_aidx%20 == 19)DEBUG_ESP_PORT.printf("\n");}
#else
#define DEBUG_PRINT(...) os_printf( __VA_ARGS__ )
#define DEBUG_ARRAY(ARR,ARR_L) for (uint16_t _aidx =0; _aidx<ARR_L;_aidx++) {os_printf("%02X ",*(ARR+_aidx)); if (_aidx%20 == 19)os_printf("\n");}
#endif
#endif

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(...)
#define DEBUG_ARRAY(...)
#define NODEBUG_PRINT
#endif
