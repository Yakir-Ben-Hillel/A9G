#include "workshop.h"

char willMsg[50] = "GPRS 123456789012345 disconnected!";
uint8_t imei[16] = "";
HANDLE mainTaskHandle = NULL;
HANDLE secondTaskHandle = NULL;
HANDLE semMqttStart = NULL;
uint32_t reconnectInterval=3000;

void SecondTask(void *pData)
{
    MQTT_Event_t *event = NULL;

    semMqttStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semMqttStart, OS_WAIT_FOREVER);
    OS_DeleteSemaphore(semMqttStart);
    semMqttStart = NULL;

    Trace(1, "start mqtt test");

    INFO_GetIMEI(imei);
    Trace(1, "IMEI:%s", imei);

    MQTT_Client_t *client = MQTT_ClientNew();

    MQTT_Error_t err;
    memset(&ci, 0, sizeof(MQTT_Connect_Info_t));
    ci.client_id = imei;
    ci.client_user = CLIENT_USER;
    ci.client_pass = CLIENT_PASS;
    ci.keep_alive = 20;
    ci.clean_session = 1;
    ci.use_ssl = false;
    ci.will_qos = 2;
    ci.will_topic = "will";
    ci.will_retain = 1;
    memcpy(strstr(willMsg, "GPRS") + 5, imei, 15);
    ci.will_msg = willMsg;
    Trace(1, "*DEBUG* client user is:%s,client password is:%s\n", ci.client_user, ci.client_pass);
    err = MQTT_Connect(client, BROKER_IP, BROKER_PORT, OnMqttConnection, NULL, &ci);
    if (err != MQTT_ERROR_NONE)
        Trace(1, "MQTT connect fail,error code:%d", err);

    while (1)
    {
        if (OS_WaitEvent(secondTaskHandle, (void **)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            SecondTaskEventDispatch(event);
            OS_Free(event);
        }
    }
}

void MainTask(void *pData)
{
    API_Event_t *event = NULL;

    secondTaskHandle = OS_CreateTask(SecondTask,
                                     NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

    while (1)
    {
        if (OS_WaitEvent(mainTaskHandle, (void **)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void workshop_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
                                   NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
