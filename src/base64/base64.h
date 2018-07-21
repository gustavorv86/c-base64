
#ifndef LIBBASE64_H
#define LIBBASE64_H

#include <string.h>

char * base64_encode(char * in_data, int in_size);
char * base64_decode(char * in_data, int in_size);

#endif 

