#include "workshop.h"

void OnMqttConnection(MQTT_Client_t *client, void *arg, MQTT_Connection_Status_t status)
{
    Trace(1, "MQTT connection status:%d", status);
    MQTT_Event_t *event = (MQTT_Event_t *)OS_Malloc(sizeof(MQTT_Event_t));
    if (!event)
    {
        Trace(1, "MQTT no memory");
        return;
    }
    if (status == MQTT_CONNECTION_ACCEPTED)
    {
        Trace(1, "MQTT succeed connect to broker");
        event->id = MQTT_EVENT_CONNECTED;
        event->client = client;
        OS_SendEvent(secondTaskHandle, event, OS_TIME_OUT_WAIT_FOREVER, OS_EVENT_PRI_NORMAL);
    }
    else
    {
        event->id = MQTT_EVENT_DISCONNECTED;
        event->client = client;
        OS_SendEvent(secondTaskHandle, event, OS_TIME_OUT_WAIT_FOREVER, OS_EVENT_PRI_NORMAL);
        Trace(1, "MQTT connect to broker fail,error code:%d", status);
    }
    Trace(1, "MQTT OnMqttConnection() end");
}
void OnMqttSubscribed(void *arg, MQTT_Error_t err)
{
    if (err != MQTT_ERROR_NONE)
        Trace(1, "MQTT subscribe fail,error code:%d", err);
    else
        Trace(1, "MQTT subscribe success,topic:%s", (const char *)arg);
}

void OnMqttReceived(void *arg, const char *topic, uint32_t payloadLen)
{
    Trace(1, "MQTT received publish data request, topic:%s, payload length:%d", topic, payloadLen);
}

void OnMqttReceviedData(void *arg, const uint8_t *data, uint16_t len, MQTT_Flags_t flags)
{
    Trace(1, "MQTT recieved publish data,  length:%d,data:%s", len, data);
    if (flags == MQTT_FLAG_DATA_LAST)
        Trace(1, "MQTT data is last frame");
}

void OnPublish(void *arg, MQTT_Error_t err)
{
    if (err == MQTT_ERROR_NONE)
        Trace(1, "MQTT publish success");
    else
        Trace(1, "MQTT publish error, error code:%d", err);
}

void OnTimerPublish(void *param)
{
    MQTT_Error_t err;
    MQTT_Client_t *client = (MQTT_Client_t *)param;
    uint8_t status = MQTT_IsConnected(client);
    Trace(1, "mqtt status:%d", status);
    if (mqttStatus != MQTT_STATUS_CONNECTED)
    {
        Trace(1, "MQTT not connected to broker! can not publish");
        return;
    }
    Trace(1, "MQTT OnTimerPublish");
    err = MQTT_Publish(client, PUBLISH_TOPIC, PUBLISH_PAYLOAD, strlen(PUBLISH_PAYLOAD), 1, 2, 0, OnPublish, NULL);
    if (err != MQTT_ERROR_NONE)
        Trace(1, "MQTT publish error, error code:%d", err);
    StartTimerPublish(PUBLISH_INTERVAL, client);
}
void OnTimerStartConnect(void *param)
{
    MQTT_Error_t err;
    MQTT_Client_t *client = (MQTT_Client_t *)param;
    uint8_t status = MQTT_IsConnected(client);
    Trace(1, "mqtt status:%d", status);
    if (mqttStatus == MQTT_STATUS_CONNECTED)
    {
        Trace(1, "already connected!");
        return;
    }
    err = MQTT_Connect(client, BROKER_IP, BROKER_PORT, OnMqttConnection, NULL, &ci);
    if (err != MQTT_ERROR_NONE)
    {
        Trace(1, "MQTT connect fail,error code:%d", err);
        reconnectInterval += 1000;
        if (reconnectInterval >= 60000)
            reconnectInterval = 60000;
        StartTimerConnect(reconnectInterval, client);
    }
}
