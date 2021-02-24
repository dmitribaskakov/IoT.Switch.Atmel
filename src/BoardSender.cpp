#include <Arduino.h>
#include "SwitchCommon.h"
#include "BoardSender.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "History.h"
using namespace std;

void BoardSender::initialize(History *_history, uint8_t _historyCategory){
	_hasChanged = false;
	swith = new SignalSwith [_BoardSender_Pins];
	for(uint8_t pin = 0; pin < _BoardSender_Pins; pin++){
		swith[pin].disable();
	};
	history = _history;
	historyCategory = _historyCategory;
	pinMode(_BoardSender_Pin_SS, OUTPUT);
	digitalWrite(_BoardSender_Pin_SS, HIGH);
	pinMode(_BoardSender_Pin_SCK, OUTPUT);
	digitalWrite(_BoardSender_Pin_SCK, LOW);
	for(uint8_t board = 0; board < _BoardSender_Count; board++){
		pinMode(_BoardSender_Pin_DATA[board], OUTPUT);
		digitalWrite(_BoardSender_Pin_DATA[board], LOW);
	};
}
void BoardSender::enable(uint8_t pin, uint32_t __timerMaxOn, uint8_t __opposited) { 
    if (pin < _BoardSender_Pins) {
        swith[pin].enable(__timerMaxOn, __opposited);
    } /* else {addToHistory(pin, History_State_Error);}*/; 
}
void BoardSender::clear() {
	uint8_t index;
	_hasChanged = false;
	for(index = 0; index<_BoardSender_Pins; index++) {
		_hasChanged = _hasChanged || swith[index].processed();
	};
}
void BoardSender::send() {
	/* http://arduino.ru/Tutorial/registr_74HC595 и http://robocraft.ru/blog/arduino/519.html
	_Pin_DATA[board] = пин 14 = DS Вход для последовательных данных = MOSI (SPI)
	_Pin_SCK = пин 11 = SH_CP Вход для тактовых импульсов = SCLK (SPI)
	_Pin_SS = пин 12 = ST_CP Синронизация ("защелкивание") выходов = SS (SPI); при установке низкого уровн¤ выводы регистра защЄлкиваютс¤
	*/
	timeToSend = micros();
	digitalWrite(_BoardSender_Pin_SCK, LOW);
	digitalWrite(_BoardSender_Pin_SS, LOW);
	delayMicroseconds(2);
	uint8_t pin;
	uint8_t offset;
	uint8_t board;
	for(uint8_t pinOnBoard = 0; pinOnBoard < _BoardSender_PinsOnBoard; pinOnBoard++){
		for(board = 0, offset = 0; board < _BoardSender_Count; board++, offset += _BoardSender_PinsOnBoard){
			pin = offset + pinOnBoard;
			digitalWrite(_BoardSender_Pin_DATA[board], swith[pin].SendState());
		};
		digitalWrite(_BoardSender_Pin_SCK, LOW);
		delayMicroseconds(2);
		digitalWrite(_BoardSender_Pin_SCK, HIGH);
		delayMicroseconds(2);
	};
	digitalWrite(_BoardSender_Pin_SS, HIGH);
	digitalWrite(_BoardSender_Pin_SCK, LOW);
	//_hasChanged = false;
	timeToSend = micros() - timeToSend;
}
bool BoardSender::on(uint8_t pin) {
	uint8_t __hasChanged = false;
	if (pin < _BoardSender_Pins) {
        if (swith[pin].opposited != 0) {
            if (swith[swith[pin].opposited].isOn()) {
				_hasChanged = swith[pin].setChanged();
                return _hasChanged;
            }
        };
		__hasChanged = swith[pin].on();
		if (__hasChanged) {
			_hasChanged = true;
			addToHistory(pin, swith[pin].isOn());
		};
	} else {
		addToHistory(pin, History_State_Error);
	};
	return __hasChanged;
}
bool BoardSender::off(uint8_t pin) {
	uint8_t __hasChanged = false;
	if (pin < _BoardSender_Pins) {
		__hasChanged = swith[pin].off();
		if (__hasChanged) {
			_hasChanged = true;
			addToHistory(pin, swith[pin].isOn());
		};
	} else {
		addToHistory(pin, History_State_Error);
	};
	return __hasChanged;
}
bool BoardSender::invert(uint8_t pin) {
	uint8_t __hasChanged = false;
	if (pin < _BoardSender_Pins) {
		__hasChanged = swith[pin].invert();
		if (__hasChanged) {
			_hasChanged = true;
			addToHistory(pin, swith[pin].isOn());
		};
	} else {
		addToHistory(pin, History_State_Error);
	};
	return __hasChanged;
};
void BoardSender::addToHistory(uint8_t __Pin, uint8_t __State) {
	history->add(historyCategory, __Pin, __State);
}
uint8_t BoardSender::print(char *printBuffer, uint8_t index_begin, uint8_t print_max_pin) {
	uint8_t index = index_begin;
	uint8_t printed_pin = 0;
	while ((printed_pin < print_max_pin) && (index < _BoardSender_Pins)) {
		if (swith[index].isOn()) {
			byte2char(printBuffer, index);
			printed_pin++;
		};
		index++;
	};
	return index;
}
uint8_t BoardSender::printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < history->size)) {
		if (history->category[index] == historyCategory) {
			if (printHistoryByIndex(index, printBuffer)) printed_index++;
		};
		index++;
	};
	return index;
}
bool BoardSender::printHistoryByIndex(int8_t index, char *printBuffer) {
	if ((index >= 0) && (index < history->size)) {
		if (history->category[index] == historyCategory) {
			byte2char(printBuffer, history->pin[index]);
			if (history->state[index] == true) {
				strcat(printBuffer, symbol_On);
			} else if (history->state[index] == false) {
				strcat(printBuffer, symbol_Off);
			} else {
				strcat(printBuffer, symbol_Error);
			};
			return true;
		};
	};
	return false;
}
/* void BoardSender::printState(char *printBuffer, uint8_t print_max_pin) {
	//Ѕайт |=(1<<3) запись 1 в 3ий бит байта
	//Ѕайт &=~(1<<3) запись 0 в 3ий бит байта
	if (swith[pin].isOn()) {
		state[board] |=  (1<<pinOnBoard);
	} else {
		state[board] &= ~(1<<pinOnBoard);
	};
} */