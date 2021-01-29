#ifndef SWITCHCOMMON_H_
#define SWITCHCOMMON_H_

#include <Arduino.h>

void byte2char(char *printBuffer, uint8_t src_byte);
void int2char(char *printBuffer, uint8_t src_byte);
uint8_t char2byte(char *a);
int freeRam ();
/*
int8_t		|	char				|	от -128 до 127
uint8_t		|	byte, unsigned char	|	от 0 до 255
int16_t		|	int					|	от -32768 до 32767
uint16_t	|	unsigned int, word	|	от 0 до 65535
int32_t 	|	long				|	от  -2147483648 до 2147483647
uint32_t	|	unsigned long		|	от 0 до 4294967295
*/
#endif /* SENSOR_H_ */
