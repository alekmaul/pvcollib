/*---------------------------------------------------------------------------------

	Copyright (C) 2018-2019
		Alekmaul 

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

	Image converter for colecovision.
	BMP BI_RLE8 compression support by Andrey Beletsky
	palette rounded option from Artemio Urbina
	
***************************************************************************/

//INCLUDES
#include "gfx2col.h"
#include "lodepic.h"

enum FILE_TYPE {
    BMP_FILE = 1,
    PCX_FILE,
    TGA_FILE,
    PNG_FILE
};

int output_palette=-1;		  // 64 colors output (F18A only) palettes
int palette_rnd=0;      	  // 1 = round palette up & down (F18A only)
int quietmode=0;              // 0 = not quiet, 1 = i can't say anything :P
int compress=0;               // 0 = not compressed, 1 = rle compression, 2 = ple compression, 3 = dan1 compression
int sprmode=0;                // 0 = background 1 = sprite
int bmpmode=0;				  // 0 = normal tile mode, 1=bitmap mode
int gramode=0;				  // 0 = TMS9918a graphic mode 2, 1 = graphic mode 1
int ecmmode=0;				  // 0 = noactive, 1=ecm0,2=ecm1, etc ... (F18A only, will generate a palette)
int offset_tile=0;			  // n = offset in tile number
int transmode=0;			  // 0 = not transparent, 1 = transprent tiles activated
int highpriority=0;     	  // 1 = high priority for map

//---------------------------------------------------------------------------
void PutWord(int data, FILE *fp)
{
	putc(LOW_BYTE(data),fp);
	putc(HI_BYTE(data),fp);
} //end of PutWord

//---------------------------------------------------------------------------
unsigned char *ArrangeBlocks( unsigned char *img, int width, int height,
							 int size, int *xsize, int *ysize, int new_width)
{
	/*
	** img = image buffer
	** width = width (in pixels) of image buffer
	** height = height (in pixels) of image buffer
	**
	** size = size (in pixels) of image blocks in the image
	** *xsize = number of image block horizontally in image block grid
	** *ysize = number of image block vertically in image block grid
	**
	** new_width = how wide (in pixels) you want the new buffer to be
	**             must be a multiple of size
	**
	**
	** returns:
	**      pointer to new buffer, and updates xsize and ysize
	**
	*/
	unsigned char *buffer;
	int rows, num;
	int i,j, line;
	int x,y;

	//get number of full image block rows in the new buffer
	rows = (*xsize)*(*ysize)/(new_width/size);   // rows = num_blocks / new_xsize

	//if it doesn't divide evenly, add another full row
	if ( ((*xsize)*(*ysize))%(new_width/size) != 0 )
		rows++;

	if (quietmode == 0)
		printf("\nrows=%d",rows);

	//get memory for the new buffer
	buffer = (unsigned char *) malloc( rows*size*new_width*sizeof(char) );
	if(buffer == NULL) {
		return 0;
	}

	// Initially clear the buffer, so if there are empty image blocks or incomplete blocks, the empty parts will be blank
	memset(buffer,0,(size_t) rows*size*new_width*sizeof(char) );

	//position in new buffer (x,y) where x and y are in pixel co-ordinates
	x=0;
	y=0;

	//go through each image block(i,j) where i and j are in block co-ordinates
	for(j=0; j < *ysize; j++)
    {
    	for(i=0; i < *xsize; i++)
	    {
		    //move each line of the block into the new buffer
    		//don't worry about reading past the end of the image here
	    	//there is an extra 64 lines to read in.
		    for(line=0;line<size;line++)
    		{
	    		//find out how much to copy
		    	//this is needed because the screen mode files may not be
    			//a multiple of 8 pixels wide
	    		//or no-border files may have the wrong width
		    	num = width - (i*(size));
			    if(num>size)
				    num=size;

    			memcpy( &buffer[ (y+line)*new_width + x ], &img[ (j*(size) + line )*width + i*(size) ], num);
	    	}

		    // move to the next location in the new buffer
    		x+=size;
	    	if(x >= new_width)
		    {
			    x=0;
    			y+=size;
	    	}
	    }
    }

    *xsize = new_width/size;
    *ysize = rows;

	return buffer;
} //end of ArrangeBlocks()

