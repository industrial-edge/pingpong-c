// Copyright 2020 Siemens AG
// This file is subject to the terms and conditions of the MIT License.
// See LICENSE file in the top-level directory.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"

#include <json-c/json.h>
#include <unistd.h>


#define CLIENTID "CPingPong"
#define QOS 1
#define TIMEOUT 10000L

const char *username;
const char *password;
const char *address;
const char *topic1;
const char *topic2;

volatile MQTTAsync_token deliveredtoken;

int disc_finished = 0;
int subscribed = 0;
int finished = 0;

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	if (cause)
		printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		finished = 1;
	}
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
	printf("Successful disconnection\n");
	finished = 0;
}
void onSend(void *context, MQTTAsync_successData *response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message with token value %d delivery confirmed\n", response->token);

	opts.onSuccess = onDisconnect;
	opts.context = client;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{

	int i;
	char *payloadptr;
	char payload[255] = "";

	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");

	payloadptr = message->payload;
	for (i = 0; i < message->payloadlen; i++)
	{
		payload[i] = *payloadptr;
		putchar(*payloadptr++);
	}
	putchar('\n');
	payload[message->payloadlen + 1] = '\n';
	printf("message payload: %s\n", payload);

	//Ping detected
	if (strcmp(payload, "Ping") == 0)
	{
		printf("Ping detected\n");

		MQTTAsync client = (MQTTAsync)context;
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
		int rc;
		printf("Successful connection\n");
		opts.onSuccess = onSend;
		opts.context = client;
		pubmsg.payload = "Pong";
		pubmsg.payloadlen = (int)strlen(pubmsg.payload);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		deliveredtoken = 0;

		if ((rc = MQTTAsync_sendMessage(client, topic2, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start sendMessage, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	//Pong detected
	if (strcmp(payload, "Pong") == 0)
	{
		printf("Pong detected\n");

		MQTTAsync client = (MQTTAsync)context;
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
		int rc;
		printf("Successful connection\n");
		opts.onSuccess = onSend;
		opts.context = client;
		pubmsg.payload = "Ping";
		pubmsg.payloadlen = (int)strlen(pubmsg.payload);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		deliveredtoken = 0;

		if ((rc = MQTTAsync_sendMessage(client, topic2, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start sendMessage, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	//something else than ping or pong detected
	if (strcmp(payload, "Pong") != 0 && strcmp(payload, "Ping") != 0)
	{
		printf("something else detected detected\n");

		MQTTAsync client = (MQTTAsync)context;
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
		int rc;
		printf("Successful connection\n");
		opts.onSuccess = onSend;
		opts.context = client;
		pubmsg.payload = "I only answer to Ping or Pong";
		pubmsg.payloadlen = (int)strlen(pubmsg.payload);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		deliveredtoken = 0;

		if ((rc = MQTTAsync_sendMessage(client, topic2, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start sendMessage, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void onSubscribe(void *context, MQTTAsync_successData *response)
{
	printf("Subscribe succeeded\n");
	subscribed = 1;
}

void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Subscribe failed, rc %d\n", response ? response->code : 0);
	finished = 1;
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Connect failed, rc %d\n", response ? response->code : 0);
	finished = 1;
}

void onConnect(void *context, MQTTAsync_successData *response)
{

	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	printf("Successful connection\n");

	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
		   "Press Q<Enter> to quit\n\n",
		   topic1, CLIENTID, QOS);
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	deliveredtoken = 0;

	if ((rc = MQTTAsync_subscribe(client, topic1, QOS, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}
int cfileexists(const char *filename)
{
	/* try to open file to read */
	FILE *file;
	if (file = fopen(filename, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{

	size_t i;

	int exist = cfileexists("/cfg-data/mqtt-config.json");

	if (exist == 1)
	{
		FILE *fp;
		char buffer[1024];
		struct json_object *parsed_json;
		struct json_object *j_username;
		struct json_object *j_password;
		struct json_object *j_address;
		struct json_object *j_topic1;
		struct json_object *j_topic2;
		fp = fopen("/cfg-data/mqtt-config.json", "r");
		fread(buffer, 1024, 1, fp);
		fclose(fp);
		parsed_json = json_tokener_parse(buffer);
		json_object_object_get_ex(parsed_json, "MQTT_USER", &j_username);
		json_object_object_get_ex(parsed_json, "MQTT_PASSWORD", &j_password);
		json_object_object_get_ex(parsed_json, "MQTT_IP", &j_address);
		json_object_object_get_ex(parsed_json, "TOPIC_1", &j_topic1);
		json_object_object_get_ex(parsed_json, "TOPIC_2", &j_topic2);

		char address2[] = "tcp://";
		char port[] = ":1883";

		strcat(address2, json_object_get_string(j_address));
		strcat(address2, port);

		username = json_object_get_string(j_username);
		password = json_object_get_string(j_password);
		topic1 = json_object_get_string(j_topic1);
		topic2 = json_object_get_string(j_topic2);
		address = address2;
	}
	else
	{

		address = getenv("MQTT_IP");
		username = getenv("MQTT_USER");
		password = getenv("MQTT_PASSWORD");
		topic1 = getenv("TOPIC_1");
		topic2 = getenv("TOPIC_2");
	}

	printf("ip:%s user:%s pw:%s t1:%s t2_%s", address, username, password, topic1, topic2);

	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	int rc;
	int ch;

	MQTTAsync_create(&client, address, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	conn_opts.username = username;
	conn_opts.password = password;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	while (!subscribed)
#if defined(WIN32)
		Sleep(100);
#else
		usleep(10000L);
#endif

	if (finished)
		goto exit;

	do
	{
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');

	disc_opts.onSuccess = onDisconnect;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	while (!disc_finished)
#if defined(WIN32)
		Sleep(100);
#else
		usleep(10000L);
#endif

exit:
	MQTTAsync_destroy(&client);
	return rc;
}
