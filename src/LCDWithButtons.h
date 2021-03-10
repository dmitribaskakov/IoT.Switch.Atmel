#ifndef LCDWithButtons_H_
#define LCDWithButtons_H_

#include <Arduino.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <string.h>
#include "LiquidCrystal.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "BoardReader.h"
#include "BoardSender.h"
#include "SignalWorker.h"
#include "History.h"

using namespace std;

const uint16_t LCD_Time_To_Off = 20000;		// Время до выключения дисплея 20 сек *1000
const uint8_t displayMode_normal = 0;		// Совмещенный режим
const uint8_t displayMode_sender = 1;		// Режим отображения включенных реле
const uint8_t displayMode_reader = 2;		// Режим отображения истории нажатий на выключатели
const uint8_t displayMode_readerPin = 3;	// Режим отображения состояния пинов
const uint8_t displayMode_readerEnable = 4;	// Режим отображения состояния плат чтения
const uint8_t displayMode_history = 5;		// Режим отображения всей истории
const uint8_t displayMode_max = displayMode_history;

class LCDWithButtons : public LiquidCrystal {
	public:
	LCDWithButtons(uint8_t RS_Pin, uint8_t ENABLE_Pin, uint8_t D4_Pin, uint8_t D5_Pin,
	uint8_t D6_Pin, uint8_t D7_Pin, uint8_t LCD_Buttons_Pin, uint8_t LCD_PowerBackLight_Pin = 0);
	void initialize(History *_history, SignalWorker *_worker);
	bool processButton();
	bool refresh;
	bool updateDisplay(uint32_t _timeStartToLoop=0);
	uint8_t displayMode;
	private:
	void displayTurnOn();
	void displayTurnOff();
	bool isDisplayOn;
	bool readButton();
	bool _buttonHasChanged;
	uint8_t _LCD_Buttons_Pin;
	uint8_t _LCD_PowerBackLight_Pin; // Выключение подсветки дисплея обычно 10
	uint16_t button_value;
	SignalButton buttonRight;
	SignalButton buttonLeft;
	SignalButton buttonUp;
	SignalButton buttonDown;
	SignalButton buttonSelect;
	History *history;
	SignalWorker *worker;
	uint8_t workerSenderIndex;
	uint32_t timeToOff;
	static const uint8_t printBuffer_size = 12;
	char *printBuffer;
	void printBufferClear() {for (uint8_t i = 0; i < printBuffer_size; i++) printBuffer[i] = 0;};
};


#endif /* LCDWithButtons_H_ */
