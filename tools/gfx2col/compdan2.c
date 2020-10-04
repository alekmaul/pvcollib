#include "compdan2.h"

#include "compdan1.h"

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
