#ifndef SWITCHCOMMON_H_
#define SWITCHCOMMON_H_

#include <Arduino.h>

void byte2char(char *printBuffer, uint8_t src_byte);
void int2char(char *printBuffer, uint8_t src_byte);
uint8_t char2byte(char *a);
int freeRam ();
/*
int8_t		|	char				|	�� -128 �� 127
uint8_t		|	byte, unsigned char	|	�� 0 �� 255
int16_t		|	int					|	�� -32768 �� 32767
uint16_t	|	unsigned int, word	|	�� 0 �� 65535
int32_t 	|	long				|	��  -2147483648 �� 2147483647
uint32_t	|	unsigned long		|	�� 0 �� 4294967295
*/
#endif /* SENSOR_H_ */
