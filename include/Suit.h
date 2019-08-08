//
//
//
#pragma once

#include <Uart.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "ArcReactor.h"
#include "FacePlate.h"
#include "SoundPlayer.h"
#include "Repulsor.h"

class Suit {
    public:
        Suit(
            uint8_t power_toggle_pin,
            uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
            uint8_t eye_pin,
            uint8_t led_ring_pin,
            uint8_t repulsor_left_i2c_address,
            uint8_t repulsor_right_i2c_address,
            SfxUart &sfx_serial, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin);

        void begin();
        void processState();
        void setAttackMode(bool attack_mode);
        bool isInAttackMode();
        bool isPoweredUp();
        Adafruit_NeoPixel_ZeroDMA getRing() { return ring; }
        SoundPlayer getSoundPlayer() { return sfx; }

    private:
        enum State {
            S_OFF,
            S_IDLE,
            S_START_ATTACK_MODE,
            S_END_ATTACK_MODE,
            S_POWER_TOGGLE
        };

        uint8_t powerTogglePin;
        Adafruit_NeoPixel_ZeroDMA ring;
        SoundPlayer sfx;
        FacePlate facePlate;
        ArcReactor arcReactor;
        Repulsor repulsorLeft;
        Repulsor repulsorRight;
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
