#ifndef __DEMO_MQTT_H_
#define __DEMO_MQTT_H_

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_mqtt.h"
#include "api_network.h"
#include "api_socket.h"
#include "api_info.h"


#define PDP_CONTEXT_APN       "cmnet"
#define PDP_CONTEXT_USERNAME  ""
#define PDP_CONTEXT_PASSWD    ""

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "MQTT Test Task"

#define BROKER_IP  "195.28.181.41"
#define BROKER_PORT 1883
#define CLIENT_USER "mqtt"
#define CLIENT_PASS "mqtt"
#define SUBSCRIBE_TOPIC "gps"
#define PUBLISH_TOPIC   "gps"
#define PUBLISH_INTERVAL 10000 //10s
#define PUBLISH_PAYLOEAD "hello I'm yakir's module"


#endif

