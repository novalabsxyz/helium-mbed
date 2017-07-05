/**
 * \copyright Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENSE.txt for license information
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdlib.h>

/*
 * \class RingBuffer
 *
 * \brief A simple circular buffer.
 *
 * This class implements a simple templated circular buffer. The
 * template variables are the type of the data in the buffer and the
 * size of the circular buffer.
 */
template <typename BufferType, size_t BufferSize>
class RingBuffer
{
  public:
    /** Create a RingBuffer */
    RingBuffer()
    {
        clear();
    }

    /** Push a value into the circular buffer
     *
     * @param value The value to push
     * @return true if pushed, false if the buffer is full
     */
    bool push(BufferType value)
    {
        // Add char to buffer
        uint16_t newhead = head + 1;
        if (newhead >= BufferSize)
        {
            newhead = 0;
        }
        else if (newhead == tail)
        {
            // Buffer full
            return false;
        }

        buf[head] = value;
        head      = newhead;

        return true;
    }

    /** Pop a value from the buffer.
     *
     * @param value A pointer to the value to populate with the popped item
     * @return true if the pop succeeded, false if the buffer is empty.
     */
    bool pop(BufferType * value)
    {
        uint16_t newtail = tail;
        if (newtail == head)
        {
            return false;
        }

        *value = buf[newtail++];
        tail   = newtail >= BufferSize ? 0 : newtail;
        return true;
    }

    /** Get the number of available items in the buffer
     *
     * @return The number of items that can be popped from the buffer
     */
    uint16_t available() const
    {
        int16_t count = head - tail;
        return count < 0 ? count + BufferSize : count;
    }

    /** Clear the buffer */
    void clear()
    {
        head = tail = 0;
    }

  private:
    volatile uint16_t head;
    volatile uint16_t tail;
    BufferType        buf[BufferSize];
};

#endif
