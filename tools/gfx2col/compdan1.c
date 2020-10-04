#include "compdan1.h"

//---------------------------------------------------------------------------
struct t_match matches[65536];
t_optimal optimals[MAXINFILE];

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