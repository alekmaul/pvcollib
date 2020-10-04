#include "comptool.h"

//---------------------------------------------------------------------------
unsigned char *d;

// Maximum offset for each mode
unsigned int maxlen[7] = {
  128,
  128 + 128,
  512 + 128,
  1024 + 128,
  2048 + 128,
  4096 + 128,
  8192 + 128
};

// Cost of coding a constant
unsigned int varcost[65536];

// Metadata per byte
struct metadata {
  unsigned int reeks;           /* Total times that byte is repeated */
  unsigned int cpos[7];
  unsigned int clen[7];
} *m;

// Compression per byte for each mode
struct pakdata {
  unsigned int cost;
  unsigned int mode;
  unsigned int mlen;
} *p[7];

int savelength;
unsigned int source_size;
unsigned int offset;

struct saves {
  unsigned char *buf;
  int ep, dp, p, e;
} s;

// Inits output buffer
void init(void)
{
  s.ep = s.dp = s.p = s.e = 0;
  s.buf = malloc(source_size * 2);
}

// Adds a zero bit to output
void add0(void)
{
  if (s.p == 0)
    claimevent();
  s.e *= 2;
  ++s.p;
  if (s.p == 8)
    addevent();
}


// Adds an one bit to output
void add1(void)
{
  if (s.p == 0)
    claimevent();
  s.e *= 2;
  ++s.p;
  ++s.e;
  if (s.p == 8)
    addevent();
}

// Add a X bit to output
void addbit(int b)
{
  if (b)
    add1();
  else
    add0();
}

/*
** Add three bits to output
*/
void add3(int b)
{
  addbit(b & 4);
  addbit(b & 2);
  addbit(b & 1);
}

/*
** Add a variable number to output
*/
void addvar(int i)
{
  int j = 0x8000;

  /*
  ** Looks for the first bit set at one
  */
  while ((i & j) == 0)
    j >>= 1;
    
  /*
  ** Like in floating point, the uncompressed adds an extra one
  */
  while (j != 1) {
    j >>= 1;
    add1();
    if ((i & j) != 0)
      add1();
    else
      add0();
  }
  add0();  /* Ending signal */
}

// Add a data to output
void adddata(unsigned char d)
{
  s.buf[s.dp++] = d;
}

// Dump bits buffer
void addevent(void)
{
  s.buf[s.ep] = s.e;
  s.e = s.p = 0;
}

// Take note where it will save the following 8 bits
void claimevent(void)
{
  s.ep = s.dp;
  ++s.dp;
}

/*
** Take note of cost of using a variable number
*/
void initvarcost(void)
{
  int v = 1;
  int b = 1;
  int r = 1;
  int j;

  /*
  ** Depict a one costs one bit  (0)
  ** 2-3 costs three bits            (1x0)
  ** 4-7 costs five bits.          (1x1x0)
  */
  while (r != 65536) {
    for (j = 0; j != r; ++j)
      varcost[v++] = b;
    b += 2;
    r *= 2;
  }
}

/*
** Pre-compresses the source file
*/
void createmetadata(void)
{
  unsigned int i;
  unsigned int j;
  unsigned int *last = malloc(65536 * sizeof(unsigned int));
  unsigned int *prev = malloc((source_size + 1) * sizeof(unsigned int));
  unsigned int r;
  unsigned int t;
  int bl;

  /*
  ** For speeding up the search for pairing strings:
  **
  ** as it advances, prev[byte] points to the string immediately previous
  ** that starts with the same two current bytes, it doesn't matter to link
  ** by individual bytes as each offset needs at least one byte.
  **
  ** last is a kind of hash table pointing to each two-byte string found.
  */
  memset(last, -1, 65536 * sizeof(unsigned int));
  for (i = 0; i != source_size; ++i) {
    m[i].cpos[0] = m[i].clen[0] = 0;
    prev[i] = last[d[i] + d[i + 1] * 256];
    last[d[i] + d[i + 1] * 256] = i;
  }

  /*
  ** Counts the bytes repeated from each starting position
  */
  r = -1;
  t = 0;
  for (i = source_size - 1; i != -1; --i) {
    if (d[i] == r)
      m[i].reeks = ++t;
    else {
      r = d[i];
      m[i].reeks = t = 1;
    }
  }

  /*
  ** Now for each possible mode
  */
  for (bl = 0; bl != 7; ++bl) {

    /*
    ** Process the input file
    */
    for (i = 0; i < source_size; ++i) {
      unsigned int l;
      unsigned int p;

      p = i;
      if (bl) {
        m[i].clen[bl] = m[i].clen[bl - 1];
        m[i].cpos[bl] = m[i].cpos[bl - 1];
        p = i - m[i].cpos[bl];
      }
      
      /*
      ** For each repeated string
      */
      while ((p = prev[p]) != -1) {
        if (i - p > maxlen[bl])  /* Exceeded possible offset? */
          break;                 /* Yes, finish */
        l = 0;
        while (d[p + l] == d[i + l] && (i + l) < source_size) {

          /*
          ** Speed up trick:
          **   If the string that is going to replace has repeated bytes...
          **   and the replacing string has less repeated bytes...
          **   then it can take only existing ones and avoids the
          **   classic problem o(n2)
          */
          if (m[i + l].reeks > 1) {
            if ((j = m[i + l].reeks) > m[p + l].reeks)
              j = m[p + l].reeks;
            l += j;
          } else
            ++l;
        }
        if (l > m[i].clen[bl]) {  /* Look for the longest string */
          m[i].clen[bl] = l;      /* Longitude */
          m[i].cpos[bl] = i - p;  /* Position (offset) */
        }
      }
    }
    putchar('.');
  }
  putchar(' ');
  free(prev);
  free(last);
}

