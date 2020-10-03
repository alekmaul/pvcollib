

#include "comptool.h"

//---------------------------------------------------------------------------
int rleCompress(byte *src,byte *dst,int n)
{
	byte *p,*q,*run,*dataend; 
	int count,maxrun;

	dataend = src + n;
	for( run = src,q = dst ; n > 0 ; run = p, n -= count ){
        maxrun = n < 127 ? n : 127;
		if(run <= (dataend-3) && run[1] == run[0] && run[2] == run[0])
        {
			for( p = run+3 ; p < (run+maxrun) && *p == run[0] ; )
				++p;
			count = p-run;
			*q++ = 127+count; /* flag byte */
			*q++ = run[0];
		}
        else
        {
			for( p = run ; p < (run+maxrun) ; )
				if(p <= (dataend-3) && p[1] == p[0] && p[2] == p[0])
					break; // 3 bytes repeated end verbatim run
				else
					++p;
			count = p-run;
			*q++ = count-1; /* flag byte */
			memcpy(q,run,count);
			q += count;
		}
	}

    // end marker
    *q++=0xff;
	return q-dst;
}

//---------------------------------------------------------------------------
// - INITIALIZE MATCHES TABLE -
#define MAXINFILE 512*1024

// COMPRESSION CONSTANTS
#define MAX_ELIAS_GAMMA (1<<16)
#define MAX_LEN (1<<16)-1
#define BIT_OFFSET1 1
#define BIT_OFFSET2 4
#define BIT_OFFSET3 8
#define BIT_OFFSET4 12
#define MAX_OFFSET1 (1<<BIT_OFFSET1)
#define MAX_OFFSET2 MAX_OFFSET1 + (1<<BIT_OFFSET2)
#define MAX_OFFSET3 MAX_OFFSET2 + (1<<BIT_OFFSET3)
#define MAX_OFFSET  MAX_OFFSET3 + (1<<BIT_OFFSET4)

struct t_match
{
	int index;
	struct t_match *next;
};
struct t_match matches[65536];
struct t_optimal
{
	int bits; //COST
	int offset;
	int len;
} optimals[MAXINFILE];

int index_src,index_dest;
int bit_index;
int rle_ntimes;
int bRLEDATA;
byte bit_mask;

// REMOVE USELESS FOUND OPTIMALS -
void cleanup_optimals(void)
{
	int j;
	int i = index_src - 1;
	int len;
	while (i > 1)
	{
		len = optimals[i].len;
		for (j = i - 1; j > i - len;j--)
		{
			optimals[j].offset = 0;
			optimals[j].len = 0;
		}
		i = i - len;
	}
}

// REMOVE MATCHE(S) FROM MEMORY -
void flush_match(struct t_match *match)
{
	struct t_match *node;
	struct t_match *head = match->next;
	while ((node = head) != NULL)
	{
		head = head->next;
		free(node);
	}
	match->next = NULL;
}

// FREE MATCHE(S) FROM TABLE -
void free_matches(void)
{
	int i;
	for (i = 0;i < 65536;i++)
	{
		flush_match(&matches[i]);
	}
}

// INITIALIZE MATCHES TABLE -
void reset_matches(void)
{
	int i;
	for (i = 0;i < 65536;i++)
	{
		matches[i].next = NULL;
	}
}

// INSERT A MATCHE IN TABLE -
void insert_match(struct t_match *match, int index)
{
	struct t_match *new_match = (struct t_match *) malloc( sizeof(struct t_match) );
	new_match->index = match->index;
	new_match->next = match->next;
	match->index = index;
	match->next = new_match;
}

// ELIAS GAMMA -
int elias_gamma_bits(int value)
{
	int bits = 1;
	while (value > 1)
	{
		bits += 2;
		value >>= 1;
	}
	return bits;
}

