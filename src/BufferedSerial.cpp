/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENSE.txt for license information
 */

#include "BufferedSerial.h"

BufferedSerial:: BufferedSerial(PinName tx,
                                PinName rx,
                                int     baud)
    : RawSerial(tx, rx, baud)
{
    this->attach(callback(this, &BufferedSerial::_rx_interrupt),
                 Serial::RxIrq);
}


BufferedSerial::~BufferedSerial()
{
    this->attach(NULL, Serial::RxIrq);
}

void BufferedSerial::_rx_interrupt()
{
    _rx_buffer.push(serial_getc(&_serial));
}


void BufferedSerial::baud(int baud)
{
    serial_baud(&_serial, baud);
}

int BufferedSerial::getc()
{
    uint8_t ch;
    if (_rx_buffer.pop(&ch))
    {
        return ch;
    }
    return -1;
}

int BufferedSerial::putc(int ch)
{
    serial_putc(&_serial, ch);
    return ch;
}


int BufferedSerial::readable()
{
    return _rx_buffer.available() > 0;
}
