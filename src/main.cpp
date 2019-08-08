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
#include "SfxUart.h"


constexpr auto POWER_TOGGLE_PIN = 10;
constexpr auto EYE_PIN = 8;
constexpr auto SERVO_PIN = 5;
constexpr auto FACEPLATE_ACTIVATE_PIN = 9;
constexpr auto LED_RING_PIN = 6;
constexpr auto SFX_PLAYING_PIN = A5;
constexpr auto SFX_TX_PIN = 13ul;
constexpr auto SFX_RX_PIN = 12ul;
constexpr auto SFX_RST_PIN = 11ul;
constexpr auto REPULSOR_LEFT_I2C_ADDRESS = 0x18;
constexpr auto REPULSOR_RIGHT_I2C_ADDRESS = 0x19;

SfxUart serialSfx(&sercom1, SFX_RX_PIN, SFX_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM1_0_Handler()
{
  serialSfx.IrqHandler();
}
void SERCOM1_1_Handler()
{
  serialSfx.IrqHandler();
}
void SERCOM1_2_Handler()
{
  serialSfx.IrqHandler();
}
void SERCOM1_3_Handler()
{
  serialSfx.IrqHandler();
}

Suit suit = Suit(POWER_TOGGLE_PIN,
					FACEPLATE_ACTIVATE_PIN, SERVO_PIN, EYE_PIN, LED_RING_PIN,
					REPULSOR_LEFT_I2C_ADDRESS, REPULSOR_RIGHT_I2C_ADDRESS,
					serialSfx, SFX_PLAYING_PIN, SFX_RST_PIN);


void setup() {
	DEBUG_BEGIN;
	DEBUG_WAIT_FOR_INIT;
	DEBUG_PRINTLN("Starting system");
	suit.begin();
	DEBUG_PRINTF("FreeMem: %u\r\n", freeMemory());
}

void loop() {
	suit.processState();
}
