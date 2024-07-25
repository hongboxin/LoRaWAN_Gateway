/********************************************************************************
 *      Copyright:  (C) 2024 lorawan<2822290591@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  broker.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(23/07/24)
 *         Author:  lorawan <2822290591@qq.com>
 *      ChangeLog:  1, Release initial version on "23/07/24 20:00:31"
 *                 
 ********************************************************************************/
#ifndef _BROKER_H_

#define _BROKER_H_

#define HOST_LEN        64
#define USERNAME_LEN    20
#define PASSWD_LEN      20
#define NETWORK_SIZE    20
#define TOPIC_LEN		20

#define BUF_SIZE		512

typedef struct mqtt_config_s
{
	char        host[HOST_LEN];
	int         port;
	char        username[USERNAME_LEN];
	char        password[NETWORK_SIZE];
	char		pub_topic[TOPIC_LEN];
	char		sub_topic[TOPIC_LEN];
}mqtt_config_t;

typedef struct emqx_config_s
{
	char        enable[BUF_SIZE];
	char        host[BUF_SIZE];
	int         port;
	char        username[BUF_SIZE];
	char        passwd[BUF_SIZE];
	char        clientid[BUF_SIZE];
	int         alive;
	int         Qos;
	char        sub_topic[BUF_SIZE];
	char        pub_topic[BUF_SIZE];
}emqx_config_t;

extern void connect_callback(struct mosquitto *mosq, void *obj, int rc);

extern void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

extern int mqtt_broker_connect(mqtt_config_t mqtt, struct mosquitto **mosq, emqx_config_t *emqx);

extern int gateway_send_server(char *buf, int buf_size, char *pub_topic);

extern int gateway_recv_server(char *buf, char *sub_topic);

extern int gateway_connect_server(emqx_config_t *emqx);

#endif

