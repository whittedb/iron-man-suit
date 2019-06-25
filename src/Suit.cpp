//
//
//

#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "Suit.h"
//#define MY_DEBUG
#include "debug.h"


Suit *Suit::instance = NULL;


Suit::Suit(
    uint8_t power_toggle_pin,
    uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
    uint8_t eye_pin,
    uint8_t arc_pin,
    uint8_t sfx_tx_pin, uint8_t sfx_rx_pin, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin) :
        powerTogglePin(power_toggle_pin),
        sfx(sfx_tx_pin, sfx_rx_pin, sfx_playing_pin, sfx_rst_pin),
        facePlate(*this, faceplate_activate_pin, faceplate_servo_pin, eye_pin, sfx),
        arcReactor(arc_pin) {
    instance = this;
}

void Suit::begin() {
	DEBUG_PRINTLN(F("Initializing suit...."));
	sfx.begin();
	facePlate.begin();
	arcReactor.begin();
	pinMode(powerTogglePin, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(powerTogglePin), debounceButtonMarshaller, FALLING);
}

void Suit::processState() {
	sfx.processState();
	facePlate.processState();
    arcReactor.processState();

    switch(state) {
        case S_OFF:
            break;

        case S_IDLE:
            break;

		case S_POWER_TOGGLE:
            if (isPoweredUp()) {
                facePlate.shutdown();
                arcReactor.shutdown();
				sfx.shutdown();
                setState(S_OFF);
            }
            else {
				sfx.startup();
                arcReactor.startup();
                facePlate.startup();
                setState(S_IDLE);
            }
			break;
    }
}

bool Suit::isPoweredUp() {
	bool rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = state != S_OFF;
	}
	return rv;
}

void Suit::setState(State new_state) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		state = new_state;
	}
}

Suit::State Suit::getState() {
	State rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = state;
	}
	return rv;
}

void Suit::debounceButton() {
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	if (interrupt_time - last_interrupt_time > 200) {
        state = S_POWER_TOGGLE;
	}
	last_interrupt_time = interrupt_time;
}
