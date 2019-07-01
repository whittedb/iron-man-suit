// ArcReactor.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_Neopixel.h>


class Suit;

class ArcReactor {
    public:
        ArcReactor(Suit &suit, uint8_t base_led, uint8_t led_count);

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
        uint8_t baseLed;
        uint8_t ledCnt;
        State state = S_OFF;
        bool firstTime = true;

        void setRingColor(uint32_t color);
};