// CALCULATE BITS COST -
int count_bits(int offset, int len)
{
	int bits = 1 + elias_gamma_bits(len);
	if (len == 1) return bits + 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1);
	if (len == 2) return bits + 1 + (offset > MAX_OFFSET2 ? BIT_OFFSET3 : 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1));
	return bits + 1 + (offset > MAX_OFFSET3 ? BIT_OFFSET4 : 1 + (offset > MAX_OFFSET2 ? BIT_OFFSET3 : 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1)));
}

// LZSS COMPRESSION -
void dan1lzss(byte *data_src)
{
	int i, j, k;
	int temp_bits;
	struct t_match *match;
	int len, best_len;//, old_best_len;
	int offset;
	int match_index;
	optimals[0].bits = 8; // FIRST ALWAYS LITERAL
	optimals[0].offset = 0;
	optimals[0].len = 1;
	for (i = 1;i < index_src;i++)
	{
		// TRY LITERALS
		optimals[i].bits = optimals[i-1].bits + 1 + 8;
		optimals[i].offset = 0;
		optimals[i].len = 1;

		// LZ MATCH OF ONE
		j = MAX_OFFSET2;
		if (j > i) j = i;
		for (k = 1; k <= j; k++)
		{
			if (data_src[i] == data_src[i-k])
			{
				temp_bits = optimals[i-1].bits + count_bits(k, 1);
				if (temp_bits < optimals[i].bits)
				{
					optimals[i].bits = temp_bits;
					optimals[i].len = 1;
					optimals[i].offset = k;
					break;
				}
			}
		}
		// LZ MATCH OF TWO OR MORE
		match_index = (((int) data_src[i-1]) << 8) | ((int) data_src[i] & 0x000000ff);
		match = &matches[match_index];
		best_len = 1;
		for (/* match = &matches[match_index] */; (match->next != NULL) && (best_len < MAX_LEN); match = match->next)
		{
			offset = i - match->index;
			if (offset > MAX_OFFSET)
			{
				flush_match(match);
				break;
			}
			for (len = 2;len <= MAX_LEN;len++)
			{
				if (len > best_len)
				{
					if (!((len == 2) && (offset > MAX_OFFSET3) ))
					{
						best_len = len;
						temp_bits = optimals[i-len].bits + count_bits(offset, len);
						if (optimals[i].bits > temp_bits)
						{
							optimals[i].bits = temp_bits;
							optimals[i].offset = offset;
							optimals[i].len = len;
						}
					}
				}
				if ((i < offset + len) || (data_src[i-len] != data_src[i-len-offset]) )
				{
					break;
				}
			}
			// SKIP SOME TESTS
			if ( (len > 6) && (len == best_len - 1) && (match->index == match->next->index + 1) )
			{
				j = 1;
				while (match->next != NULL)
				{
					match = match->next;
					if (i - match->index > MAX_OFFSET)
					{
						flush_match(match);
						break;
					}
					j++;
					if (j == len)
					{
						break;
					}
				}
				if (match->next == NULL) break;
			}
		}
		insert_match(&matches[match_index], i);
	}
	len = (optimals[index_src-1].bits + 18 + 7) / 8;
	cleanup_optimals();
}

// WRITE DATA -
void write_byte(byte value, byte *data_dest)
{
	data_dest[index_dest++] = value;
}

void write_bit(int value, byte *data_dest)
{
	if (bit_mask == 0)
	{
		bit_mask  = (unsigned char) 128;
		bit_index = index_dest;
		write_byte((unsigned char) 0,data_dest);
	}
	if (value) data_dest[bit_index] |= bit_mask;
	bit_mask >>= 1 ;
}

void write_bits(int value, int size, byte *data_dest)
{
	int i;
	int mask = 1;
	for (i = 0 ; i < size ; i++)
	{
		mask <<= 1;
	}
	while (mask > 1)
	{
		mask >>= 1;
		write_bit (value & mask,data_dest);
	}
}

