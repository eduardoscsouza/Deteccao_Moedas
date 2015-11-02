#ifndef UTILS_H
#define UTILS_H


#include <stdlib.h>

typedef unsigned char uchar;

int validPos(int, int, int, int);

char ** createMov(int);
void freeMov(char **, int);


#endif