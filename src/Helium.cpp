#include "Helium.h"
#include "helium-client/helium-client.h"

bool
helium_serial_readable(void * param)
{
    BufferedSerial * serial = (BufferedSerial *)param;
    return serial->readable() > 0;
}

bool
helium_serial_getc(void * param, uint8_t * ch)
{
    BufferedSerial * serial = (BufferedSerial *)param;
    int              val    = serial->getc();
    *ch                     = val;
    return val >= 0;
}

bool
helium_serial_putc(void * param, uint8_t ch)
{
    BufferedSerial * serial = (BufferedSerial *)param;
    serial->putc(ch);
    return true;
}

void
helium_wait_us(void * param, uint32_t us)
{
    (void)param;
    wait_us(us);
}

Helium::Helium(PinName tx, PinName rx)
    : serial(tx, rx, 9600)
{
    helium_init(&_ctx, (void *)&serial);
}

int
Helium::baud(enum helium_baud baud)
{
    int result = helium_baud(&_ctx, baud);

    uint32_t serial_baud = 9600;
    switch (baud)
    {
    case helium_baud_b9600:
        serial_baud = 9600;
        break;
    case helium_baud_b14400:
        serial_baud = 14400;
        break;
    case helium_baud_b19200:
        serial_baud = 19200;
        break;
    case helium_baud_b38400:
        serial_baud = 38400;
        break;
    case helium_baud_b57600:
        serial_baud = 57600;
        break;
    case helium_baud_b115200:
        serial_baud = 115200;
        break;
    }
    serial.baud(serial_baud);
    return result;
}

int
Helium::info(struct helium_info * info)
{
    return helium_info(&_ctx, info);
}

int
Helium::connect(struct connection * connection, uint32_t retries)
{
    return helium_connect(&_ctx, connection, retries);
}


bool
Helium::connected()
{
    return helium_connected(&_ctx) == helium_connected_CONNECTED;
}

int
Helium::sleep(struct connection * connection)
{
    return helium_sleep(&_ctx, connection);
}

//
// Channel
//

Channel::Channel(Helium * helium)
{
    _helium = helium;
}

int
Channel::begin(const char * name, uint16_t * token)
{
    return helium_channel_create(&_helium->_ctx, name, strlen(name), token);
}

int
Channel::begin(const char * name, int8_t * result)
{
    uint16_t token;
    int      status = begin(name, &token);
    _channel_id     = -1;
    if (helium_status_OK == status)
    {
        status = poll_result(token, &_channel_id);
    }

    if (result)
    {
        *result = status == helium_status_OK && _channel_id > 0 ? 0 : _channel_id;
    }

    return status;
}

int
Channel::send(void const * data, size_t len, uint16_t * token)
{
    return helium_channel_send(&_helium->_ctx, _channel_id, data, len, token);
}


int
Channel::send(void const * data, size_t len, int8_t * result)
{
    uint16_t token;
    int      status = send(data, len, &token);
    if (helium_status_OK == status)
    {
        status = poll_result(token, result);
    }
    return status;
}

int
Channel::poll_result(uint16_t token, int8_t * result, uint32_t retries)
{
    return helium_channel_poll_result(&_helium->_ctx, token, result, retries);
}

int
Channel::poll_data(uint16_t channel_id,
                   void *   data,
                   size_t   len,
                   size_t * used,
                   uint32_t retries)
{
    return helium_channel_poll_data(&_helium->_ctx,
                                    channel_id,
                                    data,
                                    len,
                                    used,
                                    retries);
}
