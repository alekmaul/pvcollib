#ifndef COMPDAN2_H
#define COMPDAN2_H

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#define bool unsigned char
#define byte unsigned char
#define false 0
#define true 0xff

extern int dan2Compress(byte *src,byte *dst,int n);

#endif