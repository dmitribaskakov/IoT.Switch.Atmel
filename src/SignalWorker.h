#ifndef SIGNALWORKER_H_
#define SIGNALWORKER_H_

#include <Arduino.h>
#include "SwitchCommon.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "BoardReader.h"
#include "BoardSender.h"
#include "History.h"



//Сиволы для вывода на дисплей
//const char symbol_Reader[2] = {(uint8_t)6, (uint8_t)0}; //6;
//const char symbol_Sender[2] = {(uint8_t)7, (uint8_t)0}; //7;

//const char *const msg = "?? oops: error: ";

#define	SRD01	0x67	//	2.1. Спальня
PROGMEM const char name_SRD01[] = "Свет основной 2";
#define	SRD02	0x66	//	2.1. Спальня
PROGMEM const char name_SRD02[] = "Свет над кроватью справа";
#define	SRD03	0x65	//	2.1. Спальня
PROGMEM const char name_SRD03[] = "Свет над кроватью слева";
#define	SRD04	0x64	//	2.1. Спальня
PROGMEM const char name_SRD04[] = "Свет подсветка LED 1";
#define	SRD05	0x63	//	2.1. Спальня
PROGMEM const char name_SRD05[] = "Свет подсветка LED 2";
#define	SRD06	0x62	//	2.1. Спальня
PROGMEM const char name_SRD06[] = "Свет подсветка LED 3";
#define	SRD07	0x61	//	2.2. Кабинет
PROGMEM const char name_SRD07[] = "Свет основной 2";
#define	SRD08	0x60	//	2.2. Кабинет
PROGMEM const char name_SRD08[] = "Свет подсветка LED 1";
#define	SRD11	0x68	//	2.2. Кабинет
PROGMEM const char name_SRD11[] = "Свет подсветка LED 2";
#define	SRD12	0x69	//	2.2. Кабинет
PROGMEM const char name_SRD12[] = "Свет подсветка LED 3";
#define	SRD13	0x6A	//	2.4. Мишина
PROGMEM const char name_SRD13[] = "Свет основной 2";
#define	SRD14	0x6B	//	2.4. Мишина
PROGMEM const char name_SRD14[] = "Свет подсветка LED 1";
#define	SRD15	0x6C	//	2.4. Мишина
PROGMEM const char name_SRD15[] = "Свет подсветка LED 2";
#define	SRD16	0x6D	//	2.4. Мишина
PROGMEM const char name_SRD16[] = "Свет подсветка LED 3";
#define	SRD17	0x6E	//	2.5. Костина
PROGMEM const char name_SRD17[] = "Свет основной 2";
#define	SRD18	0x6F	//	2.5. Костина
PROGMEM const char name_SRD18[] = "Свет подсветка LED 1";
#define	SRD21	0x77	//	2.5. Костина
PROGMEM const char name_SRD21[] = "Свет подсветка LED 2";
#define	SRD22	0x76	//	2.5. Костина
PROGMEM const char name_SRD22[] = "Свет подсветка LED 3";
#define	SRD23	0x75	//	1.1. Зал
PROGMEM const char name_SRD23[] = "Свет подсветка LED 1";
#define	SRD24	0x74	//	1.1. Зал
PROGMEM const char name_SRD24[] = "Свет подсветка LED 2";
#define	SRD25	0x73	//	1.1. Зал
PROGMEM const char name_SRD25[] = "Свет подсветка LED 3";
#define	SRD26	0x72	//	1.2. Гостевая
PROGMEM const char name_SRD26[] = "Свет подсветка LED 1";
#define	SRD27	0x71	//	1.2. Гостевая
PROGMEM const char name_SRD27[] = "Свет подсветка LED 2";
#define	SRD28	0x70	//	1.2. Гостевая
PROGMEM const char name_SRD28[] = "Свет подсветка LED 3";
#define	SRD31	0x78	//	2.9. Холл
PROGMEM const char name_SRD31[] = "Свет подсветка LED 1";
#define	SRD32	0x79	//	2.9. Холл
PROGMEM const char name_SRD32[] = "Свет подсветка LED 2";
#define	SRD33	0x7A	//	2.9. Холл
PROGMEM const char name_SRD33[] = "Свет подсветка LED 3";
#define	SRD34	0x7B	//	2.7. Ванная
PROGMEM const char name_SRD34[] = "Вентиляция в ванной 2э";
#define	SRD35	0x7C	//	2.6. Туалет
PROGMEM const char name_SRD35[] = "Вентиляция в туалете 2э";
#define	SRD36	0x7D	//	1.4. Туалет
PROGMEM const char name_SRD36[] = "Вентиляция в туалете 1э";
#define	SRD37	0x7E	//	1.5. Ванная
PROGMEM const char name_SRD37[] = "Вентиляция в ванной 1э";
#define	SRD38	0x7F	//	0. Ц Сауна
PROGMEM const char name_SRD38[] = "Вентиляция в цоколе 0э";
#define	SSR01	0x10	//	2.1. Спальня
PROGMEM const char name_SSR01[] = "Свет основной 1";
#define	SSR02	0x15	//	2.1. Спальня
PROGMEM const char name_SSR02[] = "Свет по периметру";
#define	SSR03	0x14	//	2.1. Спальня
PROGMEM const char name_SSR03[] = "Свет в гардеробной";
#define	SSR04	0x09	//	2.2. Кабинет
PROGMEM const char name_SSR04[] = "Свет основной 1";
#define	SSR05	0x08	//	2.2. Кабинет
PROGMEM const char name_SSR05[] = "Свет над столом";
#define	SSR06	0x0D	//	2.2. Кабинет
PROGMEM const char name_SSR06[] = "Свет по периметру";
#define	SSR07	0x0C	//	2.4. Мишина
PROGMEM const char name_SSR07[] = "Свет основной 1";
#define	SSR08	0x01	//	2.4. Мишина
PROGMEM const char name_SSR08[] = "Свет над столом";
#define	SSR09	0x00	//	2.4. Мишина
PROGMEM const char name_SSR09[] = "Свет по периметру";
#define	SSR10	0x05	//	2.4. Мишина
PROGMEM const char name_SSR10[] = "Свет над кроватью";
#define	SSR11	0x04	//	2.5. Костина
PROGMEM const char name_SSR11[] = "Свет основной 1";
#define	SSR12	0x12	//	2.5. Костина
PROGMEM const char name_SSR12[] = "Свет над столом";
#define	SSR13	0x17	//	2.5. Костина
PROGMEM const char name_SSR13[] = "Свет по периметру";
#define	SSR14	0x16	//	2.5. Костина
PROGMEM const char name_SSR14[] = "Свет над кроватью";
#define	SSR15	0x0B	//	1.1. Зал
PROGMEM const char name_SSR15[] = "Свет основной 1";
#define	SSR16	0x0A	//	1.1. Зал
PROGMEM const char name_SSR16[] = "Свет основной 2";
#define	SSR17	0x0F	//	1.1. Зал
PROGMEM const char name_SSR17[] = "Свет по периметру";
#define	SSR18	0x0E	//	1.1. Зал
PROGMEM const char name_SSR18[] = "Свет в кухне ???";
#define	SSR19	0x03	//	1.1. Зал
PROGMEM const char name_SSR19[] = "Свет над обеденным столом ???";
#define	SSR20	0x02	//	1.2. Гостевая
PROGMEM const char name_SSR20[] = "Свет основной 1";
#define	SSR21	0x07	//	1.2. Гостевая
PROGMEM const char name_SSR21[] = "Свет основной 2";
#define	SSR22	0x06	//	1.2. Гостевая
PROGMEM const char name_SSR22[] = "Свет по периметру";
#define	SSR23	0x11	//	2.9. Холл
PROGMEM const char name_SSR23[] = "Свет основной 1";
#define	SSR24	0x2D	//	2.9. Холл
PROGMEM const char name_SSR24[] = "Свет основной 2";
#define	SSR25	0x2C	//	2.9. Холл
PROGMEM const char name_SSR25[] = "Свет по периметру";
#define	SSR26	0x21	//	1.5. Холл
PROGMEM const char name_SSR26[] = "Свет основной";
#define	SSR27	0x20	//	1.5. Холл
PROGMEM const char name_SSR27[] = "Свет по периметру";
#define	SSR28	0x25	//	1.3. Прихожая
PROGMEM const char name_SSR28[] = "Свет по периметру";
#define	SSR29	0x24	//	2.2. Кабинет
PROGMEM const char name_SSR29[] = "Розетки для принтера";
#define	SSR30	0x19	//	2.1. Спальня
PROGMEM const char name_SSR30[] = "Все розетки в спальне и часть розеток в кабинете";
#define	SSR31	0x18	//	2.4. Мишина
PROGMEM const char name_SSR31[] = "Розетки совмещены со второй детской";
#define	SSR32	0x1D	//	2.5. Костина
PROGMEM const char name_SSR32[] = "Розетки совмещены со второй детской";
#define	SSR33	0x1C	//	2.2. Кабинет
PROGMEM const char name_SSR33[] = "Безопастные розетки для компьютера";
#define	SSR34	0x13	//	2.4. Мишина
PROGMEM const char name_SSR34[] = "Безопастные розетки, совмещены со второй детской";
#define	SSR35	0x2F	//	0. Ц Котельная
PROGMEM const char name_SSR35[] = "Розетки в Котельной";
#define	SSR36	0x2E	//	0. Автомат
PROGMEM const char name_SSR36[] = "Розетки для Автоматики";
#define	SSR37	0x23	//	0. Автомат
PROGMEM const char name_SSR37[] = "Розетки для Сетевого оборудования";
#define	SSR38	0x22	//	0. Автомат
PROGMEM const char name_SSR38[] = "Розетки для Интернет оборудования";
#define	SSR39	0x27	//	1.2. Гостевая
PROGMEM const char name_SSR39[] = "Розетки для всей гостевой комнаты";
#define	SSR40	0x26	//	1.5. Холл
PROGMEM const char name_SSR40[] = "Розетки для холла и прихожей";
#define	SSR41	0x1B	//	1.1. Зал
PROGMEM const char name_SSR41[] = "Розетки для аудио и видео";
#define	SSR42	0x1A	//	1.1. Зал
PROGMEM const char name_SSR42[] = "Розетки для кухни и балкона";
#define	SSR43	0x1F	//	1.1. Зал
PROGMEM const char name_SSR43[] = "Розетки для кухни";
#define	SSR44	0x1E	//	1.1. Зал
PROGMEM const char name_SSR44[] = "Розетка для варочной поверхности";
#define	SSR45	0x44	//	1.1. Зал
PROGMEM const char name_SSR45[] = "Розетка для духовки 2";
#define	SSR46	0x39	//	1.1. Зал
PROGMEM const char name_SSR46[] = "Розетка для духовки 1";
#define	SSR47	0x38	//	1.5. Ванная
PROGMEM const char name_SSR47[] = "Розетки в ванной и на стене в холле";
#define	SSR48	0x3D	//	1.5. Ванная
PROGMEM const char name_SSR48[] = "Свет";
#define	SSR49	0x3C	//	1.4. Туалет
PROGMEM const char name_SSR49[] = "Свет";
#define	SSR50	0x31	//	2.6. Туалет
PROGMEM const char name_SSR50[] = "Свет";
#define	SSR51	0x30	//	2.7. Ванная
PROGMEM const char name_SSR51[] = "Свет";
#define	SSR52	0x35	//	2.1. Спальня
PROGMEM const char name_SSR52[] = "Кондиционер 09";
#define	SSR53	0x34	//	2.4. Мишина
PROGMEM const char name_SSR53[] = "Кондиционер 07";
#define	SSR54	0x29	//	2.5. Костина
PROGMEM const char name_SSR54[] = "Кондиционер 07";
#define	SSR55	0x28	//	1.1. Зал
PROGMEM const char name_SSR55[] = "Кондиционер 12";
#define	SSR56	0x46	//	1.2. Гостевая
PROGMEM const char name_SSR56[] = "Кондиционер 07";
#define	SSR57	0x3B	//	1.2. Гостевая
PROGMEM const char name_SSR57[] = "Рольставни 1";
#define	SSR58	0x3A	//	1.2. Гостевая
PROGMEM const char name_SSR58[] = "Рольставни 2";
#define	SSR59	0x3F	//	1.3. Прихожая
PROGMEM const char name_SSR59[] = "Рольставни 1";
#define	SSR60	0x3E	//	1.3. Прихожая
PROGMEM const char name_SSR60[] = "Рольставни 2";
#define	SSR61	0x33	//	1.1. Зал
PROGMEM const char name_SSR61[] = "Рольставни 1";
#define	SSR62	0x32	//	1.1. Зал
PROGMEM const char name_SSR62[] = "Рольставни 2";
#define	SSR63	0x37	//	1.1. Зал
PROGMEM const char name_SSR63[] = "Рольставни 1";
#define	SSR64	0x36	//	1.1. Зал
PROGMEM const char name_SSR64[] = "Рольставни 2";
#define	SSR65	0x2B	//	0. Ц Большая
PROGMEM const char name_SSR65[] = "Свет 1";
#define	SSR66	0x2A	//	0. Ц Большая
PROGMEM const char name_SSR66[] = "Свет 2";
#define	SSR67	0x47	//	0. Ц Лестница
PROGMEM const char name_SSR67[] = "Свет";
#define	SSR68	0x45	//	0. Ц Котельная
PROGMEM const char name_SSR68[] = "Свет";
#define	SSR69	0x54	//	0. Ц Cтирка
PROGMEM const char name_SSR69[] = "Свет";
#define	SSR70	0x4F	//	0. Ц Гардероб
PROGMEM const char name_SSR70[] = "Свет";
#define	SSR71	0x4E	//	0. Ц Кладовая
PROGMEM const char name_SSR71[] = "Свет";
#define	SSR72	0x4D	//	0. Ц Сауна
PROGMEM const char name_SSR72[] = "Свет";
#define	SSR73	0x4C	//	0. Ц Большая
PROGMEM const char name_SSR73[] = "Все розетки большой комнаты";
#define	SSR74	0x43	//	0. Ц Cтирка
PROGMEM const char name_SSR74[] = "Все розетки постирочной";
#define	SSR75	0x42	//	0. Улица
PROGMEM const char name_SSR75[] = "Все розетки вокруг дома";
#define	SSR76	0x41	//	0. Улица
PROGMEM const char name_SSR76[] = "Звонок и Домофон";
#define	SSR77	0x40	//	0. Улица
PROGMEM const char name_SSR77[] = "Автоматика";
#define	SSR78	0x49	//	0. Улица
PROGMEM const char name_SSR78[] = "Свет Переднее";
#define	SSR79	0x48	//	0. Улица
PROGMEM const char name_SSR79[] = "Свет Крыльцо";
#define	SSR80	0x4B	//	0. Улица
PROGMEM const char name_SSR80[] = "Свет Дежурное";
#define	SSR81	0x4A	//	0. Улица
PROGMEM const char name_SSR81[] = "Свет Зона отдыха";
#define	relay_name_length 90

