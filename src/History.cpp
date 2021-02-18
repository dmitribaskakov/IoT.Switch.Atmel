#include <Arduino.h>
#include "History.h"
#include "SwitchCommon.h"

History::History () {
	category = new uint8_t [size];
	pin = new uint8_t [size];
	state = new uint8_t [size];
	clear();
}

void History::add(uint8_t __Category, uint8_t __Pin, uint8_t __State) {
	for(int8_t i = size - 1, j = size - 2; j >= 0; --i, --j){
		category[i] = category[j];
		pin[i] = pin[j];
		state[i] = state[j];
	};
	category[0] = __Category;
	pin[0] = __Pin;
	state[0] = __State;
};

void History::clear(){
	for (int8_t index = 0; index < size; index++) {
		category[index] = 0;
		pin[index] = 0;
		state[index] = 0;
	};
};

uint8_t History::printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < size)) {
		if (category[index] != History_Category_None) {
			if (category[index] == History_Category_Reader) {
				strcat(printBuffer, "r");
				byte2char(printBuffer, pin[index]);
				int2char(printBuffer, state[index]);
			} else if (category[index] == History_Category_Sender) {
				strcat(printBuffer, "s");
				byte2char(printBuffer, pin[index]);
				int2char(printBuffer, state[index]);
			} else {
				strcat(printBuffer, "e");
				byte2char(printBuffer, pin[index]);
				int2char(printBuffer, state[index]);
			};
			printed_index++;
		};
		index++;
	};
	return index;
};

