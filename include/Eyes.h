// Eyes.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "MyTimer.h"


class Eyes {
	public:
		Eyes(uint8_t pin);
		~Eyes();

		void begin();
		void activate();
		void deactivate();
		void startup();
		void shutdown();
		void processState();
		bool isIdle() { return state == S_IDLE; }

	private:
		enum State {
			S_OFF,
			S_STARTUP,
			S_IDLE,
			S_BLINK_EYES,
			S_BLINK_ON,
			S_WAIT_BLINK_ON,
			S_BLINK_OFF,
			S_WAIT_BLINK_OFF,
			S_FADE_ON,
			S_WAIT_FADE_ON,
			S_FADE_OFF,
			S_WAIT_FADE_OFF,
			S_EYE_DELAY,
			S_DEACTIVATE_EYES,
			S_SHUTDOWN
		};

		uint8_t pin;
		State state = S_IDLE;
		bool firstTime = true;
		long currentEyePWM;
		MyTimer timer = MyTimer();
		bool systemShuttingDown = false;
		uint8_t blinkCnt;
};