//---------------------------------------------------------------------------
unsigned int MakeCol(unsigned char *buffer, unsigned int *tilemap, unsigned char *num_col, unsigned int *colmap,int *num_tiles,int mapw, int maph)
{
	int colval,nbcol;
	int i,j, t,x,y;
	int colidx, tilidx;
	unsigned char minv,maxv,actminv,actmaxv;
	unsigned char valtil[8];
	unsigned char newcol;
	unsigned char *tilreorg;
	unsigned int *mapreorg;
	unsigned int idxcoltil[256];

	// init color values
	newcol=0;
	for (t=0;t<32;t++)
		colmap[t]=0x100; 					// outside scope value 
	for (t=0;t<256;t++)
		idxcoltil[t]= 32;					// outside score value 
		
	//loop through tiles
	for (t=0;t<*num_tiles;t++) 
	{ 
		actminv=1; actmaxv=0xF;
		for(y=0;y<8;y++) {
			minv=0xFF; maxv=0x00;

			// get the 8 values
			for (x=0;x<8;x++) {
				valtil[x]=buffer[t*64 + y*8 + x];
				// get min and max value
				if (valtil[x]<minv)
					minv=valtil[x];
				if (valtil[x]>maxv)
					maxv=valtil[x];
			}
			// put 1 if max color
			if (maxv <= 0x01) maxv=0xF;
			if (minv == 0xff) minv=1;
			// if 1st time,assign it
			if (y==0)
			{
				if ( (minv!=1) && (maxv!=minv) ) actminv=minv;
				if (maxv!=0xF) actmaxv=maxv;
			}
			else
			{
				if ( (actminv!=minv) &&  (actmaxv!=maxv) ) 
				{
					printf("\nERROR: Too much color (4) for 8x8 tile #%d\n",t);
					return 0;
				}
				if ( (actminv!=minv) && (actminv!=1)  && (maxv!=minv) )
				{
					printf("\nERROR: Too much color (min) for 8x8 tile #%d\n",t);
					return 0;
				}
				else
					if (maxv!=minv) actminv=minv;
				if ( (actmaxv!=maxv) && (actmaxv!=0xF) && (maxv!=0xF) )
				{
					printf("\nERROR: Too much color (max) for 8x8 tile #%d\n",t);
					return 0;
				}
				else
					if (maxv!=0xF) actmaxv=maxv;
			}
			colval=(actmaxv<<4) | (actminv);		// FG color | BG color 
		}
		// try to assign for a color for the 8x8 square
		colidx=-1;
		for (i=0;i<32;i++)
		{
			if (colmap[i]==0x100) {
				colmap[i]=colval;
				colidx=i;
				newcol++;
				break;
			}
			else if (colmap[i]==colval)
			{
				colidx=i;
				break;
			}
		}
		// if not assigned, huston, we have a problem ;)
		if (colidx==-1) 
		{
			printf("\nERROR: Too much colors assigned (more than 32) encountered for 8x8 tile #%d\n",t);
			return 0;
		}
		idxcoltil[t]=colval;
	}
	
	// try to alloc for new tiles arrangement
	tilreorg=(unsigned char *) malloc((size_t)256*64);
	if(tilreorg==NULL) {
		return 0;
	}
	mapreorg=(unsigned int *) malloc((size_t)mapw*maph*sizeof(int));
	if(mapreorg==NULL) {
		return 0;
	}
	memset(tilreorg,0,256*64);
	memset(mapreorg,0,mapw*maph*sizeof(int));

	// Rearrange tiles & map to fit with colors (1 colors for 8 tiles)
	printf("\nRearrange tiles & map to fit with colors (1 colors for 8 tiles)...");fflush(stdout);
	tilidx=0;
	for (i=0;i<32;i++)
	{
		// check all tiles
		nbcol=0;
		for (t=0;t<*num_tiles;t++) 
		{
			// if we find the color, just put the tile
			if (idxcoltil[t]==colmap[i])
			{
				// if we have now more than 8 tiles, need to go next color entry
				if (nbcol==8)
				{
					nbcol=0;
					newcol++;
					for (j=31;j>i;j--)
					{
						colmap[j]=colmap[j-1];
					}
					i++;
					colmap[i]=colmap[i-1];
				}

				// Put the tile
				memcpy(&tilreorg[tilidx],&buffer[t*64],64);
				// Adapt the map
				for (j=0;j<mapw*maph;j++)
				{
					if (*(tilemap+j)==t)
					{
						*(mapreorg+j)=(tilidx/64);
					}
				}
				tilidx+=64;
				nbcol++;
			}
		}
		
		// one color is finished, if we are not near a 8 tiles border, need to adapt
		if (nbcol & 7)
		{
			tilidx+=(8-nbcol)*64;
		}
	}
	// now put back to buffer & map
	printf("\nNew number of tiles is %d...",tilidx/64);fflush(stdout);

	for (i=0;i<tilidx;i++)
		*(buffer+i)=*(tilreorg+i);
	for (i=0;i<mapw*maph;i++)
		*(tilemap+i)=*(mapreorg+i);
	
	free(mapreorg);
	free(tilreorg);

	// keep track of number of colors & number of tiles
	*num_col=newcol;
	*num_tiles=tilidx/64;
	
	return 1;
}

//---------------------------------------------------------------------------
unsigned int *MakeMap(unsigned char *img, unsigned int *colf18a, int *num_tiles, int xsize, int ysize, int tile_x, int tile_y, unsigned char optimize)
{
	unsigned int *map;
	int newtiles;
	int current,palette,oldpal;	//the current tile we're looking at
	int i,t;
	int x,y;

	//allocate map
	map=(unsigned int *) malloc((size_t)tile_x*tile_y*sizeof(int));
	if(map==NULL) {
		return 0;
	}

	//clear map
	memset(map,0,tile_x*tile_y*sizeof(int));

	current=0;
	t=0;
	newtiles=0;
	oldpal=-1;

	for(y=0;y<ysize;y++)
    {
    	for(x=0;x<xsize;x++)
	    {

			//get the palette number
			if (ecmmode==4) // ecm 3
				palette = (img[current*64] >> 3) & 0x07;
			else if (ecmmode==3) // ecm 2
				palette = (img[current*64] >> 2) & 0x0F;
			else				// ecm 1 and others
				palette = (img[current*64] >> 1) & 0x1F;
			/* test purpose if (ecmmode>1) 
			{
				if (oldpal!=palette) {
					printf("\npalette=%04x",palette);fflush(stdout);
					oldpal=palette;
				}
			}*/

    		//check for matches with previous tiles if tile_reduction on
            if (optimize)
            {
    			for(i=0;i<newtiles;i++)
	    			if( memcmp(&img[i*64],&img[current*64],64) == 0 )
		    			break;
            }
            else
                i=newtiles;

			// if ecm mode, put in color table for the tile attributre
			if (ecmmode>1) 
			{
				// How colors are managed:
				// ps  = palette select from VR24
				// cs  = color value from color table (256 bytes for ECM1..3)
				// px0 = pixel from byte from pattern table 0 (original mode and EMC1)
				// px1 = pixel from byte from pattern table 1 (ECM2)
				// px2 = pixel from byte from pattern table 2 (ECM3)
			
				// Bit  Name       Expl.
				// 0-3  PS3-0      Palette selection (b3=PS0, b0=PS3)
				// 4    TRANS      When 1, colors with 0, 00 or 000 will be transparent instead of a palette index
				// 5    FLIPY      Tile flip over Y
				// 6    FLIPX      Tile flip over X
				// 7    PRI        Tile priority over sprite
  				if (ecmmode==2) colf18a[newtiles]=(palette); 					// ECM1    : ps0 cs0 cs1 cs2 cs3 px0 
				else if (ecmmode==3) colf18a[newtiles]=(palette); 				// ECM2    : cs0 cs1 cs2 cs3 px1 px0
				else if (ecmmode==4) colf18a[newtiles]=(palette<<1)& 0xFE; 		// ECM3    : cs0 cs1 cs2 px2 px1 px0
				colf18a[newtiles]|=(transmode<<4) | (highpriority<<7);
			}
			
			//is it a new tile?
			if(i==newtiles)
			{
				// yes -> add it
				memcpy(&img[newtiles*64],&img[current*64],64);
				t=newtiles;
				newtiles++;
			}
			else
			{	// no -> find what tile number it is
				t=i;
			}

    		//put tile number in map
	    	map[y*tile_x+x] = t+offset_tile;
		}

		//goto the next tile
		current++;
	}

	//also return the number of new tiles
	*num_tiles = newtiles;

	return map;

}//end of MakeMap

