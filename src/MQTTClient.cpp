#include <Arduino.h>
#include "WString.h"
#include "MQTTClient.h"
#include "SwitchCommon.h"
//#include "WString.h"
//#include "SPI.h"
#include "Ethernet2.h"
#include "PubSubClient.h"
#include "SignalWorker.h"

//EthernetClient ethernetClient;
PROGMEM const char homie_switch[]						= "homie/switch";
PROGMEM const char homie_broadcast[]					= "homie/$broadcast/#";
PROGMEM const char homie_broadcast_update[]				= "homie/$broadcast/update";
PROGMEM const char homie_all[]							= "ALL";
/*
PROGMEM const char homie_switch_homie_t[]				= "homie/switch/$homie";
PROGMEM const char homie_switch_homie_p[]				= "3.0";
PROGMEM const char homie_switch_name_t[]				= "homie/switch/$name";
PROGMEM const char homie_switch_name_p[]				= "Управление реле на основании правил";
PROGMEM const char homie_switch_localip_t[]				= "homie/switch/$localip";
PROGMEM const char homie_switch_state_t[]				= "homie/switch/$state";
PROGMEM const char homie_switch_state_p[]				= "ready";
PROGMEM const char homie_switch_implementation_t[]		= "homie/switch/$implementation";
PROGMEM const char homie_switch_implementation_p[]		= "Atmega2560";
PROGMEM const char homie_switch_fwname_t[]				= "homie/switch/$fw/name";
PROGMEM const char homie_switch_fwname_p[]				= "С поддержкой MQTT homie 3.0";
PROGMEM const char homie_switch_fwversion_t[]			= "homie/switch/$fw/version";
PROGMEM const char homie_switch_fwversion_p[]			= "2019.05.13";
PROGMEM const char homie_switch_stats_t[]				= "homie/switch/$stats";
PROGMEM const char homie_switch_stats_p[]				= "uptime,freeheap,mainloop";
PROGMEM const char homie_switch_stats_interval_t[]		= "homie/switch/$stats/interval";
PROGMEM const char homie_switch_stats_interval_p[]		= "60";
PROGMEM const char homie_switch_nodes_t[]				= "homie/switch/$nodes";
PROGMEM const char homie_switch_nodes_p[]				= "relay[]";
PROGMEM const char homie_switch_relays_name_t[]			= "homie/switch/relay/$name";
PROGMEM const char homie_switch_relays_name_p[]			= "Управление реле";
PROGMEM const char homie_switch_relays_properties_t[]	= "homie/switch/relay/$properties";
PROGMEM const char homie_switch_relays_properties_p[]	= "name,state,location";
PROGMEM const char homie_switch_relays_type_t[]			= "homie/switch/relay/$type";
PROGMEM const char homie_switch_relays_type_p[]			= "switch";
PROGMEM const char homie_switch_relays_datatype_t[]		= "homie/switch/relay/$datatype";
PROGMEM const char homie_switch_relays_datatype_p[]		= "enum";
PROGMEM const char homie_switch_relays_format_t[]		= "homie/switch/relay/$format";
PROGMEM const char homie_switch_relays_format_p[]		= "ON,OFF,DISABLE";
PROGMEM const char homie_switch_relays_settable_t[]		= "homie/switch/relay/$settable";
PROGMEM const char homie_switch_relays_settable_p[]		= "true";
PROGMEM const char homie_switch_relays_array_t[]		= "homie/switch/relay/$array";
PROGMEM const char homie_switch_stats_uptime[]			= "homie/switch/$stats/uptime";
PROGMEM const char homie_switch_stats_mainloop[]		= "homie/switch/$stats/mainloop";
*/
PROGMEM const char homie_switch_stats_freeheap[]		= "homie/switch/$stats/freeheap";
PROGMEM const char homie_switch_relay_[]				= "homie/switch/relay_";

//PROGMEM const char homie_name[]						= "/$name";
//PROGMEM const char homie_location[]					= "/$location";

