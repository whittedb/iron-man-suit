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


volatile SoundPlayer::State SoundPlayer::state = SoundPlayer::State::S_IDLE;
volatile bool SoundPlayer::fxPlaying = false;
uint8_t SoundPlayer::activePin;


SoundPlayer::SoundPlayer(uint8_t tx_pin, uint8_t rx_pin, uint8_t active_pin, uint8_t rst_pin) :
	rstPin(rst_pin), fxVolume(DEFAULT_VOLUME), ss(tx_pin, rx_pin), sfx(&ss, NULL, rst_pin) {

	activePin = active_pin;
}

SoundPlayer::~SoundPlayer() {}

void SoundPlayer::begin() {
	DEBUG_PRINTLN(F("Initializing sound system...."));
	ss.begin(FX_BOARD_SPEED);
	Wire.begin();

	pinMode(rstPin, INPUT);
	digitalWrite(rstPin, HIGH);
	pinMode(activePin, INPUT);

	if (!sfx.reset()) {
		DEBUG_PRINTLN(F("No SFX board found!!!"));
	}
	else {
		setVolume(fxVolume);

		attachInterrupt(digitalPinToInterrupt(activePin), handleFxActive, CHANGE);
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
	bool rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = fxPlaying;
	}
	return rv;
}

void SoundPlayer::playFx(const char* track, bool interrupt, bool wait_for_start) {
	if (isFxPlaying()) {
		if (interrupt) {
			sfx.stop();
			playTrackName(track, wait_for_start);
		}
		else {
			queSong(track);
		}
	}
	else {
		playTrackName(track, wait_for_start);
	}
}

void SoundPlayer::playTrackName(const char* track, bool wait_for_start) {
	bool success = sfx.playTrack((char*)track);
	if (success) {
		if (wait_for_start) {
			while (!isFxPlaying()){}
		}
	}
	else {
		DEBUG_PRINTLN2(F("Failed to play track: "), track);
	}
}

void SoundPlayer::processState() {
	switch (getState()) {
		case S_IDLE:
		case S_PLAYING:
			break;

		case S_PLAY_NEXT:
			if (!isFxPlaying()) {
				if (!songQueEmpty()) {
					playTrackName(dequeSong());
					setState(S_PLAYING);
				}
				else {
					setState(S_IDLE);
				}
			}
			break;

		default:
			break;
	}
}

void SoundPlayer::handleFxActive() {
	fxPlaying = digitalRead(activePin) == LOW;
	if (!fxPlaying) {
		state = S_PLAY_NEXT;
	}
	else {
		state = S_PLAYING;
	}
}

void SoundPlayer::setState(State new_state) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		state = new_state;
	}
}

SoundPlayer::State SoundPlayer::getState() {
	State rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = state;
	}
	return rv;
}

void SoundPlayer::setFxPlaying(bool new_value) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		fxPlaying = new_value;
	}
}

void SoundPlayer:: queSong(const char *track) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		fxQue.enqueue(track);
	}
}

const char *SoundPlayer::dequeSong() {
	const char *rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = fxQue.dequeue();
	}
	return rv;
}

bool SoundPlayer::songQueEmpty() {
	bool rv;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		rv = fxQue.isEmpty();
	}
	return rv;
}
