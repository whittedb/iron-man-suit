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
        void startup();
        void shutdown();
        void on();
        void off();

    private:
		enum State {
			S_OFF,
			S_STARTUP,
			S_IDLE,
			S_SHUTDOWN
		};

        State state = S_OFF;
        bool firstTime = true;
        Adafruit_NeoPixel ring;
};