PROGMEM const char homie_state[]						= "/state";
PROGMEM const char homie_set[]							= "/set";
PROGMEM const char homie_on[]							= "ON";
PROGMEM const char homie_off[]							= "OFF";
/*
PROGMEM const char homie_disable[]						= "DISABLE";
PROGMEM const char homie_time_0[]						= ":0";
PROGMEM const char homie_0[]							= "0";
PROGMEM const char homie_time[]							= ":";
PROGMEM const char homie_array_0[]						= "0-";
PROGMEM const char homie_outtopic[]						= "homie/outtopic";
*/
#define homie_topic_length 34
#define homie_value_length 52
#define homie_transfer_length 16
MQTTClient::MQTTClient(IPAddress addr, uint16_t port, Client& client) : PubSubClient(addr, port, client) {}

void MQTTClient::callBack(char* topic, byte* payload, unsigned int length) {
// Обрабатываем строки на Arduino
// https://habr.com/ru/post/357890/
// https://habr.com/ru/post/357892/
	uint8_t len = 0;
	uint8_t index = 0;
	uint8_t i = 0;
	strcpy_P(bTopic, homie_switch_relay_);
	len = sizeof(homie_switch_relay_)-1;
	if ( memcmp(topic, bTopic, len) == 0 ) {
		// пришел топик homie/switch/relay_
		while ((i<3) & ((char)topic[len+i]!=(char)'/')) i++;
		if (i>0) strncpy(bTransfer, &topic[len], i);
		bTransfer[i] = 0;
		if (!isdigit(bTransfer[0])) return;
		index = len+i;
		strcpy_P(bTopic, homie_set);
		len = sizeof(homie_set)-1;
		strncpy(bValue, &topic[index], len);
		bValue[len] = 0;
		index = atoi (bTransfer);
		if ( memcmp(bValue, bTopic, len) !=0 ) return;
		strcpy_P(bTopic, homie_on);
		len = sizeof(homie_on)-1;
		if ( memcmp(payload, bTopic, len) == 0 ) {
			// ON включить реле
			sender->on(index);
			} else {
			strcpy_P(bTopic, homie_off);
			len = sizeof(homie_off)-1;
			if ( memcmp(payload, bTopic, len) == 0 ) {
				// OFF выключить реле
				sender->off(index);
			};
		};	
	} else {
		strcpy_P(bTopic, homie_broadcast_update);
		len = sizeof(homie_broadcast_update)-1;
		if ( memcmp(topic, bTopic, len) == 0 ) {
			// пришел топик homie/$broadcast/update
			strcpy_P(bTopic, homie_all);
			len = sizeof(homie_all)-1;
			if ( memcmp(payload, bTopic, len) == 0 ) {
				// All
				publishFull();
			} else {
				strcpy_P(bTopic, homie_switch);
				len = sizeof(homie_switch)-1;
				if ( memcmp(payload, bTopic, len) == 0 ) {
					// homie/switch
					publishFull();
				};
			};
		};
	};
}
void MQTTClient::initialize(uint8_t *_mac_address, BoardSender *_sender) {
	timeStartToLoop = 0;
	bTopic = new char [homie_topic_length];
	bValue = new char [homie_value_length]; 
	bTransfer = new char [homie_transfer_length];
	mac_address = _mac_address;
	sender = _sender;
	Ethernet.init(53);
	if (Ethernet.begin(mac_address) != 0) {
		timeToKeepAlive = millis() + Time_To_Reconect;
		timeToStatsPublishInterval = millis() + Time_To_StatsPablish_interval;
		timeToFullPublishInterval = millis() + Time_To_FullPablish_interval;
	} else {
		//Serial.println("Failed to configure Ethernet using DHCP");

		//Ethernet.begin(mac_address, local_ip, dns_server, gateway, subnet);
	};
}
void MQTTClient::process(uint32_t _timeStartToLoop) {
	timeStartToLoop = _timeStartToLoop;
	if (connected()) {
		loop();
		if (sender->hasChanged()) {
			for(uint8_t index = 0; index < sender->pins(); index++) {
				if (sender->hasChanged(index)) {
					publishRelayState(index);
				};
			};
			publishStats();
		};
	}
}
void MQTTClient::keepAlive(uint32_t _timeStartToLoop) {
	timeStartToLoop = _timeStartToLoop;
	if (connected()) {
		timeToKeepAlive = millis() + Time_To_Reconect;
		if ( (timeToFullPublishInterval < millis()) & (millis() > Time_To_FullPablish_interval) ) {
			publishFull();
		} else if ( (timeToStatsPublishInterval < millis()) & (millis() > Time_To_StatsPablish_interval) ) {
			publishStats();
		};
	} else if ( (timeToKeepAlive < millis()) & (millis() > Time_To_Reconect) ) {
		reconnect();		
	};
}
void MQTTClient::reconnect() {
	//Ethernet.maintain();
	//if (Ethernet.localIP()[0] != 192) Ethernet.begin(mac_address);
	//if (Ethernet.localIP()[0] == 192) {
		if (!connected()) {
			strcpy_P(bTopic, homie_switch);
			if (connect(bTopic)) {
				publishFull();
			};
		};
		timeToKeepAlive = millis() + Time_To_Reconect;		
//	};
}
void MQTTClient::publishFull(){
//	if (!connected()) {
//		strcpy_P(bTopic, homie_switch);
//		connect(bTopic);
//	};
	if (connected()) {
		uint32_t timeToStart = millis();
		// https://homieiot.github.io/specification/#
		strcpy_P(bTopic, homie_broadcast);
		subscribe(bTopic);
		/*	
		strcpy_P(bTopic, homie_switch_homie_t);
		strcpy_P(bValue, homie_switch_homie_p);
		publish(bTopic, bValue);	// publish("homie/switch/$homie", "3.0");

		strcpy_P(bTopic, homie_switch_name_t);
		strcpy_P(bValue, homie_switch_name_p);
		publish(bTopic, bValue);	// publish("homie/switch/$name", "Опрос выключателей и управление реле на основании правил");
		strcpy_P(bTopic, homie_switch_localip_t);
		strcpy(bValue, (char *) Ethernet.localIP());
		publish(bTopic, bValue);	// publish("homie/switch/$localip", (char *) Ethernet.localIP());
		strcpy_P(bTopic, homie_switch_state_t);
		strcpy_P(bValue, homie_switch_state_p);
		publish(bTopic, bValue);	// publish("homie/switch/$state", "ready");
		strcpy_P(bTopic, homie_switch_implementation_t);
		strcpy_P(bValue, homie_switch_implementation_p);
		publish(bTopic, bValue);	// publish("homie/switch/$implementation", "Atmega2560");
		strcpy_P(bTopic, homie_switch_fwname_t);
		strcpy_P(bValue, homie_switch_fwname_p);
		publish(bTopic, bValue);	// publish("homie/switch/$fw/name", "С поддержкой MQTT homie 3.0");
		strcpy_P(bTopic, homie_switch_fwversion_t);
		strcpy_P(bValue, homie_switch_fwversion_p);
		publish(bTopic, bValue);	// publish("homie/switch/$fw/version", "2019.05.10");
		strcpy_P(bTopic, homie_switch_stats_t);
		strcpy_P(bValue, homie_switch_stats_p);
		publish(bTopic, bValue);	// publish("homie/switch/$stats", "uptime,freeheap");
		publishStats();
		strcpy_P(bTopic, homie_switch_stats_interval_t);
		strcpy_P(bValue, homie_switch_stats_interval_p);
		publish(bTopic, bValue);	// publish("homie/switch/$stats/interval", "60");
		strcpy_P(bTopic, homie_switch_nodes_t);
		strcpy_P(bValue, homie_switch_nodes_p);
		publish(bTopic, bValue);	// publish("homie/switch/$nodes", "relay[]");
		strcpy_P(bTopic, homie_switch_relays_name_t);
		strcpy_P(bValue, homie_switch_relays_name_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$name", "Управление реле");
		strcpy_P(bTopic, homie_switch_relays_properties_t);
		strcpy_P(bValue, homie_switch_relays_properties_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$properties", "name,state,location");
		strcpy_P(bTopic, homie_switch_relays_type_t);
		strcpy_P(bValue, homie_switch_relays_type_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$type", "switch");

		strcpy_P(bTopic, homie_switch_relays_datatype_t);
		strcpy_P(bValue, homie_switch_relays_datatype_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$datatype", "enum");
		strcpy_P(bTopic, homie_switch_relays_format_t);
		strcpy_P(bValue, homie_switch_relays_format_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$format", "ON,OFF,DISABLE");
		strcpy_P(bTopic, homie_switch_relays_settable_t);
		strcpy_P(bValue, homie_switch_relays_settable_p);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$settable", "true");
		strcpy_P(bValue, homie_array_0);
		itoa((uint8_t)sender->pins()-1, bTransfer, 10);
		strcat(bValue, bTransfer);
		strcpy_P(bTopic, homie_switch_relays_array_t);
		publish(bTopic, bValue);	// publish("homie/switch/relay/$array", bValue);
		*/
		for(uint8_t index = 0; index < sender->pins(); index++) {
			publishRelayProperty(index, 0);
			publishRelayProperty(index, 1);
			publishRelayProperty(index, 2);
			publishRelayState(index);
		};
		publishStats();
		timeToFullPublishInterval = millis() + timeToFullPublishInterval;
	};
}
void MQTTClient::publishStats() {
//	if (!connected()) {
//		strcpy_P(bTopic, homie_switch);
//		connect(bTopic);
//	};
	if (connected()) {
		uint32_t seconds = 0; /*, minutes = 0, hours = 0;
		seconds = millis();
		seconds = seconds / 1000;
		minutes = seconds / 60;
		seconds = seconds % 60;
		hours = minutes / 60;
		minutes = minutes % 60;
		strcpy_P(bTopic, homie_switch_stats_uptime);
		itoa(hours, bTransfer, 10);
		if (hours<10) {strcpy_P(bValue, homie_0);} else strcpy(bValue, "");
		strcat(bValue, bTransfer);

		itoa(minutes, bTransfer, 10);
		if (minutes<10) {strcat_P(bValue, homie_time_0);} else strcat_P(bValue, homie_time);
		strcat(bValue, bTransfer);

		itoa(seconds, bTransfer, 10);
		if (seconds<10) {strcat_P(bValue, homie_time_0);} else strcat_P(bValue, homie_time);
		strcat(bValue, bTransfer);
		publish(bTopic, bValue);
		*/
		seconds = freeRam();
		itoa(seconds, bValue, 10);
		strcpy_P(bTopic, homie_switch_stats_freeheap);
		publish(bTopic, bValue);
		/*
		minutes = millis();
		seconds = minutes - timeStartToLoop;
		if (seconds > 0) {
			itoa(seconds, bValue, 10);
		} else {
			strcpy_P(bValue, homie_0);
		}
		strcpy_P(bTopic, homie_switch_stats_mainloop);
		publish(bTopic, bValue);
		*/
		timeToStatsPublishInterval = millis() + Time_To_StatsPablish_interval;
	};

}

