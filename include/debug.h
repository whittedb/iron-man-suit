#pragma once
#include <string>

#ifdef RTT_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_BEGIN
    #define DEBUG_WAIT_FOR_INIT

    void DEBUG_PRINT(const char *msg, unsigned id=0) {SEGGER_RTT_WriteString(id, msg);}
    void DEBUG_PRINTLN(const char *msg, unsigned id=0) {SEGGER_RTT_WriteString(id, std::string(msg).append("\r\n").c_str());}
    static void DEBUG_PRINTF(unsigned id, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        SEGGER_RTT_printf(id, fmt, args);
        va_end(args);
    }
#else
    #ifdef MY_DEBUG
        #ifndef DEBUG_SERIAL_SPEED
            #define DEBUG_SERIAL_SPEED 9600
        #endif
        #define DEBUG_BEGIN Serial.begin(DEBUG_SERIAL_SPEED)
        #define DEBUG_WAIT_FOR_INIT while(!Serial){}
        #define DEBUG_PRINT(msg, id) {Serial.print(F(msg));}
        #define DEBUG_PRINTLN(msg,id) {Serial.println(F(msg));}
        static void DEBUG_PRINTF(unsigned id, const char *fmt, ...) {
            va_list args;
            va_start(args, fmt);
            Serial.printf(fmt, args);
            va_end(args);
        }
    #else
        #define DEBUG_BEGIN
        #define DEBUG_WAIT_FOR_INIT
        #define DEBUG_PRINT(...)
        #define DEBUG_PRINTLN(...)
        #define DEBUG_PRINTF(...)
    #endif
#endif

int freeMemory();
