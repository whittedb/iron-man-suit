//
//
//

#include "Suit.h"
#include "ArcReactor.h"
//#define MY_DEBUG
#include "debug.h"

ArcReactor::ArcReactor(Suit &a_suit, uint8_t base_led, uint8_t led_cnt) :
	suit(a_suit), baseLed(base_led), ledCnt(led_cnt) {

}

void ArcReactor::begin() {
	DEBUG_PRINTLN(F("Initializing Arc Reactor...."));
}

void ArcReactor::startup() {
	if (state == S_OFF) {
		state = S_STARTUP;
	}
}

void ArcReactor::shutdown() {
	if (state != S_IDLE) {
		state = S_SHUTDOWN;
	}
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
			DEBUG_PRINTLN(F("Shutting down Arc Reactor"));
			off();
			firstTime = true;
			state = S_OFF;
			break;
	}
}

void ArcReactor::normalOn() {
	setRingColor(suit.getRing().Color(0, 0, 0, 100));
    suit.getRing().show();
}

void ArcReactor::attackOn() {
    setRingColor(suit.getRing().Color(100, 0, 0, 0));
    suit.getRing().show();
}

void ArcReactor::off() {
    setRingColor(suit.getRing().Color(0, 0, 0, 0));
    suit.getRing().show();
}

void ArcReactor::setRingColor(uint32_t color) {
    suit.getRing().fill(color, baseLed, ledCnt);
    /*
	for (int i = 0; i < ARC_REACTOR_LED_COUNT; ++i) {
		ring.setPixelColor(i, color);
	}
 */
}