void MQTTClient::publishRelayState(uint8_t index) {
//	if (!connected()) {
//		strcpy_P(bTopic, homie_switch);
//		connect(bTopic);
//	};
	if (connected()) {
		strcpy_P(bTopic, homie_switch_relay_);
		itoa(index, bValue, 10);
		strcat(bTopic, bValue);
		strcpy_P(bValue, homie_state);
		strcat(bTopic, bValue);
		if (sender->isOn(index)) {
			strcpy_P(bValue, homie_on);
		} else /*if (sender->isEnable(index))*/ {
			strcpy_P(bValue, homie_off);
// 		} else {
// 			strcpy_P(bValue, homie_disable);
 		};
		publish(bTopic, bValue);
	};
}

void MQTTClient::publishRelayProperty(uint8_t index, uint8_t propertyId) {
	strcpy_P(bTopic, homie_switch_relay_);
	itoa(index, bValue, 10);
	strcat(bTopic, bValue);
	/*
	if (propertyId > 0) switch ( index ) {
		case SRD01:
			// 2.1. Спальня	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SRD01);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD02:
			// 2.1. Спальня	Свет над кроватью справа
			if (propertyId==1) strcpy_P(bValue, name_SRD02);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD03:
			// 2.1. Спальня	Свет над кроватью слева
			if (propertyId==1) strcpy_P(bValue, name_SRD03);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD04:
			// 2.1. Спальня	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD04);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD05:
			// 2.1. Спальня	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD05);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD06:
			// 2.1. Спальня	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD06);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SRD07:
			// 2.2. Кабинет	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SRD07);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SRD08:
			// 2.2. Кабинет	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD08);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SRD11:
			// 2.2. Кабинет	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD11);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SRD12:
			// 2.2. Кабинет	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD12);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SRD13:
			// 2.4. Мишина	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SRD13);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SRD14:
			// 2.4. Мишина	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD14);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SRD15:
			// 2.4. Мишина	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD15);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SRD16:
			// 2.4. Мишина	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD16);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SRD17:
			// 2.5. Костина	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SRD17);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SRD18:
			// 2.5. Костина	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD18);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SRD21:
			// 2.5. Костина	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD21);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SRD22:
			// 2.5. Костина	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD22);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SRD23:
			// 1.1. Зал	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD23);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SRD24:
			// 1.1. Зал	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD24);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SRD25:
			// 1.1. Зал	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD25);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SRD26:
			// 1.2. Гостевая	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD26);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SRD27:
			// 1.2. Гостевая	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD27);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SRD28:
			// 1.2. Гостевая	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD28);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SRD31:
			// 2.9. Холл	Свет подсветка LED 1
			if (propertyId==1) strcpy_P(bValue, name_SRD31);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SRD32:
			// 2.9. Холл	Свет подсветка LED 2
			if (propertyId==1) strcpy_P(bValue, name_SRD32);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SRD33:
			// 2.9. Холл	Свет подсветка LED 3
			if (propertyId==1) strcpy_P(bValue, name_SRD33);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SRD34:
			// 2.7. Ванная	Вентиляция в ванной 2э
			if (propertyId==1) strcpy_P(bValue, name_SRD34);
			if (propertyId==2) strcpy_P(bValue, location_2_7_Bathroom);
			break;
		case SRD35:
		// 2.6. Туалет	Вентиляция в туалете 2э
			if (propertyId==1) strcpy_P(bValue, name_SRD35);
			if (propertyId==2) strcpy_P(bValue, location_2_6_Toilet);
			break;
		case SRD36:
			// 1.4. Туалет	Вентиляция в туалете 1э
			if (propertyId==1) strcpy_P(bValue, name_SRD36);
			if (propertyId==2) strcpy_P(bValue, location_1_4_Toilet);
			break;
		case SRD37:
			// 1.5. Ванная	Вентиляция в ванной 1э
			if (propertyId==1) strcpy_P(bValue, name_SRD37);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Bathroom);
			break;
		case SRD38:
			// 0. Ц Сауна	Вентиляция в цоколе 0э
			if (propertyId==1) strcpy_P(bValue, name_SRD38);
			if (propertyId==2) strcpy_P(bValue, location_0_Sauna);
			break;
		case SSR01:
			// 2.1. Спальня	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR01);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SSR02:
			// 2.1. Спальня	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR02);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SSR03:
			// 2.1. Спальня	Свет в гардеробной
			if (propertyId==1) strcpy_P(bValue, name_SSR03);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SSR04:
			// 2.2. Кабинет	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR04);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SSR05:
			// 2.2. Кабинет	Свет над столом
			if (propertyId==1) strcpy_P(bValue, name_SSR05);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SSR06:
			// 2.2. Кабинет	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR06);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SSR07:
			// 2.4. Мишина	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR07);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR08:
			// 2.4. Мишина	Свет над столом
			if (propertyId==1) strcpy_P(bValue, name_SSR08);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR09:
			// 2.4. Мишина	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR09);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR10:
			// 2.4. Мишина	Свет над кроватью
			if (propertyId==1) strcpy_P(bValue, name_SSR10);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR11:
			// 2.5. Костина	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR11);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR12:
			// 2.5. Костина	Свет над столом
			if (propertyId==1) strcpy_P(bValue, name_SSR12);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR13:
			// 2.5. Костина	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR13);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR14:
			// 2.5. Костина	Свет над кроватью
			if (propertyId==1) strcpy_P(bValue, name_SSR14);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR15:
			// 1.1. Зал	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR15);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR16:
			// 1.1. Зал	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SSR16);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR17:
			// 1.1. Зал	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR17);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR18:
			// 1.1. Зал	Свет в кухне ???
			if (propertyId==1) strcpy_P(bValue, name_SSR18);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR19:
			// 1.1. Зал	Свет над обеденным столом ???
			if (propertyId==1) strcpy_P(bValue, name_SSR19);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR20:
			// 1.2. Гостевая	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR20);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR21:
			// 1.2. Гостевая	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SSR21);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR22:
			// 1.2. Гостевая	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR22);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR23:
			// 2.9. Холл	Свет основной 1
			if (propertyId==1) strcpy_P(bValue, name_SSR23);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SSR24:
			// 2.9. Холл	Свет основной 2
			if (propertyId==1) strcpy_P(bValue, name_SSR24);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SSR25:
			// 2.9. Холл	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR25);
			if (propertyId==2) strcpy_P(bValue, location_2_9_Hall);
			break;
		case SSR26:
			// 1.5. Холл	Свет основной
			if (propertyId==1) strcpy_P(bValue, name_SSR26);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Hall);
			break;
		case SSR27:
			// 1.5. Холл	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR27);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Hall);
			break;
		case SSR28:
			// 1.3. Прихожая	Свет по периметру
			if (propertyId==1) strcpy_P(bValue, name_SSR28);
			if (propertyId==2) strcpy_P(bValue, location_1_3_Hallway);
			break;
		case SSR29:
			// 2.2. Кабинет	Розетки для принтера
			if (propertyId==1) strcpy_P(bValue, name_SSR29);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SSR30:
			// 2.1. Спальня	Все розетки в спальне и часть розеток в кабинете
			if (propertyId==1) strcpy_P(bValue, name_SSR30);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SSR31:
			// 2.4. Мишина	Розетки совмещены со второй детской
			if (propertyId==1) strcpy_P(bValue, name_SSR31);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR32:
			// 2.5. Костина	Розетки совмещены со второй детской
			if (propertyId==1) strcpy_P(bValue, name_SSR32);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR33:
			// 2.2. Кабинет	Безопастные розетки для компьютера
			if (propertyId==1) strcpy_P(bValue, name_SSR33);
			if (propertyId==2) strcpy_P(bValue, location_2_2_Dmitri);
			break;
		case SSR34:
			// 2.4. Мишина	Безопастные розетки для компьютера, совмещены со второй детской
			if (propertyId==1) strcpy_P(bValue, name_SSR34);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR35:
			// 0. Ц Котельная	Розетки в Котельной
			if (propertyId==1) strcpy_P(bValue, name_SSR35);
			if (propertyId==2) strcpy_P(bValue, location_0_Boiler);
			break;
		case SSR36:
			// 0. Автомат	Розетки для Автоматики
			if (propertyId==1) strcpy_P(bValue, name_SSR36);
			if (propertyId==2) strcpy_P(bValue, location_0_Automatic);
			break;
		case SSR37:
			// 0. Автомат	Розетки для Сетевого оборудования
			if (propertyId==1) strcpy_P(bValue, name_SSR37);
			if (propertyId==2) strcpy_P(bValue, location_0_Automatic);
			break;
		case SSR38:
			// 0. Автомат	Розетки для Интернет оборудования
			if (propertyId==1) strcpy_P(bValue, name_SSR38);
			if (propertyId==2) strcpy_P(bValue, location_0_Automatic);
			break;
		case SSR39:
			// 1.2. Гостевая	Розетки для всей гостевой комнаты
			if (propertyId==1) strcpy_P(bValue, name_SSR39);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR40:
			// 1.5. Холл	Розетки для холла и прихожей
			if (propertyId==1) strcpy_P(bValue, name_SSR40);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Hall);
			break;
		case SSR41:
			// 1.1. Зал	Розетки для аудио и видео
			if (propertyId==1) strcpy_P(bValue, name_SSR41);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR42:
			// 1.1. Зал	Розетки для кухни и балкона
			if (propertyId==1) strcpy_P(bValue, name_SSR42);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR43:
			// 1.1. Зал	Розетки для кухни
			if (propertyId==1) strcpy_P(bValue, name_SSR43);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR44:
			// 1.1. Зал	Розетка для варочной поверхности
			if (propertyId==1) strcpy_P(bValue, name_SSR44);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR45:
			// 1.1. Зал	Розетка для духовки 2
			if (propertyId==1) strcpy_P(bValue, name_SSR45);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR46:
			// 1.1. Зал	Розетка для духовки 1
			if (propertyId==1) strcpy_P(bValue, name_SSR46);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR47:
			// 1.5. Ванная	Розетки в ванной комнате и на стене в холле
			if (propertyId==1) strcpy_P(bValue, name_SSR47);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Bathroom);
			break;
		case SSR48:
			// 1.5. Ванная	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR48);
			if (propertyId==2) strcpy_P(bValue, location_1_5_Bathroom);
			break;
		case SSR49:
			// 1.4. Туалет	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR49);
			if (propertyId==2) strcpy_P(bValue, location_1_4_Toilet);
			break;
		case SSR50:
			// 2.6. Туалет	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR50);
			if (propertyId==2) strcpy_P(bValue, location_2_6_Toilet);
			break;
		case SSR51:
			// 2.7. Ванная	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR51);
			if (propertyId==2) strcpy_P(bValue, location_2_7_Bathroom);
			break;
		case SSR52:
			// 2.1. Спальня	Кондиционер 09
			if (propertyId==1) strcpy_P(bValue, name_SSR52);
			if (propertyId==2) strcpy_P(bValue, location_2_1_Bedroom);
			break;
		case SSR53:
			// 2.4. Мишина	Кондиционер 07
			if (propertyId==1) strcpy_P(bValue, name_SSR53);
			if (propertyId==2) strcpy_P(bValue, location_2_4_Michael);
			break;
		case SSR54:
			// 2.5. Костина	Кондиционер 07
			if (propertyId==1) strcpy_P(bValue, name_SSR54);
			if (propertyId==2) strcpy_P(bValue, location_2_5_Konstantin);
			break;
		case SSR55:
			// 1.1. Зал	Кондиционер 12
			if (propertyId==1) strcpy_P(bValue, name_SSR55);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR56:
			// 1.2. Гостевая	Кондиционер 07
			if (propertyId==1) strcpy_P(bValue, name_SSR56);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR57:
			// 1.2. Гостевая	Рольставни 1
			if (propertyId==1) strcpy_P(bValue, name_SSR57);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
		break;
			case SSR58:
			// 1.2. Гостевая	Рольставни 2
			if (propertyId==1) strcpy_P(bValue, name_SSR58);
			if (propertyId==2) strcpy_P(bValue, location_1_2_Guesthouse);
			break;
		case SSR59:
			// 1.3. Прихожая	Рольставни 1
			if (propertyId==1) strcpy_P(bValue, name_SSR59);
			if (propertyId==2) strcpy_P(bValue, location_1_3_Hallway);
			break;
		case SSR60:
			// 1.3. Прихожая	Рольставни 2
			if (propertyId==1) strcpy_P(bValue, name_SSR60);
			if (propertyId==2) strcpy_P(bValue, location_1_3_Hallway);
			break;
		case SSR61:
			// 1.1. Зал	Рольставни 1
			if (propertyId==1) strcpy_P(bValue, name_SSR61);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR62:
			// 1.1. Зал	Рольставни 2
			if (propertyId==1) strcpy_P(bValue, name_SSR62);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR63:
			// 1.1. Зал	Рольставни 1
			if (propertyId==1) strcpy_P(bValue, name_SSR63);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR64:
			// 1.1. Зал	Рольставни 2
			if (propertyId==1) strcpy_P(bValue, name_SSR64);
			if (propertyId==2) strcpy_P(bValue, location_1_1_Hall);
			break;
		case SSR65:
			// 0. Ц Большая	Свет 1
			if (propertyId==1) strcpy_P(bValue, name_SSR65);
			if (propertyId==2) strcpy_P(bValue, location_0_Large);
			break;
		case SSR66:
			// 0. Ц Большая	Свет 2
			if (propertyId==1) strcpy_P(bValue, name_SSR66);
			if (propertyId==2) strcpy_P(bValue, location_0_Large);
			break;
		case SSR67:
			// 0. Ц Лестница	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR67);
			if (propertyId==2) strcpy_P(bValue, location_0_Ladder);
			break;
		case SSR68:
			// 0. Ц Котельная	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR68);
			if (propertyId==2) strcpy_P(bValue, location_0_Boiler);
			break;
		case SSR69:
			// 0. Ц Cтирка	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR69);
			if (propertyId==2) strcpy_P(bValue, location_0_Washing);
			break;
		case SSR70:
			// 0. Ц Гардероб	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR70);
			if (propertyId==2) strcpy_P(bValue, location_0_Closet);
			break;
		case SSR71:
			// 0. Ц Кладовая	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR71);
			if (propertyId==2) strcpy_P(bValue, location_0_Storage);
			break;
		case SSR72:
			// 0. Ц Сауна	Свет
			if (propertyId==1) strcpy_P(bValue, name_SSR72);
			if (propertyId==2) strcpy_P(bValue, location_0_Sauna);
			break;
		case SSR73:
			// 0. Ц Большая	Все розетки большой комнаты
			if (propertyId==1) strcpy_P(bValue, name_SSR73);
			if (propertyId==2) strcpy_P(bValue, location_0_Large);
			break;
		case SSR74:
			// 0. Ц Cтирка	Все розетки постирочной
			if (propertyId==1) strcpy_P(bValue, name_SSR74);
			if (propertyId==2) strcpy_P(bValue, location_0_Washing);
			break;
		case SSR75:
			// 0. Улица	Все розетки вокруг дома
			if (propertyId==1) strcpy_P(bValue, name_SSR75);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR76:
			// 0. Улица	Звонок и Домофон
			if (propertyId==1) strcpy_P(bValue, name_SSR76);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR77:
			// 0. Улица	Автоматика
			if (propertyId==1) strcpy_P(bValue, name_SSR77);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR78:
			// 0. Улица	Свет Переднее
			if (propertyId==1) strcpy_P(bValue, name_SSR78);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR79:
			// 0. Улица	Свет Крыльцо
			if (propertyId==1) strcpy_P(bValue, name_SSR79);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR80:
			// 0. Улица	Свет Дежурное
			if (propertyId==1) strcpy_P(bValue, name_SSR80);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		case SSR81:
			// 0. Улица	Свет Зона отдыха
			if (propertyId==1) strcpy_P(bValue, name_SSR81);
			if (propertyId==2) strcpy_P(bValue, location_0_Street);
			break;
		default:
			if (propertyId==1) strcpy_P(bValue, name_None);
			if (propertyId==2) strcpy_P(bValue, location_None);
			break;
	};	
	*/
//	if (!connected()) {
//		strcpy_P(bTopic, homie_switch);
//		connect(bTopic);
//	};
	if (connected()) {
		if (propertyId==0) {
			strcat_P(bTopic, homie_set);
			subscribe(bTopic);			//"homie/switch/relay_%u/set"
		/*} else if (propertyId==1) {
			strcat_P(bTopic, homie_name);
			publish(bTopic, bValue);	// "homie/switch/relay_%u/$name"
		} else if (propertyId==2) {
			strcat_P(bTopic, homie_location);
			publish(bTopic, bValue);	// "homie/switch/relay_%u/$location"
		*/};	
	};
	
}