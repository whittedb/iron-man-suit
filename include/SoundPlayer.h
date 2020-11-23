// SoundPlayer.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <ArduinoQueue.h>

class SoundPlayer {
	public:
		SoundPlayer(uint8_t tx_pin, uint8_t rx_pin, uint8_t active_pin, uint8_t rst_pin);
		~SoundPlayer();

		void begin();
		void startup();
		void shutdown();
		bool setVolume(uint8_t volume);
		uint8_t getVolume() { return fxVolume; }
		bool isFxPlaying();
		bool playFx(const char *track, bool interrupt = false);
		void processState();

	private:
		enum State {
			S_OFF,
			S_IDLE,
			S_STARTUP,
			S_SHUTDOWN,
			S_PLAYING,
			S_PLAY_NEXT
		};

		State state = S_OFF;
		bool fxPlaying = false;
		uint8_t activePin;
		uint8_t rstPin;
		uint8_t fxVolume;		// We'll track the volume level in this variable.
		SoftwareSerial ss;
		Adafruit_Soundboard sfx;
		ArduinoQueue<const char *> fxQue;
		bool initialized = false;

		bool playTrackName(const char *track);
		void setFxPlaying(bool new_value);
		void queSong(const char *track);
		const char *dequeSong();
		bool songQueEmpty();
		void checkFxActive();
};
