/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2018 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include "MyConfig.h"
#include "MyTransport.h"
#include "MyProtocol.h"
#include <string.h>

uint8_t protocolH2i(char c);

char _fmtBuffer[MY_GATEWAY_MAX_SEND_LENGTH];
char _convBuffer[MAX_PAYLOAD*2+1];

bool protocolParse(MyMessage &message, char *inputString)
{
	char *str, *p, *value=NULL;
	uint8_t bvalue[MAX_PAYLOAD];
	uint8_t blen = 0;
	int i = 0;
	uint8_t command = 0;

	// Extract command data coming on serial line
	for (str = strtok_r(inputString, ";", &p); // split using semicolon
	        str && i < 6; // loop while str is not null an max 5 times
	        str = strtok_r(NULL, ";", &p) // get subsequent tokens
	    ) {
		switch (i) {
		case 0: // Radio id (destination)
			message.destination = atoi(str);
			break;
		case 1: // Child id
			message.sensor = atoi(str);
			break;
		case 2: // Message type
			command = atoi(str);
			mSetCommand(message, command);
			break;
		case 3: // Should we request ack from destination?
			mSetRequestAck(message, atoi(str)?1:0);
			break;
		case 4: // Data type
			message.type = atoi(str);
			break;
		case 5: // Variable value
			if (command == C_STREAM) {
				while (*str) {
					uint8_t val;
					val = protocolH2i(*str++) << 4;
					val += protocolH2i(*str++);
					bvalue[blen] = val;
					blen++;
				}
			} else {
				value = str;
				// Remove trailing carriage return and newline character (if it exists)
				uint8_t lastCharacter = strlen(value)-1;
				if (value[lastCharacter] == '\r') {
					value[lastCharacter] = 0;
				}
				if (value[lastCharacter] == '\n') {
					value[lastCharacter] = 0;
				}
			}
			break;
		}
		i++;
	}
	// Check for invalid input
	if (i < 5) {
		return false;
	}
	message.sender = GATEWAY_ADDRESS;
	message.last = GATEWAY_ADDRESS;
	mSetAck(message, false);
	if (command == C_STREAM) {
		message.set(bvalue, blen);
	} else {
		message.set(value);
	}
	return true;
}

char * protocolFormat(MyMessage &message)
{
	snprintf_P(_fmtBuffer, MY_GATEWAY_MAX_SEND_LENGTH,
	           PSTR("%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%s\n"), message.sender,
	           message.sensor, (uint8_t)mGetCommand(message), (uint8_t)mGetAck(message), message.type,
	           message.getString(_convBuffer));
	return _fmtBuffer;
}

char * protocolFormatMQTTTopic(const char* prefix, MyMessage &message)
{
	snprintf_P(_fmtBuffer, MY_GATEWAY_MAX_SEND_LENGTH,
	           PSTR("%s/%" PRIu8 "/%" PRIu8 "/%" PRIu8 "/%" PRIu8 "/%" PRIu8 ""), prefix,
	           message.sender, message.sensor, mGetCommand(message), mGetAck(message), message.type);
	return _fmtBuffer;
}

char * protocolFormatMQTTSubscribe(const char* prefix)
{
	snprintf_P(_fmtBuffer, MY_GATEWAY_MAX_SEND_LENGTH, PSTR("%s/+/+/+/+/+"), prefix);
	return _fmtBuffer;
}

#ifdef MY_GATEWAY_MQTT_CLIENT
bool protocolMQTTParse(MyMessage &message, char* topic, uint8_t* payload, unsigned int length)
{
	char *str, *p;
	uint8_t i = 0;
	uint8_t command = 0;
	if (topic != strstr(topic, MY_MQTT_SUBSCRIBE_TOPIC_PREFIX)) {
		// Prefix doesn't match incoming topic
		return false;
	}
	for (str = strtok_r(topic + strlen(MY_MQTT_SUBSCRIBE_TOPIC_PREFIX) + 1, "/", &p); str && i <= 5;
	        str = strtok_r(NULL, "/", &p)) {
		switch (i) {
		case 0: {
			// Node id
			message.destination = atoi(str);
			break;
		}
		case 1: {
			// Sensor id
			message.sensor = atoi(str);
			break;
		}
		case 2: {
			// Command type
			command = atoi(str);
			mSetCommand(message, command);
			break;
		}
		case 3: {
			// Ack flag
			mSetRequestAck(message, atoi(str)?1:0);
			break;
		}
		case 4: {
			// Sub type
			message.type = atoi(str);
			break;
		}
		}
		i++;
	}

	if (i != 5) {
		return false;
	}

	message.sender = GATEWAY_ADDRESS;
	message.last = GATEWAY_ADDRESS;
	mSetAck(message, false);

	// Add payload
	if (command == C_STREAM) {
		uint8_t bvalue[MAX_PAYLOAD];
		uint8_t blen = 0;
		while (*payload) {
			uint8_t val;
			val = protocolH2i(*payload++) << 4;
			val += protocolH2i(*payload++);
			bvalue[blen] = val;
			blen++;
		}
		message.set(bvalue, blen);
	} else {
		char* ca;
		ca = (char *) payload;
		ca += length;
		*ca = '\0';
		message.set((const char*) payload);
	}

	return true;
}
#endif

uint8_t protocolH2i(char c)
{
	uint8_t i = 0;
	if (c <= '9') {
		i += c - '0';
	} else if (c >= 'a') {
		i += c - 'a' + 10;
	} else {
		i += c - 'A' + 10;
	}
	return i;
}
