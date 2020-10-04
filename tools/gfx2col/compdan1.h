#ifndef COMPDAN1_H
#define COMPDAN1_H

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#define bool unsigned char
#define byte unsigned char
#define false 0
#define true 0xff

#define BIT_OFFSET1 1
#define BIT_OFFSET2 4
#define BIT_OFFSET3 8
#define BIT_OFFSET4 12
#define MAX_OFFSET1 (1<<BIT_OFFSET1)
#define MAX_OFFSET2 MAX_OFFSET1 + (1<<BIT_OFFSET2)
#define MAX_OFFSET3 MAX_OFFSET2 + (1<<BIT_OFFSET3)

// - INITIALIZE MATCHES TABLE -
#define MAXINFILE 512*1024

// COMPRESSION CONSTANTS
#define MAX_ELIAS_GAMMA (1<<16)
#define MAX_LEN (1<<16)-1

#define MAX_OFFSET  MAX_OFFSET3 + (1<<BIT_OFFSET4)

//---------------------------------------------------------------------------
typedef struct 
{
	int bits; //COST
	int offset;
	int len;
} t_optimal;

struct t_match
{
	int index;
	struct t_match *next;
};

extern t_optimal optimals[MAXINFILE];
extern struct t_match matches[65536];

extern int index_src,index_dest;
extern int rle_ntimes;
extern int bRLEDATA;
extern byte bit_mask;

//---------------------------------------------------------------------------
extern int elias_gamma_bits(int value);
extern void cleanup_optimals(void);
extern void reset_matches(void);
extern void free_matches(void);
extern void write_lz(byte *data_src, byte *data_dest);
extern void flush_match(struct t_match *match);
extern void insert_match(struct t_match *match, int index);
extern int dan1Compress(byte *src,byte *dst,int n);

#endif