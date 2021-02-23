#ifndef BOARDREADER_H_
#define BOARDREADER_H_

#include <Arduino.h>
#include "SignalButton.h"
#include "SignalSwith.h"
#include "History.h"
using namespace std;

#define _BoardReader_Group 2												//=2 колличество групп
#define _BoardReader_Boards 6												//=8 колличество плат всего
#define _BoardReader_PinsOnBoard 16											//=16 колличество пин на одной плате
#define _BoardReader_Pins _BoardReader_PinsOnBoard * _BoardReader_Boards	//=96 всего пин на всех платах
#define _BoardReader_Pins_Active_Max_For_Disable 4		//Максимальное колличество одновременных сигналов на плате
const uint8_t _BoardReader_Pin_LOAD = 31;
const uint8_t _BoardReader_Pin_CLOCK = 26;
const uint8_t _BoardReader_Pin_INT[_BoardReader_Group] = {27, 33};
const uint8_t _BoardReader_BoardInGroup[_BoardReader_Group] = {4, 1};
const uint8_t _BoardReader_OffsetBoardInGroup[_BoardReader_Group] = {0, 4};
const uint8_t _BoardReader_Pin_DATA[_BoardReader_Boards] = {22, 23, 24, 25, 28, 29}; //, 30, 32};


// http://homes-smart.ru/index.php/oborudovanie/prochee/rasshiryaem-porty
class BoardReader {
public:
	BoardReader(){};
	void initialize(History *_history, uint8_t _historyCategory);
	bool read();
	void clear();
	bool hasSignal();
	bool hasChanged() {return _hasChanged;};
	bool expectCompletion() {return _expectCompletion;};;
	uint8_t pins() {return _BoardReader_Pins;};
	void enable (uint8_t pin, uint8_t __signalType = Signal_Type_Button_Click) { if (pin<_BoardReader_Pins) {button[pin].enable(__signalType);}/* else {addToHistory(pin, History_State_Error);}*/; };
	void disable (uint8_t pin, uint8_t __signalType = ~Signal_Type_Button_Disabled) { if (pin<_BoardReader_Pins) {button[pin].disable(__signalType);}/* else {addToHistory(pin, History_State_Error);}*/; };
	bool isEnable (uint8_t pin, uint8_t __signalType = ~Signal_Type_Button_Disabled) { if (pin<_BoardReader_Pins) {return button[pin].isEnable(__signalType);}/* else {addToHistory(pin, History_State_Error);}; return false*/; };
	bool isSignalNone (uint8_t pin) { if (pin<_BoardReader_Pins) {return button[pin].isSignalNone();}/* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isButtonPressed (uint8_t pin) { if (pin<_BoardReader_Pins) {return button[pin].isButtonPressed();}/* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isButtonClick (uint8_t pin) { if (pin<_BoardReader_Pins) {return button[pin].isButtonClick();}/* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isButtonLongClick (uint8_t pin) { if (pin<_BoardReader_Pins) {return button[pin].isButtonLongClick();}/* else {addToHistory(pin, History_State_Error);}*/; return false;};
	bool isButtonDoubleClick (uint8_t pin) { if (pin<_BoardReader_Pins) {return button[pin].isButtonDoubleClick();}/* else {addToHistory(pin, History_State_Error);}*/; return false;};
	void addToHistory(uint8_t __Pin, uint8_t __State);
	uint8_t print(char *printBuffer, uint8_t index_begin,uint8_t print_max_pin);
	uint8_t printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max);
	bool printHistoryByIndex(int8_t index, char *printBuffer);
	uint8_t printSignalOnPin(char *printBuffer, uint8_t index_begin, uint8_t print_max_pin);
	uint8_t printBoardEnable(char *printBuffer, uint8_t index_begin, uint8_t print_max_pin);
	bool _isSetupMode;
	SignalButton *button;
	bool _hasChanged;
	bool _expectCompletion;
	unsigned long timeToRead;
	History *history;
	uint8_t historyCategory;
	bool *boardEnable;
	bool *interuptOnBoard;
	bool *signalOnPin;
	bool autoCheckAllBoardEnable;
	uint32_t timeToAllBoardEnable;
	uint16_t timeIntervalToAllBoardEnable = 2000;	// Время до включения всех плат 5*1000
};


#endif /* BOARDREADER_H_ */
