// 
// 
// 

#include <avr/pgmspace.h>
#include "Eyes.h"
//#define MY_DEBUG
#include "debug.h"


constexpr auto MIN_EYE_LEVEL = 0;
constexpr auto MAX_EYE_LEVEL = 255;
//speed of the eye 'fade'
constexpr auto FADE_DELAY = 20;
constexpr auto FADE_STEP = 2;
//length to wait to start eye flicker after face plate comes down
constexpr auto EYE_DELAY = 1500;
//delay between init blink on/off
constexpr auto BLINK_SPEED = 1000;
constexpr auto BLINKS = 4;


struct BlinkInfo {
	uint8_t	brightnessOn,
			brightnessOff;
	unsigned long 	timeOn,
					timeOff;
};

const BlinkInfo blinkInfo[BLINKS] = {
	{125, 0, 100, 50},
	{150, 0, 200, 50},
	{200, 0, 300, 200},
	{MAX_EYE_LEVEL, 0, 400, 100}
};


Eyes::Eyes(uint8_t pin) : pin(pin), currentEyePWM(MIN_EYE_LEVEL) {
}

Eyes::~Eyes() {}

void Eyes::begin() {
	DEBUG_PRINTLN(F("Initializing Eyes"));
	pinMode(pin, OUTPUT);
}

void Eyes::activate() {
	timer.start(EYE_DELAY);
	state = S_EYE_DELAY;
}

void Eyes::deactivate() {
	state = S_DEACTIVATE_EYES;
}

void Eyes::startup() {
	if (!poweredUp) {
		state = S_STARTUP;
	}
}

void Eyes::shutdown() {
	if (poweredUp) {
		state = S_SHUTDOWN;
	}
}

void Eyes::processState() {
	switch (state) {
		case S_OFF:
			break;

		case S_IDLE:
			break;

		case S_STARTUP:
			DEBUG_PRINTLN(F("Starting eye system"));
			state = S_IDLE;
			break;
			
		case S_EYE_DELAY:
			if (timer.expired()) {
				if (!poweredUp) {
					state = S_BLINK_EYES;
				}
				else {
					state = S_FADE_ON;
				}
			}
			break;

		case S_FADE_ON:
			currentEyePWM = currentEyePWM > MAX_EYE_LEVEL ? MAX_EYE_LEVEL : currentEyePWM;
			analogWrite(pin, currentEyePWM);
			state = S_WAIT_FADE_ON;
			timer.start(FADE_DELAY);
			break;

		case S_WAIT_FADE_ON:
			if (timer.expired()) {
				if (currentEyePWM < MAX_EYE_LEVEL) {
					currentEyePWM += FADE_STEP;
					state = S_FADE_ON;
				}
				else {
					state = S_IDLE;
				}
			}
			break;

		case S_FADE_OFF:
			currentEyePWM = currentEyePWM < MIN_EYE_LEVEL ? MIN_EYE_LEVEL : currentEyePWM;
			analogWrite(pin, currentEyePWM);
			state = S_WAIT_FADE_OFF;
			timer.start(FADE_DELAY);
			break;

		case S_WAIT_FADE_OFF:
			if (timer.expired()) {
				if (currentEyePWM > MIN_EYE_LEVEL) {
					currentEyePWM -= FADE_STEP;
					state = S_FADE_OFF;
				}
				else {
					DEBUG_PRINTLN(F("Eye dim done...."));
					analogWrite(pin, 0);
					if (systemShuttingDown) {
						systemShuttingDown = false;
						poweredUp = false;
						state = S_OFF;
					}
					else {
						state = S_IDLE;
					}
				}
			}
			break;

		case S_BLINK_EYES:
			blinkCnt = 0;
			state = S_BLINK_ON;
			poweredUp = true;
			break;

		case S_BLINK_ON:
			if (blinkCnt == BLINKS) {
				analogWrite(pin, MAX_EYE_LEVEL);
				currentEyePWM = MAX_EYE_LEVEL;
				state = S_IDLE;
			}
			else
			{
				state = S_WAIT_BLINK_ON;
				analogWrite(pin, blinkInfo[blinkCnt].brightnessOn);
				timer.start(blinkInfo[blinkCnt].timeOn);
			}
			break;

		case S_WAIT_BLINK_ON:
			if (timer.expired()) {
				state = S_BLINK_OFF;
			}
			break;

		case S_BLINK_OFF:
			state = S_WAIT_BLINK_OFF;
			analogWrite(pin, blinkInfo[blinkCnt].brightnessOff);
			timer.start(blinkInfo[blinkCnt].timeOff);
			break;

		case S_WAIT_BLINK_OFF:
			if (timer.expired()) {
				++blinkCnt;
				state = S_BLINK_ON;
			}
			break;

		case S_DEACTIVATE_EYES:
			state = S_FADE_OFF;
			break;

		case S_SHUTDOWN:
			DEBUG_PRINTLN(F("Shutting down eyes"));
			systemShuttingDown = true;
			state = S_DEACTIVATE_EYES;
			break;
	}
}