//---------------------------------------------------------------------------
void ConvertPalette(RGB_color *palette, unsigned int *new_palette)
{
	int i,data;
	int rounded;
	int temp;

	//Convert the colors and put them in the new array
	//
	//alternate rounding down and rounding up
	//this is an attempt to preserve the brightness of a color
	//which the human eye notices easier than a change in color
	rounded=0;
	for(i=0;i<64;i++) 
	{
		if(palette_rnd) 
		{
			data=0;

			//get red portion and round it
			temp = (palette[i].red & 0x01);		//see if this needs rounding
			if(palette[i].red == 31) 			//if value == 63, then we can't round up
			{
				temp = 0;
				rounded = 1;
			}
			data = (palette[i].red >> 1) + (temp & rounded);				//round up if necessary
			rounded = (temp ^ rounded);			//reset rounded down flag after rounding up

			//get green portion and round it
			temp = (palette[i].green & 0x01);	//see if this needs rounding
			if(palette[i].green == 31) 			//if value == 63, then we can't round up
			{
				temp = 0;
				rounded = 1;
			}
			data = (data<<4) + (palette[i].green >> 1) + (temp & rounded);	 //round up if necessary
			rounded = (temp ^ rounded);			//reset rounded down flag after rounding up

			//get blue portion and round it off
			temp = (palette[i].blue & 0x01);	//see if this needs rounding
			if(palette[i].blue == 31) 			//if value == 63, then we can't round up
			{
				temp = 0;
				rounded = 1;
			}
			data = (data<<4) + (palette[i].blue >> 1) + (temp & rounded);				//round up if necessary
			rounded = (temp ^ rounded);			//reset rounded down flag after rounding up

			//store converted color
			new_palette[i] = data;
		}
		else
		{
			data=0;

			data = (palette[i].red >> 1);
			data = (data<<4) + (palette[i].green >> 1);
			data = (data<<4) + (palette[i].blue >> 1);

			//store converted color
			new_palette[i] = data;
		}
	}//loop through all colors

} //end of ConvertPalette

//---------------------------------------------------------------------------
void addcomment(FILE *fp, unsigned int ratio,unsigned int size,unsigned int compress) {
	if (compress==1)
		fprintf(fp, "// rle compression %d bytes (%d%%)\n", size,ratio);
	else if (compress==2)
		fprintf(fp, "// ple compression %d bytes (%d%%)\n", size,ratio);
	else if (compress==3)
		fprintf(fp, "// dan1 compression %d bytes (%d%%)\n", size,ratio);
}

