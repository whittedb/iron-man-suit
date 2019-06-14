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


constexpr auto EYE_PIN = 6;
constexpr auto SERVO_PIN = 5;
constexpr auto FACEPLATE_ACTIVATE_PIN = 2;
constexpr auto ARC_PIN = 4;
constexpr auto SFX_PLAYING_PIN = 3;
constexpr auto SFX_TX = 10;
constexpr auto SFX_RX = 11;
#define SFX_RST_PIN A0


Suit suit = Suit(FACEPLATE_ACTIVATE_PIN, SERVO_PIN, EYE_PIN, ARC_PIN, SFX_TX, SFX_RX, SFX_PLAYING_PIN, SFX_RST_PIN);


void setup() {
	DEBUG_BEGIN;
	DEBUG_PRINTLN(F("Starting system...."));
	suit.begin();
}

void loop() {
	suit.processState();
}
