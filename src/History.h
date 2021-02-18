/*
 * History.h
 *
 *  Created on: 04 ����. 2016 �.
 *      Author: BaskakovDG
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include <Arduino.h>

const uint8_t History_State_Error = 0xFF;		// B 1111 1111 ��������� ������
const uint8_t History_Category_None = 0;		// B 0000 0000 ��� ���������
const uint8_t History_Category_Error = 0xFF;	// B 1111 1111 ��� ���������
//������������ �������� ������
const uint8_t History_Category_Reader = 1;		// B 0000 0001 ������� BoardReader 0
const uint8_t History_Category_Sender = 2;		// B 0000 0010 ������� BoardSender 0


class History {
	public:
	History ();
	uint8_t *category;
	uint8_t *pin;
	uint8_t *state;
	static const int8_t size = 24;
	void add(uint8_t __Category, uint8_t __Pin, uint8_t __State);
	void clear();
	uint8_t printHistory(char *printBuffer, uint8_t index_begin, uint8_t print_index_max);
};

#endif /* HISTORY_H_ */
