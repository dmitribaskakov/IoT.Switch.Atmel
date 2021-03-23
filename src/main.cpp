#include <Arduino.h>

#include "WString.h"
#include "SwitchCommon.h"
#include "SignalButton.h"
#include "SignalSwith.h"
#include "BoardReader.h"
#include "BoardSender.h"
#include "History.h"
#include "LCDWithButtons.h"
#include "SignalWorker.h"
#include "Ethernet2.h"
#include "mqttClient.h"
#include "StatusLed.h"
uint32_t timeToStartMainLoop = 0;
#define timeMainLoopMax 25  // время для сна
StatusLed statusLed(13);


void setup();
void loop();

History history;
BoardReader reader;
BoardSender sender;
SignalWorker worker;
StatusLed statusLed(13);

//LCDWithButtons	lcd(	46,	48,	45,	47,	49,	51,	15, 44);	// это для старого боевого
//LCDWithButtons	lcd(	44,	46,	43,	45,	47,	49,	15, 42);	// это для боевого
LCDWithButtons		lcd(	8,	9,	4,	5,	6,	7,	0, 10);		// это для теста

byte mac_address[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };	// это для боевого
//IPAddress local_ip(192, 168, 1, 42);							// это для боевого
IPAddress mqttServer(192, 168, 1, 39);							// это для боевого mosquito on orion

//byte mac_address[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };	// это для теста
//IPAddress local_ip(192, 168, 1, 51);							// это для теста
//IPAddress mqttServer(192, 168, 1, 44);							// это для теста mosquito on prometey

//byte mac_address[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x97, 0xFB };	

EthernetClient ethernetClient;
MQTTClient mqttClient(mqttServer, 1883, ethernetClient);

uint32_t timeToStartMainLoop=0;
#define timeMainLoopMax 25
#define Serial_baud_count 115200

void setup() {
	//Serial.begin(Serial_baud_count);
	reader.initialize(&history, History_Category_Reader);
	sender.initialize(&history, History_Category_Sender);
	worker.initialize(&history, &reader, &sender);
	lcd.initialize(&history, &worker);
	mqttClient.initialize(mac_address, &sender);
	mqttClient.reconnect();
}

void loop() {
	timeToStartMainLoop = millis();
	worker.clearReader();	//новый цикл. все кнопки были ранее обработаны. очищаем состояния кнопок чтобы не обработать их повторно
	worker.clearSender();	//новый цикл. все реле ранее обработаны. очищаем состояния реле чтобы не обработать их повторно
	statusLed.update();		//мигнем светодиодом что контроллер жив
	lcd.processButton();	//опрос кнопок на LCD1602
	lcd.refresh |= worker.read();// Чтение сигнала с плат 74hc165
	worker.process(); 		//обработка сигналов по правилам
	mqttClient.process(timeToStartMainLoop);	//обработка mqttClient
	if (worker.isRequiredSend()) lcd.refresh |= worker.send();	//запись в 74hc595
	if (! reader.expectCompletion()) mqttClient.keepAlive(timeToStartMainLoop);
	if (lcd.refresh) lcd.updateDisplay(timeToStartMainLoop);
	timeToStartMainLoop = millis() - timeToStartMainLoop;
	if (timeToStartMainLoop<timeMainLoopMax) delay(timeMainLoopMax - timeToStartMainLoop);
}
