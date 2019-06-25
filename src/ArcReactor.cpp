//
//
//

#include "ArcReactor.h"
//#define MY_DEBUG
#include "debug.h"

ArcReactor::ArcReactor(uint8_t driver_pin) : ring(ARC_REACTOR_LED_COUNT, driver_pin, NEO_RBGW) {

}

void ArcReactor::begin() {
	DEBUG_PRINTLN(F("Initializing Arc Reactor...."));
    ring.begin();               // INITIALIZE NeoPixel strip object (REQUIRED)
	ring.show();                // Turn OFF all pixels ASAP
	ring.setBrightness(200);    // Set BRIGHTNESS to about 1/5 (max = 255)
}

void ArcReactor::processState() {
	switch(state) {
		case S_OFF:
			break;

		case S_STARTUP:
			DEBUG_PRINTLN(F("Starting Arc Reactor"));
			on();
			firstTime = false;
			state = S_IDLE;
			break;

		case S_IDLE:
			break;
		
		case S_SHUTDOWN:
			off();
			firstTime = true;
			state = S_OFF;
			break;
	}
}

void ArcReactor::startup() {
	if (state == S_OFF) {
		state = S_STARTUP;
	}
}

void ArcReactor::shutdown() {
	if (state == S_IDLE) {
		state = S_SHUTDOWN;
	}
}

void ArcReactor::on() {
	uint32_t color = ring.Color(0, 0, 0, 100);

	for (int i = 0; i < ARC_REACTOR_LED_COUNT; ++i) {
		ring.setPixelColor(i, color);
	}
	ring.show();
}

void ArcReactor::off() {
	uint32_t color = ring.Color(0, 0, 0, 0);

	for (int i = 0; i < ARC_REACTOR_LED_COUNT; ++i) {
		ring.setPixelColor(i, color);
	}
	ring.show();
}