int Convert2Pic(char *filebase, unsigned char *buffer, unsigned int *tilemap, unsigned int *palet, unsigned int *tilecol, int num_tiles, int mapw, int maph, int savemap, int savepal)
{
	char filenamec[256],filenameh[256],filenamef[256];
	unsigned int val16b;
	unsigned char bufsw1[8];
	unsigned char value, minv,maxv;
	unsigned char valtil[8];
	unsigned char *tiMem,*tiMemEncode,*coMem,*coMemEncode, *maMem, *maMemEncode;
	int x,y,t,b;
    int lenencode,lenencode1,lenencode2;
	int lenencodet1,lenencodet2,lenencodet3;
	FILE *fpc,*fph;
	unsigned char bitplanes,mask;
	
	int len = strlen(filebase)-1;
	int i;
	for (i = len; i >= 0; i--) {
		if (filebase[i] == '/') {
			break;
		}
	}
	if (i==-1)
	{
		sprintf(filenamef,"%sgfx",filebase);
	}
	else
	{
		sprintf(filenamef,"%sgfx",&filebase[i+1]);
	}

	sprintf(filenamec,"%sgfx.inc",&filebase[i+1]);
	sprintf(filenameh,"%sgfx.h",&filebase[i+1]);

	sprintf(filenamec,"%sgfx.inc",filebase);
	sprintf(filenameh,"%sgfx.h",filebase);

	if (quietmode == 0) {
		printf("\nSaving gfx source file: [%s]",filenamec);
		printf("\nSaving gfx header file: [%s]",filenameh);
	}
	fpc = fopen(filenamec,"wb");
	fph = fopen(filenameh,"wb");

	if(fpc==NULL)
	{
		printf("\nERROR: Can't open file [%s] for writing\n",filenamec);
		return 0;
	}
	if(fph==NULL)
	{
		printf("\nERROR: Can't open file [%s] for writing\n",filenameh);
		return 0;
	}
	if (bmpmode) num_tiles=768;
	
    //tiMem = (unsigned char *) malloc(num_tiles*8);
    //tiMemEncode = (unsigned char *) malloc(num_tiles*8);
    tiMem = (unsigned char *) malloc(256*8*3); 			// max number for bitmap mode or f18a emc3 mode
    tiMemEncode = (unsigned char *) malloc(256*8*3);	// max number for bitmap mode or f18a emc3 mode
	coMem = (unsigned char *) malloc(num_tiles*8);
    coMemEncode = (unsigned char *) malloc(num_tiles*8);
    maMem = (unsigned char *) malloc(mapw*maph);
    maMemEncode = (unsigned char *) malloc(mapw*maph);

	if ( (tiMem==NULL) || (coMem==NULL) || (maMem==NULL) || 
		 (tiMemEncode==NULL) || (coMemEncode==NULL) || (maMemEncode==NULL)  )
	{
		printf("\nERROR: Could not allocate enough memory\n");
		return 0;
	}
	
	if (quietmode == 0) {
		printf("\nDecode for %d tiles...\n",num_tiles);
	}

	// different check for ecm1-3
	if (ecmmode>1)
	{
		// grab number of planes
		bitplanes=(ecmmode-1);
		
		for(t=0;t<num_tiles;t++) //loop through tiles
		{
			for(b=0;b<bitplanes;b++) //loop through bitplane pairs
			{
				//get bit-mask
				mask = 1 << b;
				for(y=0;y<8;y++)
				{
					value = 0;

					//get row of bit-plane and save row
					for(x=0;x<8;x++)
					{
						value = value << 1;
						if(buffer[t*64 + y*8 + x] & mask)
							value = value+1;
					}
					*(tiMem+y+t*8+(b*256*8))=value;
				}
			}
			*(coMem+t)=*(tilecol+t);    // tile priority
		}
	}
	else
	{
		for(t=0;t<num_tiles;t++) { //loop through tiles
			for(y=0;y<8;y++) {
				value=0; minv=0xFF; maxv=0x00;

				// get the 8 values
				for (x=0;x<8;x++) {
					valtil[x]=buffer[t*64 + y*8 + x];
					// get min and max value
					if (valtil[x]<minv)
						minv=valtil[x];
					if (valtil[x]>maxv)
						maxv=valtil[x];
				}
				// put 1 if max color
				if (maxv <= 0x01) maxv=0xF;
				if (minv == 0xff) minv=1;
				for (x=0;x<8;x++) {
					if (valtil[x]==maxv)
						value=value | (1<<(7-x));
				}
				*(tiMem+y+t*8)=value;
				if (gramode!=1)
					*(coMem+y+t*8)=(maxv<<4) | (minv);    // FG color | BG color 
			}
		}
		if (gramode==1)
		{
			for (t=0;t<32;t++)
			 *(coMem+t)=*(tilecol+t);
		}
	}

	// sprites are 16x16 but we need to swap tile 2<>3  
	if (sprmode)
	{
		// different check for ecm1-3
		if (ecmmode>1)
		{
			// grab number of planes
			bitplanes=(ecmmode-1);
		
			for(t=0;t<num_tiles;t+=4)  //loop through tiles
			{
				for(b=0;b<bitplanes;b++) //loop through bitplane pairs
				{
					memcpy(bufsw1,(tiMem+(t+1)*8+(b*256*8)),8);
					memcpy((tiMem+(t+1)*8+(b*256*8)),(tiMem+(t+2)*8+(b*256*8)),8);
					memcpy((tiMem+(t+2)*8+(b*256*8)),bufsw1,8);
				}
			}
		}
		else 
		{
			for(t=0;t<num_tiles;t+=4) { //loop through tiles
				memcpy(bufsw1,(tiMem+(t+1)*8),8);
				memcpy((tiMem+(t+1)*8),(tiMem+(t+2)*8),8);
				memcpy((tiMem+(t+2)*8),bufsw1,8);
			}
		}
	}

    // Get map if possible (so not in sprite mode)
	if (tilemap!=NULL)
	{
		for (t=0;t<mapw*maph;t++) {
			// if bitmap mode, adapt it
			if (bmpmode)
			{
				if (tilemap[t]>=256*2)
					*(maMem+t)=tilemap[t]-256*2;
				else if (tilemap[t]>=256)
					*(maMem+t)=tilemap[t]-256;
				else
					*(maMem+t)=tilemap[t];
			}
			else
				*(maMem+t)=tilemap[t];
		}
    }

	// write files regarding compression type
	lenencode=num_tiles*8;lenencode1=num_tiles*8;lenencode2=mapw*maph;
	lenencodet1=lenencodet2=lenencodet3=num_tiles*8;
	if (ecmmode>1)
	{
		lenencode1=num_tiles;
	}
	if (compress==0) { // no compression
		memcpy(tiMemEncode,tiMem,lenencode);
		if (gramode==1)
		{
			memcpy(coMemEncode,coMem,32);
			lenencode1=32;
		}
		else
			memcpy(coMemEncode,coMem,lenencode1);
		memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==1) { // rle compression
		if (ecmmode>1)
		{
			lenencodet1=rleCompress( tiMem,tiMemEncode,lenencode);
			lenencodet2=rleCompress( tiMem+256*8,tiMemEncode+256*8,lenencode);
			lenencodet3=rleCompress( tiMem+256*8*2,tiMemEncode+256*8*2,lenencode);
		}
		else
			lenencode=rleCompress( tiMem,tiMemEncode,lenencode);
		if (gramode==1)
		{
			memcpy(coMemEncode,coMem,32);
			lenencode1=32;
		}
		else
			lenencode1=rleCompress(coMem,coMemEncode,lenencode1);
        if (savemap)
            lenencode2=rleCompress(maMem,maMemEncode,lenencode2);
        else
            memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==2) { // ple compression
		if (ecmmode>1)
		{
			lenencodet1=pletterCompress( tiMem,tiMemEncode,lenencode);
			lenencodet2=pletterCompress( tiMem+256*8,tiMemEncode+256*8,lenencode);
			lenencodet3=pletterCompress( tiMem+256*8*2,tiMemEncode+256*8*2,lenencode);
		}
		else
			lenencode=pletterCompress( tiMem,tiMemEncode,lenencode);
		if (gramode==1)
		{
			memcpy(coMemEncode,coMem,32);
			lenencode1=32;
		}
		else
			lenencode1=pletterCompress(coMem,coMemEncode,lenencode1);
        if (savemap)
            lenencode2=pletterCompress(maMem,maMemEncode,lenencode2);
        else
            memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==3) { // dan1 compression
		if (ecmmode>1)
		{
			lenencodet1=dan1Compress( tiMem,tiMemEncode,lenencode);
			lenencodet2=dan1Compress( tiMem+256*8,tiMemEncode+256*8,lenencode);
			lenencodet3=dan1Compress( tiMem+256*8*2,tiMemEncode+256*8*2,lenencode);
		}
		else
        {
			lenencode=dan1Compress( tiMem,tiMemEncode,lenencode);
        }
		if (gramode==1)
		{
			memcpy(coMemEncode,coMem,32);
			lenencode1=32;
		}
		else
        {
			lenencode1=dan1Compress(coMem,coMemEncode,lenencode1);
        }
        if (savemap)
        {
            lenencode2=dan1Compress(maMem,maMemEncode,lenencode2);
        }
        else
        {
            memcpy(maMemEncode,maMem,lenencode2);
        }
    }		
	
	// write to files
	fprintf(fpc,"// Generated by gfx2col\n\n");
	
	if (ecmmode>1)
	{
		addcomment(fpc, (((num_tiles*8)-lenencodet1)*100)/(num_tiles*8),lenencodet1,compress);
		fprintf(fpc, "const unsigned char TILP1%s[%d]={\n", filenamef,lenencodet1);
		// write characters & colors
		for (t = 0; t < lenencodet1; t++) {
			if(t) {
				if((t & 31) == 0)
					fprintf(fpc, ",\n");
				else
					fprintf(fpc, ", ");
			}
			fprintf(fpc, "0x%02X", *(tiMemEncode+t));
		}
		fprintf(fpc, "\n};\n\n");
		if (ecmmode>2)
		{
			addcomment(fpc, (((num_tiles*8)-lenencodet2)*100)/(num_tiles*8),lenencodet2,compress);
			fprintf(fpc, "const unsigned char TILP2%s[%d]={\n", filenamef,lenencodet2);
			// write characters & colors
			for (t = 0; t < lenencodet2; t++) {
				if(t) {
					if((t & 31) == 0)
						fprintf(fpc, ",\n");
					else
						fprintf(fpc, ", ");
				}
				fprintf(fpc, "0x%02X", *(tiMemEncode+t+256*8));
			}
			fprintf(fpc, "\n};\n\n");
		}
		if (ecmmode>3)
		{
			addcomment(fpc, ((num_tiles*8-lenencodet3)*100)/(num_tiles*8),lenencodet3,compress);
			fprintf(fpc, "const unsigned char TILP3%s[%d]={\n", filenamef,lenencodet3);
			// write characters & colors
			for (t = 0; t < lenencodet3; t++) {
				if(t) {
					if((t & 31) == 0)
						fprintf(fpc, ",\n");
					else
						fprintf(fpc, ", ");
				}
				fprintf(fpc, "0x%02X", *(tiMemEncode+t+256*8*2));
			}
			fprintf(fpc, "\n};\n\n");
		}
	}
	else
	{
		addcomment(fpc, ((num_tiles*8-lenencode)*100)/(num_tiles*8),lenencode,compress);
		fprintf(fpc, "const unsigned char TIL%s[%d]={\n", filenamef,lenencode);
		// write characters & colors
		for (t = 0; t < lenencode; t++) {
			if(t) {
				if((t & 31) == 0)
					fprintf(fpc, ",\n");
				else
					fprintf(fpc, ", ");
			}
			fprintf(fpc, "0x%02X", *(tiMemEncode+t));
		}
		fprintf(fpc, "\n};\n\n");
	}
	
	// do that only if it is not a sprite
	if (sprmode==0) 
	{
		if (gramode!=1)
		{
			if (ecmmode>1)
			{
				addcomment(fpc, ((num_tiles-lenencode1)*100)/(num_tiles),lenencode1,compress);
			}
			else
				addcomment(fpc, ((num_tiles*8-lenencode1)*100)/(num_tiles*8),lenencode1,compress);
		}
		fprintf(fpc, "const unsigned char COL%s[%d]={\n", filenamef,lenencode1);
		for (t = 0; t < lenencode1; t++) {
			if(t) {
				if((t & 31) == 0)
					fprintf(fpc, ",\n");
				else
					fprintf(fpc, ", ");
			}
			fprintf(fpc, "0x%02X", *(coMemEncode+t));
		}
		fprintf(fpc, "\n};\n\n");
			
		// write map if needed
		if (savemap) {
			addcomment(fpc, ((mapw*maph-lenencode2)*100)/(mapw*maph),lenencode2,compress);
			fprintf(fpc, "const unsigned char MAP%s[%d]={\n", filenamef,lenencode2);
			for (t = 0; t < lenencode2; t++) {
				if(t) {
					if((t & 31) == 0)
						fprintf(fpc, ",\n");
					else
						fprintf(fpc, ", ");
				}
				fprintf(fpc, "0x%02X", *(maMemEncode+t));
			}
			fprintf(fpc, "\n};\n\n");
		}
	}

	// do that only if we need palette
	if (savepal) 
	{
		if (output_palette==64) 
		{
			fprintf(fpc, "const unsigned char PAL%s[%d]={\n", filenamef,(ecmmode==1) ? 16*2 : 64*2);
			if (ecmmode==1)
			{
				y=15;
				for (t = 0; t <  16; t++) 
				{
					if (t) 
					{
						if((t & 15) == 0)
							fprintf(fpc, ",\n");
						else
							fprintf(fpc, ", ");
					}

					val16b=*(palet+t);
					fprintf(fpc, "0x%02X", (val16b & 0xF00)>>8);
					fprintf(fpc, ",0x%02X", (val16b & 0xFF));
				}
			}
			else
			{
				y = (1<<(ecmmode-1))-1;
				for (t = 0; t < 64; t++) 
				{
					if (t) 
					{
						if((t & y) == 0)
							fprintf(fpc, ", // PAL %d\n",(t/(y+1))-1);
						else
							fprintf(fpc, ", ");
					}

					val16b=*(palet+t);
					fprintf(fpc, "0x%02X", (val16b & 0xF00)>>8);
					fprintf(fpc, ",0x%02X", (val16b & 0xFF));
				}
			}
			fprintf(fpc, "  // PAL %d\n};\n\n",(t/(y+1))-1);
		}
	}

	// write hearder file
	fprintf(fph, "#ifndef %s_INC_\n", filenamef);
	fprintf(fph, "#define %s_INC_\n\n", filenamef);
	if (ecmmode>1)
	{
		fprintf(fph, "#define SZTILP1%s %d\n", filenamef,lenencodet1);
		fprintf(fph, "#define SZTILP2%s %d\n", filenamef,lenencodet2);
		fprintf(fph, "#define SZTILP3%s %d\n", filenamef,lenencodet3);

	}
	else
		fprintf(fph, "#define SZTIL%s %d\n", filenamef,lenencode);
	// do that only if it is not a sprite
	if (sprmode==0) 
	{

		fprintf(fph, "#define SZCOL%s %d\n", filenamef,lenencode1);
		if (savemap)
			fprintf(fph, "#define SZMAP%s %d\n", filenamef,lenencode2);
		fprintf(fph,"\n");
	}
	if (ecmmode>1)
	{
		fprintf(fph, "extern const unsigned char TILP1%s[];\n", filenamef);
		fprintf(fph, "extern const unsigned char TILP2%s[];\n", filenamef);
		fprintf(fph, "extern const unsigned char TILP3%s[];\n", filenamef);
	}
	else
		fprintf(fph, "extern const unsigned char TIL%s[];\n", filenamef);
	// do that only if it is not a sprite
	if (sprmode==0) 
	{
		fprintf(fph, "extern const unsigned char COL%s[];\n", filenamef);
		if (savemap)
			fprintf(fph, "extern const unsigned char MAP%s[];\n", filenamef);
	}
	if (savepal)
	{
		if (output_palette==64) 
		{
			fprintf(fph, "extern const unsigned char PAL%s[];\n", filenamef);
		}
	}

	fprintf(fph, "\n\n");
	fprintf(fph, "#endif\n");
		
	fclose(fpc);fclose(fph);

    // free memory
    if (maMemEncode) free(maMemEncode);
    if (maMem) free(maMem);
    if (coMemEncode) free(coMemEncode);
    if (coMem) free(coMem);
    if (tiMemEncode) free(tiMemEncode);
    if (tiMem) free(tiMem);

	return -1;
} //end of Convert2Pic


