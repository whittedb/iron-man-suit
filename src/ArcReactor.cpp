//
//
//

#include "Suit.h"
#include "ArcReactor.h"
//#define MY_DEBUG
#include "debug.h"

ArcReactor::ArcReactor(Suit &suit, uint8_t driver_pin) :
	suit(suit), ring(ARC_REACTOR_LED_COUNT, driver_pin, NEO_GRBW) {

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
			if (suit.isInAttackMode()) {
				attackOn();
			}
			else {
				normalOn();
			}
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

void ArcReactor::normalOn() {
	setRingColor(ring.Color(0, 0, 0, 100));
	ring.show();
}

void ArcReactor::attackOn() {
	setRingColor(ring.Color(0, 100, 0, 0));
	ring.show();
}

void ArcReactor::off() {
	setRingColor(ring.Color(0, 0, 0, 0));
	ring.show();
}

void ArcReactor::setRingColor(uint32_t color) {
	ring.fill(color, START_PIXEL, ARC_REACTOR_LED_COUNT);
/*
	for (int i = 0; i < ARC_REACTOR_LED_COUNT; ++i) {
		ring.setPixelColor(i, color);
	}
 */
}
