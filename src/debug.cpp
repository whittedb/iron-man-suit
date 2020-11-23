#include "debug.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

#ifdef RTT_DEBUG
  #include <SEGGER_RTT.h>
  void DEBUG_PRINT(const char *msg, unsigned id) {SEGGER_RTT_printf(id, msg);}
  void DEBUG_PRINTLN(const char *msg, unsigned id) {SEGGER_RTT_printf(id, msg); SEGGER_RTT_printf(id, "\r\n");}
  void DEBUG_PRINTF(unsigned id, const char *fmt, ...) {
      va_list args;
      va_start(args, fmt);
      SEGGER_RTT_printf(id, fmt, args);
      va_end(args);
  }
#else
    #ifdef MY_DEBUG
        void DEBUG_PRINT(const char *msg, unsigned id) {Serial.print(msg);}
        void DEBUG_PRINT(const __FlashStringHelper *msg, unsigned id) {Serial.print(msg);}
        void DEBUG_PRINTLN(const char *msg, unsigned id) {Serial.print(msg); Serial.print("\r\n");}
        void DEBUG_PRINTLN(const __FlashStringHelper *msg, unsigned id) {
          Serial.print(msg);
          Serial.print("\r\n");
        }
        void DEBUG_PRINTF(unsigned id, const char *fmt, ...) {
            char b[128];
            va_list args;
            va_start(args, fmt);
            sprintf(b, fmt, args);
            Serial.print(b);
            va_end(args);
        }

        void DEBUG_PRINTF(unsigned id, const __FlashStringHelper *fmt...) {
            char b[128];
            va_list args;
            va_start(args, fmt);
            sprintf(b, String(fmt).c_str(), args);
            Serial.print(b);
            va_end(args);
        }
    #endif
#endif

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
