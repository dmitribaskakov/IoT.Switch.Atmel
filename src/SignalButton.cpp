#include <Arduino.h>
//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>
#include "SignalButton.h"

//using namespace std;

SignalButton::SignalButton () {
	signalType = Signal_None;
	signal = Signal_None;
	state = Signal_None;
	nextState = Signal_None;
	timeToHigh = 0;
	timeToLow = 0;
};
uint32_t SignalButton::calculateTimeToHigh() {
	uint32_t result = 0;
	uint32_t time = 0;
	time = millis();
	if (time<2) { time = 2; };
	if (timeToHigh>time) { timeToHigh = time - 1; };
	if (timeToHigh>0) { result = time - timeToHigh; };
	return result;
};
uint32_t SignalButton::calculateTimeToLow() {
	uint32_t result = 0;
	uint32_t time = 0;
	time = millis();
	if (time < 2) { time = 2; };
	if (timeToLow > time) { timeToLow = time - 1; };
	if (timeToLow > 0) { result = time - timeToLow; };
	return result;
};
bool SignalButton::expectCompletion() {
	if ( (timeToLow > 0) || (timeToHigh>0) || (nextState != Signal_None) ) { return true; };
	return false;
}
void SignalButton::disable(uint8_t __signalType) {
	signalType &= ~__signalType;
	if (signalType == Signal_Type_Button_Disabled) {
		//signal = Signal_None;
		state = Signal_None;
		nextState = Signal_None;
		timeToHigh = 0;
		timeToLow = 0;
	};
};

uint8_t SignalButton::setSignal (uint8_t Signal) {
	signal = Signal;
	if (signalType == Signal_Type_Button_Disabled) { return Signal_None; };

	uint32_t nowTime = millis();
	state = Signal_None;

	if (Signal == HIGH) {
		//--------------------------------------------------------------------------------------------------
		//Есть сигнал на входе
		if (((nextState == Signal_None) || (nextState & Signal_Button_DetectToClick)) && (!(nextState & Signal_Button_Pressed))) {
			//следущее состояние тоже нет сигнала
			//тогда ожидаем несколько миллисикунд на устранение дребезга и ждем нажатия
			nextState |= Signal_Button_Pressed;
			timeToHigh = nowTime;
		};
		if (nextState & Signal_Button_Pressed) {
			if (calculateTimeToHigh() >= Signal_timeTo_Debounce) {
				//прождали несколько миллисикунд на устранение дребезга
				//Сигнал остался. Можно считать что кнопка нажата
				timeToLow = 0;
				nextState &= ~Signal_Button_Released;

				if (signalType & Signal_Type_Button_Pressed) {
					state |= Signal_Button_Pressed;
				};

				if (calculateTimeToHigh() >= Signal_timeTo_Button_Click) {

					if ((signalType & Signal_Type_Button_DoubleClick) && (nextState & Signal_Button_DetectToClick)) {
						state |= Signal_Button_DoubleClick;
						nextState = Signal_Button_ReleasedDoubleClick;
					};
					if ( ( !((signalType & Signal_Type_Button_LongClick) || (signalType & Signal_Type_Button_DoubleClick)) && (signalType & Signal_Type_Button_Click)) ){
						state |= Signal_Button_Click;
						nextState = Signal_Button_Released;
					};
					if ((signalType & Signal_Type_Button_LongClick) && (calculateTimeToHigh() >= Signal_timeTo_Button_LongClick)) {
						state |= Signal_Button_LongClick;
						nextState = Signal_Button_ReleasedLongClick;
					};
				};
			};
		};
		} else if (nextState != Signal_None) {
		//--------------------------------------------------------------------------------------------------
		//Нет сигнала на входе (был но пропал - определяем что это было)
		if (nextState & Signal_Button_Pressed) {
			//текущее состояние - кнопка нажата и сигнал пропадает
			//планируем что следущее состояние будет - кнопка отжата
			nextState |= Signal_Button_Released;
		};
		if (((nextState & Signal_Button_Released) || (nextState & Signal_Button_ReleasedDoubleClick) || (nextState & Signal_Button_ReleasedLongClick)) && (timeToLow==0)) {
			//как только кнопка была реально отажата, сначала устраним дребезг
			timeToLow = nowTime;
		};
		if (calculateTimeToLow() < Signal_timeTo_Debounce) {
			//Ждем несколько миллисикунд на устранение дребезга
			if ((nextState & Signal_Button_Pressed) && (signalType & Signal_Type_Button_Pressed)) {
				//считаем что кнопка все еще была нажата
				state |= Signal_Button_Pressed;
			};
			} else if (calculateTimeToLow() >= Signal_timeTo_Debounce) {
			//прождали несколько миллисикунд на устранение дребезга
			//Сигнала нет. Можно считать что кнопка была отжата
			//теперь распознаем в завистимости от типа кнопки
			nextState &= ~Signal_Button_Pressed;
			if (signalType & Signal_Type_Button_Pressed) {
				state &= ~Signal_Button_Pressed;
			};

			if (nextState & Signal_Button_ReleasedDoubleClick) {
				timeToHigh = 0;
				timeToLow = 0;
				nextState = Signal_None;
				state = Signal_None;
				} else if ((nextState & Signal_Button_DetectToClick) && (calculateTimeToLow() >= Signal_timeBetween_Button_Clicks)) {
				//пауза между нажатиями кнопки в DoubleClick была слишком большой считаем что это уже не DoubleClick
				timeToHigh = 0;
				timeToLow = 0;
				nextState = Signal_None;
				if (signalType & Signal_Type_Button_Click) {
					state |= Signal_Button_Click;
					} else {
					state = Signal_None;
				};
				} else if (nextState & Signal_Button_Released) {
				if ((signalType & Signal_Type_Button_DoubleClick) && (calculateTimeToLow() < Signal_timeBetween_Button_Clicks)) {
					//возможен DoubleClick нужно указать что следующий статус это ожидание втрого нажатия
					//и на этом статусе уже проверять по времени какой сигнал распознается
					if (calculateTimeToHigh() > Signal_timeTo_Button_Click) {
						//кнопка была нажата досточно долго чтобы зафиксировать Click
						//теперь нужно подожать будет ли DoubleClick
						nextState |= Signal_Button_DetectToClick;
					};
					} else if ((signalType & Signal_Type_Button_LongClick) && (signalType & Signal_Type_Button_Click)) {
					//возможны LongClick или Click тогда нужно проверить по времени отпускания, что же произошло
					if ((calculateTimeToHigh() > Signal_timeTo_Button_Click) && (calculateTimeToHigh() < Signal_timeTo_Button_LongClick)) {
						state |= Signal_Button_Click;
					};
					timeToHigh = 0;
					timeToLow = 0;
					nextState = Signal_None;
					} else if (!((signalType & Signal_Type_Button_DoubleClick) && (calculateTimeToLow() < Signal_timeBetween_Button_Clicks))) {
					timeToHigh = 0;
					timeToLow = 0;
					nextState = Signal_None;
					state = Signal_None;
				};
				} else if (nextState & Signal_Button_ReleasedLongClick) {
				timeToHigh = 0;
				timeToLow = 0;
				nextState = Signal_None;
				state = Signal_None;
			};
		};
	};
	return state;
};
