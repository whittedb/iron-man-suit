// ArcReactor.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_Neopixel.h>


constexpr auto START_PIXEL = 0;
constexpr auto ARC_REACTOR_LED_COUNT = 12;

class Suit;

class ArcReactor {
    public:
        ArcReactor(Suit &suit, uint8_t driver_pin);

        void begin();
        void processState();
        void startup();
        void shutdown();
        void normalOn();
        void attackOn();
        void off();

    private:
		enum State {
			S_OFF,
			S_STARTUP,
			S_IDLE,
			S_SHUTDOWN
		};

        Suit &suit;
        State state = S_OFF;
        bool firstTime = true;
        Adafruit_NeoPixel ring;

        void setRingColor(uint32_t color);
};
