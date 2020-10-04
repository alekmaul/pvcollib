#include "compdan3.h"

//---------------------------------------------------------------------------
#define DAN3RAW_MIN	1
#define DAN3RAW_RANGE (1<<8)
#define DAN3RAW_MAX DAN3RAW_MIN + DAN3RAW_RANGE - 1

#define DAN3BIT_GOLOMG_MAX	7
#define DAN3BIT_OFFSET_MIN	9
#define DAN3BIT_OFFSET_MAX	16
#define DAN3MAX 256*1024
#define DAN3MAX_GAMMA		(1<<(DAN3BIT_GOLOMG_MAX+1)) - 2

#define DAN3BIT_OFFSET00	0
#define DAN3BIT_OFFSET0		1
#define DAN3BIT_OFFSET1		5
#define DAN3BIT_OFFSET2		8
#define DAN3MAX_OFFSET00	(1<<DAN3BIT_OFFSET00)
#define DAN3MAX_OFFSET0		(1<<DAN3BIT_OFFSET0) + DAN3MAX_OFFSET00
#define DAN3MAX_OFFSET1		(1<<DAN3BIT_OFFSET1)
#define DAN3MAX_OFFSET2		(1<<DAN3BIT_OFFSET2) + DAN3MAX_OFFSET1
#define DAN3MAX_OFFSET		(1<<DAN3BIT_OFFSET_MAX) + DAN3MAX_OFFSET2
#define DAN3BIT_OFFSET_NBR	DAN3BIT_OFFSET_MAX - DAN3BIT_OFFSET_MIN + 1

// MATCHES -
struct t_match
{
	int index;
	struct t_match *next;
};
struct t_match dan3matches[65536];

struct t_optimal
{
	int bits[DAN3BIT_OFFSET_NBR]; /* COST */
	int offset[DAN3BIT_OFFSET_NBR];
	int len[DAN3BIT_OFFSET_NBR];
} dan3optimals[DAN3MAX];

int DAN3BIT_OFFSET_NBR_ALLOWED;
int DAN3BIT_OFFSET_MAX_ALLOWED;
int DAN3BIT_OFFSET3;
int DAN3MAX_OFFSET3;
int dan3bit_index;
int dan3index_dest;
int dan3index_src;
int dan3bit_mask;

// - INITIALIZE MATCHES TABLE -
void dan3reset_matches(void)
{
	int i;
	for (i = 0;i < 65536;i++)
	{
		dan3matches[i].next = NULL;
	}
}

// REMOVE MATCHE(S) FROM MEMORY -
void dan3flush_match(struct t_match *match)
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

// - INSERT A MATCHE IN TABLE -
void dan3insert_match(struct t_match *match, int index)
{
	struct t_match *new_match = (struct t_match *) malloc( sizeof(struct t_match) );
	new_match->index = match->index;
	new_match->next = match->next;
	match->index = index;
	match->next = new_match;
}

void set_BIT_OFFSET3(int i)
{
	DAN3BIT_OFFSET3 = DAN3BIT_OFFSET_MIN + i;
	DAN3MAX_OFFSET3 = (1 << DAN3BIT_OFFSET3) + DAN3MAX_OFFSET2;
}

// WRITE DATA -
void dan3write_byte(byte value, byte *data_dest)
{
	data_dest[dan3index_dest++] = value;
}
void dan3write_bit(int value, byte *data_dest)
{
	if (dan3bit_mask == 0)
	{
		dan3bit_mask  = (unsigned char) 128;
		dan3bit_index = dan3index_dest;
		dan3write_byte((unsigned char) 0, data_dest);
	}
	if (value) data_dest[dan3bit_index] |= dan3bit_mask;
	dan3bit_mask >>= 1 ;
}
void dan3write_bits(int value, int size, byte *data_dest)
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
		dan3write_bit (value & mask,data_dest);
	}
}
void dan3write_golomb_gamma(int value, byte *data_dest)
{
	int i;
	value++;
	for (i = 4; i <= value; i <<= 1)
	{
		dan3write_bit(0,data_dest);
	}
	while ((i >>= 1) > 0)
	{
		dan3write_bit(value & i,data_dest);
	}
}
void dan3write_offset(int value, int option,byte *data_dest)
{
	value--;
	if (option == 1)
	{
		if (value >= DAN3MAX_OFFSET00)
		{
			dan3write_bit(1,data_dest);
			value -= DAN3MAX_OFFSET00;
			dan3write_bits(value, DAN3BIT_OFFSET0,data_dest);
		}
		else
		{
			dan3write_bit(0,data_dest);
			dan3write_bits(value, DAN3BIT_OFFSET00,data_dest);
		}
	}
	else
	{
		if (value >= DAN3MAX_OFFSET2)
		{
			dan3write_bit(1,data_dest);
			dan3write_bit(1,data_dest);
			value -= DAN3MAX_OFFSET2;
			dan3write_bits(value >> DAN3BIT_OFFSET2, DAN3BIT_OFFSET3 - DAN3BIT_OFFSET2,data_dest);
			dan3write_byte((unsigned char) (value & 255),data_dest);; /* BIT_OFFSET2 = 8 */
		}
		else
		{
			if (value >= DAN3MAX_OFFSET1)
			{
				dan3write_bit(0,data_dest);;
				value -= DAN3MAX_OFFSET1;
				dan3write_byte((unsigned char) (value & 255),data_dest); /* BIT_OFFSET2 = 8 */
			}
			else
			{
				dan3write_bit(1,data_dest);
				dan3write_bit(0,data_dest);
				dan3write_bits(value, DAN3BIT_OFFSET1,data_dest);
			}
		}
	}
}
void dan3write_literal(unsigned char c,byte *data_dest)
{
	dan3write_bit(1,data_dest);
	dan3write_byte(c,data_dest);
}
void dan3write_literals_length(int length,byte *data_dest)
{
	dan3write_bit(0,data_dest);
	dan3write_bits(0, DAN3BIT_GOLOMG_MAX,data_dest);
	dan3write_bit(1,data_dest);
	length -= DAN3RAW_MIN;
	dan3write_byte((unsigned char) length,data_dest);
}
void dan3write_doublet(int length, int offset,byte *data_dest)
{
	dan3write_bit(0,data_dest);
	dan3write_golomb_gamma(length,data_dest);
	dan3write_offset(offset, length,data_dest);
}
void dan3write_end(byte *data_dest)
{
	dan3write_bit(0,data_dest);
	dan3write_bits(0, DAN3BIT_GOLOMG_MAX,data_dest);
	dan3write_bit(0,data_dest);
}

