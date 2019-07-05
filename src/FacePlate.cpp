// 
// 
//
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "FacePlate.h"
#include "Sounds.h"
// Turn on serial output debug statements for this file only
//#define MY_DEBUG
#include "debug.h"


constexpr auto FACE_POS_CLOSED = 180;
constexpr auto FACE_POS_OPEN = 20;
constexpr auto MIN_SRVO_PWM = 905;
constexpr auto MAX_SRVO_PWM = 2068;
constexpr auto SERVO_DELAY = 0;


FacePlate *FacePlate::instance = NULL;


FacePlate::FacePlate(Suit &suit, uint8_t activate_pin, uint8_t servo_pin, uint8_t eye_pin, SoundPlayer &sfx)
		: suit(suit), activatePin(activate_pin), servoPin(servo_pin), eyes(eye_pin), sfx(sfx) {
	instance = this;
}

FacePlate::~FacePlate() {
	servo.detach();
	detachInterrupt(activatePin);
}

void FacePlate::begin() {
	DEBUG_PRINTLN(F("Initializing faceplate"));

	eyes.begin();

	pinMode(activatePin, INPUT_PULLUP);

	servo.attach(servoPin, MIN_SRVO_PWM, MAX_SRVO_PWM);
	servo.write(FACE_POS_OPEN);

	attachInterrupt(digitalPinToInterrupt(activatePin), debounceButtonMarshaller, FALLING);
}

void FacePlate::startup() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (state == S_OFF) {
			setState(S_STARTUP);
		}
	}
}

void FacePlate::shutdown() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (state != S_OFF) {
			setState(S_SHUTDOWN);
		}
	}
}

void FacePlate::open() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (state == S_IDLE && !faceplateOpen) {
			setState(S_OPENING);
		}
	}
}

void FacePlate::close() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (state == S_IDLE && faceplateOpen) {
			setState(S_CLOSING);
		}
	}
}

bool FacePlate::isIdle() {
	bool rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = state == S_IDLE;
	}
	return rv;
}

void FacePlate::processState() {
	eyes.processState();

	switch(getState()) {
		case S_OFF:
			break;

		case S_IDLE:
			break;

		case S_STARTUP:
			DEBUG_PRINTLN(F("Starting faceplate system"));
			setState(S_OPENING);
			break;

		case S_FACEPLATE_REQUEST:
			DEBUG_PRINTLN(F("Faceplate Request"));
			if (isOpen()) {
				setState(S_CLOSING);
			}
			else {
				setState(S_OPENING);
			}
			break;

		case S_OPENING:
			DEBUG_PRINTLN(F("Opening helmet..."));
			setState(S_WAIT_FOR_OPEN);
			servo.write(FACE_POS_OPEN);
			timer.start(SERVO_DELAY);
			break;

		case S_CLOSING:
			DEBUG_PRINTLN(F("Closing helmet..."));
			setState(S_WAIT_FOR_CLOSE);
			servo.write(FACE_POS_CLOSED);
			timer.start(SERVO_DELAY);
			break;

		case S_WAIT_FOR_OPEN:
			if(timer.expired()) {
				DEBUG_PRINTLN(F("Helmet open..."));
				if (shuttingDown) {
					eyes.shutdown();
					shuttingDown = false;
					firstTime = true;
					setState(S_OFF);
				}
				else {
					eyes.deactivate();
					setState(S_IDLE);
				}
				faceplateOpen = true;
			}
			break;

		case S_WAIT_FOR_CLOSE:
			if(timer.expired()) {
				DEBUG_PRINTLN(F("Helmet closed..."));
				eyes.activate();
				sfx.playFx(SFX_HELMET_CLOSE_SND, true);
				setState(S_WAIT_FOR_CLOSE_CLANG);
				timer.start(1000);
			}
			break;

		case S_WAIT_FOR_CLOSE_CLANG:
			if (timer.expired()) {
				if (firstTime) {
					sfx.playFx(SFX_SUIT_POWER_UP_SND);
					firstTime = false;
				}
				faceplateOpen = false;
				setState(S_IDLE);
			}
			break;

		case S_SHUTDOWN:
			DEBUG_PRINTLN(F("Shutting down faceplate"));
			shuttingDown = true;
			setState(S_OPENING);
			break;
	}
}

void FacePlate::setState(State new_state) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		state = new_state;
	}
}

FacePlate::State FacePlate::getState() {
	State rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = state;
	}
	return rv;
}

void FacePlate::debounceButton() {
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	if (interrupt_time - last_interrupt_time > 200) {
		if (state == S_IDLE) {
			state = S_FACEPLATE_REQUEST;
		}
	}
	last_interrupt_time = interrupt_time;
}
