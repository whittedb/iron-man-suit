#include "Repulsor.h"
#include "Suit.h"
#include "MyTimer.h"
#include "Sounds.h"
#include "debug.h"


constexpr auto CHARGE_FADE_DELAY = 20;
constexpr auto FIRE_TIME = 3000;
constexpr auto COOLDOWN_TIME = 5000;

Repulsor::Repulsor(Suit &suit, uint8_t base_led, uint8_t led_count, uint8_t accel_i2c_address) :
    suit(suit),
    baseLed(base_led), ledCnt(led_count),
    i2cAddress(accel_i2c_address) {
}

Repulsor::~Repulsor() {
}

void Repulsor::begin() {
	DEBUG_PRINTLN("Initializing repulsor");
    if (!lis.begin(i2cAddress)) {
        DEBUG_PRINTLN("Failed to start accellerometer");
        return;
    } else
    {
        initialized = true;
    }
    

//    lis.setRange(LIS3DH_RANGE_4_G);
}

void Repulsor::startup() {
    if (state != S_OFF) {
        DEBUG_PRINTLN("Starting repulsor");
        pixelColor = 0;
        setColor(suit.getRing().Color(0, 0, 0, pixelColor));
        state = S_IDLE;
    }
}

void Repulsor::shutdown() {
    if (state != S_OFF) {
        state = S_SHUTDOWN;
    }
}

void Repulsor::processState() {
    switch (state) {
        case S_OFF:
            break;
        
        case S_IDLE: {
            if (initialized) {
                sensors_event_t event;
                lis.getEvent(&event);
                DEBUG_PRINTF("Pitch: %d\r\n", event.orientation.pitch);
                DEBUG_PRINTF("Roll: %d\r\n", event.orientation.roll);
                DEBUG_PRINTF("Heading: %d\r\n", event.orientation.heading);
                DEBUG_PRINTLN("");
                if (event.orientation.pitch > 70.0) {
                    state = S_FIRE;
                }
            }
            break;
        }

        case S_START_FIRING:
            suit.setAttackMode(true);
            state = S_CHARGE;
            break;

        case S_CHARGE:
            pixelColor = 0;
            setColor(suit.getRing().Color(0, 0, 0, pixelColor));
            state = S_CHARGE_FADE;
            timer.start(CHARGE_FADE_DELAY);
            break;

        case S_CHARGE_FADE:
            if (timer.expired()) {
                pixelColor += 10;
                if (pixelColor > 200) {
                    pixelColor = 200;
                }
                setColor(suit.getRing().Color(0, 0, 0, pixelColor));
                if (pixelColor == 200) {
                    state = S_FIRE;
                } else
                {
                    timer.start(CHARGE_FADE_DELAY);
                }
            }
            break;

        case S_FIRE:
            pixelColor = 255;
            setColor(suit.getRing().Color(0, 0, 0, pixelColor));
            suit.getSoundPlayer().playFx(SFX_REPULSOR, true);
            state = S_FIRE_WAIT;
            timer.start(FIRE_TIME);
            break;

        case S_FIRE_WAIT:
            if (timer.expired()) {
                pixelColor = 0;
                setColor(suit.getRing().Color(0, 0, 0, pixelColor));
                state = S_COOLDOWN;
                timer.start(FIRE_TIME);
                break;
            }
            break;

        case S_COOLDOWN: {
            if (initialized) {
                sensors_event_t event;
                lis.getEvent(&event);
                DEBUG_PRINTF("Pitch: %d\r\n", event.orientation.pitch);
                DEBUG_PRINTF("Roll: %d\r\n", event.orientation.roll);
                DEBUG_PRINTF("Heading: %d\r\n", event.orientation.heading);
                DEBUG_PRINTLN("");
                if (event.orientation.pitch < 0.0 && timer.expired()) {
                    suit.setAttackMode(false);
                    state = S_IDLE;
                }
            }
            break;
        }

        case S_SHUTDOWN:
            DEBUG_PRINTLN("Shutting down repulsor");
            pixelColor = 0;
            setColor(suit.getRing().Color(0, 0, 0, pixelColor));
            state = S_OFF;
            break;
    }
}

void Repulsor::setColor(uint32_t color) {
    suit.getRing().fill(color, baseLed, ledCnt);
    /*
	for (int i = 0; i < ARC_REACTOR_LED_COUNT; ++i) {
		ring.setPixelColor(i, color);
	}
 */
}
