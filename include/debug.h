#ifndef DEBUG_SERIAL_SPEED
#define DEBUG_SERIAL_SPEED 9600
#endif

#ifndef MY_DEBUG
#define DEBUG_BEGIN
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT2(x,y)
#define DEBUG_PRINTLN2(x,y)

#else
#define DEBUG_BEGIN Serial.begin(DEBUG_SERIAL_SPEED)
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT2(x,y) Serial.print(x); Serial.print(y)
#define DEBUG_PRINTLN2(x,y) Serial.print(x); Serial.println(y)
#endif

int freeMemory();
