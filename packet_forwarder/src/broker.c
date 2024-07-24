/*********************************************************************************
 *      Copyright:  (C) 2024 lorawan<2822290591@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  broker.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(23/07/24)
 *         Author:  lorawan <2822290591@qq.com>
 *      ChangeLog:  1, Release initial version on "23/07/24 17:12:44"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "mosquitto.h"
#include "broker.h"

struct mosquitto	*mosq_up;
struct mosquitto	*mosq_down;

void connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
	(void)mosq;
	(void)obj;

	if(rc == 0)
	{
		printf("MQTT connected successfully\n");
	}
	else
	{
		printf("MQTT connect failed \n");

	}
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	char		buf[1024];
	int			rc;

	(void)mosq;
	(void)obj;
	printf("=============================================================================\n");
	printf("INFO:Receive elora message from node\n", (char *)message->payload);
	
	strcpy(buf, (char *)message->payload);
	if((rc = gateway_send_server(buf, strlen(buf))) == 0)
	{
		printf("INFO:Send message to server:%s\n", buf);
		printf("=============================================================================\n");
	}
	else
	{
		printf("ERROR:send message to server failure:%s\n", mosquitto_strerror(rc));
	}
}

int mqtt_broker_connect(mqtt_config_t mqtt, struct mosquitto **mosq)
{
	char				buf[1024];
	int					rv = -1;

	mosquitto_lib_init();

	*mosq = mosquitto_new(NULL, true, NULL);
	if( !*mosq )
	{
		printf("mosmosq new() failure:%s\n", strerror(errno));
		rv = -1;
		goto cleanup;
	}

	if(mosquitto_username_pw_set(*mosq, mqtt.username, mqtt.password) != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto set username and passwd failure:%s\n",strerror(errno));
		rv = -2;
		goto cleanup;
	}

	mosquitto_connect_callback_set(*mosq, connect_callback);
	mosquitto_message_callback_set(*mosq, message_callback);

	if(mosquitto_connect(*mosq, mqtt.host, mqtt.port, 60) != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto connect failure:%s\n",strerror(errno));
		rv = -3;
		goto cleanup;
	}
	
	if(gateway_connect_server() <  0)
	{
		printf("gateway connect sever failed\n");
		goto cleanup;
	}

	if(mosquitto_subscribe(*mosq, NULL, mqtt.sub_topic, 0) != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto Subscribe failed\n");
		rv = -4;
		goto cleanup;
	}

	mosquitto_loop_start(*mosq);

	while(1)
	{
		rv = gateway_recv_server(buf);
		if(rv > 0)
		{
			printf("=============================================================================\n");
			printf("INFO:Receive message from server:%s\n", buf);
			
			if(mosquitto_publish(*mosq, NULL, mqtt.pub_topic, strlen(buf), buf, 0, false) != MOSQ_ERR_SUCCESS)
			{
				printf("ERROR:mqtt publish failure\n");
			}
			else
			{
				printf("INFO:Send elora message to node:%s\n", buf);
				printf("=============================================================================\n");
			}
		}
		else
		{
			usleep(10000);
		}
	}
	mosquitto_loop_stop(*mosq, true);

cleanup:
	if(*mosq)
		mosquitto_destroy(*mosq);
	mosquitto_destroy(mosq_up);
	mosquitto_lib_cleanup();
	return rv;
}

int gateway_send_server(char *buf, int buf_size)
{
	char                topic[128] = "/sys/k123ztJa2Gk/node/thing/event/property/post";

	if(mosquitto_publish(mosq_up, NULL, topic, buf_size, buf, 0, false) != MOSQ_ERR_SUCCESS)
	{
		printf("gateway send message to server failure\n");
		return -1;
	}

	if(mosquitto_loop(mosq_up, 1000, 1) != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto loop error\n");
		return -1;
	}
}

int gateway_connect_server(void)
{
	char				clientid[128] = "k123ztJa2Gk.node|securemode=2,signmethod=hmacsha256,timestamp=1721784820395|";
	char				host[128] = "iot-06z00gtm7spgofv.mqtt.iothub.aliyuncs.com";
	char				username[128] = "node&k123ztJa2Gk";
	char				passwd[128] = "91a811ecd66dabc4bb5b7c11b0ff5c9c0aeb0dbe98f7d16a766fe4045975c55e";
	int					port = 1883;

	mosquitto_lib_init();

	mosq_up = mosquitto_new(clientid, true, NULL);

	mosquitto_username_pw_set(mosq_up, username, passwd);

	if(mosquitto_connect(mosq_up, host, port, 60) != MOSQ_ERR_SUCCESS)
	{
		printf("ERROR:gateway connect server failure\n");
		return -1;
	}
}

static char		*recv_buf = NULL;
static int		recv_buf_len = 0;

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen)
	{
		recv_buf_len = message->payloadlen;
		recv_buf = (char *)malloc(recv_buf_len + 1);
		if(recv_buf)
		{
			memcpy(recv_buf, message->payload, recv_buf_len);
			recv_buf[recv_buf_len] = '\0';
		}
		else
		{
			printf("Save Message failure:%s\n", strerror(errno));
		}
	}
	else
	{
		printf("Message received with empty payload\n");
	}
}

int gateway_recv_server(char *buf)
{
	char                topic[128] = "/k123ztJa2Gk/node/user/get";

	mosquitto_message_callback_set(mosq_up, on_message);

	if(mosquitto_subscribe(mosq_up, NULL, topic, 0) != MOSQ_ERR_SUCCESS)
	{
		printf("ERROR:gateway recv message from server failure:%s\n", strerror(errno));
		return -1;
	}
	
	while(recv_buf == NULL)
	{
	 	if(mosquitto_loop(mosq_up, -1, 1) != MOSQ_ERR_SUCCESS)
		{
			printf("ERROR:mosquitto loop error:%s\n", strerror(errno));
			mosquitto_destroy(mosq_up);
			mosquitto_lib_cleanup();
			return -1;
		}

	}

	strcpy(buf, recv_buf);
	free(recv_buf);
	recv_buf = NULL;
	
	return recv_buf_len;
}