PROGMEM const char location_0_Automatic[] = "0. Автомат";
PROGMEM const char location_0_Street[] = "0. Улица";
PROGMEM const char location_0_Washing[] = "0. Ц Cтирка";
PROGMEM const char location_0_Large[] = "0. Ц Большая";
PROGMEM const char location_0_Closet[] = "0. Ц Гардероб";
PROGMEM const char location_0_Storage[] = "0. Ц Кладовая";
PROGMEM const char location_0_Boiler[] = "0. Ц Котельная";
PROGMEM const char location_0_Ladder[] = "0. Ц Лестница";
PROGMEM const char location_0_Sauna[] = "0. Ц Сауна";
PROGMEM const char location_1_1_Hall[] = "1.1. Зал";
PROGMEM const char location_1_2_Guesthouse[] = "1.2. Гостевая";
PROGMEM const char location_1_3_Hallway[] = "1.3. Прихожая";
PROGMEM const char location_1_4_Toilet[] = "1.4. Туалет";
PROGMEM const char location_1_5_Bathroom[] = "1.5. Ванная";
PROGMEM const char location_1_5_Hall[] = "1.5. Холл";
PROGMEM const char location_2_1_Bedroom[] = "2.1. Спальня";
PROGMEM const char location_2_2_Dmitri[] = "2.2. Кабинет";
PROGMEM const char location_2_4_Michael[] = "2.4. Мишина";
PROGMEM const char location_2_5_Konstantin[] = "2.5. Костина";
PROGMEM const char location_2_6_Toilet[] = "2.6. Туалет";
PROGMEM const char location_2_7_Bathroom[] = "2.7. Ванная";
PROGMEM const char location_2_9_Hall[] = "2.9. Холл";
PROGMEM const char name_None[] = "Не определено";
PROGMEM const char location_None[] = "Не определено";
#define	location_length 36


