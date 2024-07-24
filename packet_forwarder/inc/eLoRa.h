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

#include "broker.h"


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

#endif