// Get the final size of file with a mode
int getlen(struct pakdata *p, unsigned int mode)
{
  unsigned int i;
  unsigned int j;
  unsigned int cc;
  unsigned int ccc;
  unsigned int kc;
  unsigned int kmode;
  unsigned int kl;

  p[source_size].cost = 0;
  
  /*
  ** Trick: goes from onwards to backwards, this way can know all
  ** possible combinations for compressing.
  */
  for (i = source_size - 1; i != -1; --i) {
    kmode = 0;  /* Literal */
    kl = 0;
    kc = 9 + p[i + 1].cost;
    
    /* Test every size until getting the most short */
    j = m[i].clen[0];
    while (j > 1) {
      cc = 9 + varcost[j - 1] + p[i + j].cost;
      if (cc < kc) {
        kc = cc;
        kmode = 1;  /* Short offset */
        kl = j;
      }
      --j;
    }
    
    /* Test all sizes until getting the most short */
    j = m[i].clen[mode];
    if (mode == 1)
      ccc = 9;
    else
      ccc = 9 + mode;
    while (j > 1) {
      cc = ccc + varcost[j - 1] + p[i + j].cost;
      if (cc < kc) {
        kc = cc;
        kmode = 2;  /* Long offset */
        kl = j;
      }
      --j;
    }
    p[i].cost = kc;
    p[i].mode = kmode;
    p[i].mlen = kl;
  }
  return p[0].cost;
}

// Save the compressed file
int  save(struct pakdata *p, unsigned int mode,byte *dstcomp)
{
  unsigned int i;
  unsigned int j;

  init();
  if (savelength) {
    adddata(source_size & 255);
    adddata(source_size >> 8);
  }
  add3(mode - 1);
  adddata(d[0]);
  i = 1;
  while (i < source_size) {
    switch (p[i].mode) {
      case 0:  /* Literal */
        add0();
        adddata(d[i]);
        ++i;
        break;
      case 1:  /* Short offset */
        add1();
        addvar(p[i].mlen - 1);
        j = m[i].cpos[0] - 1;
        //if (j > 127)
        //  printf("-j>128-");
        adddata(j);
        i += p[i].mlen;
        break;
      case 2:  /* Long offset */
        add1();
        addvar(p[i].mlen - 1);
        j = m[i].cpos[mode] - 1;
        //if (j < 128)
        //  printf("-j<128-");
        adddata(128 | (j & 127));
        j -= 128;
        switch (mode) {
          case 6:
            addbit(j & 4096);
          case 5:
            addbit(j & 2048);
          case 4:
            addbit(j & 1024);
          case 3:
            addbit(j & 512);
          case 2:
            addbit(j & 256);
            addbit(j & 128);
          case 1:
            break;
          default:
            //printf("-2-");
            break;
        }
        i += p[i].mlen;
        break;
      default:
        //printf("-?-");
        break;
    }
  }
  for (i = 0; i != 34; ++i)
    add1();
  //done();

	if (s.p != 0) {
		while (s.p != 8) {
			s.e *= 2;
			s.p++;
		}
		addevent();
	}
	fflush(stdout);
	memcpy(dstcomp,s.buf,s.dp);
	
    return s.dp;
}

int pleCompress(byte *src,byte *dst,int n)
{
	int i,count;
	int l;
	int minlen;
	int minbl;

	offset = 0;
	source_size = n;

	d = malloc(source_size + 1);
	m = malloc(sizeof(struct metadata) * (source_size + 1));
	memcpy(d,src,source_size);
	d[source_size] = 0;

	initvarcost();
	createmetadata();
	
	minlen = source_size * 1000;
	minbl = 0;
	for (i = 1; i != 7; ++i) {
		p[i] = malloc(sizeof(struct pakdata) * (source_size + 1));
		l = getlen(p[i], i);
		if (l < minlen && i) {
			minlen = l;
			minbl = i;
		}
		putchar('.');
	}

	count=save(p[minbl], minbl,dst);
	for (i = 1; i != 7; ++i)
		free(p[i]);
	free(m);
	free(d);

    return count;
}