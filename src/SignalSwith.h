#ifndef SIGNALSWITH_H_
#define SIGNALSWITH_H_

#include <Arduino.h>

class SignalSwith {
	public:
	SignalSwith ();
	void enable(uint32_t __timerMaxOn = 0, uint8_t __opposited = 0);
	void disable() { _isEnable = false; };
	void inverted();
	bool processed();
	bool on();
	bool off();
	bool invert();
	bool isOn();
	bool isOff();
	bool isEnable() {return _isEnable;};
	bool hasChanged() {return _hasChanged;};
	bool setChanged() {_hasChanged = true; return _hasChanged;};
	//bool hasSended() {return _hasSended;};
	bool SendState() { _hasChanged = false; return _state;};
	uint8_t opposited;             //Противоположный, оба не могут быть одновременно включены
	private:
	bool _isEnable;
	bool _stateIsOff;
	bool _state;
	bool _hasChanged;
	uint32_t _timerMaxOn;           //Максимальное время в включенном состоянии если 0 то не используется
	uint32_t _timerLastChangeOn;    //Время последнего изменения состояния
	//	bool _hasSended;
};

#endif /* SIGNALSWITH_H_ */