void dan3write_lz(byte *data_src, byte *data_dest,int subset)
{
	int i, j;
	int index;
	dan3index_dest = 0;
	dan3bit_mask = 0;
	dan3write_bits(0xFE, subset + 1,data_dest);
	dan3write_byte(data_src[0],data_dest);
	for (i = 1;i < dan3index_src;i++)
	{
		if (dan3optimals[i].len[subset] > 0)
		{
			index = i -  dan3optimals[i].len[subset] + 1;
			if (dan3optimals[i].offset[subset] == 0)
			{
				if (dan3optimals[i].len[subset] == 1)
				{
					dan3write_literal(data_src[index],data_dest);
				}
				else
				{
					dan3write_literals_length(dan3optimals[i].len[subset],data_dest);
					for (j = 0;j < dan3optimals[i].len[subset];j++)
					{
						dan3write_byte(data_src[index + j], data_dest);
					}
				}
			}
			else
			{
				dan3write_doublet(dan3optimals[i].len[subset], dan3optimals[i].offset[subset],data_dest);
			}
		}
	}
	dan3write_end(data_dest);
	//write_destination();
}

// - GOLOMB GAMMA LIMIT TO 254 -
int golomb_gamma_bits(int value)
{
	int bits = 0;
	value++;
	while (value > 1)
	{
		bits += 2;
		value >>= 1;
	}
	return bits;
}

int dan3count_bits(int offset, int len)
{
	int bits = 1 + golomb_gamma_bits(len);
	if (len == 1)
	{
		if (DAN3BIT_OFFSET00 == -1)
		{
			return bits + DAN3BIT_OFFSET0;
		}
		else
		{
			return bits + 1 + (offset > DAN3MAX_OFFSET00 ? DAN3BIT_OFFSET0 : DAN3BIT_OFFSET00);
		}
	}
	return bits + 1 + (offset > DAN3MAX_OFFSET2 ? 1 + DAN3BIT_OFFSET3 : (offset > DAN3MAX_OFFSET1 ? DAN3BIT_OFFSET2 : 1 + DAN3BIT_OFFSET1));
}

void update_optimal(int index, int len, int offset)
{
	int i;
	int cost;
	i = DAN3BIT_OFFSET_NBR_ALLOWED - 1;
	while (i >= 0)
	{
		if (offset == 0)
		{
			if (index > 0)
			{
				if (len == 1)
				{
					dan3optimals[index].bits[i] = dan3optimals[index-1].bits[i] + 1 + 8;
					dan3optimals[index].offset[i] = 0;
					dan3optimals[index].len[i] = 1;
				}
				else
				{
					cost = dan3optimals[index-len].bits[i] + 1 + DAN3BIT_GOLOMG_MAX + 1 + 8 + len * 8;
					if (dan3optimals[index].bits[i] > cost)
					{
						dan3optimals[index].bits[i] = cost;
						dan3optimals[index].offset[i] = 0;
						dan3optimals[index].len[i] = len;
					}
				}
			}
			else
			{
				dan3optimals[index].bits[i] = 8;
				dan3optimals[index].offset[i] = 0;
				dan3optimals[index].len[i] = 1;
			}
		}
		else
		{
			if (offset > DAN3MAX_OFFSET1)
			{
				set_BIT_OFFSET3(i);
				if (offset > DAN3MAX_OFFSET3) break;
			}
			cost = dan3optimals[index-len].bits[i] + dan3count_bits(offset, len);
			if (dan3optimals[index].bits[i] > cost)
			{
				dan3optimals[index].bits[i] = cost;
				dan3optimals[index].offset[i] = offset;
				dan3optimals[index].len[i] = len;
			}
		}
		i--;
	}
}

