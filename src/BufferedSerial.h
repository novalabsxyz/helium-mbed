/**
 * \copyright Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Ringbuffer.h"
#include "mbed.h"

#ifndef BUFFEREDSERIAL_H
#define BUFFEREDSERIAL_H

/**
 * \class BufferedSerial
 *
 * \brief Implements a buffered serial port
 *
 * This is a simple buffered serial port to work around `readable`
 * issues on a number of mbed implementations. Receiving on the serial
 * port is buffered through an interrupt handler.
 *
 * Note that writing is not buffered at this time since writing to the
 * serial port at speed had not been an issue so far.
 */
template <size_t BufferSize>
class BufferedSerial : RawSerial
{
  private:
    RingBuffer<uint8_t, BufferSize> _rx_buffer;
    void _rx_interrupt()
    {
        _rx_buffer.push(serial_getc(&_serial));
    }

  public:
    /** Crete a buffered serial port.
     *
     * @param tx Serial port tx pin to use
     * @param rx Serial port rx pin to use
     * @param baud The baud rate to use. Defaults to the default for the current board
     */
    BufferedSerial(PinName tx,
                   PinName rx,
                   int     baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
        : RawSerial(tx, rx, baud)
    {
        this->attach(callback(this, &BufferedSerial::_rx_interrupt),
                     Serial::RxIrq);
    }

    /** Destructor */
    ~BufferedSerial()
    {
        this->attach(NULL, Serial::RxIrq);
    }


    /** Sets the baud rate for the serial port
     *
     * @param baud The baud rate to set on the serial port
     */
    void baud(int baud)
    {
        serial_baud(&_serial, baud);
    }

    /** Get a byte from the serial port buffer.
     *
     * @return a buffered byte or -1 if no bytes are available
     */
    int getc()
    {
        uint8_t ch;
        if (_rx_buffer.pop(&ch))
        {
            return ch;
        }
        return -1;
    };

    /** Write a byte to the serial port
     *
     * @param ch The byte to write
     * @return the byte that was written
     */
    int putc(int ch)
    {
        serial_putc(&_serial, ch);
        return ch;
    }

    /** Check if there are bytes to read.
     *
     * @return 1 if bytes are available, 0 otherwise
     */
    int readable()
    {
        return _rx_buffer.available() > 0;
    };
};



#endif
