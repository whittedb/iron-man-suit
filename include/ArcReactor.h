// ArcReactor.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_Neopixel.h>


constexpr auto ARC_REACTOR_LED_COUNT = 12;


class ArcReactor {
    public:
        ArcReactor(uint8_t driver_pin);

        void begin();
        void processState();
        void on();
        void off();

    private:
        Adafruit_NeoPixel ring;
};
