#include <Arduino.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <string.h>
#include "SwitchCommon.h"
#include "WString.h"
using namespace std;

void byte2char(char *printBuffer, uint8_t src_byte) {
	static const char *base[] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
	strcat(printBuffer, base[src_byte / 16]);
	strcat(printBuffer, base[src_byte % 16]);
}

void int2char(char *printBuffer, uint8_t src_byte) {
	static const char *base[] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","^"};
	if (src_byte>16) {src_byte=16;};
	strcat(printBuffer, base[src_byte]);
}
uint8_t char2byte(char *a){
	uint8_t result=0;
	for (uint8_t i=0; i<2; i++) {
		if (i>0) {result = result << 4;};
		if ((a[i]>='0') && (a[i]<='9')) {result = result + (a[i] - '0');}
		else if ((a[i]>='A') && (a[i]<='F')) {result = result + (a[i] - 'A' + 10);}
		else if ((a[i]>='a') && (a[i]<='f')) {result = result + (a[i] - 'a' + 10);};
	};
	return result;
}

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

