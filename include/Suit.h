//
//
//
#pragma once
#include "FacePlate.h"
#include "ArcReactor.h"

class Suit {
    public:
        Suit(
            uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
            uint8_t eye_pin,
            uint8_t arc_pin,
            uint8_t sfx_tx_pin, uint8_t sfx_rx_pin, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin);

        void begin();
        void processState();

    private:
        SoundPlayer sfx;
        FacePlate facePlate;
        ArcReactor arcReactor;
};
