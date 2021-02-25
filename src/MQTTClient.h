#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include <Arduino.h>
//#include "WString.h"
//#include "SPI.h"
#include "Ethernet2.h"
#include "PubSubClient.h"
#include "BoardSender.h"

const uint32_t Time_To_Reconect = 3600000;					// Время до повторной попытки переподключения  к серверу =1000*60*60
const uint32_t Time_To_StatsPablish_interval = 60000;		// Время до повторной публикации статистики =1000*60*1
const uint32_t Time_To_FullPablish_interval = 14400000;		// Время до повторной полной публикации = 1000*60*60*4

class MQTTClient : public PubSubClient{
public:
	MQTTClient(IPAddress, uint16_t, Client& client);	
	void initialize(uint8_t *_mac_address, BoardSender *_sender);
	void process(uint32_t _timeStartToLoop);
	void keepAlive(uint32_t _timeStartToLoop);
	void reconnect();
	void publishFull();
	void publishStats();
	void publishRelayProperty(uint8_t index, uint8_t propertyId);
	void publishRelayState(uint8_t index);
protected:
	uint8_t *mac_address;
	BoardSender *sender;
	char *bTopic;
	char *bValue;
	char *bTransfer;
	uint32_t timeStartToLoop;
	uint32_t timeToKeepAlive;
	uint32_t timeToStatsPublishInterval;
	uint32_t timeToFullPublishInterval;

//public:
	void callBack(char* topic, byte* payload, unsigned int length) override;
};

#endif /* MQTT_CLIENT_H_ */
