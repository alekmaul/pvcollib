#include "comprle.h"

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
