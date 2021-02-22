#ifndef SIGNALBUTTON_H_
#define SIGNALBUTTON_H_

#include <Arduino.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <string.h>
//using namespace std;

//Типы сигналов которые может кнока распозновать
const uint8_t Signal_Type_Button_Disabled = 0; 			//B 0000 0000 Кнопка отключена, сигналы не распознаются
const uint8_t Signal_Type_Button_Pressed = 1; 			//B 0000 0001 Возможный тип сигнала - нажатие на кнопку
const uint8_t Signal_Type_Button_Click = 2;				//B 0000 0010 Возможный тип сигнала - Click - срабатывает во время нажатия на кнопку
const uint8_t Signal_Type_Button_DoubleClick = 4; 		//B 0000 0100 Возможный тип сигнала - DoubleClick - срабатывает во время второго нажатия на кнопку
const uint8_t Signal_Type_Button_LongClick = 8;			//B 0000 1000 Возможный тип сигнала - LongClick - если время нажатия на кнопку превышает LongClick срабатывает

//Распознанное состяние кнопки
const uint8_t Signal_None = 0;							// B 0000 0000 Сигнал отсутствует
const uint8_t Signal_Button_Pressed = 1;				// B 0000 0001 Кнопка нажата
const uint8_t Signal_Button_Click = 2;					// B 0000 0010 Зафиксировано одинарное нажатие
const uint8_t Signal_Button_DoubleClick = 4;			// B 0000 0100 Зафиксировано двойное нажатие на кнопку
const uint8_t Signal_Button_LongClick = 8;				// B 0000 1000 Зафиксировано одинарное длинное нажатие на кнопку
const uint8_t Signal_Button_Released = 16;				// B 0001 0000 Кнопка отпущена
const uint8_t Signal_Button_DetectToClick = 32;			// B 0010 0000 Обнаружено одно нажатие на кнопку и кнопку отпустили
const uint8_t Signal_Button_ReleasedDoubleClick = 64;	// B 0100 0000 Обнаружено двойное нажатие на кнопку и кнопку отпустили
const uint8_t Signal_Button_ReleasedLongClick = 128;	// B 1000 0000 Обнаружено длинное нажатие на кнопку и кнопку отпустили

//Временные интервалы для распознования разных сигналов кнопки
const uint16_t Signal_timeTo_Debounce = 50;				// Время наличия сигнала на «устранение дребезга»
const uint16_t Signal_timeTo_Button_Click = 75;			// Время наличия сигнала через которое считается что было нажатие на кнопку
const uint16_t Signal_timeTo_Button_LongClick = 500; 	// Время наличия сигнала через которое считается что было долгое нажатие на кнопку
const uint16_t Signal_timeBetween_Button_Clicks = 500; 	// Максимальное время отстувия сигнала как пауза между двойным нажатием кнопки

const char symbol_Button_Click[2] 		= {(uint8_t)'c', (uint8_t)0}; //1;
const char symbol_Button_DoubleClick[2]	= {(uint8_t)'d', (uint8_t)0}; //2;
const char symbol_Button_LongClick[2]	= {(uint8_t)'L', (uint8_t)0}; //3;
const char symbol_On[2]	 				= {(uint8_t)'o', (uint8_t)0}; //4;
const char symbol_Off[2]				= {(uint8_t)'f', (uint8_t)0}; //5;
const char symbol_Error[2]				= {(uint8_t)'E', (uint8_t)0};

class SignalButton {
	public:
	SignalButton ();
	//const uint8_t operator = (uint8_t newSignal) { return setSignal(newSignal); };
	//const uint8_t operator & (uint8_t testSignal) { return state & testSignal; };
	//const uint8_t operator | (uint8_t testSignal) { return state | testSignal; };
	//const bool operator == (uint8_t testSignal){ if ((state==0)&&(testSignal==0)) {return true;}; return (bool)(state & testSignal); };
	//const bool operator != (uint8_t testSignal){ if ((state==0)&&(testSignal==0)) {return false;}; return (!(bool)(state & testSignal)); };
	bool expectCompletion();
	uint8_t setSignal (uint8_t Signal);
	uint8_t state;
	void enable (uint8_t __signalType = Signal_Type_Button_Click) { signalType |= __signalType; };
	void disable (uint8_t __signalType = ~Signal_Type_Button_Disabled);
	void clear() {state = Signal_None; signal = LOW;};
	bool isEnable (uint8_t __signalType = ~Signal_Type_Button_Disabled) { return signalType & __signalType; };
	bool isSignalNone () { return state == Signal_None; };
	bool isButtonPressed () { return state & Signal_Button_Pressed; };
	bool isButtonClick () { return state & Signal_Button_Click; };
	bool isButtonLongClick () { return state & Signal_Button_LongClick; };
	bool isButtonDoubleClick () { return state & Signal_Button_DoubleClick; };
	//private:
	uint8_t signal;
	uint8_t signalType;
	uint8_t nextState;
	uint32_t timeToHigh;
	uint32_t timeToLow;
	uint32_t calculateTimeToHigh();
	uint32_t calculateTimeToLow();
};

#endif /* SIGNALBUTTON_H_ */
