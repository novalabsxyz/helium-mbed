/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#ifndef HELIUMUTIL_H
#define HELIUMUTIL_H

#include "Helium.h"

#if DEBUG
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif

int
report_status(int status, int result = 0);

void
helium_connect(Helium * helium);

void
channel_create(Channel * channel, const char * channel_name);

void
channel_send(Channel *    channel,
             const char * channel_name,
             void const * data,
             size_t       len);


#endif /* HELIUMUTIL_H */
