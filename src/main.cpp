/*
 Name:		main.cpp
 Created:	5/25/2019 16:39:14
 Author:	whittedb
*/

#include <Arduino.h>
#include <Wire.h>
#include <Queue.h>
#include <avr/pgmspace.h>
// Turn on serial output debug statements for all code
//#define MY_DEBUG
#include "Suit.h"
// Turn on serial output debug statements for this file only
//#define MY_DEBUG
#include "debug.h"


constexpr auto POWER_TOGGLE_PIN = 3;
constexpr auto EYE_PIN = 6;
constexpr auto SERVO_PIN = 9;
constexpr auto FACEPLATE_ACTIVATE_PIN = 2;
constexpr auto LED_RING_PIN = 4;
constexpr auto SFX_PLAYING_PIN = 7;
constexpr auto SFX_TX_PIN = 10;
constexpr auto SFX_RX_PIN = 11;
constexpr auto REPULSOR_LEFT_I2C_ADDRESS = 0x18;
constexpr auto REPULSOR_RIGHT_I2C_ADDRESS = 0x19;

#define SFX_RST_PIN A0


Suit suit = Suit(POWER_TOGGLE_PIN,
					FACEPLATE_ACTIVATE_PIN, SERVO_PIN, EYE_PIN, LED_RING_PIN,
					REPULSOR_LEFT_I2C_ADDRESS, REPULSOR_RIGHT_I2C_ADDRESS,
					SFX_TX_PIN, SFX_RX_PIN, SFX_PLAYING_PIN, SFX_RST_PIN);


void setup() {
	DEBUG_BEGIN;
	DEBUG_PRINTLN(F("Starting system"));
	suit.begin();
	DEBUG_PRINTLN2(F("FreeMem: "), freeMemory());
}

void loop() {
	suit.processState();
}
