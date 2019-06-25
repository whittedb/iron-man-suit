// SoundPlayer.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <Queue.h>

class SoundPlayer {
	public:
		SoundPlayer(uint8_t tx_pin, uint8_t rx_pin, uint8_t active_pin, uint8_t rst_pin);
		~SoundPlayer();

		void begin();
		bool setVolume(uint8_t volume);
		uint8_t getVolume() { return fxVolume; }
		bool isFxPlaying();
		void playFx(const char *track, bool interrupt = false, bool wait_for_start = false);
		void processState();

	private:
		enum State {
			S_IDLE,
			S_PLAYING,
			S_PLAY_NEXT
		};

		State state = S_IDLE;
		bool fxPlaying = false;
		uint8_t activePin;
		uint8_t rstPin;
		uint8_t fxVolume;		// We'll track the volume level in this variable.
		SoftwareSerial ss;
		Adafruit_Soundboard sfx;
		Queue<const char *> fxQue;

		void playTrackName(const char *track, bool wait_for_start = false);
		void setFxPlaying(bool new_value);
		void queSong(const char *track);
		const char *dequeSong();
		bool songQueEmpty();
		void checkFxActive();
};
