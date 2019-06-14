//
//
//

#include <avr/pgmspace.h>
#include "Suit.h"
//#define MY_DEBUG
#include "debug.h"


Suit::Suit(uint8_t faceplate_activate_pin, uint8_t faceplate_servo_pin,
    uint8_t eye_pin, uint8_t arc_pin,
    uint8_t sfx_tx_pin, uint8_t sfx_rx_pin, uint8_t sfx_playing_pin, uint8_t sfx_rst_pin) :
        sfx(sfx_tx_pin, sfx_rx_pin, sfx_playing_pin, sfx_rst_pin),
        facePlate(faceplate_activate_pin, faceplate_servo_pin, eye_pin, sfx),
        arcReactor(arc_pin) {

}

void Suit::begin() {
	DEBUG_PRINTLN(F("Initializing suit...."));
	sfx.begin();
	facePlate.begin();
	arcReactor.begin();
}

void Suit::processState() {
	sfx.processState();
	facePlate.processState();
    arcReactor.processState();
}
