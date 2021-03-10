#include <Arduino.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <string.h>
#include "SwitchCommon.h"
#include "LiquidCrystal.h"
#include "LCDWithButtons.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "BoardReader.h"
#include "BoardSender.h"
#include "SignalWorker.h"
#include "History.h"

using namespace std;

#define BUTTON_VALUE_RIGHT 50		// 10 - 15
#define BUTTON_VALUE_UP 150			// 130 - 132
#define BUTTON_VALUE_DOWN 300		// 310-311
#define BUTTON_VALUE_LEFT 500		// 479
#define BUTTON_VALUE_SELECT 1000		// 720

uint8_t picture_Button_Click[8] 		= {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00};
uint8_t picture_Button_DoubleClick[8] 	= {0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00};
uint8_t picture_Button_LongClick[8] 	= {0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x00};
uint8_t picture_On[8] 					= {0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t picture_Off[8]					= {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00};
uint8_t picture_Reader[8]	 			= {0x07, 0x05, 0x05, 0x07, 0x00, 0x02, 0x07, 0x02};
uint8_t picture_Sender[8]	 			= {0x07, 0x05, 0x05, 0x07, 0x00, 0x00, 0x07, 0x00};

LCDWithButtons::LCDWithButtons(uint8_t RS_Pin, uint8_t ENABLE_Pin, uint8_t D4_Pin, uint8_t D5_Pin, uint8_t D6_Pin, uint8_t D7_Pin, 
uint8_t LCD_Buttons_Pin, uint8_t LCD_PowerBackLight_Pin)
: LiquidCrystal (RS_Pin, ENABLE_Pin, D4_Pin, D5_Pin, D6_Pin, D7_Pin) {
	/* LCD RS pin to digital pin 12
	* LCD Enable pin to digital pin 11
	* LCD D4 pin to digital pin 5
	* LCD D5 pin to digital pin 4
	* LCD D6 pin to digital pin 3
	* LCD D7 pin to digital pin 2
	* LCD R/W pin to ground
	* LCD_Buttons_Pin */
	_LCD_Buttons_Pin = LCD_Buttons_Pin;
	_LCD_PowerBackLight_Pin = LCD_PowerBackLight_Pin; // Выключение подсветки дисплея
};

bool LCDWithButtons::readButton(){
	_buttonHasChanged = false;
	button_value = analogRead(_LCD_Buttons_Pin);
	if (button_value < BUTTON_VALUE_RIGHT)			{buttonRight.setSignal(HIGH);	buttonLeft.setSignal(LOW);	buttonUp.setSignal(LOW);	buttonDown.setSignal(LOW);	buttonSelect.setSignal(LOW);}
	else if (button_value < BUTTON_VALUE_UP)		{buttonRight.setSignal(LOW);	buttonLeft.setSignal(LOW);	buttonUp.setSignal(HIGH);	buttonDown.setSignal(LOW);	buttonSelect.setSignal(LOW);}
	else if (button_value < BUTTON_VALUE_DOWN)		{buttonRight.setSignal(LOW);	buttonLeft.setSignal(LOW);	buttonUp.setSignal(LOW);	buttonDown.setSignal(HIGH);	buttonSelect.setSignal(LOW);}
	else if (button_value < BUTTON_VALUE_LEFT)		{buttonRight.setSignal(LOW);	buttonLeft.setSignal(HIGH);	buttonUp.setSignal(LOW);	buttonDown.setSignal(LOW);	buttonSelect.setSignal(LOW);}
	else if (button_value < BUTTON_VALUE_SELECT)	{buttonRight.setSignal(LOW);	buttonLeft.setSignal(LOW);	buttonUp.setSignal(LOW);	buttonDown.setSignal(LOW);	buttonSelect.setSignal(HIGH);}
	else											{buttonRight.setSignal(LOW);	buttonLeft.setSignal(LOW);	buttonUp.setSignal(LOW);	buttonDown.setSignal(LOW);	buttonSelect.setSignal(LOW);};
	if ( !(buttonRight.isSignalNone() && buttonLeft.isSignalNone() && buttonUp.isSignalNone() && buttonDown.isSignalNone() && buttonSelect.isSignalNone()) ) {
		if (!isDisplayOn) {
			displayTurnOn();
			return false;
		};
		_buttonHasChanged = true;
	} else if (isDisplayOn) {
		if ( (timeToOff < millis()) & (millis() > LCD_Time_To_Off) ) displayTurnOff(); 
	};
	return _buttonHasChanged;
};
void LCDWithButtons::displayTurnOn(){
	timeToOff = millis() + LCD_Time_To_Off;
	if (!isDisplayOn) {
		isDisplayOn = true;
		refresh = true;
		display();
		if (_LCD_PowerBackLight_Pin > 0) { digitalWrite(_LCD_PowerBackLight_Pin, HIGH); };
	};
};
void LCDWithButtons::displayTurnOff(){
	if (isDisplayOn) {
		isDisplayOn = false;
		noDisplay();
		timeToOff = millis();
		if (_LCD_PowerBackLight_Pin > 0) { digitalWrite(_LCD_PowerBackLight_Pin, LOW); };
	};
};
void LCDWithButtons::initialize(History *_history, SignalWorker *_worker){
	if (_LCD_PowerBackLight_Pin > 0) { pinMode(_LCD_PowerBackLight_Pin, OUTPUT); };	// Выключение подсветки дисплея - LOW
	history = _history;
	worker = _worker;
	workerSenderIndex = 0;
	button_value = 0;
	timeToOff = millis() + LCD_Time_To_Off;
	isDisplayOn = false;
	_buttonHasChanged = false;
	buttonRight.enable(Signal_Type_Button_Click);
	buttonLeft.enable(Signal_Type_Button_Click);
	buttonUp.enable(Signal_Type_Button_Click | Signal_Type_Button_LongClick);
	buttonDown.enable(Signal_Type_Button_Click | Signal_Type_Button_LongClick);
	buttonSelect.enable(Signal_Type_Button_Click | Signal_Type_Button_LongClick);
	displayMode = displayMode_normal;
	printBuffer = new char [printBuffer_size];
	printBufferClear();
	begin(16, 2);
	createChar((uint8_t)symbol_Button_Click[0], picture_Button_Click);
	createChar((uint8_t)symbol_Button_DoubleClick[0], picture_Button_DoubleClick);
	createChar((uint8_t)symbol_Button_LongClick[0], picture_Button_LongClick);
	createChar((uint8_t)symbol_On[0], picture_On);
	createChar((uint8_t)symbol_Off[0], picture_Off);
	clear();
	displayTurnOn();
};
bool LCDWithButtons::processButton(){
	if (readButton()) {
		if (buttonSelect.isButtonClick()) {
			refresh = true;
			displayMode++;
			if (displayMode > displayMode_max) {displayMode = 0;};
		} else if (buttonSelect.isButtonLongClick()) {
			refresh = false;
			displayTurnOff();
		} else if ((displayMode == displayMode_normal)) {
			if (buttonRight.isButtonClick()) {
				if (!((workerSenderIndex >= 0) && (workerSenderIndex < worker->sender->pins()))) { workerSenderIndex = 0; };
				worker->sender->invert(workerSenderIndex);
			} else if (buttonUp.isButtonClick()) {
				workerSenderIndex++;
				refresh = true;
				if (workerSenderIndex >= worker->sender->pins()) { workerSenderIndex = 0; };
			} else if (buttonUp.isButtonLongClick()) {
				workerSenderIndex = workerSenderIndex + 0x10;
				refresh = true;
				if (workerSenderIndex >= worker->sender->pins()) { workerSenderIndex = 0; };
			} else if (buttonDown.isButtonClick()) {
				workerSenderIndex--;
				refresh = true;
				if ((workerSenderIndex < 0) || (workerSenderIndex > (worker->sender->pins()-  1))) { workerSenderIndex = worker->sender->pins() - 1; };
			} else if (buttonDown.isButtonLongClick()) {
				workerSenderIndex = workerSenderIndex - 0x10;
				refresh = true;
				if ((workerSenderIndex < 0) || (workerSenderIndex > (worker->sender->pins() - 1))) { workerSenderIndex = worker->sender->pins() - 1; };
			};
		} else if ((displayMode == displayMode_reader) || (displayMode == displayMode_sender) 
			|| (displayMode == displayMode_history) || (displayMode == displayMode_readerPin)) {
			if (buttonRight.isButtonClick()) {
				history->clear();
				refresh = true;
			};
		};
		buttonRight.clear();
		buttonLeft.clear();
		buttonUp.clear();
		buttonDown.clear();
		buttonSelect.clear();
		timeToOff = millis() + LCD_Time_To_Off;
	};
	return refresh;
};
bool LCDWithButtons::updateDisplay (uint32_t _timeStartToLoop) {
	displayTurnOn();
	clear();
	setCursor(0, 0);
	printBufferClear();
	uint8_t index = 0;
	if (displayMode == displayMode_sender) {
		//	Режим отображения включенных реле
		print("S");
		index = worker->printHistory(History_Category_Sender, printBuffer, index, 5);
		print(printBuffer);
		setCursor(1, 1);
		printBufferClear();
		index = worker->printHistory(History_Category_Sender, printBuffer, index, 5);
		print(printBuffer);

	} else if (displayMode == displayMode_reader) {
		//	Режим отображения истории нажатия на выключатели
		print("R");
		index = worker->printHistory(History_Category_Reader, printBuffer, index, 5);
		print(printBuffer);
		setCursor(1, 1);
		printBufferClear();
		index = worker->printHistory(History_Category_Reader, printBuffer, index, 5);
		print(printBuffer);

	} else if (displayMode == displayMode_readerPin) {
		//	Режим отображения состояния пинов
		print("Signal On Pins ");
		setCursor(0, 1);
		index = worker->reader->printSignalOnPin(printBuffer, index, 5);
		print(printBuffer);
		
	} else if (displayMode == displayMode_readerEnable) {
		// Режим отображения состояния плат чтения
		print("Reader Boards ON");
		setCursor(0, 1);
		index = worker->reader->printBoardEnable(printBuffer, index, 8);
		print(printBuffer);

	} else if (displayMode == displayMode_history) {
		//	Режим отображения всей истории
		print("Hst ");
		index = history->printHistory(printBuffer, index, 4);
		print(printBuffer);
		setCursor(0, 1);
		printBufferClear();
		index = history->printHistory(printBuffer, index, 5);
		print(printBuffer);

	} else {
		//	Совмещенный режим
		worker->reader->print(printBuffer, 0, 6);
		print(printBuffer);
		
		setCursor(4, 0);
		print(freeRam());

		setCursor(0, 1);
		printBufferClear();
		worker->sender->print(printBuffer, 0, 8);
		print(printBuffer);
		
		setCursor(14, 0);
		printBufferClear();
		byte2char(printBuffer, workerSenderIndex);
		print(printBuffer);

		setCursor(9, 0);
		print(millis() - _timeStartToLoop);
	};
	refresh = false;
};