//---------------------------------------------------------------------------
void PrintOptions(char *str) {
	printf("\n\nUsage : gfx2col [options] bmp/pcx/tga filename ...");
	printf("\n  where filename is a 256 color PNG, BMP, PCX or TGA file");
	printf("\n   and palette is only the 1st 16 colors in bitmap or");
	printf("\n   the 1st 64 colors in bitmap for F18A mode.");
	
	if(str[0]!=0)
		printf("\nThe [%s] parameter is not recognized.",str);

	printf("\n\nOptions are:");
	printf("\n\n--- General options ---");
	printf("\n-c[no|rle|ple|dan]    Compression method [no]");
	printf("\n\n--- Graphic options ---");
	printf("\n-s                    Generate sprite graphics");
	printf("\n-b                    Bitmap mode (no more 256 tiles limit)");
	printf("\n-g[m1|m2]             TMS9918 Graphic mode (mode 2 or mode 1) [m2]");
	printf("\n-e[0|1|2|3]           Enhanced Color Mode (F18A only) [0]");
	printf("\n-t                    Enable transparent tiles (color 0)");
	printf("\n\n--- Map options ---");
	printf("\n-m!                   Exclude map from output");
	printf("\n-m                    Convert the whole picture");
	printf("\n-mR!                  No tile reduction (not advised)");
	printf("\n-mn#              	Generate the whole picture with an offset for tile number");
	printf("\n                   	 where # is the offset in decimal (0 to 2047)");
	printf("\n\n--- Palette options ---");
	printf("\n-p!                   Exclude palette from output");
	printf("\n-po                   Export palette (16 colors (ecm0) or 64 colors(ecm1-3)");
	printf("\n-pR                   Palette rounding");
	printf("\n\n--- File options ---");
	printf("\n-f[bmp|pcx|tga|png]   Convert a bmp or pcx or tga or png file [bmp]");
	printf("\n\n--- Misc options ---");
	printf("\n-q                quiet mode");
	printf("\n");
	
	printf("\nExample:");
	printf("\ngfx2col -crle -fpng -m myimage.png");
	printf("\n  will convert a myimage png file to a inc file with 8x8 tiles, rle compressed");
	printf("\n  and a map.");
} //end of PrintOptions()

