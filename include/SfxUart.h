#pragma once

#include <Arduino.h>
#include <Uart.h>
#include <wiring_private.h>

class SfxUart : public Uart {
public:
    SfxUart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, bool sercom_alt=false) :
        Uart(_s, _pinRX, _pinTX, _padRX, _padTX), tx(_pinTX), rx(_pinRX), txPad(_pinTX), rxPad(_padRX), sercomAlt(sercom_alt)
    {
    }
    
    void begin(unsigned long baudrate) {
        Uart::begin(baudrate);
    }

    void begin(unsigned long baudrate, uint16_t config) {
        Uart::begin(baudrate, config);
        EPioType pio = sercomAlt ? PIO_SERCOM_ALT : PIO_SERCOM;
        pinPeripheral(tx, pio);
        pinPeripheral(rx, pio);
    }

private:
    uint8_t tx, rx, txPad, rxPad;
    bool sercomAlt;
};
