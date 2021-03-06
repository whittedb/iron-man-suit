// 
// 
// 

#include <Wire.h>
#include <Adafruit_Soundboard.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "SoundPlayer.h"
//#define MY_DEBUG
#include "debug.h"


// 0x4B is the default i2c address of the audio amp
constexpr auto MAX9744_I2CADDR = 0x4B;
constexpr auto DEFAULT_VOLUME = 25;
constexpr auto FX_BOARD_SPEED = 9600;


SoundPlayer::SoundPlayer(uint8_t tx_pin, uint8_t rx_pin, uint8_t active_pin, uint8_t rst_pin) :
	activePin(active_pin), rstPin(rst_pin),
	fxVolume(DEFAULT_VOLUME),
	ss(tx_pin, rx_pin),
	sfx(&ss, NULL, rst_pin) {
}

SoundPlayer::~SoundPlayer() {}

void SoundPlayer::begin() {
	DEBUG_PRINTLN(F("Initializing sound system"));
	ss.begin(FX_BOARD_SPEED);
	Wire.begin();

	pinMode(rstPin, INPUT);
	digitalWrite(rstPin, HIGH);
	pinMode(activePin, INPUT);

	if (!sfx.reset()) {
		DEBUG_PRINTLN(F("No SFX board found!!!"));
		return;
	}
	else {
		initialized = true;
		setVolume(fxVolume);
	}
}

void SoundPlayer::startup() {
	if (state == S_OFF) {
		DEBUG_PRINTLN(F("Starting sound system"));
		state = S_STARTUP;
	}
}

void SoundPlayer::shutdown() {
	if (state != S_OFF) {
		DEBUG_PRINTLN(F("Shutting down sound system"));
		state = S_SHUTDOWN;
	}
}

bool SoundPlayer::setVolume(uint8_t v) {
	// cant be higher than 63 or lower than 0
	if (v > 63) v = 63;
	if (v < 0) v = 0;

	DEBUG_PRINTLN2(F("Vol: "), v);

	Wire.beginTransmission(MAX9744_I2CADDR);
	Wire.write(v);
	if (Wire.endTransmission() == 0) {
		fxVolume = v;
		return true;
	}
	else
		return false;
}

bool SoundPlayer::isFxPlaying() {
	return fxPlaying;
}

bool SoundPlayer::playFx(const char* track, bool interrupt) {
	bool rv = false;
	if (isFxPlaying()) {
		if (interrupt) {
			if (initialized) {
				sfx.stop();
			}
			rv = playTrackName(track);
		}
		else {
			queSong(track);
			rv = true;
		}
	}
	else {
		rv = playTrackName(track);
	}

	return rv;
}

bool SoundPlayer::playTrackName(const char* track) {
	if (initialized) {
		bool success = sfx.playTrack((char*)track);
		if (!success) {
			DEBUG_PRINTLN2(F("Failed to play track: "), track);
		}

		return success;
	} else
	{
		return false;
	}
	
}

void SoundPlayer::processState() {
	checkFxActive();

	switch (state) {
		case S_OFF:
		case S_IDLE:
			break;

		case S_STARTUP:
			state = S_IDLE;
			break;

		case S_SHUTDOWN:
			// Empty the queue and stop any playing
			while (!fxQue.isEmpty()) {
				fxQue.dequeue();
			}
			if (isFxPlaying()) {
				if (initialized) {
					sfx.stop();
				}
			}
			state = S_OFF;
			break;

		case S_PLAYING:
			break;

		case S_PLAY_NEXT:
			if (!isFxPlaying()) {
				if (!songQueEmpty()) {
					playTrackName(dequeSong());
					state = S_PLAYING;
				}
				else {
					state = S_IDLE;
				}
			}
			break;

		default:
			break;
	}
}

void SoundPlayer:: queSong(const char *track) {
	fxQue.enqueue(track);
}

const char *SoundPlayer::dequeSong() {
	return fxQue.dequeue();
}

bool SoundPlayer::songQueEmpty() {
	return fxQue.isEmpty();
}

void SoundPlayer::checkFxActive() {
	// If playing state has changed then update the state variable
	if (!(state == S_OFF || state == S_SHUTDOWN || state == S_STARTUP)) {
		bool pin_state = digitalRead(activePin) == LOW;
		if (pin_state != fxPlaying) {
			fxPlaying = pin_state;
			if (!fxPlaying) {
				state = S_PLAY_NEXT;
			}
			else {
				state = S_PLAYING;
			}
		}
	}
}