void write_literals_length(int length, byte *data_dest)
{
	write_bit(0,data_dest);
	write_bits(0, 16,data_dest);
	write_bit(1,data_dest);
	length -= 27;
	write_byte((unsigned char) length,data_dest);
}

void write_literal(byte c, byte *data_dest)
{
	write_bit(1,data_dest);
	write_byte(c,data_dest);
}

void write_elias_gamma(int value, byte *data_dest)
{
	int i;
	for (i = 2; i <= value; i <<= 1)
	{
		write_bit(0,data_dest);
	}
	while ((i >>= 1) > 0)
	{
		write_bit(value & i,data_dest);
	}
}

void write_offset(int value, int option, byte *data_dest)
{
	value--;
	if (value >= MAX_OFFSET3)
	{
		write_bit(1,data_dest);
		value -= (MAX_OFFSET3);
		write_bits((value >> 8), BIT_OFFSET4 - 8,data_dest);
		write_byte((unsigned char) (value & 0x000000ff),data_dest);
	}
	else if (value >= MAX_OFFSET2)
	{
		if (option > 2) write_bit(0,data_dest);
		write_bit(1,data_dest);
		value -= (MAX_OFFSET2);
		write_byte((unsigned char) (value & 0x000000ff),data_dest);
	}
	else if (value >= MAX_OFFSET1)
	{
		if (option > 2) write_bit(0,data_dest);
		if (option > 1) write_bit(0,data_dest);
		write_bit(1,data_dest);
		value -= MAX_OFFSET1;
		write_bits(value, BIT_OFFSET2,data_dest);
	}
	else
	{
		if (option > 2) write_bit(0,data_dest);
		if (option > 1) write_bit(0,data_dest);
		write_bit(0,data_dest);
		write_bits(value, BIT_OFFSET1,data_dest);
	}
}

void write_doublet(int length, int offset, byte *data_dest)
{
	write_bit(0,data_dest);
	write_elias_gamma(length,data_dest);
	write_offset(offset, length,data_dest);
}

void write_end(byte *data_dest)
{
	write_bit(0,data_dest);
	write_bits(0, 16,data_dest);
	write_bit(0,data_dest);
}

void write_lz(byte *data_src, byte *data_dest)
{
	int i, j;
	int index;

	// FIRST IS LITERAL
	write_byte(data_src[0],data_dest);
	for (i = 1;i < index_src;i++)
	{
		if (optimals[i].len > 0)
		{
			index = i -  optimals[i].len + 1;
			if (optimals[i].offset == 0)
			{
				if (optimals[i].len == 1)
				{
					write_literal(data_src[index],data_dest);
				}
				else
				{
					write_literals_length(optimals[i].len,data_dest);
					for (j = 0;j < optimals[i].len;j++)
					{
						write_byte(data_src[index + j],data_dest);
					}
					rle_ntimes++;
					bRLEDATA = 1;
				}
			}
			else
			{
				write_doublet(optimals[i].len, optimals[i].offset,data_dest);
			}
		}
	}
	write_end(data_dest);
}

int dan1Compress(byte *src,byte *dst,int n)
{
    size_t count;

	// Initialize Matches Array
    index_src=n;
	reset_matches();

	// Apply compression
	dan1lzss(src);
	free_matches();

    // flush destination
    index_dest=0;
    rle_ntimes = 0;
    bRLEDATA = 0;
    bit_mask = 0;
    write_lz(src, dst);

    // compute ratio
	//index_dest = optimals[n-1].bits += 1 + 16 + 1 + 7;
	//index_dest /= 8;
    count=index_dest;

    return count;
}
//---------------------------------------------------------------------------
#define DAN2BIT_OFFSET4_MIN 10
#define DAN2BIT_OFFSET4_MAX 16
int DAN2MAX_OFFSET4;
int DAN2BIT_OFFSET4;

