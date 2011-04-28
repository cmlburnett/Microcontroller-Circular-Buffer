/*
 * cbuff.c -- Microcontroller circular buffer.
 *
 * By Colin ML Burnett (Copyright 2011)
 *
 * Licensed under Creative Commons Attributeion-ShareAlike 3.0 (CC BY-SA 3.0)
 * http://creativecommons.org/licenses/by-sa/3.0/
 */

#include "cbuff.h"

cbuff_t*
cbuff_init(cbuff_t *buff, unsigned char *staticbuff, int len)
{
	buff->buff = staticbuff;
	buff->size = len;

	cbuff_reset(buff);

	return buff;
}

int
cbuff_reset(cbuff_t *buff)
{
	int ret = buff->cnt;

	buff->start = 0;
	buff->end = 0;
	buff->cnt = 0;
	buff->remain = buff->size;

	return ret;
}

int
cbuff_enqueue(cbuff_t *buff, unsigned char *dat, int len)
{
	// Assume everything will be queued
	int ret=0;
	register int end = buff->end;
	register int size = buff->size;

	// No room at the inn
	if (cbuff_isfull(buff)) { return len; }

	// Not enough space in the buffer
	if (buff->remain < len)
	{
		ret = len - buff->remain;
		len = buff->remain;
	}

	// Update counters before len is modified in the for-loop
	buff->cnt += len;
	buff->remain -= len;

	// Loop to end of data or end of buffer, whichever was decided above
	for (; len > 0; len--)
	{
		// Copy byte
		buff->buff[end++] = *(dat++);

		// Wrap around if need be
		if (end >= size)
		{
			end = 0;
		}
	}

	// Update buffer with new end value
	buff->end = end;

	return ret;
}

int
cbuff_enqueue1_from_register(cbuff_t *buff, unsigned char *reg)
{
	// Read value only once
	unsigned char dat = *reg;

	return cbuff_enqueue(buff, &dat, 1);
}

int
cbuff_dequeue1(cbuff_t *buff)
{
	unsigned char ret;

	if (cbuff_isempty(buff)) { return -1; }

	ret = buff->buff[buff->start];
	buff->start++;

	// Wrap around if need be
	if (buff->start >= buff->size)
	{
		buff->start = 0;
	}

	// One less in the buffer
	buff->cnt--;
	buff->remain++;

	return ret;
}

int
cbuff_dequeue1_to_register(cbuff_t *buff, unsigned char *reg)
{
	int ret = cbuff_dequeue1(buff);
	if (ret < 0)
	{
		return ret;
	}

	*reg = (unsigned char)ret;
	return 0;
}

int
cbuff_dequeue(cbuff_t *buff, unsigned char *dest, int len)
{
	int ret;
	register int start = buff->start;
	register int size = buff->size;

	// Nothing to write
	if (cbuff_isempty(buff)) { return 0; }

	// If buffer does not contain len bytes then truncate
	// the amount dequeued to that of what's in the buffer
	if (len > buff->cnt) { len = buff->cnt; }

	// Number of bytes dequeues into dest
	ret = len;

	// Update counters before len is modified in the for-loop
	buff->cnt -= len;
	buff->remain += len;

	// copy bytes
	for (; len > 0; len--)
	{
		*(dest++) = buff->buff[start++];

		// Wrap around if need be
		if (start >= size)
		{
			start = 0;
		}
	}

	// Update buffer with new start value
	buff->start = start;

	return ret;
}
