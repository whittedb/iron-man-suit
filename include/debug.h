#pragma once

#ifdef RTT_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_BEGIN
    #define DEBUG_WAIT_FOR_INIT

    #define DEBUG_PRINT(id, x) SEGGER_RTT_WriteString(id, x)
    #define DEBUG_PRINTLN(id, x) SEGGER_RTT_WriteString(id, x "\r\n")
    #define DEBUG_PRINTF(id, x, ...) SEGGER_RTT_printf(id, x, __VA_ARGS__)

    #define DEBUG_PRINT(x) SEGGER_RTT_WriteString(0, x)
    #define DEBUG_PRINTLN(x) SEGGER_RTT_WriteString(0, x "\r\n")
    #define DEBUG_PRINTF(x, ...) SEGGER_RTT_printf(0, x, __VA_ARGS__)
#else
    #ifdef MY_DEBUG
        #ifndef DEBUG_SERIAL_SPEED
            #define DEBUG_SERIAL_SPEED 9600
        #endif
        #define DEBUG_BEGIN Serial.begin(DEBUG_SERIAL_SPEED)
        #define DEBUG_WAIT_FOR_INIT while(!Serial){}
        #define DEBUG_PRINT(id, x) Serial.print(F(x))
        #define DEBUG_PRINTLN(id, x) Serial.println(F(x))
        #define DEBUG_PRINTF(id, x, ...)

        #define DEBUG_PRINT(x) Serial.print(F(x))
        #define DEBUG_PRINTLN(x) Serial.println(F(x))
        #define DEBUG_PRINTF(x, ...)
    #else
        #define DEBUG_BEGIN
        #define DEBUG_WAIT_FOR_INIT
        #define DEBUG_PRINT(id, x)
        #define DEBUG_PRINTLN(id, x)
        #define DEBUG_PRINTF(id, x)

        #define DEBUG_PRINT(x)
        #define DEBUG_PRINTLN(x)
        #define DEBUG_PRINTF(x)
    #endif
#endif

int freeMemory();