// CALCULATE BITS COST -
int dan2count_bits(int offset, int len)
{
	int bits = 1 + elias_gamma_bits(len);
	if (len == 1) return bits + 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1);
	if (len == 2) return bits + 1 + (offset > MAX_OFFSET2 ? BIT_OFFSET3 : 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1));
	return bits + 1 + (offset > MAX_OFFSET3 ? DAN2BIT_OFFSET4 : 1 + (offset > MAX_OFFSET2 ? BIT_OFFSET3 : 1 + (offset > MAX_OFFSET1 ? BIT_OFFSET2 : BIT_OFFSET1)));
}

void update_maxoffset4(int bits)
{
	// Update OFFSET4 BIT and MAX values
	DAN2BIT_OFFSET4 = bits;
	DAN2MAX_OFFSET4 = MAX_OFFSET3 + (1<<DAN2BIT_OFFSET4);
}

// - LZSS COMPRESSION -
void dan2lzss(byte *data_src)
{
	int i, j, k;
	int temp_bits;
	struct t_match *match;
	int len, best_len;
	int offset;
	int match_index;
	optimals[0].bits = 8; // FIRST ALWAYS LITERAL 
	optimals[0].offset = 0;
	optimals[0].len = 1;
	for (i = 1;i < index_src;i++)
	{
        // TRY LITERALS 
        optimals[i].bits = optimals[i-1].bits + 1 + 8;
        optimals[i].offset = 0;
        optimals[i].len = 1;
        // LZ MATCH OF ONE : LEN = 1 
        j = MAX_OFFSET2;
        if (j > i) j = i;
			/* temp_bits = optimals[i-1].bits + 1+1+1+2; */
			for (k = 1; k <= j; k++)
			{
				/* if (k==5) temp_bits += 3; */
				if (data_src[i] == data_src[i-k])
				{
					temp_bits = optimals[i-1].bits + dan2count_bits(k, 1);
					if (temp_bits < optimals[i].bits)
					{
						optimals[i].bits = temp_bits;
						optimals[i].len = 1;
						optimals[i].offset = k;
                        break;
					}
				}
			}
			// LZ MATCH OF TWO OR MORE : LEN = 2, 3 , 4 ... 
			match_index = ((int) data_src[i-1]) << 8 | ((int) data_src[i] & 0x000000ff);
			match = &matches[match_index];
			best_len = 1;
			for (/* match = &matches[match_index] */; match->next != NULL && best_len < MAX_LEN; match = match->next)
			{
				offset = i - match->index;
				if (offset > DAN2MAX_OFFSET4)
				{
					flush_match(match);
					break;
				}
				for (len = 2;len <= MAX_LEN;len++)
				{
					if (len > best_len)
					{
						if (!(len == 2 && offset > MAX_OFFSET3))
						{
							best_len = len;
							temp_bits = optimals[i-len].bits + dan2count_bits(offset, len);
							if (optimals[i].bits > temp_bits)
							{
								optimals[i].bits = temp_bits;
								optimals[i].offset = offset;
								optimals[i].len = len;
							}
						}
					}
					if (i < offset + len || data_src[i-len] != data_src[i-len-offset])
					{
                        break;
					}
				}
			}
			insert_match(&matches[match_index], i);
		}
	len = (optimals[index_src-1].bits + 17 + 7) / 8;
	cleanup_optimals();
}

int dan2Compress(byte *src,byte *dst,int n)
{
    size_t count;
	int maxbits = DAN2BIT_OFFSET4_MIN+1;

	// Initialize Matches Array
    index_src=n;
	reset_matches();

	// SET MAX BITS FOR BIG OFFSET VALUES -
	update_maxoffset4(maxbits);

	// Apply compression
	dan2lzss(src);
	free_matches();

    // flush destination
    index_dest=0;
    rle_ntimes = 0;
    bRLEDATA = 0;
    bit_mask = 0;
    write_lz(src, dst);

    // compute ratio
	//index_dest = optimals[n-1].bits += 1 + 16 + 1 + 7;
	//index_dest /= 8;
    count=index_dest;

    return count;
}

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