#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include "MyTimer.h"
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Neopixel.h>


class Suit;


class Repulsor
{
    public:
        Repulsor(Suit &suit, uint8_t base_led, uint8_t led_count, uint8_t accel_i2c_address);
        ~Repulsor();

        void begin();
        void startup();
        void shutdown();
        void processState();

    private:
        enum State
        {
            S_OFF,
            S_IDLE,
            S_START_FIRING,
            S_CHARGE,
            S_CHARGE_FADE,
            S_FIRE,
            S_FIRE_WAIT,
            S_COOLDOWN,
            S_SHUTDOWN
        };

        Suit &suit;
        uint8_t baseLed;
        uint8_t ledCnt;
        uint8_t i2cAddress;
        Adafruit_LIS3DH lis;

        State state = S_OFF;
        MyTimer timer;
        uint32_t pixelColor;

        void setColor(uint32_t color);
};
