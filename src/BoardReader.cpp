#include <Arduino.h>
#include "SwitchCommon.h"
#include "BoardReader.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "History.h"

using namespace std;

void BoardReader::initialize(History *_history, uint8_t _historyCategory){
	_hasChanged = false;
	_expectCompletion = false;
	timeToRead = 0;
	history = _history;
	historyCategory = _historyCategory;
	uint8_t i;
	
	button = new SignalButton [_BoardReader_Pins];
	signalOnPin = new bool [_BoardReader_Pins];
	for(i = 0; i < _BoardReader_Pins; i++){
		button[i].disable();
	};

	pinMode(_BoardReader_Pin_LOAD, OUTPUT);
	digitalWrite(_BoardReader_Pin_LOAD, LOW);
	pinMode(_BoardReader_Pin_CLOCK, OUTPUT);
	digitalWrite(_BoardReader_Pin_CLOCK, LOW);
	for(i = 0; i < _BoardReader_Group; i++){
		pinMode(_BoardReader_Pin_INT[i], INPUT);
	};

	interuptOnBoard = new bool [_BoardReader_Boards];
	boardEnable = new bool[_BoardReader_Boards];
	for(i = 0; i < _BoardReader_Boards; i++) {
		pinMode(_BoardReader_Pin_DATA[i], INPUT);
		interuptOnBoard[i] = false;
		boardEnable[i] = true;
	};
	autoCheckAllBoardEnable = true;
	if (autoCheckAllBoardEnable) timeToAllBoardEnable = millis() + timeIntervalToAllBoardEnable;
	clear();
}
void BoardReader::clear() {
	uint8_t i;
	for(i = 0; i<_BoardReader_Pins; i++) {
		button[i].clear();
		signalOnPin[i] = false;
	};
	for(i = 0; i<_BoardReader_Boards; i++) {
		interuptOnBoard[i] = false;
	};
	_hasChanged = false;
}
bool BoardReader::read() {
	uint8_t pinOnBoard;
	uint8_t board;
	uint8_t offsetPinOfBoard;
	uint8_t buttonPin;
	
	if (autoCheckAllBoardEnable) {
		uint32_t timeTestToAllBoardEnable = millis() + timeIntervalToAllBoardEnable;
		if ((timeToAllBoardEnable < millis()) || (timeToAllBoardEnable > timeTestToAllBoardEnable)) {
			for(board = 0; board < _BoardReader_Boards; board++) boardEnable[board] = true;
			timeToAllBoardEnable = millis() + timeIntervalToAllBoardEnable;
		};
	};
	
	_hasChanged = false;
	_expectCompletion = false;
	timeToRead = micros();
	// http://robocraft.ru/blog/arduino/520.html
	// _LOAD_Pin = PIN 1 = SH/LD  защелка, SS (SPI) = PL / asynchronous parallel load input (active LOW)
	// _CLOCK_Pin = PIN 2 = CLK  тактовый вход, SCLK (SPI) = CP / clock input (LOW-to-HIGH edge-triggered)
	// _DATA_Pin = PIN 9 = QH  последовательный вывод, MISO (SPI) = Q7 / serial output from the last stage
	
	digitalWrite(_BoardReader_Pin_LOAD, LOW); //сначала дергается линия _LOAD_Pin вниз-вверх и состояния входов защелкиваются во внутренний регистр
	delayMicroseconds(8);
	digitalWrite(_BoardReader_Pin_LOAD, HIGH);
	delayMicroseconds(8);

	for(pinOnBoard = 0; pinOnBoard < _BoardReader_PinsOnBoard; pinOnBoard++){	//цикл на 16 пин
		delayMicroseconds(8);
		for(board = 0, offsetPinOfBoard = 0; board < _BoardReader_Boards; board++, offsetPinOfBoard += _BoardReader_PinsOnBoard){ // пробегаем по каждой плате
			buttonPin = pinOnBoard + offsetPinOfBoard;
			if (boardEnable[board]){
				if (digitalRead(_BoardReader_Pin_DATA[board])==HIGH) {
					signalOnPin[buttonPin] = true;
					} else {
					signalOnPin[buttonPin] = false;
				};
				} else {
				signalOnPin[buttonPin] = false;
			};
		};	// end for board
		digitalWrite(_BoardReader_Pin_CLOCK, HIGH);
		delayMicroseconds(8);
		digitalWrite(_BoardReader_Pin_CLOCK, LOW);
		delayMicroseconds(8);
	};	// end for pinOnBoard
	digitalWrite(_BoardReader_Pin_LOAD, LOW);
	
	// проверяем на Максимальное колличество одновременных сигналов на плате
    #if _BoardReader_Pins_Active_Max_For_Disable > 0
		uint8_t boardPinActive;
		for(board = 0, offsetPinOfBoard = 0; board < _BoardReader_Boards; board++, offsetPinOfBoard += _BoardReader_PinsOnBoard){ // пробегаем по каждой плате
			for(pinOnBoard = 0, boardPinActive = 0; pinOnBoard < _BoardReader_PinsOnBoard; pinOnBoard++){	//цикл на 16 пин
				buttonPin = pinOnBoard + offsetPinOfBoard;
				if (pinOnBoard == 0) {
					interuptOnBoard[board] = signalOnPin[buttonPin];
					} else if (signalOnPin[buttonPin]==HIGH) {
					if (!interuptOnBoard[board]) {
						boardPinActive = _BoardReader_Pins_Active_Max_For_Disable;
						} else {
						boardPinActive++;
					};
				};
			};
			if (boardPinActive >= _BoardReader_Pins_Active_Max_For_Disable) {
				boardEnable[board] = false;
				for(pinOnBoard = 0; pinOnBoard < _BoardReader_PinsOnBoard; pinOnBoard++){	//цикл на 16 пин
					buttonPin = pinOnBoard + offsetPinOfBoard;
					signalOnPin[buttonPin] = false;
					button[buttonPin].setSignal(LOW);
					button[buttonPin].clear();
				};
			};
		};
    #endif
	//Работаем с сигналами
	for(board = 0, offsetPinOfBoard = 0; board < _BoardReader_Boards; board++, offsetPinOfBoard += _BoardReader_PinsOnBoard){ // пробегаем по каждой плате
		for(pinOnBoard = 0; pinOnBoard < _BoardReader_PinsOnBoard; pinOnBoard++){	//цикл на 16 пин
			buttonPin = pinOnBoard + offsetPinOfBoard;
			if (boardEnable[board]){
				if (pinOnBoard == 0) {
					interuptOnBoard[board] = signalOnPin[buttonPin];
					} else {
					if (interuptOnBoard[board] || button[buttonPin].expectCompletion()) {
						button[buttonPin].setSignal(signalOnPin[buttonPin]);
						} else {
						button[buttonPin].setSignal(LOW);
					};
					if (!(button[buttonPin].isSignalNone())) {
						if (button[buttonPin].isButtonClick()) {
							addToHistory(buttonPin, Signal_Button_Click);
							} else if (button[buttonPin].isButtonLongClick()) {
							addToHistory(buttonPin, Signal_Button_LongClick);
							} else if (button[buttonPin].isButtonDoubleClick()) {
							addToHistory(buttonPin, Signal_Button_DoubleClick);
						};
						_hasChanged = true;
					};
					if (!_expectCompletion) {
						_expectCompletion |= button[buttonPin].expectCompletion();
					};
				};
				//if ((signalOnPin[buttonPin]) || (!(button[buttonPin].isSignalNone()))) {
				//	TO DO brdHistory.add(board, pinOnBoard, buttonPin, button[buttonPin].state);
				//};
			};
		};
	};
	
	timeToRead = micros() - timeToRead;
	return _hasChanged;
}
void BoardReader::addToHistory(uint8_t __Pin, uint8_t __State) {
	history->add(historyCategory, __Pin, __State);
}
uint8_t BoardReader::printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
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
bool BoardReader::printHistoryByIndex(int8_t index, char *printBuffer) {
	if ((index >= 0) && (index < history->size)) {
		if (history->category[index] == historyCategory) {
			byte2char(printBuffer, history->pin[index]);
			if (history->state[index] == Signal_Button_LongClick) {
				strcat(printBuffer, symbol_Button_LongClick);
				} else if (history->state[index] == Signal_Button_DoubleClick) {
				strcat(printBuffer, symbol_Button_DoubleClick);
				} else if (history->state[index] == Signal_Button_Click) {
				strcat(printBuffer, symbol_Button_Click);
				} else {
				strcat(printBuffer, symbol_Error);
			};
			return true;
		};
	};
	return false;
}
uint8_t BoardReader::print(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < _BoardReader_Pins)) {
		if (!button[index].isSignalNone()) {
			byte2char(printBuffer, index);
			if (button[index].isButtonClick()) { strcat(printBuffer, symbol_Button_Click); };
			if (button[index].isButtonLongClick()) { strcat(printBuffer, symbol_Button_LongClick); };
			if (button[index].isButtonDoubleClick()) { strcat(printBuffer, symbol_Button_DoubleClick); };
			printed_index++;
		};
		index++;
	};
	return index;
}
uint8_t BoardReader::printSignalOnPin(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < _BoardReader_Pins)) {
		if (signalOnPin[index]) {
			byte2char(printBuffer, index);
			strcat(printBuffer, " ");
			printed_index++;
		};
		index++;
	};
	return index;
}
uint8_t BoardReader::printBoardEnable(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < _BoardReader_Boards)) {
		if (boardEnable[index]) {
			int2char(printBuffer, index);
			strcat(printBuffer, " ");
			printed_index++;
		};
		index++;
	};
	return index;
}
bool BoardReader::hasSignal() {
	uint8_t board, index;
	for(uint8_t group = 0 ; group <_BoardReader_Group; group++){
		if (digitalRead(_BoardReader_Pin_INT[group]) == HIGH) {
			for(board = 0; board < _BoardReader_BoardInGroup[group]; board++){
				index = board + _BoardReader_OffsetBoardInGroup[group];
				if (boardEnable[index]) return true;
			};
		};
	};
	return false;
}