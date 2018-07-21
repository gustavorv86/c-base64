
#include "base64.h"

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char b64table[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

int base64_encode_nbytes(int n) {
	int r = (n % 3);
	int d = n / 3;
	if(r) {
		d++;
	}
	return (d * 4) + 1;
}

int base64_decode_nbytes(int n) {
	int r = (n % 4);
	int d = n / 4;
	if(r) {
		d++;
	}
	return (d * 3) + 1;
}

char * base64_encode(char * in_data, int in_size) {
	char * out_data;
	int x;
	int n = 0;
	int count;
	int index;
	unsigned char n0, n1, n2, n3;

	index = 0;
	out_data = calloc(base64_encode_nbytes(in_size), sizeof(char));
	
	/* increment over the length of the string, three characters at a time */
	for (x = 0; x < in_size; x += 3) {
		/* these three 8-bit (ASCII) characters become one 24-bit number */
		n = ((int) in_data[x]) << 16;

		if ((x + 1) < in_size) {
			n += ((int) in_data[x + 1]) << 8;
		}
		if ((x + 2) < in_size) {
			n += in_data[x + 2];
		}
		
		/* this 24-bit number gets separated into four 6-bit numbers */
		n0 = (n >> 18) & 63;
		n1 = (n >> 12) & 63;
		n2 = (n >> 6) & 63;
		n3 =  n & 63;

		/*
		 * if we have one byte available, then its encoding is spread
		 * out over two characters
		 */
		out_data[index++] = b64chars[n0];		
		out_data[index++] = b64chars[n1];

		/*
		 * if we have only two bytes available, then their encoding is
		 * spread out over three chars
		 */
		if ((x + 1) < in_size) {
			out_data[index++] = b64chars[n2];
		}

		/*
		 * if we have all three bytes available, then their encoding is spread
		 * out over four characters
		 */
		if ((x + 2) < in_size) {
			out_data[index++] = b64chars[n3];
		}
	}

	/*
	 * create and add padding that is required if we did not have a multiple of 3
	 * number of characters available
	 */
	count = in_size % 3;
	if (count > 0) {
		for (; count < 3; count++) {
			out_data[index++] = '=';
		}
	}
	out_data[index] = 0x00;
	return out_data;
}

char * base64_decode(char * in_data, int in_size) { 
	char * end = in_data + in_size;
	char iter = 0;
	int index = 0;
	int buf = 0;
	char * out_data;
    
	out_data = calloc(base64_decode_nbytes(in_size), sizeof(char));
	index = 0;
	
	while (in_data < end) {
		char c = b64table[(unsigned char)*in_data++];
        
		switch (c) {
			case WHITESPACE: 
				continue;	/* skip whitespace */
			case INVALID:    
				free(out_data);
				return NULL;	/* invalid input, return error */
			case EQUALS:		/* pad character, end of data */
				in_data = end;
				continue;
			default:
				buf = buf << 6 | c;
				iter++;
				/* If the buffer is full, split it into bytes */
				if (iter == 4) {
					out_data[index++] = (buf >> 16) & 255;
					out_data[index++] = (buf >> 8) & 255;
					out_data[index++] = buf & 255;
					buf = 0; iter = 0;
				}
		}
	}
   
	if (iter == 3) {
		out_data[index++]= (buf >> 10) & 255;
		out_data[index++] = (buf >> 2) & 255;
	} else if (iter == 2) {
		out_data[index++] = (buf >> 4) & 255;
	}

	out_data[index] = 0x00;
	return out_data;
}
