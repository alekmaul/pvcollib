#ifndef COMPTOOL_H
#define COMPTOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#define bool unsigned char
#define byte unsigned char
#define false 0
#define true 0xff

extern int rleCompress(byte *src,byte *dst,int n);
extern int dan1Compress(byte *src,byte *dst,int n);

extern void addevent(void);
extern void claimevent(void);
extern int pletterCompress(byte *src,byte *dst,int n);

#endif