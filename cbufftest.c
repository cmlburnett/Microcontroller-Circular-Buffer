/*
 * cbufftest.c -- Microcontroller circular buffer.
 *
 * By Colin ML Burnett (Copyright 2011)
 *
 * Licensed under Creative Commons Attributeion-ShareAlike 3.0 (CC BY-SA 3.0)
 * http://creativecommons.org/licenses/by-sa/3.0/
 */

#include "cbuff.h"

#include <stdio.h>
#include <assert.h>

int main()
{
	long ret;

	unsigned char buff[12];
	cbuff_t c;


	cbuff_init(&c, buff, 12);
	assert(c.start == 0);
	assert(c.end == 0);
	assert(c.cnt == 0);
	assert(c.remain == 12);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert( cbuff_isempty(&c));
	assert(!cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert( cbuff_hasspace(&c, 4));
	assert( cbuff_hasspace(&c, 5));
	assert( cbuff_hasspace(&c, 6));
	assert( cbuff_hasspace(&c, 7));
	assert( cbuff_hasspace(&c, 8));
	assert( cbuff_hasspace(&c, 9));
	assert( cbuff_hasspace(&c, 10));
	assert( cbuff_hasspace(&c, 11));
	assert( cbuff_hasspace(&c, 12));


	ret = cbuff_enqueue(&c, (unsigned char*)"Hello", 5);
	assert(ret == 0);
	assert(c.start == 0);
	assert(c.end == 5);
	assert(c.cnt == 5);
	assert(c.remain == 7);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert( cbuff_hasspace(&c, 4));
	assert( cbuff_hasspace(&c, 5));
	assert( cbuff_hasspace(&c, 6));
	assert( cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_enqueue(&c, (unsigned char*)"Hello", 5);
	assert(ret == 0);
	assert(c.start == 0);
	assert(c.end == 10);
	assert(c.cnt == 10);
	assert(c.remain == 2);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	// Exceed capacity mid-string
	ret = cbuff_enqueue(&c, (unsigned char*)"Hello", 5);
	assert(ret == 3);
	assert(c.start == 0);
	assert(c.end == 0);
	assert(c.cnt == 12);
	assert(c.remain == 0);
	assert(c.size == 12);
	assert( cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert(!cbuff_hasspace(&c, 1));
	assert(!cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	// Outright exceed capacity
	ret = cbuff_enqueue(&c, (unsigned char*)"Hello", 5);
	assert(ret == 5);
	assert(c.start == 0);
	assert(c.end == 0);
	assert(c.cnt == 12);
	assert(c.remain == 0);
	assert(c.size == 12);
	assert( cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert(!cbuff_hasspace(&c, 1));
	assert(!cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));




	ret = cbuff_dequeue1(&c);
	assert(ret == 72);
	assert(c.start == 1);
	assert(c.end == 0);
	assert(c.cnt == 11);
	assert(c.remain == 1);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert(!cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_dequeue1(&c);
	assert(ret == 101);
	assert(c.start == 2);
	assert(c.end == 0);
	assert(c.cnt == 10);
	assert(c.remain == 2);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_dequeue1(&c);
	assert(ret == 108);
	assert(c.start == 3);
	assert(c.end == 0);
	assert(c.cnt == 9);
	assert(c.remain == 3);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_dequeue1(&c);
	assert(ret == 108);
	assert(c.start == 4);
	assert(c.end == 0);
	assert(c.cnt == 8);
	assert(c.remain == 4);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert( cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));



	// Fill back up
	ret = cbuff_enqueue(&c, (unsigned char*)"Hello", 5);
	assert(ret == 1);
	assert(c.start == 4);
	assert(c.end == 4);
	assert(c.cnt == 12);
	assert(c.remain == 0);
	assert(c.size == 12);
	assert( cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert(!cbuff_hasspace(&c, 1));
	assert(!cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));



	ret = cbuff_dequeue1(&c);
	assert(ret == 111);
	assert(c.start == 5);
	assert(c.end == 4);
	assert(c.cnt == 11);
	assert(c.remain == 1);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert(!cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_dequeue1(&c);
	assert(ret == 72);
	assert(c.start == 6);
	assert(c.end == 4);
	assert(c.cnt == 10);
	assert(c.remain == 2);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert(!cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));


	ret = cbuff_dequeue1(&c);
	assert(ret == 101);
	assert(c.start == 7);
	assert(c.end == 4);
	assert(c.cnt == 9);
	assert(c.remain == 3);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert(!cbuff_isempty(&c));
	assert( cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert(!cbuff_hasspace(&c, 4));
	assert(!cbuff_hasspace(&c, 5));
	assert(!cbuff_hasspace(&c, 6));
	assert(!cbuff_hasspace(&c, 7));
	assert(!cbuff_hasspace(&c, 8));
	assert(!cbuff_hasspace(&c, 9));
	assert(!cbuff_hasspace(&c, 10));
	assert(!cbuff_hasspace(&c, 11));
	assert(!cbuff_hasspace(&c, 12));



	unsigned char hey[21];
	ret = cbuff_dequeue(&c, hey, 20);
	hey[ret] = '\0';
	assert(hey[0] == 'l');
	assert(hey[1] == 'l');
	assert(hey[2] == 'o');
	assert(hey[3] == 'H');
	assert(hey[4] == 'e');
	assert(hey[5] == 'H');
	assert(hey[6] == 'e');
	assert(hey[7] == 'l');
	assert(hey[8] == 'l');
	assert(ret == 9);
	assert(c.start == 4);
	assert(c.end == 4);
	assert(c.cnt == 0);
	assert(c.remain == 12);
	assert(c.size == 12);
	assert(!cbuff_isfull(&c));
	assert( cbuff_isempty(&c));
	assert(!cbuff_isnonempty(&c));

	assert( cbuff_hasspace(&c, 1));
	assert( cbuff_hasspace(&c, 2));
	assert( cbuff_hasspace(&c, 3));
	assert( cbuff_hasspace(&c, 4));
	assert( cbuff_hasspace(&c, 5));
	assert( cbuff_hasspace(&c, 6));
	assert( cbuff_hasspace(&c, 7));
	assert( cbuff_hasspace(&c, 8));
	assert( cbuff_hasspace(&c, 9));
	assert( cbuff_hasspace(&c, 10));
	assert( cbuff_hasspace(&c, 11));
	assert( cbuff_hasspace(&c, 12));

	return 0;
}

