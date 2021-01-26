#ifndef STATUSLED_H_
#define STATUSLED_H_

#include <Arduino.h>

//Описание state machine мигания сигнальным светодиодом
const uint8_t _StatusLed_state_count = 4;	//Колличество состояний
const bool _StatusLed_states[_StatusLed_state_count] = {LOW,HIGH,LOW,HIGH};
const uint16_t _StatusLed_times[_StatusLed_state_count] = {1200,25,250,25};

class StatusLed {
public:
	StatusLed(uint8_t LED_PIN=13);
	void update();
private:
	uint8_t _led_pin;					//Пин сигнального светодиода
	uint8_t _led_state;					//Текущее сотояние
	uint32_t _led_last_change_time;		//Время последнего изменения состояния
};

#endif /* STATUSLED_H_ */