//---------------------------------------------------------------------------
// M A I N 
int main(int argc, char **arg) {
	unsigned int palette[64];
	unsigned int colorspal[256];//,coltilopt[256];
	unsigned char colornumbers;
	
	int height, width;
	int xsize, ysize,xsize1, ysize1;
	pcx_picture image;

	unsigned char *buffer, *buff=NULL;
	unsigned int *tilemap,*timap=NULL;

	char filebase[256]="";
	char filename[256];

	int i, x,y; 

    int file_type=BMP_FILE;
    int tile_reduction=1;         // 0 = no tile reduction (warning !)
    int savemap=1;                // 1 = save the map
    int savepal=1;                // 1 = save the palette
    
	// Show something to begin :)
	if (quietmode == 0) {
		printf("\n==============================");
		printf("\n---gfx2col v"GFX2COLVERSION" "GFX2COLDATE"---");
		printf("\n------------------------------");
		printf("\n(c) 2018-2019 Alekmaul ");
		printf("\n==============================\n");
	}

	//parse the arguments
	for(i=1;i<argc;i++)	
	{
		if(arg[i][0]=='-') 
		{
			if(arg[i][1]=='q') //quiet mode
			{
				quietmode=1;
			}
			else if(arg[i][1]=='t') // transparent tile mode 
			{
				transmode=1;
			}
			else if(arg[i][1]=='s') // sprite mode
			{
				sprmode=1;
				tile_reduction=0;
                savemap=0;          // Fix 200318 , no need for sprite
			}
			else if(arg[i][1]=='b') // bitmap mode
			{
				bmpmode=1;
			}			
			else if(arg[i][1]=='m') //map options
			{
				if( strcmp(&arg[i][1],"m") == 0)
				{
				}
				else if( strcmp(&arg[i][1],"m!") == 0)
				{
					savemap=0;
				}
				else if( strcmp(&arg[i][1],"mp") == 0)
				{
					highpriority=1;
				}
				else if(arg[i][2]=='n') //offset for tiles
				{
					offset_tile = atoi(&arg[i][3]);
				}
				else if( strcmp(&arg[i][1],"mR!") == 0)
				{
					tile_reduction=0;
				}
				else
				{
					PrintOptions(arg[i]);
					return 1;
				}
			}
			else if(arg[i][1]=='g')		// graphic mode
			{
				if( strcmp(&arg[i][1],"gm1") == 0)
				{
					gramode = 1; 
				}
			}
			else if(arg[i][1]=='p') 	//palette options
			{
				if( strcmp(&arg[i][1],"p!") == 0)
				{
					savepal=0;
				}
				else if(arg[i][2]=='o') 		//palette output (64 colors)
				{
					output_palette=64;
				}
				else if(arg[i][2]=='R') // color rounded
				{
					palette_rnd=1;
				}
			}
			else if(arg[i][1]=='e') 	//ecm mode
			{
				if(arg[i][2]=='o') 		//palette output (64 colors)
				{
					output_palette=64;
				}
				else if(arg[i][2]=='R') // color rounded
				{
					palette_rnd=1;
				}
				else if ( (arg[i][2]>='0') && (arg[i][2]<='3') )
				{
					ecmmode=1+arg[i][2]-48;
					output_palette=64;
				}
				else
				{
					PrintOptions(arg[i]);
					return 1;
				}
			}
			else if(arg[i][1]=='c')		//compression method
			{
				if( strcmp(&arg[i][1],"crle") == 0)
				{
					compress = 1; 
				}
				else if( strcmp(&arg[i][1],"cple") == 0)
				{
					compress = 2;
				}
				else if( strcmp(&arg[i][1],"cdan") == 0)
				{
					compress = 3; // DAN1 comrpession, everithing else is none
				}
			}
			else if(arg[i][1]=='f') //file type specification
			{
				if( strcmp(&arg[i][1],"fpcx") == 0)
				{
					file_type = 2; // PCX,
				}
				else if( strcmp(&arg[i][1],"ftga") == 0)
				{
					file_type = 3; // TGA
				}
				else if( strcmp(&arg[i][1],"fpng") == 0)
				{
					file_type = 4; // PNG, evething else is bmp
				}
			}
			else //invalid option
			{
				PrintOptions(arg[i]);
				return 1;
			}
		}
		else
		{
			//its not an option flag, so it must be the filebase
			if(filebase[0]!=0) // if already defined... there's a problem
			{
				PrintOptions(arg[i]);
				return 1;
			}
			else
				strcpy(filebase,arg[i]);
		}
	}

	if( filebase[0] == 0 ) {
		printf("\nERROR: You must specify a base filename.");
		PrintOptions("");
		return 1;
	}

	//Load picture
	if (filebase[strlen(filebase)-4] == '.') {
		filebase[strlen(filebase)-4] = '\0';
	}
	switch (file_type) {
		case 2 : // PCX
			sprintf(filename,"%s.pcx",filebase);
			if (quietmode == 0)
				printf("\nOpening graphics file: [%s]",filename);
			if(!PCX_Load(filename,(pcx_picture_ptr) &image))
				return 1;
			break;
		case 3 : // TGA
			sprintf(filename,"%s.tga",filebase);
			if (quietmode == 0)
				printf("\nOpening graphics file: [%s]",filename);
			if(!TGA_Load(filename,(pcx_picture_ptr) &image))
				return 1;
			break;
		case 4 : // PNG
			sprintf(filename,"%s.png",filebase);
			if (quietmode == 0)
				printf("\nOpening graphics file: [%s]",filename);
			if(!PNG_Load(filename,(pcx_picture_ptr) &image))
				return 1;
			break;
		default : // BMP for everithing else
			sprintf(filename,"%s.bmp",filebase);
			if (quietmode == 0)
				printf("\nOpening graphics file: [%s]",filename);
			if(!BMP_Load(filename,(pcx_picture_ptr) &image))
				return 1;
			break;
	}

	//convert the palette
	if (output_palette==64)
		ConvertPalette(image.palette, palette);
	
	height = image.header.height;
	width = image.header.width;
	xsize = width>>3; ysize = height>>3;
	
	//Print what the user has selected
	if (quietmode == 0) {
		printf("\n****** O P T I O N S ***************");
		if (sprmode)
			printf("\nSprite mode=ON");
		else {
			printf("\nSprite mode=OFF");
			if (transmode)
				printf("\nF18A Tile transparent mode=ON");
			else 
				printf("\nF18A Tile transparent mode=OFF");
			if (highpriority)
				printf("\nF18A Tile highpriority mode=ON");
			else
				printf("\nF18A Tile highpriority mode=OFF");
			if (ecmmode==0) 
			{
				if (bmpmode)
					printf("\nTMS9918 Bitmap mode2=ON");
				else {
					printf("\nTMS9918 Bitmap mode2=OFF");
					if (gramode == 0)
						printf("\nTMS9918 Graphic mode 2");
					else if (gramode == 1)
						printf("\nTMS9918 Graphic mode 1");
				}
			}
			if (tile_reduction)
				printf("\nOptimize tilemap=ON");
			else
				printf("\nOptimize tilemap=OFF");
		}
		if (ecmmode!=0) 
		{
			printf("\nF18A Enhanced Color Mode=%d",ecmmode-1);
		}
			
		if (file_type == 2)
			printf("\nPCX file: %dx%d pixels",width,height);
		else if (file_type == 3)
			printf("\nTGA file: %dx%d pixels",width,height);
		else if (file_type == 4)
			printf("\nPNG file: %dx%d pixels",width,height);
		else
			printf("\nBMP file: %dx%d pixels",width,height);

		if (compress == 0)
			printf("\nNo compression");
		else if (compress == 1)
			printf("\nRLE compression");
		else if (compress == 2)
			printf("\nPLE compression");
		else
			printf("\nDAN1 compression");

		if (output_palette)
			printf("\nPalette output=ON");
		else 
			printf("\nPalette output=OFF");

		printf("\n************************************");
	}
	fflush(stdout);

	if ( (bmpmode==1) && (width != 256) && (height != 192) )
	{
		printf("\nERROR : Image must have 256x192 pixel size in bitmap mode (%d,%d here).\n",width, height);
		return 1;
	}
		
	if (bmpmode) 
	{
		// prepare global map & tiles
		tilemap=(unsigned int *) malloc(32*24*sizeof(int));
		buffer = (unsigned char *) malloc( 768*8*8*sizeof(char) );
		ysize=0;

		// 3 parts for tiles in bitmap mode
        for (i=0;i<3;i++) {
			ysize1=8;xsize1=32;

			// Get tiles
            if (buff) free(buff);
            buff=ArrangeBlocks(image.buffer+i*256*64,256,64,8,&xsize1,&ysize1,8);
			if(buff==NULL)
			{
				printf("\nERROR:Not enough memory to do image operations...\n");
				return 1;
			
			}
			
			// Optimize map with current tile set
            if (timap) free (timap);
            timap=MakeMap(buff, colorspal, &ysize1, xsize1, ysize1, xsize1, ysize1, tile_reduction );
			if(timap==NULL) 
			{
				free(buff);
				printf("\nERROR:Not enough memory to do tile map optimizations..\n");
				return 1;
			}

			// load tiles
			ysize+=ysize1;
			if ( (ysize1>0)) {
				memcpy(buffer+i*256*8*8,buff,256*8*8);
							
				// change map at the end
                for (y=0;y<8;y++)
					for (x=0;x<32;x++)
						tilemap[x+y*32+i*256]=timap[x+y*32]+i*256;
			} // if ( (ysize>0))
		} // for (i=0;i<3;i++) {
		free(image.buffer);
		free(buff);
		free(timap);
	}
	else {
		// now re-arrange into a list of 8x8 blocks for easy conversion
		buffer=ArrangeBlocks( image.buffer, width, height, 8, &xsize, &ysize, 8);
		free(image.buffer);
		if(buffer==NULL)
		{
			printf("\nERROR:Not enough memory to do image operations...\n");
			return 1;
		}
			
		//make the tile map now
		tilemap=MakeMap(buffer, colorspal, &ysize, xsize, ysize, xsize, ysize, tile_reduction );
		if(tilemap==NULL) 
		{
			free(buffer);
			printf("\nERROR:Not enough memory to do tile map optimizations..\n");
			return 1;
		}

		// make color map now for mode 1 and if not F18A enhanced mode 1-3 (will erase colorspal, but don't care)
		if ( (ecmmode<=1) && (gramode==1) )
		{
			if ( MakeCol(buffer, tilemap,&colornumbers, colorspal,  &ysize,width/8, height/8) == 0)
			{
				return 1;
			}
			if (quietmode == 0) 
				printf("\nScreen uses %d colors.",colornumbers);
				
		}
	}

	if (ysize>0) 
	{
		if (quietmode == 0) 
		{
			if (tile_reduction)
				printf("\nReduced screen to %d tiles.",ysize);
			else
				printf("\nComputed screen with %d tiles.",ysize);
		}
		
		if ( (ysize>255) && (bmpmode==0)) 
		{
			printf("\nERROR : Image must have less than 256 tiles (%d here).\n",ysize);
			return 1;
		}
		
		//convert pictures and save to file
		if(!Convert2Pic(filebase, buffer, tilemap, palette, colorspal, ysize, width/8, height/8,savemap,savepal)) 
		{
			//free up image & tilemap memory
			free(tilemap);
			free(buffer);
			return 1;
		}
	}

	//free up image & tilemap memory
	free(tilemap);
	free(buffer);

	if (quietmode == 0)
		printf("\nDone !\n\n");

	return 0;
}
