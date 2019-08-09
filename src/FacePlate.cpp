// 
// 
//
#include "FacePlate.h"
#include "Sounds.h"
// Turn on serial output debug statements for this file only
//#define MY_DEBUG
#include "debug.h"


constexpr auto FACE_POS_CLOSED = 170;
constexpr auto FACE_POS_OPEN = 70;
constexpr auto MIN_SRVO_PWM = 905;
constexpr auto MAX_SRVO_PWM = 2068;
constexpr auto SERVO_DELAY = 0;


FacePlate *FacePlate::instance = NULL;


FacePlate::FacePlate(Suit &suit, uint8_t activate_pin, uint8_t servo_pin, uint8_t eye_pin, SoundPlayer &sfx)
		: suit(suit), activatePin(activate_pin), servoPin(servo_pin), eyes(eye_pin), sfx(sfx) {
	instance = this;
}

FacePlate::~FacePlate() {
	servo.detach();
	detachInterrupt(activatePin);
}

void FacePlate::begin() {
	DEBUG_PRINTLN("Initializing faceplate");

	eyes.begin();

	pinMode(activatePin, INPUT_PULLUP);

	servo.attach(servoPin, MIN_SRVO_PWM, MAX_SRVO_PWM);
	servo.write(FACE_POS_OPEN);

	attachInterrupt(digitalPinToInterrupt(activatePin), debounceButtonMarshaller, FALLING);
}

void FacePlate::startup() {
	__disable_irq();
	if (state == S_OFF) {
		state = S_STARTUP;
	}
	__enable_irq();
}

void FacePlate::shutdown() {
	__disable_irq();
	if (state != S_OFF) {
		state = S_SHUTDOWN;
	}
	__enable_irq();
}

void FacePlate::open() {
	__disable_irq();
	if (state == S_IDLE && !faceplateOpen) {
		state = S_OPENING;
	}
	__enable_irq();
}

void FacePlate::close() {
	__disable_irq();
	if (state == S_IDLE && faceplateOpen) {
		state = S_CLOSING;
	}
	__enable_irq();
}

bool FacePlate::isIdle() {
	return getStateAtomic() == S_IDLE;
}

void FacePlate::processState() {
	eyes.processState();

	switch(getStateAtomic()) {
		case S_OFF:
			break;

		case S_IDLE:
			break;

		case S_STARTUP:
			DEBUG_PRINTLN("Starting faceplate system");
			setStateAtomic(S_OPENING);
			eyes.startup();
			break;

		case S_FACEPLATE_REQUEST:
			DEBUG_PRINTLN("Faceplate Request");
			if (isOpen()) {
				setStateAtomic(S_CLOSING);
			}
			else {
				setStateAtomic(S_OPENING);
			}
			break;

		case S_OPENING:
			DEBUG_PRINTLN("Opening helmet");
			setStateAtomic(S_WAIT_FOR_OPEN);
			servo.write(FACE_POS_OPEN);
			timer.start(SERVO_DELAY);
			break;

		case S_CLOSING:
			DEBUG_PRINTLN("Closing helmet");
			setStateAtomic(S_WAIT_FOR_CLOSE);
			servo.write(FACE_POS_CLOSED);
			timer.start(SERVO_DELAY);
			break;

		case S_WAIT_FOR_OPEN:
			if(timer.expired()) {
				DEBUG_PRINTLN("Helmet open");
				if (shuttingDown) {
					eyes.shutdown();
					shuttingDown = false;
					firstTime = true;
					setStateAtomic(S_OFF);
				}
				else {
					eyes.deactivate();
					setStateAtomic(S_IDLE);
				}
				faceplateOpen = true;
			}
			break;

		case S_WAIT_FOR_CLOSE:
			if(timer.expired()) {
				DEBUG_PRINTLN("Helmet closed");
				eyes.activate();
				sfx.playFx(SFX_HELMET_CLOSE_SND, true);
				setStateAtomic(S_WAIT_FOR_CLOSE_CLANG);
				timer.start(1000);
			}
			break;

		case S_WAIT_FOR_CLOSE_CLANG:
			if (timer.expired()) {
				if (firstTime) {
					sfx.playFx(SFX_SUIT_POWER_UP_SND);
					firstTime = false;
				}
				faceplateOpen = false;
				setStateAtomic(S_IDLE);
			}
			break;

		case S_SHUTDOWN:
			DEBUG_PRINTLN("Shutting down faceplate");
			shuttingDown = true;
			setStateAtomic(S_OPENING);
			break;
	}
}

void FacePlate::setStateAtomic(State new_state) {
	__disable_irq();
	state = new_state;
	__enable_irq();
}

FacePlate::State FacePlate::getStateAtomic() {
	__disable_irq();
	State v = state;
	__enable_irq();
	return v;
}

void FacePlate::debounceButton() {
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	__disable_irq();
	if (interrupt_time - last_interrupt_time > 200) {
		if (state == S_IDLE) {
			state = S_FACEPLATE_REQUEST;
		}
	}
	last_interrupt_time = interrupt_time;
	__enable_irq();
}