// Выключатели:
#define	Sw01	0x01	//	1	2.2. Кабинет	В комнате	В01	5х1,5
#define	Sw02	0x02	//	2	2.2. Кабинет	В комнате	В01	5х1,5
#define	Sw03	0x03	//	3	2.2. Кабинет	В комнате	В01	5х1,5
#define	Sw04	0x04	//	4	2.2. Кабинет	В комнате	В01	5х1,5
#define	Sw05	0x05	//	5	2.1. Спальня	В комнате	В02	5х1,5
#define	Sw06	0x06	//	6	2.1. Спальня	В комнате	В02	5х1,5
#define	Sw07	0x07	//	7	2.1. Спальня	В комнате	В02	5х1,5
#define	Sw08	0x08	//	8	2.1. Спальня	В комнате	В02	5х1,5
#define	Sw09	0x09	//	9	2.1. Спальня	Над кроватью справа	В03	3х1,5
#define	Sw0A	0x0A	//	10	2.1. Спальня	Над кроватью справа	В03	3х1,5
#define	Sw0B	0x0B	//	11	2.1. Спальня	Над кроватью слева	В04	3х1,5
#define	Sw0C	0x0C	//	12	2.1. Спальня	Над кроватью слева	В04	3х1,5
#define	Sw0D	0x0D	//	13	2.9. Холл	На выходе из кабинета	В05	5х1,5
#define Sw0E	0x0E	//	14	2.9. Холл	На выходе из кабинета	В05	5х1,5
#define Sw0F	0x0F	//	15	2.9. Холл	На выходе из кабинета	В05	5х1,5
#define Sw11	0x11	//	17	2.9. Холл	На выходе из кабинета	В05	5х1,5
#define	Sw12	0x12	//	18	2.9. Холл	Ванна туалет холл	В06	5х1,5
#define	Sw13	0x13	//	19	2.9. Холл	Ванна туалет холл	В06	5х1,5
#define	Sw14	0x14	//	20	2.9. Холл	Ванна туалет холл	В06	5х1,5
#define	Sw15	0x15	//	21	2.9. Холл	Ванна туалет холл	В06	5х1,5
#define	Sw16	0x16	//	22	2.9. Холл	На выходе из детских	В07	3х1,5
#define	Sw17	0x17	//	23	2.9. Холл	На выходе из детских	В07	3х1,5
#define	Sw18	0x18	//	24	2.9. Холл	На выходе из детских	В07	3х1,5
#define	Sw19	0x19	//	25	2.4. Костина	На входе	В08	5х1,5
#define	Sw1A	0x1A	//	26	2.4. Костина	На входе	В08	5х1,5
#define	Sw1B	0x1B	//	27	2.4. Костина	На входе	В08	5х1,5
#define	Sw1C	0x1C	//	28	2.4. Костина	На входе	В08	5х1,5
#define	Sw1D	0x1D	//	29	2.5. Мишина	На входе	В09	5х1,5
#define	Sw1E	0x1E	//	30	2.5. Мишина	На входе	В09	5х1,5
#define	Sw1F	0x1F	//	31	2.5. Мишина	На входе	В09	5х1,5
#define	Sw21	0x21	//	33	2.5. Мишина	На входе	В09	5х1,5
#define	Sw22	0x22	//	34	2.4. Костина	Возле кровати	В10	3х1,5
#define	Sw23	0x23	//	35	2.4. Костина	Возле кровати	В10	3х1,5
#define	Sw24	0x24	//	36	2.5. Мишина	Возле кровати	В11	3х1,5
#define	Sw25	0x25	//	37	2.5. Мишина	Возле кровати	В11	3х1,5
#define	Sw26	0x26	//	38	1.2. Гостевая	На входе	В12	5х1,5
#define	Sw27	0x27	//	39	1.2. Гостевая	На входе	В12	5х1,5
#define	Sw28	0x28	//	40	1.2. Гостевая	На входе	В12	5х1,5
#define	Sw29	0x29	//	41	1.2. Гостевая	На входе	В12	5х1,5
#define	Sw2A	0x2A	//	42	1.2. Гостевая	Рольставней возле окна	В13	3х1,5
#define	Sw2B	0x2B	//	43	1.2. Гостевая	Рольставней возле окна	В13	3х1,5
#define	Sw2C	0x2C	//	44	1.5. Холл	Ванна туалет холл	В14	5х1,5
#define	Sw2D	0x2D	//	45	1.5. Холл	Ванна туалет холл	В14	5х1,5
#define	Sw2E	0x2E	//	46	1.5. Холл	Ванна туалет холл	В14	5х1,5
#define	Sw2F	0x2F	//	47	1.5. Холл	Ванна туалет холл	В14	5х1,5
#define	Sw31	0x31	//	49	1.3. Прихожая	На входе	В15	5х1,5
#define	Sw32	0x32	//	50	1.3. Прихожая	На входе	В15	5х1,5
#define	Sw33	0x33	//	51	1.3. Прихожая	На входе	В15	5х1,5
#define	Sw34	0x34	//	52	1.3. Прихожая	На входе	В15	5х1,5
#define	Sw35	0x35	//	53	1.3. Прихожая	На входе	В16	5х1,5
#define	Sw36	0x36	//	54	1.3. Прихожая	На входе	В16	5х1,5
#define	Sw37	0x37	//	55	1.3. Прихожая	На входе	В16	5х1,5
#define	Sw38	0x38	//	56	1.3. Прихожая	На входе	В16	5х1,5
#define	Sw39	0x39	//	57	1.3. Прихожая	Рольставней возле окна	В17	3х1,5
#define	Sw3A	0x3A	//	58	1.3. Прихожая	Рольставней возле окна	В17	3х1,5
#define	Sw3B	0x3B	//	59	1.5. Холл	На выходе из прихожей	В18	5х1,5
#define	Sw3C	0x3C	//	60	1.5. Холл	На выходе из прихожей	В18	5х1,5
#define	Sw3D	0x3D	//	61	1.5. Холл	На выходе из прихожей	В18	5х1,5
#define	Sw3E	0x3E	//	62	1.5. Холл	На выходе из прихожей	В18	5х1,5
#define	Sw3F	0x3F	//	63	1.5. Холл	На входе в цоколь	В22	2х1,5
#define	Sw41	0x41	//	65	1.5. Холл	Возле лестницы	В19	5х1,5
#define	Sw42	0x42	//	66	1.5. Холл	Возле лестницы	В19	5х1,5
#define	Sw43	0x43	//	67	1.5. Холл	Возле лестницы	В19	5х1,5
#define	Sw44	0x44	//	68	1.5. Холл	Возле лестницы	В19	5х1,5
#define	Sw45	0x45	//	69	1.1. Зал	На входе	В20	5х1,5
#define	Sw46	0x46	//	70	1.1. Зал	На входе	В20	5х1,5
#define	Sw47	0x47	//	71	1.1. Зал	На входе	В20	5х1,5
#define	Sw48	0x48	//	72	1.1. Зал	На входе	В20	5х1,5
#define	Sw49	0x49	//	73	1.1. Зал	Рольставней на балконе	В21	5х1,5
#define	Sw4A	0x4A	//	74	1.1. Зал	Рольставней на балконе	В21	5х1,5
#define	Sw4B	0x4B	//	75	1.1. Зал	Рольставней на балконе	В21	5х1,5
#define	Sw4C	0x4C	//	76	1.1. Зал	Рольставней на балконе	В21	5х1,5
#define	Sw4D	0x4D	//	77	1.1. Зал	На балконе	В21.2	3х1,5
#define	Sw4E	0x4E	//	78	1.1. Зал	На балконе	В21.2	3х1,5
#define	Sw4F	0x4F	//	79	1.1. Зал	На балконе	В21.2	3х1,5
#define	Sw51	0x51	//	81	0. Цоколь	На лестнице	В23	2х1,5
#define	Sw52	0x52	//	82	0. Цоколь	В комнате	В24	3х1,5
#define	Sw53	0x53	//	83	0. Цоколь	В комнате	В24	3х1,5
#define	Sw54	0x54	//	84	0. Цоколь	В котельной	В25	2х1,5
#define	Sw55	0x55	//	85	0. Цоколь	Под лестницей	В26	2х1,5
#define	Sw56	0x56	//	86	0. Цоколь	В мастерской	В27	3х1,5
#define	Sw57	0x57	//	87	0. Цоколь	В мастерской	В27	3х1,5
#define	Sw58	0x58	//	88	0. Цоколь	В кладовой	В28	2х1,5
#define	Sw59	0x59	//	89	0. Цоколь	В комнате 2	В29	2х1,5
#define	Sw5A	0x5A	//	90	0. Цоколь	В комнате 3	В30	2х1,5
#define	Sw5B	0x5B	//	91	0. Цоколь	В комнате 4	В31	2х1,5
#define	Sw5C	0x5C	//	92	0. Цоколь	В сауне	В32	3х1,5
#define	Sw5D	0x5D	//	93	0. Цоколь	В сауне	В33	2х1,5
#define	Sw5E	0x5E	//	94	0. Цоколь	В сауне	В34	2х1,5
#define	Sw5F	0x5F	//	95	0. Цоколь	Звонок	В35	2х1,5


