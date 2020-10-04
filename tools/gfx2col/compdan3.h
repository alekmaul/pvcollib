#ifndef COMPDAN3_H
#define COMPDAN3_H

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#define bool unsigned char
#define byte unsigned char
#define false 0
#define true 0xff

extern int dan3Compress(byte *src,byte *dst,int n);

#endif