#ifndef SENDERBAOARD_H_
#define SENDERBAOARD_H_

#include <Arduino.h>
#include "SignalButton.h"
#include "SignalSwith.h"
#include "History.h"
using namespace std;

#define _BoardSender_Count 4
#define _BoardSender_PinsOnBoard 32
#define _BoardSender_Pins _BoardSender_Count*_BoardSender_PinsOnBoard
#define _BoardSender_Pin_SCK 41
#define _BoardSender_Pin_SS 40
const uint8_t _BoardSender_Pin_DATA[_BoardSender_Count] = {37, 39, 36, 38};

class BoardSender {
	public:
	BoardSender(){};
	void initialize(History *_history, uint8_t _historyCategory);
	void clear();
	void send();
	uint8_t pins() {return _BoardSender_Pins;};
	void enable(uint8_t pin, uint32_t __timerMaxOn = 0, uint8_t __opposited = 0);
	void disable(uint8_t pin) { if (pin < _BoardSender_Pins) {swith[pin].disable();} /* else {addToHistory(pin, History_State_Error);}*/; };
	void inverted(uint8_t pin) { if (pin < _BoardSender_Pins) {swith[pin].inverted();} /* else {addToHistory(pin, History_State_Error);}*/; };
	bool on (uint8_t pin);
	bool off (uint8_t pin);
	bool invert (uint8_t pin);
	bool isOn (uint8_t pin) { if (pin < _BoardSender_Pins) {return swith[pin].isOn();} /* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isOff (uint8_t pin) { if (pin < _BoardSender_Pins) {return swith[pin].isOff();} /* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isEnable (uint8_t pin) { if (pin < _BoardSender_Pins) {return swith[pin].isEnable();} /* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool hasChanged() {return _hasChanged;};
	bool hasChanged (uint8_t pin) { if (pin < _BoardSender_Pins) {return swith[pin].hasChanged();} /* else {addToHistory(pin, History_State_Error);}*/; return false;};
	//void printState(char *printBuffer, uint8_t print_max_pin);

	unsigned long timeToSend;
	History *history;
	uint8_t historyCategory;
	void addToHistory(uint8_t __Pin, uint8_t __State);
	uint8_t print(char *printBuffer, uint8_t index_begin, uint8_t print_index_max);
	uint8_t printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max);
	bool printHistoryByIndex(int8_t index, char *printBuffer);

	private:
	bool _hasChanged;
	SignalSwith *swith;
	//uint32_t *state;
	//const uint8_t _BoardSender_Count = 4;
	//const uint8_t _BoardSender_PinsOnBoard = 32;
	//const uint8_t _BoardSender_Pins = _BoardSender_Count*_BoardSender_PinsOnBoard;
	//static const unsigned char _Pin_DATA[_BoardSender_Count] = {(static const unsigned char)41,(static const unsigned char)42,(static const unsigned char)43,(static const unsigned chart)44};
	//static const uint8_t _Pin_DATA[_BoardSender_Count] = {(static const uint8_t)41,(static const uint8_t)42,(static const uint8_t)43,(static const uint8_t)44};
};

#endif /* SENDERBAOARD_H_ */
