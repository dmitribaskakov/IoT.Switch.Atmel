#include <Arduino.h>
#include "SwitchCommon.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "BoardReader.h"
#include "BoardSender.h"
#include "History.h"
#include "SignalWorker.h"

bool SignalWorker::isRequiredRead() {
	return (reader->hasSignal() || reader->expectCompletion());
}
bool SignalWorker::isRequiredSend() {
	return sender->hasChanged();
}
bool SignalWorker::read() {
	bool result_read = false;
	if (reader->hasSignal() || reader->expectCompletion()) {
		result_read |= reader->read();
	};
	return result_read;
}
void SignalWorker::clearReader() {
	reader->clear();
}
void SignalWorker::clearSender() {
	sender->clear();
}
bool SignalWorker::send() {
	if (sender->hasChanged()) {
		sender->send();
	};
	return true;
}
uint8_t SignalWorker::printHistory(uint8_t _Category, char *printBuffer, uint8_t index_begin, uint8_t print_index_max) {
	uint8_t index = index_begin;
	uint8_t printed_index = 0;
	while ((printed_index < print_index_max) && (index < history->size)) {
		if ((_Category & History_Category_Reader) && (history->category[index] == History_Category_Reader)) {
			if (reader->printHistoryByIndex(index, printBuffer)) printed_index++;
			} else if ((_Category & History_Category_Sender) && (history->category[index] == History_Category_Sender)) {
			if (sender->printHistoryByIndex(index, printBuffer)) printed_index++;
		};
		index++;
	};
	return index;
}
void SignalWorker::initialize(History *_history, BoardReader *_reader, BoardSender *_sender) {
	history = _history;
	reader = _reader;
	sender = _sender;
	history->clear();
	for(uint8_t i = 0x60; i < 0x80; i++) {
		sender->inverted(i); // Для SRD
	};

	for(uint8_t i = 0; i < reader->pins(); i++) {
		reader->disable(i);
	};

	//sender->enable(SSR36);// 0. Автомат	Розетки для Автоматики
	//sender->enable(SSR38);// 0. Автомат	Розетки для Интернет оборудования
	//sender->enable(SSR37);// 0. Автомат	Розетки для Сетевого оборудования
	//sender->enable(SSR76);// 0. Улица	Автоматика
	sender->enable(SSR75);// 0. Улица	Все розетки вокруг дома
	//sender->enable(SSR77);// 0. Улица	Звонок и Домофон
	//sender->enable(SSR80);// 0. Улица	Свет Дежурное
	//sender->enable(SSR81);// 0. Улица	Свет Зона отдыха
	//sender->enable(SSR79);// 0. Улица	Свет Крыльцо
	sender->enable(SSR78);// 0. Улица	Свет Переднее

	sender->enable(SSR74);// 0. Ц Cтирка	Все розетки постирочной
	sender->enable(SSR69);// 0. Ц Cтирка	Свет
	sender->enable(SSR73);// 0. Ц Большая	Все розетки большой комнаты
	sender->enable(SSR65);// 0. Ц Большая	Свет 1
	sender->enable(SSR66);// 0. Ц Большая	Свет 2
	sender->enable(SSR70);// 0. Ц Гардероб	Свет
	sender->enable(SSR71);// 0. Ц Кладовая	Свет
	sender->enable(SSR35);// 0. Ц Котельная	Розетки в Котельной
	sender->enable(SSR68);// 0. Ц Котельная	Свет
	sender->enable(SSR67);// 0. Ц Лестница	Свет
	sender->enable(SRD38);// 0. Ц Сауна	Вентиляция в цоколе 0э
	sender->enable(SSR72);// 0. Ц Сауна	Свет

	//sender->enable(SSR55);// 1.1. Зал	Кондиционер 12
	//sender->enable(SSR44);// 1.1. Зал	Розетка для варочной поверхности
	//sender->enable(SSR46);// 1.1. Зал	Розетка для духовки 1
	//sender->enable(SSR45);// 1.1. Зал	Розетка для духовки 2
	//sender->enable(SSR41);// 1.1. Зал	Розетки для аудио и видео
	//sender->enable(SSR43);// 1.1. Зал	Розетки для кухни
	//sender->enable(SSR42);// 1.1. Зал	Розетки для кухни и балкона
	sender->enable(SSR61, 20000, SSR62);// 1.1. Зал Рольставни - Окно - закрыть
	sender->enable(SSR62, 20000, SSR61);// 1.1. Зал Рольставни - Окно - открыть
	sender->enable(SSR63, 30000, SSR64);// 1.1. Зал Рольставни - Балкон - закрыть
	sender->enable(SSR64, 30000, SSR63);// 1.1. Зал Рольставни - Балкон - открыть
	//sender->enable(SSR18);// 1.1. Зал	Свет в кухне ???
	//sender->enable(SSR19);// 1.1. Зал	Свет над обеденным столом ???
	sender->enable(SSR15);// 1.1. Зал	Свет основной 1
	sender->enable(SSR16);// 1.1. Зал	Свет основной 2
	sender->enable(SSR17);// 1.1. Зал	Свет по периметру
	//sender->enable(SRD23);// 1.1. Зал	Свет подсветка LED 1
	//sender->enable(SRD24);// 1.1. Зал	Свет подсветка LED 2
	//sender->enable(SRD25);// 1.1. Зал	Свет подсветка LED 3

	//sender->enable(SSR56);// 1.2. Гостевая Кондиционер 07
	//sender->enable(SSR39);// 1.2. Гостевая Розетки для всей гостевой комнаты
	sender->enable(SSR57, 20000, SSR58);// 1.2. Гостевая Рольставни - Окно - закрыть
	sender->enable(SSR58, 20000, SSR57);// 1.2. Гостевая Рольставни - Окно - открыть
	sender->enable(SSR20);// 1.2. Гостевая	Свет основной 1
	sender->enable(SSR21);// 1.2. Гостевая	Свет основной 2
	sender->enable(SSR22);//1.2. Гостевая	Свет по периметру
	//sender->enable(SRD26);// 1.2. Гостевая	Свет подсветка LED 1
	//sender->enable(SRD27);// 1.2. Гостевая	Свет подсветка LED 2
	//sender->enable(SRD28);// 1.2. Гостевая	Свет подсветка LED 3

	sender->enable(SSR59, 20000, SSR60);// 1.3. Прихожая Рольставни - Окно - закрыть
	sender->enable(SSR60, 20000, SSR59);// 1.3. Прихожая Рольставни - Окно - открыть
	sender->enable(SSR28);// 1.3. Прихожая	Свет по периметру
	sender->enable(SRD36);// 1.4. Туалет	Вентиляция в туалете 1э
	sender->enable(SSR49);// 1.4. Туалет	Свет
	sender->enable(SRD37);// 1.5. Ванная	Вентиляция в ванной 1э
	//sender->enable(SSR47);// 1.5. Ванная	Розетки в ванной комнате и на стене в холле
	sender->enable(SSR48);// 1.5. Ванная	Свет
	//sender->enable(SSR40);// 1.5. Холл	Розетки для холла и прихожей
	sender->enable(SSR26);// 1.5. Холл	Свет основной
	sender->enable(SSR27);// 1.5. Холл	Свет по периметру

	//sender->enable(SSR30);// 2.1. Спальня	Все розетки в спальне и часть розеток в кабинете
	//sender->enable(SSR52);// 2.1. Спальня	Кондиционер 09
	//sender->enable(SSR03);// 2.1. Спальня	Свет в гардеробной
	sender->enable(SRD03);// 2.1. Спальня	Свет над кроватью слева
	sender->enable(SRD02);// 2.1. Спальня	Свет над кроватью справа
	sender->enable(SSR01);// 2.1. Спальня	Свет основной 1
	sender->enable(SRD01);// 2.1. Спальня	Свет основной 2
	sender->enable(SSR02);// 2.1. Спальня	Свет по периметру
	//sender->enable(SRD04);// 2.1. Спальня	Свет подсветка LED 1
	//sender->enable(SRD05);// 2.1. Спальня	Свет подсветка LED 2
	//sender->enable(SRD06);// 2.1. Спальня	Свет подсветка LED 3
	//sender->enable(SSR33);// 2.2. Кабинет	Безопастные розетки для компьютера
	//sender->enable(SSR29);// 2.2. Кабинет	Розетки для принтера и возле дивана
	sender->enable(SSR05);// 2.2. Кабинет	Свет над столом
	sender->enable(SSR04);// 2.2. Кабинет	Свет основной 1
	sender->enable(SRD07);// 2.2. Кабинет	Свет основной 2
	sender->enable(SSR06);// 2.2. Кабинет	Свет по периметру
	//sender->enable(SRD08);// 2.2. Кабинет	Свет подсветка LED 1
	//sender->enable(SRD11);// 2.2. Кабинет	Свет подсветка LED 2
	//sender->enable(SRD12);// 2.2. Кабинет	Свет подсветка LED 3

	//sender->enable(SSR34);// 2.4. Мишина	Безопастные розетки для компьютера, совмещены со второй детской
	//sender->enable(SSR53);// 2.4. Мишина	Кондиционер 07
	//sender->enable(SSR31);// 2.4. Мишина	Розетки совмещены со второй детской
	sender->enable(SSR10);// 2.4. Мишина	Свет над кроватью
	sender->enable(SSR08);// 2.4. Мишина	Свет над столом
	sender->enable(SSR07);// 2.4. Мишина	Свет основной 1
	sender->enable(SRD13);// 2.4. Мишина	Свет основной 2
	sender->enable(SSR09);// 2.4. Мишина	Свет по периметру
	//sender->enable(SRD14);// 2.4. Мишина	Свет подсветка LED 1
	//sender->enable(SRD15);// 2.4. Мишина	Свет подсветка LED 2
	//sender->enable(SRD16);// 2.4. Мишина	Свет подсветка LED 3

	//sender->enable(SSR54);// 2.5. Костина	Кондиционер 07
	//sender->enable(SSR32);// 2.5. Костина	Розетки совмещены со второй детской
	sender->enable(SSR14);// 2.5. Костина	Свет над кроватью
	sender->enable(SSR12);// 2.5. Костина	Свет над столом
	sender->enable(SSR11);// 2.5. Костина	Свет основной 1
	sender->enable(SRD17);// 2.5. Костина	Свет основной 2
	sender->enable(SSR13);// 2.5. Костина	Свет по периметру
	//sender->enable(SRD18);// 2.5. Костина	Свет подсветка LED 1
	//sender->enable(SRD21);// 2.5. Костина	Свет подсветка LED 2
	//sender->enable(SRD22);// 2.5. Костина	Свет подсветка LED 3

	sender->enable(SRD35);// 2.6. Туалет	Вентиляция в ванной 2э
	sender->enable(SSR50);// 2.6. Туалет	Свет
	sender->enable(SRD34);// 2.7. Ванная	Вентиляция в туалетете 2э
	sender->enable(SSR51);// 2.7. Ванная	Свет
	sender->enable(SSR23);// 2.9. Холл	Свет основной 1
	sender->enable(SSR24);// 2.9. Холл	Свет основной 2
	sender->enable(SSR25);// 2.9. Холл	Свет по периметру
	//sender->enable(SRD31);// 2.9. Холл	Свет подсветка LED 1
	//sender->enable(SRD32);// 2.9. Холл	Свет подсветка LED 2
	//sender->enable(SRD33);// 2.9. Холл	Свет подсветка LED 3


	//reader->enable(Sw51);// 0. Цоколь - В комнате
	//reader->enable(Sw52);// 0. Цоколь - В комнате
	//reader->enable(Sw53);// 0. Цоколь - В котельной
	reader->enable(Sw45, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 1.1. Зал - На входе
	reader->enable(Sw46);// 1.1. Зал - На входе
	reader->enable(Sw47);// 1.1. Зал - На входе
	reader->enable(Sw48);// 1.1. Зал - На входе
	reader->enable(Sw49, Signal_Type_Button_Click);// 1.1. Зал - Рольставней на балконе - Верхняя левая Окно - закрыть
	reader->enable(Sw4A, Signal_Type_Button_Click);// 1.1. Зал - Рольставней на балконе - Верхняя правая Окно - открыть
	reader->enable(Sw4B, Signal_Type_Button_Click);// 1.1. Зал - Рольставней на балконе - Нижняя левая Балкон - закрыть
	reader->enable(Sw4C, Signal_Type_Button_Click);// 1.1. Зал - Рольставней на балконе - Нижняя правая Балкон - открыть
	//reader->enable(Sw4D);// 1.1. Зал - На балконе
	//reader->enable(Sw4E);// 1.1. Зал - На балконе
	//reader->enable(Sw4F);// 1.1. Зал - На балконе
	reader->enable(Sw26, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 1.2. Гостевая - На входе
	reader->enable(Sw27);// 1.2. Гостевая - На входе
	//reader->enable(Sw28);// 1.2. Гостевая - На входе
	//reader->enable(Sw29);// 1.2. Гостевая - На входе
	reader->enable(Sw2A, Signal_Type_Button_Click);// 1.2. Гостевая - Рольставней Окно - закрыть
	reader->enable(Sw2B, Signal_Type_Button_Click);// 1.2. Гостевая - Рольставней Окно - открыть
	reader->enable(Sw31);// 1.3. Прихожая - На входе
	reader->enable(Sw32);// 1.3. Прихожая - На входе
	//reader->enable(Sw33);// 1.3. Прихожая - На входе
	//reader->enable(Sw34);// 1.3. Прихожая - На входе
	reader->enable(Sw35);// 1.3. Прихожая - На входе
	reader->enable(Sw36);// 1.3. Прихожая - На входе
	//reader->enable(Sw37);// 1.3. Прихожая - На входе
	//reader->enable(Sw38);// 1.3. Прихожая - На входе
	reader->enable(Sw39, Signal_Type_Button_Click);// 1.3. Прихожая - Рольставней Окно - закрыть
	reader->enable(Sw3A, Signal_Type_Button_Click);// 1.3. Прихожая - Рольставней Окно - открыть
	reader->enable(Sw2C);// 1.5. Холл - Ванна туалет холл
	reader->enable(Sw2D);// 1.5. Холл - Ванна туалет холл
	//reader->enable(Sw2E);// 1.5. Холл - Ванна туалет холл
	//reader->enable(Sw2F);// 1.5. Холл - Ванна туалет холл
	reader->enable(Sw3B);// 1.5. Холл - На выходе из прихожей
	reader->enable(Sw3C, Signal_Type_Button_Click | Signal_Type_Button_LongClick);// 1.5. Холл - На выходе из прихожей
	//reader->enable(Sw3D);// 1.5. Холл - На выходе из прихожей
	//reader->enable(Sw3E);// 1.5. Холл - На выходе из прихожей
	reader->enable(Sw3F, Signal_Type_Button_Click | Signal_Type_Button_LongClick);// 1.5. Холл - На лестнице в подвал
	reader->enable(Sw41, Signal_Type_Button_Click | Signal_Type_Button_LongClick);// 1.5. Холл - Возле лестницы
	reader->enable(Sw42, Signal_Type_Button_Click | Signal_Type_Button_LongClick);// 1.5. Холл - Возле лестницы
	reader->enable(Sw43);// 1.5. Холл - Возле лестницы
	reader->enable(Sw44);// 1.5. Холл - Возле лестницы
	reader->enable(Sw05, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.1. Спальня - В комнате
	reader->enable(Sw06);// 2.1. Спальня - В комнате
	reader->enable(Sw07);// 2.1. Спальня - В комнате
	reader->enable(Sw08);// 2.1. Спальня - В комнате
	reader->enable(Sw09);// 2.1. Спальня - Над кроватью справа
	reader->enable(Sw0A);// 2.1. Спальня - Над кроватью справа
	reader->enable(Sw0B);// 2.1. Спальня - Над кроватью слева
	reader->enable(Sw0C);// 2.1. Спальня - Над кроватью слева
	reader->enable(Sw01, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.2. Кабинет - В комнате
	reader->enable(Sw02);// 2.2. Кабинет - В комнате
	reader->enable(Sw03);// 2.2. Кабинет - В комнате
	reader->enable(Sw04);// 2.2. Кабинет - В комнате
	reader->enable(Sw19, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.4. Мишина - На входе
	reader->enable(Sw1A);// 2.4. Мишина - На входе
	reader->enable(Sw1B);// 2.4. Мишина - На входе
	reader->enable(Sw1C);// 2.4. Мишина - На входе
	reader->enable(Sw22, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick);// 2.4. Мишина - Возле кровати
	reader->enable(Sw23);// 2.4. Мишина - Возле кровати
	reader->enable(Sw1D, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.5. Костина - На входе
	reader->enable(Sw1E);// 2.5. Костина - На входе
	reader->enable(Sw1F);// 2.5. Костина - На входе
	reader->enable(Sw21);// 2.5. Костина - На входе
	reader->enable(Sw24, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick);// 2.5. Костина - Возле кровати
	reader->enable(Sw25);// 2.5. Костина - Возле кровати
	reader->enable(Sw0D, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.9. Холл - На выходе из кабинета
	reader->enable(Sw0E);// 2.9. Холл - На выходе из кабинета
	reader->enable(Sw0F);// 2.9. Холл - На выходе из кабинета
	reader->enable(Sw11);// 2.9. Холл - На выходе из кабинета
	reader->enable(Sw12);// 2.9. Холл - Ванна туалет холл
	reader->enable(Sw13, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.9. Холл - Ванна туалет холл
	reader->enable(Sw14);// 2.9. Холл - Ванна туалет холл
	//reader->enable(Sw15);// 2.9. Холл - Ванна туалет холл
	reader->enable(Sw16, Signal_Type_Button_Click | Signal_Type_Button_DoubleClick | Signal_Type_Button_LongClick);// 2.9. Холл - На выходе из детских
	reader->enable(Sw17);// 2.9. Холл - На выходе из детских
	//reader->enable(Sw18);// 2.9. Холл - На выходе из детских

	sender->send();
}
void SignalWorker::process() {
	if (!reader->hasChanged()) return;
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 0. Цоколь
	//
	//if (reader->isButtonClick(Sw51)) {/* 0. Цоколь - В комнате */};
	//if (reader->isButtonClick(Sw52)) {/* 0. Цоколь - В комнате */};
	if (reader->isButtonClick(Sw53)) {/* 0. Цоколь - В котельной */
		sender->invert(SSR68);// 0. Ц Котельная	Свет
	};

	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// Рольставни
	//

	if (reader->isButtonClick(Sw4B)) {/* 1.1. Зал - Рольставней на балконе - Нижняя левая Балкон - закрыть */
		if (sender->isOff(SSR63) && sender->isOff(SSR64)) {
			sender->on(SSR63);// 1.1. Зал	Рольставни - Балкон - закрыть
		} else {
			sender->off(SSR63);// 1.1. Зал	Рольставни - Балкон - закрыть
			sender->off(SSR64);// 1.1. Зал	Рольставни - Балкон - открыть
		}
	};
	if (reader->isButtonClick(Sw4C)) {/* 1.1. Зал - Рольставней на балконе - Нижняя правая Балкон - открыть */
		if (sender->isOff(SSR63) && sender->isOff(SSR64)) {
			sender->on(SSR64);// 1.1. Зал	Рольставни - Балкон - открыть
		} else {
			sender->off(SSR63);// 1.1. Зал	Рольставни - Балкон - закрыть
			sender->off(SSR64);// 1.1. Зал	Рольставни - Балкон - открыть
		}
	};

	if (reader->isButtonClick(Sw49)) {/* 1.1. Зал - Рольставней на балконе - Верхняя левая Окно - закрыть */
		if (sender->isOff(SSR61) && sender->isOff(SSR62)) {
			sender->on(SSR61);// 1.1. Зал	Рольставни - Окно - закрыть
		} else {
			sender->off(SSR61);// 1.1. Зал	Рольставни - Окно - закрыть
			sender->off(SSR62);// 1.1. Зал	Рольставни - Окно - открыть
		}
	};
	if (reader->isButtonClick(Sw4A)) {/* 1.1. Зал - Рольставней на балконе - Верхняя правая Окно - открыть */
		if (sender->isOff(SSR61) && sender->isOff(SSR62)) {
			sender->on(SSR62);// 1.1. Зал	Рольставни - Окно - открыть
		} else {
			sender->off(SSR61);// 1.1. Зал	Рольставни - Окно - закрыть
			sender->off(SSR62);// 1.1. Зал	Рольставни - Окно - открыть
		}
	};

	if (reader->isButtonClick(Sw39)) {/* 1.3. Прихожая - Рольставней Окно - закрыть */
		if (sender->isOff(SSR59) && sender->isOff(SSR60)) {
			sender->on(SSR59);// 1.3. Прихожая Рольставни - Окно - закрыть
		} else {
			sender->off(SSR59);// 1.3. Прихожая	Рольставни - Окно - закрыть
			sender->off(SSR60);// 1.3. Прихожая	Рольставни - Окно - открыть
		}
	};
	if (reader->isButtonClick(Sw3A)) {/* 1.3. Прихожая - Рольставней Окно - открыть */
		if (sender->isOff(SSR59) && sender->isOff(SSR60)) {
			sender->on(SSR60);// 1.3. Прихожая Рольставни - Окно - открыть
		} else {
			sender->off(SSR59);// 1.3. Прихожая	Рольставни - Окно - закрыть
			sender->off(SSR60);// 1.3. Прихожая	Рольставни - Окно - открыть
		}
	};

	if (reader->isButtonClick(Sw2A)) {/* 1.2. Гостевая - Рольставней Окно - закрыть */
		if (sender->isOff(SSR57) && sender->isOff(SSR58)) {
			sender->on(SSR57);// 1.2. Гостевая Рольставни - Окно - закрыть
		} else {
			sender->off(SSR57);// 1.2. Гостевая Рольставни - Окно - закрыть
			sender->off(SSR58);// 1.2. Гостевая	Рольставни - Окно - открыть
		}
	};
	if (reader->isButtonClick(Sw2B)) {/* 1.2. Гостевая - Рольставней Окно - открыть */
		if (sender->isOff(SSR57) && sender->isOff(SSR58)) {
			sender->on(SSR58);// 1.2. Гостевая	Рольставни - Окно - открыть
		} else {
			sender->off(SSR57);// 1.2. Гостевая Рольставни - Окно - закрыть
			sender->off(SSR58);// 1.2. Гостевая	Рольставни - Окно - открыть
		}
	};

	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 1.1. Зал
	//
	if (reader->isButtonLongClick(Sw45)) {/* 1.1. Зал - На входе */
		if (sender->isOn(SSR16) && sender->isOff(SSR15) && sender->isOff(SSR17)) {
			sender->on(SSR15);// 1.1. Зал - Свет основной 1
			} else if (sender->isOn(SSR15) && sender->isOn(SSR16) && sender->isOff(SSR17)) {
			sender->off(SSR15);// 1.1. Зал - Свет основной 1
			} else if (sender->isOn(SSR15) || sender->isOn(SSR16) || sender->isOn(SSR17)) {
			sender->off(SSR15);// 1.1. Зал - Свет основной 1
			sender->off(SSR16);// 1.1. Зал - Свет основной 2
			sender->off(SSR17);// 1.1. Зал - Свет по периметру
			} else {
			sender->on(SSR15);// 1.1. Зал - Свет основной 1
			sender->on(SSR16);// 1.1. Зал - Свет основной 2
		};
		} else if (reader->isButtonDoubleClick(Sw45)) {/* 1.1. Зал - На входе */
		if (sender->isOn(SSR15) && sender->isOn(SSR16)) {
			sender->off(SSR15);// 1.1. Зал - Свет основной 1
			} else {
			sender->on(SSR15);// 1.1. Зал - Свет основной 1
			sender->on(SSR16);// 1.1. Зал - Свет основной 2
		};
		} else if (reader->isButtonClick(Sw45)) {/* 1.1. Зал - На входе */
		if (sender->isOn(SSR15) || sender->isOn(SSR16)) {
			sender->off(SSR15);// 1.1. Зал - Свет основной 1
			sender->off(SSR16);// 1.1. Зал - Свет основной 2
			} else {
			sender->on(SSR16);// 1.1. Зал - Свет основной 2
		};
	};
	if (reader->isButtonClick(Sw46)) {/* 1.1. Зал - На входе */
		sender->invert(SSR17);// 1.1. Зал - Свет по периметру
	};
	if (reader->isButtonClick(Sw47)) {/* 1.1. Зал - На входе */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	if (reader->isButtonClick(Sw48)) {/* 1.1. Зал - На входе */
		if (sender->isOn(SSR26) || sender->isOn(SSR27)) {
			sender->off(SSR26);// 1.5. Холл - Свет основной
			sender->off(SSR27);// 1.5. Холл - Свет по периметру
			} else {
			sender->on(SSR26);// 1.5. Холл - Свет основной
		};
	};
	
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 1.2. Гостевая
	//
	if (reader->isButtonLongClick(Sw26)) {/* 1.2. Гостевая - На входе */
		if (sender->isOn(SSR20) && sender->isOff(SSR21) && sender->isOff(SSR22)) {
			sender->on(SSR21);// 1.2. Гостевая - Свет основной 2
			} else if (sender->isOn(SSR20) && sender->isOn(SSR21) && sender->isOff(SSR22)) {
			sender->off(SSR21);// 1.2. Гостевая - Свет основной 2
			} else if (sender->isOn(SSR20) || sender->isOn(SSR21) || sender->isOn(SSR22)) {
			sender->off(SSR20);// 1.2. Гостевая - Свет основной 1
			sender->off(SSR21);// 1.2. Гостевая - Свет основной 2
			sender->off(SSR22);// 1.2. Гостевая - Свет по периметру
			} else {
			sender->on(SSR20);// 1.2. Гостевая - Свет основной 1
			sender->on(SSR21);// 1.2. Гостевая - Свет основной 2
		};
		} else if (reader->isButtonDoubleClick(Sw26)) {/* 1.2. Гостевая - На входе */
		if (sender->isOn(SSR20) && sender->isOn(SSR21)) {
			sender->off(SSR21);// 1.2. Гостевая - Свет основной 2
			} else {
			sender->on(SSR20);// 1.2. Гостевая - Свет основной 1
			sender->on(SSR21);// 1.2. Гостевая - Свет основной 2
		};
		} else if (reader->isButtonClick(Sw26)) {/* 1.2. Гостевая - На входе */
		if (sender->isOn(SSR20) || sender->isOn(SSR21)) {
			sender->off(SSR20);// 1.2. Гостевая - Свет основной 1
			sender->off(SSR21);// 1.2. Гостевая - Свет основной 2
			} else {
			sender->on(SSR20);// 1.2. Гостевая - Свет основной 1
		};
	};
	if (reader->isButtonClick(Sw27)) {/* 1.2. Гостевая - На входе */
		sender->invert(SSR22);// 1.2. Гостевая - Свет по периметру
	};
	//if (reader->isButtonClick(Sw28)) {/* 1.2. Гостевая - На входе */};
	//if (reader->isButtonClick(Sw29)) {/* 1.2. Гостевая - На входе */};

	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 1.3. Прихожая
	//
	//if (reader->isButtonClick(Sw31)) {/* 1.3. Прихожая - На входе */};
	if (reader->isButtonClick(Sw32)) {/* 1.3. Прихожая - На входе */
		runScript(Scr01); //	Никого нет дома
		runScript(0x0D); //	1. Рольставни - закрыть
	};
	//if (reader->isButtonClick(Sw33)) {/* 1.3. Прихожая - На входе */};
	//if (reader->isButtonClick(Sw34)) {/* 1.3. Прихожая - На входе */};
	if (reader->isButtonClick(Sw35)) {/* 1.3. Прихожая - На входе */
		sender->invert(SSR28);// 1.3. Прихожая - Свет по периметру
	};
	//if (reader->isButtonClick(Sw36)) {/* 1.3. Прихожая - На входе */};
	//if (reader->isButtonClick(Sw37)) {/* 1.3. Прихожая - На входе */};
	//if (reader->isButtonClick(Sw38)) {/* 1.3. Прихожая - На входе */};

	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 1.5. Холл
	//
	if (reader->isButtonClick(Sw2C)) {/* 1.5. Холл - Ванна туалет холл */
		sender->invert(SSR49);// 1.4. Туалет - Свет
	};
	if (reader->isButtonClick(Sw2D)) {/* 1.5. Холл - Ванна туалет холл */
		sender->invert(SSR48);// 1.5. Ванная - Свет
	};
	if (reader->isButtonClick(Sw3B)) {/* 1.5. Холл - На выходе из прихожей */
		sender->invert(SSR28);// 1.3. Прихожая - Свет по периметру
	};
	if (reader->isButtonLongClick(Sw3C)) {/* 1.5. Холл - На выходе из прихожей */
		if (sender->isOn(SSR26) || sender->isOn(SSR27)) {
			sender->off(SSR26);// 1.5. Холл - Свет основной
			sender->off(SSR27);// 1.5. Холл - Свет по периметру
			} else {
			sender->on(SSR26);// 1.5. Холл - Свет основной
			sender->on(SSR27);// 1.5. Холл - Свет по периметру
		};
		} else if (reader->isButtonDoubleClick(Sw3C)) {/* 1.5. Холл - На выходе из прихожей */
		if (sender->isOff(SSR26) && sender->isOn(SSR27)) {
			sender->on(SSR26);// 1.5. Холл - Свет основной
			} else if (sender->isOn(SSR26) && sender->isOff(SSR27)) {
			sender->on(SSR27);// 1.5. Холл - Свет по периметру
			} else {
			sender->invert(SSR26);// 1.5. Холл - Свет основной
			sender->invert(SSR27);// 1.5. Холл - Свет по периметру
		};
		} else if (reader->isButtonClick(Sw3C)) {/* 1.5. Холл - На выходе из прихожей */
		if (sender->isOn(SSR26) || sender->isOn(SSR27)) {
			sender->off(SSR26);// 1.5. Холл - Свет основной
			sender->off(SSR27);// 1.5. Холл - Свет по периметру
			} else {
			sender->on(SSR26);// 1.5. Холл - Свет основной
		};
	};
	//if (reader->isButtonClick(Sw3D)) {/* 1.5. Холл - На выходе из прихожей */};
	//if (reader->isButtonClick(Sw3E)) {/* 1.5. Холл - На выходе из прихожей */};
	if (reader->isButtonLongClick(Sw41)) {/* 1.5. Холл - Возле лестницы */
		if (sender->isOn(SSR15) || sender->isOn(SSR16) || sender->isOn(SSR17) || sender->isOn(SSR20)
		|| sender->isOn(SSR21) || sender->isOn(SSR22) || sender->isOn(SSR28) || sender->isOn(SSR49)
		|| sender->isOn(SSR48) || sender->isOn(SSR26) || sender->isOn(SSR27)) {
			runScript(Scr04); //	1.1. Зал - Выключить свет
			runScript(Scr05); //	1.2. Гостевая - Выключить свет
			runScript(Scr06); //	1.3. Прихожая - Выключить свет
			runScript(Scr07); //	1.5. Холл - Выключить свет
			} else {
			sender->on(SSR26);// 0x21 1.5. Холл - Свет основной
			sender->on(SSR27);// 0x20 1.5. Холл - Свет по периметру
		};
		} else if (reader->isButtonClick(Sw41)) {/* 1.5. Холл - Возле лестницы */
		sender->invert(SSR26);// 0x21 1.5. Холл - Свет основной
	};
	if (reader->isButtonLongClick(Sw42)) {/* 1.5. Холл - Возле лестницы */
		if (sender->isOn(SSR01) || sender->isOn(SRD01) || sender->isOn(SSR02) || sender->isOn(SSR03)
		|| sender->isOn(SSR04) || sender->isOn(SRD07) || sender->isOn(SSR05) || sender->isOn(SSR06)
		|| sender->isOn(SSR07) || sender->isOn(SRD13) || sender->isOn(SSR08) || sender->isOn(SSR09)
		|| sender->isOn(SSR10) || sender->isOn(SSR12) || sender->isOn(SSR11) || sender->isOn(SRD17)
		|| sender->isOn(SSR13) || sender->isOn(SSR14) || sender->isOn(SSR50) || sender->isOn(SSR51)
		|| sender->isOn(SSR23) || sender->isOn(SSR24) || sender->isOn(SSR25)) {
			runScript(Scr08); //	2.1. Спальня - Выключить свет
			runScript(Scr09); //	2.2. Кабинет - Выключить свет
			runScript(Scr0A); //	2.4. Мишина - Выключить свет
			runScript(Scr0B); //	2.5. Костина - Выключить свет
			runScript(Scr0C); //	2.9. Холл - Выключить свет
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
			sender->on(SSR24);// 2.9. Холл - Свет основной 2
			sender->on(SSR25);// 2.9. Холл - Свет по периметру
		};
		} else if (reader->isButtonClick(Sw42)) {/* 1.5. Холл - Возле лестницы */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	if (reader->isButtonLongClick(Sw3F)) {/* 1.5. Холл - На лестнице в подвал */
		if (sender->isOn(SSR65) || sender->isOn(SSR66)) {
			sender->off(SSR65);// 0. Ц Большая - Свет 1
			sender->off(SSR66);// 0. Ц Большая - Свет 2
			} else {
			sender->on(SSR65);// 0. Ц Большая - Свет 1
			sender->on(SSR66);// 0. Ц Большая - Свет 2
		};
		} else if (reader->isButtonClick(Sw3F)) {/* 1.5. Холл - На лестнице в подвал */
		if (sender->isOn(SSR65) || sender->isOn(SSR66)) {
			sender->off(SSR65);// 0. Ц Большая - Свет 1
			sender->off(SSR66);// 0. Ц Большая - Свет 2
			} else {
			sender->on(SSR65);// 0. Ц Большая - Свет 1
		};
	};
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 2.1. Спальня
	//
	if (reader->isButtonLongClick(Sw05)) {/* 2.1. Спальня - В комнате */
		if (sender->isOn(SSR01) && sender->isOff(SRD01) && sender->isOff(SSR02) && sender->isOff(SSR03)) {
			sender->on(SRD01);// 2.1. Спальня - Свет основной 2
			} else if (sender->isOn(SSR01) && sender->isOn(SRD01) && sender->isOff(SSR02) && sender->isOff(SSR03)) {
			sender->off(SRD01);// 2.1. Спальня - Свет основной 2
			} else if (sender->isOn(SSR01) || sender->isOn(SRD01) || sender->isOn(SSR02) || sender->isOn(SSR03)) {
			sender->off(SSR01);// 2.1. Спальня - Свет основной 1
			sender->off(SRD01);// 2.1. Спальня - Свет основной 2
			sender->off(SSR02);// 2.1. Спальня - Свет по периметру
			sender->off(SSR03);// 2.1. Спальня - Свет в гардеробной
			} else {
			sender->on(SSR01);// 2.1. Спальня - Свет основной 1
			sender->on(SRD01);// 2.1. Спальня - Свет основной 2
			sender->on(SSR03);// 2.1. Спальня - Свет в гардеробной
		};
		} else if (reader->isButtonDoubleClick(Sw05)) {/* 2.1. Спальня - В комнате */
		if (sender->isOn(SSR01) && sender->isOn(SRD01)) {
			sender->off(SRD01);// 2.1. Спальня - Свет основной 2
			} else {
			sender->on(SSR01);// 2.1. Спальня - Свет основной 1
			sender->on(SRD01);// 2.1. Спальня - Свет основной 2
		};
		} else if (reader->isButtonClick(Sw05)) {/* 2.1. Спальня - В комнате */
		if (sender->isOn(SSR01) || sender->isOn(SRD01)) {
			sender->off(SSR01);// 2.1. Спальня - Свет основной 1
			sender->off(SRD01);// 2.1. Спальня - Свет основной 2
			} else {
			sender->on(SSR01);// 2.1. Спальня - Свет основной 1
		};
	};
	if (reader->isButtonClick(Sw06)) {/* 2.1. Спальня - В комнате */
		sender->invert(SSR02);// 2.1. Спальня - Свет по периметру
	};
	//if (reader->isButtonClick(Sw07)) {/* 2.1. Спальня - В комнате */};
	if (reader->isButtonClick(Sw08)) {/* 2.1. Спальня - В комнате */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// D 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	//Sw0A /* 2.1. Спальня - Над кроватью справа */
	//Sw0B /* 2.1. Спальня - Над кроватью слева */
	if (reader->isButtonClick(Sw0A) || reader->isButtonClick(Sw0B)) {
		if (sender->isOn(SSR01) || sender->isOn(SRD01) || sender->isOn(SSR02) || sender->isOn(SSR03)) {
			sender->off(SSR01);// 2.1. Спальня - Свет основной 1
			sender->off(SRD01);// 2.1. Спальня - Свет основной 2
			sender->off(SSR02);// 2.1. Спальня - Свет по периметру
			sender->off(SSR03);// 2.1. Спальня - Свет в гардеробной
			} else {
			sender->on(SSR01);// 2.1. Спальня - Свет основной 1
		};
	};
	//Sw09 /* 2.1. Спальня - Над кроватью справа */
	//Sw0C /* 2.1. Спальня - Над кроватью слева */
	if (reader->isButtonClick(Sw09) || reader->isButtonClick(Sw0C)) {
		if (sender->isOn(SSR04) || sender->isOn(SRD07) || sender->isOn(SSR06)){
			sender->off(SSR04);// 2.2. Кабинет - Свет основной 1
			sender->off(SRD07);//	2.2. Кабинет - Свет основной 2
			sender->off(SSR06);// 2.2. Кабинет - Свет по периметру
			} else {
			sender->on(SSR04);// 2.2. Кабинет - Свет основной 1
		};
	};
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 2.2. Кабинет
	//
	if (reader->isButtonLongClick(Sw01)) {/* 2.2. Кабинет - В комнате */
		if (sender->isOn(SSR04) && sender->isOff(SRD07) && sender->isOff(SSR06)) {
			sender->on(SRD07);// 2.2. Кабинет - Свет основной 2
			sender->on(SSR05);// 2.2. Кабинет - Свет над столом
			} else if (sender->isOn(SSR04) && sender->isOn(SRD07) && sender->isOff(SSR06)) {
			sender->off(SRD07);// 2.2. Кабинет - Свет основной 2
			} else if (sender->isOn(SSR04) || sender->isOn(SRD07) || sender->isOn(SSR06)) {
			sender->off(SSR04);// 2.2. Кабинет - Свет основной 1
			sender->off(SRD07);// 2.2. Кабинет - Свет основной 2
			sender->off(SSR06);// 2.2. Кабинет - Свет по периметру
			sender->off(SSR05);// 2.2. Кабинет - Свет над столом
			} else {
			sender->on(SSR04);// 2.2. Кабинет - Свет основной 1
			sender->on(SRD07);// 2.2. Кабинет - Свет основной 2
			sender->on(SSR05);// 2.2. Кабинет - Свет над столом
		};
		} else if (reader->isButtonDoubleClick(Sw01)) {/* 2.2. Кабинет - В комнате */
		if (sender->isOn(SSR04) && sender->isOn(SRD07)) {
			sender->off(SRD07);// 1.1. Зал - Свет основной 1
			} else {
			sender->on(SSR04);// 2.2. Кабинет - Свет основной 1
			sender->on(SRD07);// 2.2. Кабинет - Свет основной 2
		};
		} else if (reader->isButtonClick(Sw01)) {/* 2.2. Кабинет - В комнате */
		if (sender->isOn(SSR04)){
			sender->off(SSR04);// 2.2. Кабинет - Свет основной 1
			} else {
			sender->on(SSR04);// 2.2. Кабинет - Свет основной 1
			//sender->on(SSR05);// 2.2. Кабинет - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw02)) {/* 2.2. Кабинет - В комнате */
		if (sender->isOn(SSR06)){
			sender->off(SSR06);// 2.2. Кабинет - Свет по периметру
			} else {
			sender->on(SSR06);// 2.2. Кабинет - Свет по периметру
			//sender->on(SSR05);// 2.2. Кабинет - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw03)) {/* 2.2. Кабинет - В комнате */
		sender->invert(SSR05);// 2.2. Кабинет - Свет над столом
	};
	if (reader->isButtonClick(Sw04)) {/* 2.2. Кабинет - В комнате */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 2.4. Мишина
	//
	if (reader->isButtonLongClick(Sw19)) {/* 2.4. Мишина - На входе */
		if (sender->isOn(SSR07) && sender->isOff(SRD13) && sender->isOff(SSR09)) {
			sender->on(SSR07);// 2.4. Мишина - Свет основной 1
			sender->on(SRD13);// 2.4. Мишина - Свет основной 2
			} else if (sender->isOn(SSR07) && sender->isOn(SRD13) && sender->isOff(SSR09)) {
			sender->off(SRD13);// 2.4. Мишина - Свет основной 2
			} else if (sender->isOn(SSR07) || sender->isOn(SRD13) || sender->isOn(SSR08) || sender->isOn(SSR09) || sender->isOn(SSR10)) {
			sender->off(SSR07);// 2.4. Мишина - Свет основной 1
			sender->off(SRD13);// 2.4. Мишина - Свет основной 2
			sender->off(SSR08);// 2.4. Мишина - Свет над столом
			sender->off(SSR09);// 2.4. Мишина - Свет по периметру
			sender->off(SSR10);// 2.4. Мишина - Свет над кроватью
			} else {
			sender->on(SSR07);// 2.4. Мишина - Свет основной 1
			sender->on(SRD13);// 2.4. Мишина - Свет основной 2
			sender->on(SSR08);// 2.4. Мишина - Свет над столом
		};
		} else if (reader->isButtonDoubleClick(Sw19)) {/* 2.4. Мишина - На входе */
		if (sender->isOn(SSR07) && sender->isOn(SRD13)) {
			sender->off(SRD13);// 0x2A 2.4. Мишина - Свет основной 2
			} else {
			sender->on(SSR07);// 2.4. Мишина - Свет основной 1
			sender->on(SRD13);// 2.4. Мишина - Свет основной 2
			sender->on(SSR08);// 2.4. Мишина - Свет над столом
		};
		} else if (reader->isButtonClick(Sw19)) {/* 2.4. Мишина - На входе */
		if (sender->isOn(SSR07) || sender->isOn(SRD13)) {
			sender->off(SSR07);// 2.4. Мишина - Свет основной 1
			sender->off(SRD13);// 2.4. Мишина - Свет основной 2
			} else {
			sender->on(SSR07);// 2.4. Мишина - Свет основной 1
			sender->on(SSR08);// 2.4. Мишина - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw1A)) {/* 2.4. Мишина - На входе */
		if (sender->isOn(SSR09)) {
			sender->off(SSR09);// 2.4. Мишина - Свет по периметру
			} else {
			sender->on(SSR09);// 2.4. Мишина - Свет по периметру
			sender->on(SSR08);// 2.4. Мишина - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw1B)) {/* 2.4. Мишина - На входе */
		sender->invert(SSR08);// 2.4. Мишина - Свет над столом
	};
	if (reader->isButtonClick(Sw1C)) {/* 2.4. Мишина - На входе */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// D 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	if (reader->isButtonDoubleClick(Sw22)) {/* 2.4. Мишина - Возле кровати */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) || sender->isOn(SSR25)) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			sender->off(SSR25);// 2.9. Холл - Свет по периметру
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
		} else if (reader->isButtonClick(Sw22)) {/* 2.4. Мишина - Возле кровати */
		if (sender->isOn(SSR10)) {
			sender->off(SSR10);// 2.4. Мишина - Свет над кроватью
			} else {
			sender->on(SSR10);// 2.4. Мишина - Свет над кроватью
		};
	};
	if (reader->isButtonClick(Sw23)) {/* 2.4. Мишина - Возле кровати */
		if (sender->isOn(SSR07) || sender->isOn(SRD13) || sender->isOn(SSR08) || sender->isOn(SSR09)) {
			sender->off(SSR07);// 2.4. Мишина - Свет основной 1
			sender->off(SRD13);// 2.4. Мишина - Свет основной 2
			sender->off(SSR08);// 2.4. Мишина - Свет над столом
			sender->off(SSR09);// 2.4. Мишина - Свет по периметру
			} else {
			sender->on(SSR07);// 2.4. Мишина - Свет основной 1
		};
	};
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 2.5. Костина
	//
	if (reader->isButtonLongClick(Sw1D)) {/* 2.5. Костина - На входе */
		if (sender->isOn(SSR11) && sender->isOff(SRD17) && sender->isOff(SSR13)) {
			sender->on(SSR11);// 2.5. Костина - Свет основной 1
			sender->on(SRD17);// 2.5. Костина - Свет основной 2
			} else if (sender->isOn(SSR11) && sender->isOn(SRD17) && sender->isOff(SSR13)) {
			sender->off(SRD17);// 2.5. Костина - Свет основной 2
			} else if (sender->isOn(SSR11) || sender->isOn(SRD17) || sender->isOn(SSR12) || sender->isOn(SSR13) || sender->isOn(SSR14)) {
			sender->off(SSR11);// 2.5. Костина - Свет основной 1
			sender->off(SRD17);// 2.5. Костина - Свет основной 2
			sender->off(SSR12);// 2.5. Костина - Свет над столом
			sender->off(SSR13);// 2.5. Костина - Свет по периметру
			sender->off(SSR14);// 2.5. Костина - Свет над кроватью
			} else {
			sender->on(SSR11);// 2.5. Костина - Свет основной 1
			sender->on(SRD17);// 2.5. Костина - Свет основной 2
			sender->on(SSR12);// 2.5. Костина - Свет над столом
		};
		} else if (reader->isButtonDoubleClick(Sw1D)) {/* 2.5. Костина - На входе */
		if (sender->isOn(SSR11) && sender->isOn(SRD17)) {
			sender->off(SRD17);// 2.5. Костина - Свет основной 2
			} else {
			sender->on(SSR11);// 2.5. Костина - Свет основной 1
			sender->on(SRD17);// 2.5. Костина - Свет основной 2
			sender->on(SSR12);// 2.5. Костина - Свет над столом
		};
		} else if (reader->isButtonClick(Sw1D)) {/* 2.5. Костина - На входе */
		if (sender->isOn(SSR11) || sender->isOn(SRD17)) {
			sender->off(SSR11);// 2.5. Костина - Свет основной 1
			sender->off(SRD17);// 2.5. Костина - Свет основной 2
			} else {
			sender->on(SSR11);// 2.5. Костина - Свет основной 1
			sender->on(SSR12);// 2.5. Костина - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw1E)) {/* 2.5. Костина - На входе */
		if (sender->isOn(SSR13)) {
			sender->off(SSR13);// 2.5. Костина - Свет по периметру
			} else {
			sender->on(SSR12);// 2.5. Костина - Свет над столом
			sender->on(SSR13);// 2.5. Костина - Свет по периметру
		};
	};
	if (reader->isButtonClick(Sw1F)) {/* 2.5. Костина - На входе */
		sender->invert(SSR12);// 2.5. Костина - Свет над столом
	};
	if (reader->isButtonClick(Sw21)) {/* 2.5. Костина - На входе */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	if (reader->isButtonDoubleClick(Sw24)) {/* 2.5. Костина - Возле кровати */
		if (sender->isOn(SSR23) || sender->isOn(SSR24) || sender->isOn(SSR25)) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			sender->off(SSR25);// 2.9. Холл - Свет по периметру
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
		} else if (reader->isButtonClick(Sw24)) {/* 2.5. Костина - Возле кровати */
		if (sender->isOn(SSR11) || sender->isOn(SRD17)|| sender->isOn(SSR12) || sender->isOn(SSR13)) {
			sender->off(SSR11);// 2.5. Костина - Свет основной 1
			sender->off(SRD17);// 2.5. Костина - Свет основной 2
			sender->off(SSR12);// 2.5. Костина - Свет над столом
			sender->off(SSR13);// 2.5. Костина - Свет по периметру
			} else {
			sender->on(SSR11);// 2.5. Костина - Свет основной 1
		};
	};
	if (reader->isButtonClick(Sw25)) {/* 2.5. Костина - Возле кровати */
		if (sender->isOn(SSR14)) {
			sender->off(SSR14);// 2.5. Костина - Свет над кроватью
			} else {
			sender->on(SSR14);// 2.5. Костина - Свет над кроватью
		};
	};
	// ==============================================================================================================
	// ==============================================================================================================
	// ==============================================================================================================
	//
	// 2.9. Холл и лестн
	//
	//Sw0D	/* 2.9. Холл - На выходе из кабинета */
	//Sw13	/* 2.9. Холл - Ванна туалет холл */
	//Sw16	/* 2.9. Холл - На выходе из детских */
	if (reader->isButtonLongClick(Sw0D) || reader->isButtonLongClick(Sw13) || reader->isButtonLongClick(Sw16)) {
		if (sender->isOn(SSR23) && sender->isOff(SSR25)) {
			sender->on(SSR24);// 2.9. Холл - Свет основной 2
			sender->on(SSR25);// 2.9. Холл - Свет по периметру
			} else if (sender->isOff(SSR23) && sender->isOn(SSR25)) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->on(SSR24);// 2.9. Холл - Свет основной 2
			} else if (sender->isOn(SSR23) && sender->isOn(SSR25)) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			sender->off(SSR25);// 2.9. Холл - Свет по периметру
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
			sender->on(SSR24);// 2.9. Холл - Свет основной 2
			sender->on(SSR25);// 2.9. Холл - Свет по периметру
		};
		} else if (reader->isButtonDoubleClick(Sw0D) || reader->isButtonDoubleClick(Sw13) || reader->isButtonDoubleClick(Sw16)) {
		if (sender->isOn(SSR23) && sender->isOn(SSR24) ) {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
			sender->on(SSR24);// 2.9. Холл - Свет основной 2
		};
		} else if (reader->isButtonClick(Sw0D) || reader->isButtonClick(Sw13) || reader->isButtonClick(Sw16)) {
		if (sender->isOn(SSR23) || sender->isOn(SSR24) ) {
			sender->off(SSR23);// 2.9. Холл - Свет основной 1
			sender->off(SSR24);// 2.9. Холл - Свет основной 2
			} else {
			sender->on(SSR23);// 2.9. Холл - Свет основной 1
		};
	};
	//Sw0E	/* 2.9. Холл - На выходе из кабинета */
	//Sw17	/* 2.9. Холл - На выходе из детских */
	if (reader->isButtonClick(Sw0E) || reader->isButtonClick(Sw17)) {
		sender->invert(SSR25);// 2.9. Холл - Свет по периметру
	};
	if (reader->isButtonClick(Sw11)) {/* 2.9. Холл - На выходе из кабинета */
		if (sender->isOn(SSR26) || sender->isOn(SSR27) ){
			sender->off(SSR26);// 1.5. Холл - Свет основной
			sender->off(SSR27);// 1.5. Холл - Свет по периметру
			} else {
			sender->on(SSR26);// 1.5. Холл - Свет основной
		};
	};
	if (reader->isButtonClick(Sw0F)) {/* 2.9. Холл - На выходе из кабинета */
		if (sender->isOn(SSR04) || sender->isOn(SSR05) ){
			sender->off(SSR04);// 2.2. Кабинет - Свет основной 1
			sender->off(SSR05);// 2.2. Кабинет - Свет над столом
			} else {
			sender->on(SSR04);// 2.2. Кабинет - Свет основной 1
			//sender->on(SSR05);// 2.2. Кабинет - Свет над столом
		};
	};
	if (reader->isButtonClick(Sw12)) {/* 2.9. Холл - Ванна туалет холл */
		sender->invert(SSR50);// 2.6. Туалет - Свет
	};
	if (reader->isButtonClick(Sw14)) {/* 2.9. Холл - Ванна туалет холл */
		sender->invert(SSR51);// 2.7. Ванная - Свет
	};
}
bool SignalWorker::runScript(uint8_t script) {
	switch (script) {
	case Scr01: //	Никого нет дома
		runScript(Scr02); //	0. Улица - Выключить свет
		runScript(Scr03); //	0. Цоколь - Выключить свет
		runScript(Scr04); //	1.1. Зал - Выключить свет
		runScript(Scr05); //	1.2. Гостевая - Выключить свет
		runScript(Scr06); //	1.3. Прихожая - Выключить свет
		runScript(Scr07); //	1.5. Холл - Выключить свет
		runScript(Scr08); //	2.1. Спальня - Выключить свет
		runScript(Scr09); //	2.2. Кабинет - Выключить свет
		runScript(Scr0A); //	2.4. Мишина - Выключить свет
		runScript(Scr0B); //	2.5. Костина - Выключить свет
		runScript(Scr0C); //	2.9. Холл - Выключить свет
		break;
	case Scr02: //	0. Улица - Выключить свет
		sender->off(SSR80);//	0. Улица	Свет Дежурное
		sender->off(SSR81);//	0. Улица	Свет Зона отдыха
		sender->off(SSR79);//	0. Улица	Свет Крыльцо
		sender->off(SSR78);//	0. Улица	Свет Переднее
		break;
	case Scr03: //	0. Цоколь - Выключить свет
		sender->off(SSR69);//	0. Ц Cтирка	Свет
		sender->off(SSR65);//	0. Ц Большая	Свет 1
		sender->off(SSR66);//	0. Ц Большая	Свет 2
		sender->off(SSR70);//	0. Ц Гардероб	Свет
		sender->off(SSR71);//	0. Ц Кладовая	Свет
		sender->off(SSR68);//	0. Ц Котельная	Свет
		sender->off(SSR67);//	0. Ц Лестница	Свет
		sender->off(SSR72);//	0. Ц Сауна	Свет
		break;
	case Scr04: //	1.1. Зал - Выключить свет
		sender->off(SSR18);//	1.1. Зал	Свет в кухне ???	С30	2х1,5
		sender->off(SSR19);//	1.1. Зал	Свет над обеденным столом ???	С31	2х1,5
		sender->off(SSR15);//	1.1. Зал	Свет основной 1	С32	3х1,5
		sender->off(SSR16);//	1.1. Зал	Свет основной 2	С32	3х1,5
		sender->off(SSR17);//	1.1. Зал	Свет по периметру	С29	2х1,5
		sender->off(SRD23);//	1.1. Зал	Свет подсветка LED 1	С32.2	5х1,5
		sender->off(SRD24);//	1.1. Зал	Свет подсветка LED 2	С32.2	5х1,5
		sender->off(SRD25);//	1.1. Зал	Свет подсветка LED 3	С32.2	5х1,5
		break;
	case Scr05: //	1.2. Гостевая - Выключить свет
		sender->off(SSR20);//	1.2. Гостевая	Свет основной 1	С21	2х1,5
		sender->off(SSR21);//	1.2. Гостевая	Свет основной 2	С21	2х1,5
		sender->off(SSR22);//	1.2. Гостевая	Свет по периметру	С22	2х1,5
		sender->off(SRD26);//	1.2. Гостевая	Свет подсветка LED 1	С21.2	5х1,5
		sender->off(SRD27);//	1.2. Гостевая	Свет подсветка LED 2	С21.2	5х1,5
		sender->off(SRD28);//	1.2. Гостевая	Свет подсветка LED 3	С21.2	5х1,5
		break;
	case Scr06: //	1.3. Прихожая - Выключить свет
		sender->off(SSR28);//	1.3. Прихожая	Свет по периметру	С26	2х1,5
		break;
	case Scr07: //	1.5. Холл - Выключить свет
		sender->off(SSR49);//	1.4. Туалет	Свет	С23	2х1,5
		sender->off(SSR48);//	1.5. Ванная	Свет	С24	2х1,5
		sender->off(SSR26);//	1.5. Холл	Свет основной	С27	2х1,5
		sender->off(SSR27);//	1.5. Холл	Свет по периметру	С28	2х1,5
		break;
	case Scr08: //	2.1. Спальня - Выключить свет
		sender->off(SSR03);//	2.1. Спальня	Свет в гардеробной	С08	2х1,5
		sender->off(SRD03);//	2.1. Спальня	Свет над кроватью слева	С05	2х1,5
		sender->off(SRD02);//	2.1. Спальня	Свет над кроватью справа	С04	2х1,5
		sender->off(SSR01);//	2.1. Спальня	Свет основной 1	С06	3х1,5
		sender->off(SRD01);//	2.1. Спальня	Свет основной 2	С06	3х1,5
		sender->off(SSR02);//	2.1. Спальня	Свет по периметру	С07	2х1,5
		sender->off(SRD04);//	2.1. Спальня	Свет подсветка LED 1	С06.2	5х1,5
		sender->off(SRD05);//	2.1. Спальня	Свет подсветка LED 2	С06.2	5х1,5
		sender->off(SRD06);//	2.1. Спальня	Свет подсветка LED 3	С06.2	5х1,5
		break;
	case Scr09: //	2.2. Кабинет - Выключить свет
		sender->off(SSR05);//	2.2. Кабинет	Свет над столом	С01	2х1,5
		sender->off(SSR04);//	2.2. Кабинет	Свет основной 1	С02	3х1,5
		sender->off(SRD07);//	2.2. Кабинет	Свет основной 2	С02	3х1,5
		sender->off(SSR06);//	2.2. Кабинет	Свет по периметру	С03	2х1,5
		sender->off(SRD08);//	2.2. Кабинет	Свет подсветка LED 1	С02.2	5х1,5
		sender->off(SRD11);//	2.2. Кабинет	Свет подсветка LED 2	С02.2	5х1,5
		sender->off(SRD12);//	2.2. Кабинет	Свет подсветка LED 3	С02.2	5х1,5
		break;
	case Scr0A: //	2.4. Мишина - Выключить свет
		sender->off(SSR10);//	2.4. Мишина	Свет над кроватью	С13	2х1,5
		sender->off(SSR08);//	2.4. Мишина	Свет над столом	С15	2х1,5
		sender->off(SSR07);//	2.4. Мишина	Свет основной 1	С14	3х1,5
		sender->off(SRD13);//	2.4. Мишина	Свет основной 2	С14	3х1,5
		sender->off(SSR09);//	2.4. Мишина	Свет по периметру	С12	2х1,5
		sender->off(SRD14);//	2.4. Мишина	Свет подсветка LED 1	С14.2	5х1,5
		sender->off(SRD15);//	2.4. Мишина	Свет подсветка LED 2	С14.2	5х1,5
		sender->off(SRD16);//	2.4. Мишина	Свет подсветка LED 3	С14.2	5х1,5
		break;
	case Scr0B: //	2.5. Костина - Выключить свет
		sender->off(SSR14);//	2.5. Костина	Свет над кроватью	С19	2х1,5
		sender->off(SSR12);//	2.5. Костина	Свет над столом	С16	2х1,5
		sender->off(SSR11);//	2.5. Костина	Свет основной 1	С17	2х1,5
		sender->off(SRD17);//	2.5. Костина	Свет основной 2	С17	2х1,5
		sender->off(SSR13);//	2.5. Костина	Свет по периметру	С18	2х1,5
		sender->off(SRD18);//	2.5. Костина	Свет подсветка LED 1	С17.2	5х1,5
		sender->off(SRD21);//	2.5. Костина	Свет подсветка LED 2	С17.2	5х1,5
		sender->off(SRD22);//	2.5. Костина	Свет подсветка LED 3	С17.2	5х1,5
		break;
	case Scr0C: //	2.9. Холл - Выключить свет
		sender->off(SSR50);//	2.6. Туалет	Свет	С09	2х1,5
		sender->off(SSR51);//	2.7. Ванная	Свет	С11	2х1,5
		sender->off(SSR23);//	2.9. Холл	Свет основной 1	С20	3х1,5
		sender->off(SSR24);//	2.9. Холл	Свет основной 2	С20	3х1,5
		sender->off(SSR25);//	2.9. Холл	Свет по периметру	С10	2х1,5
		sender->off(SRD31);//	2.9. Холл	Свет подсветка LED 1	С20.2	5х1,5
		sender->off(SRD32);//	2.9. Холл	Свет подсветка LED 2	С20.2	5х1,5
		sender->off(SRD33);//	2.9. Холл	Свет подсветка LED 3	С20.2	5х1,5
		break;
	case Scr0D: //1. Рольставни - закрыть
		sender->off(SSR63);// 1.1. Зал Рольставни - Окно - открыть
		sender->off(SSR64);// 1.1. Зал Рольставни - Балкон - открыть
		sender->off(SSR58);// 1.2. Гостевая Рольставни - Окно - открыть
		sender->off(SSR60);// 1.3. Прихожая Рольставни - Окно - открыть
		if (sender->isOff(SSR61)) {
			sender->on(SSR61);// 1.1. Зал Рольставни - Окно - закрыть
		};
		if (sender->isOff(SSR62)) {
			sender->on(SSR62);// 1.1. Зал Рольставни - Балкон - закрыть
		};
		if (sender->isOff(SSR57)) {
			sender->on(SSR57);// 1.2. Гостевая Рольставни - Окно - закрыть
		};
		if (sender->isOff(SSR59)) {
			sender->on(SSR59);// 1.3. Прихожая Рольставни - Окно - закрыть
		};
		break;
	case Scr0E: //1. Рольставни - открыть
		sender->off(SSR61);// 1.1. Зал Рольставни - Окно - закрыть
		sender->off(SSR62);// 1.1. Зал Рольставни - Балкон - закрыть
		sender->off(SSR57);// 1.2. Гостевая Рольставни - Окно - закрыть
		sender->off(SSR59);// 1.3. Прихожая Рольставни - Окно - закрыть
		if (sender->isOff(SSR63)) {
			sender->on(SSR63);// 1.1. Зал Рольставни - Окно - открыть
		};
		if (sender->isOff(SSR64)) {
			sender->on(SSR64);// 1.1. Зал Рольставни - Балкон - открыть
		};
		if (sender->isOff(SSR63)) {
			sender->on(SSR58);// 1.2. Гостевая Рольставни - Окно - открыть
		};
		if (sender->isOff(SSR63)) {
			sender->on(SSR60);// 1.3. Прихожая Рольставни - Окно - открыть
		};
		break;
	};
	return true;
}