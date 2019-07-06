// FacePlate.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_TiCoServo.h>
#include "MyTimer.h"
#include "SoundPlayer.h"
#include "Eyes.h"


class Suit;

class FacePlate {
	public:
		FacePlate(Suit &suit, uint8_t activate_pin, uint8_t servo_pin, uint8_t eye_pin, SoundPlayer &sfx);
		~FacePlate();

		void begin();
		void open();
		void close();
		bool isOpen() { return faceplateOpen;  }
		bool isIdle();
		void processState();
		void startup();
		void shutdown();

	private:
		enum State {
			S_OFF,
			S_STARTUP,
			S_IDLE,
			S_FACEPLATE_REQUEST,
			S_OPENING,
			S_WAIT_FOR_OPEN,
			S_CLOSING,
			S_WAIT_FOR_CLOSE,
			S_WAIT_FOR_CLOSE_CLANG,
			S_SHUTDOWN
		};

		void setState(State new_state);
		State getState();
		void debounceButton();
		static void debounceButtonMarshaller() { instance->debounceButton(); }

		Suit &suit;
		uint8_t activatePin;
		uint8_t servoPin;
		Eyes eyes;
		SoundPlayer &sfx;
		Adafruit_TiCoServo servo;
		bool faceplateOpen = true;
		bool firstTime = true;
		bool shuttingDown = false;
		MyTimer timer;
		State state = S_OFF;

		static FacePlate *instance;
};
