#include <Arduino.h>
#include "StatusLed.h"

StatusLed::StatusLed(uint8_t LED_PIN){
	_led_pin = LED_PIN;
	_led_state = 0;
	pinMode(_led_pin, OUTPUT);
	digitalWrite(_led_pin, _StatusLed_states[0]);
	_led_last_change_time = millis();
};

void StatusLed::update(){
	if ( (millis()>=(_led_last_change_time + _StatusLed_times[_led_state])) ) {
		_led_last_change_time = millis();
		_led_state++;
		if (_led_state >= _StatusLed_state_count) { _led_state = 0; };
		digitalWrite(_led_pin, _StatusLed_states[_led_state]);
	};
};