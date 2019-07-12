/*
 Name:		main.cpp
 Created:	5/25/2019 16:39:14
 Author:	whittedb
*/

#include <Arduino.h>
#include <wiring_private.h>
#include <Wire.h>
#include <Queue.h>
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

Uart SerialSfx(&sercom1, SFX_RX_PIN, SFX_TX_PIN, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() {
	SerialSfx.IrqHandler();
}

Suit suit = Suit(POWER_TOGGLE_PIN,
					FACEPLATE_ACTIVATE_PIN, SERVO_PIN, EYE_PIN, LED_RING_PIN,
					REPULSOR_LEFT_I2C_ADDRESS, REPULSOR_RIGHT_I2C_ADDRESS,
					SerialSfx, SFX_PLAYING_PIN, SFX_RST_PIN);


void setup() {
	DEBUG_BEGIN;
	DEBUG_PRINTLN(F("Starting system"));
	pinPeripheral(10, PIO_SERCOM);
	pinPeripheral(11, PIO_SERCOM);
	suit.begin();
	DEBUG_PRINTLN2(F("FreeMem: "), freeMemory());
}

void loop() {
	suit.processState();
}