// - REMOVE USELESS FOUND OPTIMALS -
void dan3cleanup_optimals(int subset)
{
	int j;
	int i = dan3index_src - 1;
	int len;
	while (i > 1)
	{
		len = dan3optimals[i].len[subset];
		for (j = i - 1; j > i - len;j--)
		{
			dan3optimals[j].offset[subset] = 0;
			dan3optimals[j].len[subset] = 0;
		}
		i = i - len;
	}
}

// - LZ ENCODE -
void dan3lzss_slow(byte *data_src,byte *data_dest)
{
	//int temp_bits;
	int best_len;
	int len;
	int i, j, k;
	int offset;
	int match_index, prev_match_index = -1;
	int bits_minimum_temp, bits_minimum;
	struct t_match *match;
	dan3reset_matches();
	update_optimal(0, 1, 0);
	i = 1;
	while (i < dan3index_src)
	{
		// TRY LITERALS 
		update_optimal(i, 1, 0);
		// STRING OF LITERALS 
        if (i >= DAN3RAW_MIN)
        {
            j = DAN3RAW_MAX;
            if (j > i) j = i;
            if (DAN3RAW_MIN == 1)
            {
                for (k = j ; k > DAN3RAW_MIN; k--)
                {
                    update_optimal(i, k, 0);
                }
            }
            else
            {
                // RAW MINIMUM > 1
                for (k = j ; k >= DAN3RAW_MIN; k--)
                {
                    update_optimal(i, k, 0);
                }
            }
        }
		// LZ MATCH OF 1
		j = (DAN3BIT_OFFSET00 == -1 ? (1 << DAN3BIT_OFFSET0) : DAN3MAX_OFFSET0);
		if (j > i) j = i;
		for (k = 1; k <= j; k++)
		{
			if (data_src[i] == data_src[i-k])
			{
				update_optimal(i, 1, k);
			}
		}
		// LZ MATCH OF 2+ 
		match_index = ((int) data_src[i-1]) << 8 | ((int) data_src[i] & 255);
		match = &dan3matches[match_index];
		if (prev_match_index == match_index /*&& bFAST == TRUE*/ && dan3optimals[i-1].offset[0] == 1 && dan3optimals[i-1].len[0] > 2)
		{
			len = dan3optimals[i-1].len[0];
			if (len < DAN3MAX_GAMMA)
				update_optimal(i, len + 1, 1);
		}
		else
		{
			best_len = 1;
			for (;match->next != NULL; match = match->next)
			{
				offset = i - match->index;
				if (offset > DAN3MAX_OFFSET)
				{
					dan3flush_match(match);
					break;
				}
				for (len = 2;len <= DAN3MAX_GAMMA;len++)
				{
					update_optimal(i, len, offset);
					//best_len = len;
					if (i < offset + len || data_src[i-len] != data_src[i-len-offset])
					{
						break;
					}
				}
				//if (bFAST && best_len > 255) break;
                if (best_len > 255) break;
			}
		}
		//prev_match_index = match_index;
		dan3insert_match(&dan3matches[match_index], i);
		i++;
	}
	bits_minimum = dan3optimals[dan3index_src-1].bits[0];
	j = 0;
	for (i = 0;i < DAN3BIT_OFFSET_NBR_ALLOWED;i++)
	{
		bits_minimum_temp = dan3optimals[dan3index_src-1].bits[i];
		if (bits_minimum_temp < bits_minimum)
		{
			bits_minimum = bits_minimum_temp;
			j = i;
		}
	}
	set_BIT_OFFSET3(j);
	dan3cleanup_optimals(j);
	dan3write_lz(data_src,data_dest,j);

	len = (1 + j + bits_minimum + 2 + DAN3BIT_GOLOMG_MAX + 7) / 8;
}

// - SET MAXIMUM BITS ALLOWED TO ENCODE OFFSET
void set_max_bits_allowed(int bits)
{
	if (bits > DAN3BIT_OFFSET_MAX) bits = DAN3BIT_OFFSET_MAX;
	if (bits < DAN3BIT_OFFSET_MIN) bits = DAN3BIT_OFFSET_MIN;
	DAN3BIT_OFFSET_MAX_ALLOWED = bits;
	DAN3BIT_OFFSET_NBR_ALLOWED = DAN3BIT_OFFSET_MAX_ALLOWED - DAN3BIT_OFFSET_MIN + 1;
}

int dan3Compress(byte *src,byte *dst,int n)
{
    size_t count;

	// SET MAX BITS FOR BIG OFFSET VALUES -
    set_max_bits_allowed(DAN3BIT_OFFSET_MAX);

	// Initialize Matches Array
    dan3index_src=n;

	// Apply compression
    dan3lzss_slow(src,dst);

    // compute ratio
    count=dan3index_dest;

    return count;
}
