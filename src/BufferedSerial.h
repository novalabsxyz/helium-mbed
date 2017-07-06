/**
 * \copyright Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENSE.txt for license information
 */

#include "mbed.h"
#include "RingBuffer.h"

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
class BufferedSerial : public RawSerial
{
  public:
    /** Crete a buffered serial port.
     *
     * @param tx Serial port tx pin to use
     * @param rx Serial port rx pin to use
     * @param baud The baud rate to use. Defaults to the default for the current
     * board
     */
    BufferedSerial(PinName tx,
                   PinName rx,
                   int     baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);

    /** Destructor */
    ~BufferedSerial();


    /** Sets the baud rate for the serial port
     *
     * @param baud The baud rate to set on the serial port
     */
    void baud(int baud);

    /** Get a byte from the serial port buffer.
     *
     * @return a buffered byte or -1 if no bytes are available
     */
    int getc();

    /** Write a byte to the serial port
     *
     * @param ch The byte to write
     * @return the byte that was written
     */
    int putc(int ch);

    /** Check if there are bytes to read.
     *
     * @return 1 if bytes are available, 0 otherwise
     */
    int readable();


  private:
    RingBuffer<uint8_t, 16> _rx_buffer;
    void _rx_interrupt();

};


#endif
