/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "mbed.h"
#include "HeliumUtil.h"

int
report_status(int status, int result)
{
    if (helium_status_OK == status)
    {
        if (result == 0)
        {
            DBG_PRINTF("Succeeded\n");
        }
        else
        {
            DBG_PRINTF("Failed - %d\n", result);
        }
    }
    else
    {
        DBG_PRINTF("Failed\n");
    }
    return status;
}


void
helium_connect(Helium * helium)
{
    while (!helium->connected())
    {
        DBG_PRINTF("Connecting - ");
        int status = helium->connect();
        if (report_status(status) != helium_status_OK)
        {
            wait_ms(1000);
        }
    }
}

void
channel_create(Channel * channel, const char * channel_name)
{
    int8_t result;
    int    status;
    do
    {
        // Ensure we're connected
        helium_connect(channel->helium);
        DBG_PRINTF("Creating Channel - ");
        status = channel->begin(channel_name, &result);
        // Print status and result
        if (report_status(status, result) != helium_status_OK)
        {
            wait_ms(1000);
        }
    } while (helium_status_OK != status || result != 0);
}

void
channel_send(Channel *    channel,
             const char * channel_name,
             void const * data,
             size_t       len)
{
    int    status;
    int8_t result;

    do
    {
        // Try to send
        DBG_PRINTF("Sending - ");
        status = channel->send(data, len, &result);
        report_status(status, result);
        // Create the channel if any service errors are returned
        if (status == helium_status_OK && result != 0)
        {
            channel_create(channel, channel_name);
        }
        else if (status != helium_status_OK)
        {
            wait_ms(1000);
        }
    } while (helium_status_OK != status || result != 0);
}
