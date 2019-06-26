//
//
//
#pragma once
#include "SoundPlayer.h"
#include "FacePlate.h"
#include "ArcReactor.h"

class Suit {
    public:
        Suit(
            uint8_t power_toggle_pin,
            uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
            uint8_t eye_pin,
            uint8_t arc_pin,
            uint8_t sfx_tx_pin, uint8_t sfx_rx_pin, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin);

        void begin();
        void processState();
        bool isInAttackMode();
        bool isPoweredUp();

    private:
		enum State {
			S_OFF,
			S_IDLE,
            S_POWER_TOGGLE
		};

        uint8_t powerTogglePin;
        SoundPlayer sfx;
        FacePlate facePlate;
        ArcReactor arcReactor;
        State state = S_OFF;
        bool poweredUp = false;
        bool attackMode = false;
		static Suit *instance;

		void setState(State new_state);
        void setPoweredUp(bool powered_up);
		State getState();
		void debounceButton();
		static void debounceButtonMarshaller() { instance->debounceButton(); }
};
