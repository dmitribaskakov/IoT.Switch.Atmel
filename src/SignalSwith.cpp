
#include <Arduino.h>
#include "SignalSwith.h"

SignalSwith::SignalSwith () {
	_isEnable = false;
	_hasChanged = false;
	_stateIsOff = LOW;
	_state = _stateIsOff;
	_timerMaxOn = 0;
	opposited = 0;
}
void SignalSwith::enable(uint32_t __timerMaxOn, uint8_t __opposited) {
	_isEnable = true;
	_timerMaxOn = __timerMaxOn;
	opposited = __opposited;
}
bool SignalSwith::processed() {
	_hasChanged = false;
	//_hasSended = false;
	if (_timerMaxOn != 0) {
		if ( isOn() ) {
			if ( millis() >= (_timerMaxOn + _timerLastChangeOn) ) {
				off();
			}
		}
	}
	return _hasChanged;
}
void SignalSwith::inverted() {
	_stateIsOff = HIGH;
	_state = _stateIsOff;
	_hasChanged = true;
}
bool SignalSwith::on() {
	bool __state;
	__state = _state;
	if (_isEnable) { __state = ! _stateIsOff; };
	if (__state != _state) {
		_state = __state;
		_hasChanged = true;
		if (_timerMaxOn != 0) {
			_timerLastChangeOn = millis();
		}
	};
	return _hasChanged;
}
bool SignalSwith::off() {
	bool __state;
	__state = _state;
	if (_isEnable) { __state = _stateIsOff; };
	if (__state != _state) {
		_state = __state;
		_timerLastChangeOn = 0;
		_hasChanged = true;
	};
	return _hasChanged;
}
bool SignalSwith::invert() {
	bool __state;
	__state = _state;
	if (_isEnable) { __state = ! _state; };
	if (__state != _state) {
		_state = __state;
		_hasChanged = true;
	};
	return _hasChanged;
}
bool SignalSwith::isOn() {
	if (_isEnable) {
		if (_stateIsOff == LOW) {return _state;} else {return !_state;};
	};
	return false;
}
bool SignalSwith::isOff() {	return !isOn(); }
