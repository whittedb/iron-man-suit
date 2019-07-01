//
//
//

#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "Suit.h"
//#define MY_DEBUG
#include "debug.h"

constexpr auto LED_RING_COUNT = 24;
constexpr auto ARC_REACTOR_START_PIXEL = 0;
constexpr auto ARC_REACTOR_LED_COUNT = 12;
constexpr auto REPULSOR_LEFT_START_PIXEL = 12;
constexpr auto REPULSOR_LEFT_LED_COUNT = 12;
constexpr auto REPULSOR_RIGHT_START_PIXEL = 24;
constexpr auto REPULSOR_RIGHT_LED_COUNT = 12;


Suit *Suit::instance = NULL;

Suit::Suit(
    uint8_t power_toggle_pin,
    uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
    uint8_t eye_pin,
    uint8_t led_ring_pin,
	uint8_t repulsor_left_i2c_address,
	uint8_t repulsor_right_i2c_address,
	uint8_t sfx_tx_pin, uint8_t sfx_rx_pin, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin) :
		powerTogglePin(power_toggle_pin),
		ring(LED_RING_COUNT, led_ring_pin, NEO_GRBW),
		sfx(sfx_tx_pin, sfx_rx_pin, sfx_playing_pin, sfx_rst_pin),
		facePlate(*this, faceplate_activate_pin, faceplate_servo_pin, eye_pin, sfx),
		arcReactor(*this, ARC_REACTOR_START_PIXEL, ARC_REACTOR_LED_COUNT),
		repulsorLeft(*this, REPULSOR_LEFT_START_PIXEL, REPULSOR_LEFT_LED_COUNT, repulsor_left_i2c_address),
		repulsorRight(*this, REPULSOR_RIGHT_START_PIXEL, REPULSOR_RIGHT_LED_COUNT, repulsor_right_i2c_address) {
    instance = this;
}

void Suit::begin() {
	DEBUG_PRINTLN(F("Initializing suit...."));
    ring.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
    ring.show();             // Turn OFF all pixels ASAP
    ring.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)
    
	sfx.begin();
    facePlate.begin();
	arcReactor.begin();
	repulsorLeft.begin();
	repulsorRight.begin();
	
	pinMode(powerTogglePin, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(powerTogglePin), debounceButtonMarshaller, FALLING);
}

void Suit::processState() {
	sfx.processState();
	facePlate.processState();
    arcReactor.processState();
	repulsorLeft.processState();
	repulsorRight.processState();

    switch(state) {
        case S_OFF:
            break;

        case S_IDLE:
            break;

		case S_POWER_TOGGLE:
            if (isPoweredUp()) {
                facePlate.shutdown();
                arcReactor.shutdown();
				repulsorRight.shutdown();
				repulsorLeft.shutdown();
				sfx.shutdown();
                setState(S_OFF);
				setPoweredUp(false);
            }
            else {
				sfx.startup();
                arcReactor.startup();
				repulsorRight.startup();
				repulsorLeft.startup();
                facePlate.startup();
                setState(S_IDLE);
				setPoweredUp(true);
            }
			break;

		case S_START_ATTACK_MODE:
            attackMode = true;
            arcReactor.attackOn();
            break;

		case S_END_ATTACK_MODE:
            attackMode = false;
            arcReactor.normalOn();
            break;
    }
}

void Suit::setAttackMode(bool attack_mode) {
	if (getState() == S_OFF || attack_mode == attackMode)
        return;

    if (attack_mode) {
        setState(S_START_ATTACK_MODE);
    } else
	{
        setState(S_END_ATTACK_MODE);
    }
	
}

bool Suit::isPoweredUp() {
	bool rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = poweredUp;
	}
	return rv;
}

void Suit::setPoweredUp(bool powered_up) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		poweredUp = powered_up;
	}
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

bool Suit::isInAttackMode() {
	return attackMode;
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
