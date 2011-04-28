/*
 * cbuff.h -- Microcontroller circular buffer.
 *
 * By Colin ML Burnett (Copyright 2011)
 *
 * Licensed under Creative Commons Attributeion-ShareAlike 3.0 (CC BY-SA 3.0)
 * http://creativecommons.org/licenses/by-sa/3.0/
 */

#ifndef CBUFF_H_
#define CBUFF_H_

typedef struct
{
	// Buffer space that this circular buffer manages
	// Best performance if this is first since then &cbuff_t == buff
	unsigned char *buff;

	// Start and end indices of valid data in the buffer
	// Read from start; write to end
	int start;
	int end;

	// Number of bytes in the buffer
	int cnt;

	// Number of bytes remaining in the buffer
	int remain;

	// Fix size of the buffer
	int size;
} cbuff_t;

// Initialize a circular buffer to use the designated static buffer that is len bytes big
cbuff_t* cbuff_init(cbuff_t *buff, unsigned char *staticbuff, int len);

/*
 * cbuff_reset -- Reset the buffer to an empty buffer starting at the first byte.
 *
 * This is essentially the same as cbuff_init() but without needing to
 * pass the other parameters.
 *
 * Returns the number of bytes discard (i.e., the bytes in the buffer).
 */
int cbuff_reset(cbuff_t *buff);

// True if the buffer is full
static inline int cbuff_isfull(cbuff_t *buff)
{
	return buff->remain == 0;
}

// True if the buffer is empty
static inline int cbuff_isempty(cbuff_t *buff)
{
	return buff->cnt == 0;
}

// True if the buffer is not empty (could be full)
static inline int cbuff_isnonempty(cbuff_t *buff)
{
	return buff->cnt > 0;
}

// True if the buffer has room for size bytes (or less)
static inline int cbuff_hasspace(cbuff_t *buff, int size)
{
	return buff->remain >= size;
}

/*
 * cbuff_enqueue -- Add data to the circular buffer
 *
 * Since this implementation is not aware of how it is being used, then it must
 * return if not all of the data can be written to the buffer and the caller must
 * then deal with waiting for space to be made in the buffer.
 *
 * Returns the number of remaining bytes in dat that was NOT enqueued (this means
 * that a return of zero is complete copy of dat into buff).
 */
int cbuff_enqueue(cbuff_t *buff, unsigned char *dat, int len);

/*
 * cbuff_enqueue1_from_register -- Add one byte of data by reading a register.
 *
 * This ensures the register is read only once per call.
 *
 * Returns 0 on success, 1 if no room in the buffer.
 * This return value is actually from cbuff_enqueue and the 1 signifies there is
 * one byte not written to the buffer.
 */
int cbuff_enqueue1_from_register(cbuff_t *buff, unsigned char *reg);

/*
 * cbuff_dequeue1 -- Dequeues one byte of data from the buffer
 *
 * Returns -1 if the buffer is empty.  Otherwise you should cast this to an
 * unsigned char since the buffer stores 8-bit values but a larger value is
 * returned so that -1 can be passed without ambiguity.
 */
int cbuff_dequeue1(cbuff_t *buff);

/*
 * cbuff_dequeue_to_register -- Dequeue a single value to a register.
 *
 * Returns -1 if the buffer is empty and nothing was written, zero otherwise.
 */
int cbuff_dequeue1_to_register(cbuff_t *buff, unsigned char *reg);

/*
 * cbuff_dequeue -- Dequeue an arbitrary number of bytes into dest.
 *
 * It's your job to ensure dest has len bytes (or less) available to it.
 *
 * Return the number of bytes copied from buff to dest
 */
int cbuff_dequeue(cbuff_t *buff, unsigned char *dest, int len);

#endif /* CBUFF_H_ */
