// FacePlate.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>
#include "MyTimer.h"
#include "SoundPlayer.h"
#include "Eyes.h"


class FacePlate {
	public:
		FacePlate(uint8_t activate_pin, uint8_t servo_pin, uint8_t eye_pin, SoundPlayer &sfx);
		~FacePlate();

		void begin();
		void open();
		void close();
		bool isOpen() { return faceplateOpen;  }
		bool isIdle();
		void processState();

		enum State {
			S_IDLE,
			S_FACEPLATE_REQUEST,
			S_OPENING,
			S_WAIT_FOR_OPEN,
			S_CLOSING,
			S_WAIT_FOR_CLOSE,
			S_WAIT_FOR_CLOSE_CLANG
		};

	private:
		void setState(State new_state);
		State getState();

		uint8_t activatePin;
		uint8_t servoPin;
		Eyes eyes;
		SoundPlayer &sfx;
		Servo servo;
		bool faceplateOpen = true;
		bool poweredUp = false;
		MyTimer timer;
		static State state;

		static void debounceButton();
};
