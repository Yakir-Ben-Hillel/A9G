#ifndef __WORKSHOP_H_
#define __WORKSHOP_H_
//All libaries are LOCAL for the A9/A9G chips, meaning standard 
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

#define PDP_CONTEXT_APN "cmnet"
#define PDP_CONTEXT_USERNAME ""
#define PDP_CONTEXT_PASSWD ""

#define MAIN_TASK_STACK_SIZE (2048 * 2)
#define MAIN_TASK_PRIORITY 0
#define MAIN_TASK_NAME "Main Test Task"

#define SECOND_TASK_STACK_SIZE (2048 * 2)
#define SECOND_TASK_PRIORITY 1
#define SECOND_TASK_NAME "MQTT Test Task"

#define BROKER_IP "195.28.181.41"
#define BROKER_PORT 1883
#define CLIENT_USER "mqtt"
#define CLIENT_PASS "mqtt"
#define SUBSCRIBE_TOPIC "gps"
#define PUBLISH_TOPIC "gps"
#define PUBLISH_INTERVAL 10000 //10s
#define PUBLISH_PAYLOAD "hello I'm yakir's module"
extern HANDLE mainTaskHandle;
extern HANDLE secondTaskHandle;
extern HANDLE semMqttStart;
extern uint32_t reconnectInterval;
MQTT_Connect_Info_t ci;


typedef enum
{
    MQTT_EVENT_CONNECTED = 0,
    MQTT_EVENT_DISCONNECTED,
    MQTT_EVENT_MAX
} MQTT_Event_ID_t;
typedef struct
{
    MQTT_Event_ID_t id;
    MQTT_Client_t *client;
} MQTT_Event_t;

typedef enum
{
    MQTT_STATUS_DISCONNECTED = 0,
    MQTT_STATUS_CONNECTED,
    MQTT_STATUS_MAX
} MQTT_Status_t;

MQTT_Status_t mqttStatus = MQTT_STATUS_DISCONNECTED;
//event_dispatchers.c
void EventDispatch(API_Event_t *pEvent);
void NetworkEventDispatch(API_Event_t *pEvent);
void SecondTaskEventDispatch(MQTT_Event_t* pEvent);
//mqtt_callbacks.c
void OnMqttReceived(void *arg, const char *topic, uint32_t payloadLen);
void OnMqttReceivedData(void *arg, const uint8_t *data, uint16_t len, MQTT_Flags_t flags);
void OnMqttSubscribed(void *arg, MQTT_Error_t err);
void OnMqttConnection(MQTT_Client_t *client, void *arg, MQTT_Connection_Status_t status);
void OnPublish(void* arg, MQTT_Error_t err);
void OnTimerPublish(void* param);
void OnTimerStartConnect(void* param);
//start_connections.c
bool AttachActivate();
void StartTimerConnect(uint32_t interval,MQTT_Client_t* client);
void StartTimerPublish(uint32_t interval,MQTT_Client_t* client);


#endif
