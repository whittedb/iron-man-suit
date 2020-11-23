#pragma once

#ifdef RTT_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_BEGIN
    #define DEBUG_WAIT_FOR_INIT

    void DEBUG_PRINT(const char *msg, unsigned id=0);
    void DEBUG_PRINTLN(const char *msg, unsigned id=0);
    void DEBUG_PRINTF(unsigned id, const char *fmt, ...);
#else
    #ifdef MY_DEBUG
        #ifndef DEBUG_SERIAL_SPEED
            #define DEBUG_SERIAL_SPEED 9600
        #endif
        #define DEBUG_BEGIN Serial.begin(DEBUG_SERIAL_SPEED)
        #define DEBUG_WAIT_FOR_INIT while(!Serial){}
        void DEBUG_PRINT(const char *msg, unsigned id=0);
        void DEBUG_PRINT(const __FlashStringHelper *msg, unsigned id=0);
        void DEBUG_PRINTLN(const char *msg, unsigned id=0);
        void DEBUG_PRINTLN(const __FlashStringHelper *msg, unsigned id=0);
        void DEBUG_PRINTF(unsigned id, const char *fmt, ...);
        void DEBUG_PRINTF(unsigned id, const __FlashStringHelper *fmt...);
    #else
        #define DEBUG_BEGIN
        #define DEBUG_WAIT_FOR_INIT
        #define DEBUG_PRINT(...)
        #define DEBUG_PRINTLN(...)
        #define DEBUG_PRINTF(...)
    #endif
#endif

int freeMemory();
