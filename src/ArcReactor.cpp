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
			poweredUp = true;
			on();
			state = S_IDLE;
			break;

		case S_IDLE:
			break;
		
		case S_SHUTDOWN:
			poweredUp = false;
			off();
			state = S_OFF;
			break;
	}
}

void ArcReactor::startup() {
	poweredUp = true;
	on();
}

void ArcReactor::shutdown() {
	poweredUp = false;
	off();
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
