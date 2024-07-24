/*********************************************************************************
 *      Copyright:  (C) 2024 lorawan<2822290591@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  eLoRa.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(22/07/24)
 *         Author:  lorawan <2822290591@qq.com>
 *      ChangeLog:  1, Release initial version on "22/07/24 18:54:34"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "broker.h"
#include "mosquitto.h"
#include "eLoRa.h"
#include "parson.h"
#include "cJSON.h"

int parse_mqtt_configuration(const char *conf_file, mqtt_config_t *mqtt)
{
	const char conf_obj_name[] = "mqtt_conf";
	JSON_Value				*root_val = NULL;
	JSON_Object				*conf_obj = NULL;
	const char				*str;
	JSON_Value				*val = NULL;

	/* try to parse JSON */
	root_val = json_parse_file_with_comments(conf_file);
	if(root_val == NULL)
	{
		printf("ERROR:%s is not a valid JSON file\n", conf_file);
		return ELORA_FAILURE;
	}

	/* point to the mqtt configuration object */
	conf_obj = json_object_get_object(json_value_get_object(root_val), conf_obj_name);
	if(conf_obj == NULL)
	{
		printf("ERROR:%s does not contain a JSON object named %s\n",conf_file, conf_obj_name);
		json_value_free(root_val);
		return ELORA_FAILURE;
	}
	else
	{
		printf("INFO: %s does contain a JSON object named %s, parsing MQTT parameters\n",conf_file, conf_obj_name);
	}

	/* parse MQTT host */
	str = json_object_get_string(conf_obj, "host");
	if(str != NULL)
	{
		snprintf(mqtt->host, sizeof(mqtt->host), "%s", str);
		printf("INFO:MQTT host is configured to \"%s\"\n", mqtt->host);
	}
	else
	{
		printf("ERROR:MQTT set host failure:%s\n", strerror(errno));
		return ELORA_FAILURE;
	}

	/* parse MQTT port */
	val = json_object_get_value(conf_obj, "port");
	if(val != NULL)
	{
		mqtt->port = (int)json_value_get_number(val);
		printf("INFO:MQTT port is configured to %d\n", mqtt->port);
	}
	else
	{
		printf("ERROR:MQTT set port failure:%s\n", strerror(errno));
		return ELORA_FAILURE;
	}

	/* parse MQTT username */
	str = json_object_get_string(conf_obj, "username");
	if(str != NULL)
	{
		snprintf(mqtt->username, sizeof(mqtt->username), "%s", str);
		printf("INFO:MQTT username is configured to \"%s\"\n", mqtt->username);
	}
	else
	{
		printf("ERROR:MQTT set username failure:%s\n", strerror(errno));
	}

	/* parse MQTT password */
	str = json_object_get_string(conf_obj, "password");
	if(str != NULL)
	{
		snprintf(mqtt->password, sizeof(mqtt->password), "%s", str);
		printf("INFO:MQTT password is configured to \"%s\"\n", mqtt->password);
	}
	else
	{
		printf("ERROR:MQTT set password failure:%s\n", strerror(errno));
	}

	/* parse MQTT sub_topic */
	str = json_object_get_string(conf_obj, "sub_topic");
	if(str != NULL)
	{
		snprintf(mqtt->sub_topic, sizeof(mqtt->sub_topic), "%s", str);
		printf("INFO:MQTT sub topic is configured to \"%s\"\n", mqtt->sub_topic);
	}
	else
	{
		printf("ERROR:MQTT set sub topic failure:%s\n", strerror(errno));
	}

	/* parse MQTT pub_topic */
	str = json_object_get_string(conf_obj, "pub_topic");
	if(str != NULL)
	{
		snprintf(mqtt->pub_topic, sizeof(mqtt->pub_topic), "%s", str);
		printf("INFO:MQTT pub topic is configured to \"%s\"\n", mqtt->pub_topic);
	}
	else
	{
		printf("ERROR:MQTT set pub topic failure:%s\n", strerror(errno));
	}

	/* free JSON parsing data structure */
	json_value_free(root_val);

	return ELORA_SUCCESS;
}