// Сценарии
#define Scr00 0x00	//	пустая сцена
#define Scr01 0x01	//	Никого нет дома
#define Scr02 0x02	//	Мы дома
#define Scr03 0x03	//	0. Цоколь - Выключить свет
#define Scr04 0x04	//	1.1. Зал - Выключить свет
#define Scr05 0x05	//	1.2. Гостевая - Выключить свет
#define Scr06 0x06	//	1.3. Прихожая - Выключить свет
#define Scr07 0x07	//	1.5. Холл - Выключить свет
#define Scr08 0x08	//	2.1. Спальня - Выключить свет
#define Scr09 0x09	//	2.2. Кабинет - Выключить свет
#define Scr0A 0x0A	//	2.4. Мишина - Выключить свет
#define Scr0B 0x0B	//	2.5. Костина - Выключить свет
#define Scr0C 0x0C	//	2.9. Холл - Выключить свет
#define Scr0D 0x0D	//	1. Рольставни - закрыть
#define Scr0E 0x0E	//	1. Рольставни - открыть


class SignalWorker {
	public:
	SignalWorker() {};
	History *history;
	BoardReader *reader;
	BoardSender *sender;
	uint8_t printHistory(uint8_t _Category, char *printBuffer, uint8_t index_begin, uint8_t print_index_max);
	bool isRequiredRead();
	bool isRequiredSend();
	bool read();
	void clearReader();
	void clearSender();
	bool send();
	void initialize(History *_history, BoardReader *_reader, BoardSender *_sender);
	void process();
	bool runScript(uint8_t script);
	private:
};

#endif /* SIGNALWORKER_H_ */
