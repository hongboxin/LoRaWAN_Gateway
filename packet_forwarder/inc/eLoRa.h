/********************************************************************************
 *      Copyright:  (C) 2024 lorawan<2822290591@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mqtt.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(22/07/24)
 *         Author:  lorawan <2822290591@qq.com>
 *      ChangeLog:  1, Release initial version on "22/07/24 17:01:13"
 *                 
 ********************************************************************************/
#ifndef _ELORA_H_

#define _ELORA_H_

#define ELORA_SUCCESS	0
#define ELORA_FAILURE	-1

#define HOST_LEN		64
#define USERNAME_LEN	20
#define PASSWD_LEN		20
#define NETWORK_SIZE	20

typedef struct mqtt_config_s
{
	char		host[HOST_LEN];
	int			port;
	int			keepalive;
	char		username[USERNAME_LEN];
	char		password[NETWORK_SIZE];
}mqtt_config_t;

typedef struct mqtt_packet_s
{
	char	temp[10];
}mqtt_packet_t;

/* **************************************************************
 *	func:Parse the mqtt broker configuration.					*
 *																*
 *	para:				                                        *
 *		1.conf_file is path of mqtt configuration.              *
 *		2.mqtt is used to save parameters of configuration.     *
 *													            *
 *	return:													    *
 *	    ELORA_SUCCESS								            *
 *		ELORA_FAILURE								            *
 * **************************************************************/
extern int parse_mqtt_configuration(const char *conf_file, mqtt_config_t *mqtt);

/* **************************************************************
 *  func:Connect to mqtt broker server.                         *
 *  															*
 *	para:                                                       *
 *		1.mqtt is saved parameters of configuring broker.       *
 *		2.mosq is operation handler of broker.                  * 
 *                                                              *
 *	return:                                                     *
 *		ELORA_SUCCESS                                           *
 *		ELORA_FAILURE                                           *
 * **************************************************************/
extern int mqtt_connect_broker(mqtt_config_t mqtt, struct mosquitto **mosq);

/* **************************************************************
 *	func:Parse the data uploaded by the node.				    *
 *																*
 *	para:														*
 *		1.ep is used to save data of parsing.					*
 *		2.sp is the node data.									*
 *																*	
 *	return:														*
 *		ELORA_SUCCESS											*
 *		ELORA_FAILURE											*
 * **************************************************************/
extern int parse_elora_packet(mqtt_packet_t *ep, uint8_t *sp);

/* **************************************************************
 *	func:Package the matt data into JSON.                       *
 *	                                                            *
 *	para:                                                       *
 *		eq saved data to be packaged.                           *
 *                                                              *
 *	return:                                                     *
 *		SUCCESS:return a pointer to a JSON structure.           *
 *		Fail:NULL                                               *
 * **************************************************************/
extern char *elora_packet_json(mqtt_packet_t *ep);

#endif



