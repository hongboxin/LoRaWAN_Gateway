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

#define DEFAULT_ENABLE		"no"
#define DEFAULT_HOST		"iotcraft.cn"
#define DEFAULT_PORT		10883
#define DEFAULT_CLIENTID	"lorawan_gateway"
#define DEFAULT_SUBTOPIC	"server_down"
#define DEFAULT_PUBTOPIC	"server_up"
#define DEFAULT_ALIVE		60
#define DEFAULT_QOS			0

/* **************************************************************
 *	func:Parse the mqtt broker configuration.					
 *																
 *	para:				                                        
 *		1.conf_file is path of mqtt configuration.              
 *		2.mqtt is used to save parameters of configuration.     
 *													            
 *	return:													    
 *	    ELORA_SUCCESS								            
 *		ELORA_FAILURE								            
 * **************************************************************/
extern int parse_mqtt_configuration(const char *conf_file, mqtt_config_t *mqtt);

/**************************************************************** 
 *	func:Determine whether to enable the eLoRa thread
 *
 *	para:
 *		1.ini_path is the path of emqx configuration.
 *		2.emqx is saved the information of emqx server.
 *
 *	return:
 *		open:1
 *		close:0
 * **************************************************************/
extern int start_thread_elora(char *ini_path, emqx_config_t *emqx);

/*************************************************************** 
 *	func:obtain the configuration of the EQMX server.
 *
 *	para:
 *		1.ini_path is the path of emqx configuration.
 *		2.emqx is saved the information of emqx server.
 *
 *	return:
 *		success:0
 *		failed:-1
 **************************************************************/
extern int get_emqx_conf(char *ini_path, emqx_config_t *emqx);
#endif



