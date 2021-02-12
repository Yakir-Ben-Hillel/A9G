#include "workshop.h"
bool AttachActivate()
{
    uint8_t status;
    bool ret = Network_GetAttachStatus(&status);
    if(!ret)
    {
        Trace(2,"get attach status failed");
        return false;
    }
    Trace(2,"attach status:%d",status);
    if(!status)
    {
        ret = Network_StartAttach();
        if(!ret)
        {
            Trace(2,"network attach failed");
            return false;
        }
    }
    else
    {
        ret = Network_GetActiveStatus(&status);
        if(!ret)
        {
            Trace(2,"get activate status fail");
            return false;
        }
        Trace(2,"activate status:%d",status);
        if(!status)
        {
            Network_PDP_Context_t context = {
                .apn        = PDP_CONTEXT_APN,
                .userName   = PDP_CONTEXT_USERNAME,
                .userPasswd = PDP_CONTEXT_PASSWD
            };
            Network_StartActive(context);
        }
    }
    return true;
}

void StartTimerPublish(uint32_t interval, MQTT_Client_t *client)
{
    OS_StartCallbackTimer(mainTaskHandle, interval, OnTimerPublish, (void *)client);
}

void StartTimerConnect(uint32_t interval, MQTT_Client_t *client)
{
    OS_StartCallbackTimer(mainTaskHandle, interval, OnTimerStartConnect, (void *)client);
}
