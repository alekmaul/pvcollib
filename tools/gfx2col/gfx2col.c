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
	
***************************************************************************/

//INCLUDES
#include "gfx2col.h"

enum FILE_TYPE {
    BMP_FILE = 1,
    PCX_FILE,
    TGA_FILE,
    PNG_FILE
};

int quietmode=0;              // 0 = not quiet, 1 = i can't say anything :P
int compress=0;               // 0 = not compressed, 1 = rle compression, 2 = ple compression, 3 = dan1 compression
int sprmode=0;                // 0 = background 1 = sprite
int bmpmode=0;				  // 0 = normal tile mode, 1=bitmap mode

//// F U N C T I O N S //////////////////////////////////////////////////////////
int PCX_Load(char *filename, pcx_picture_ptr image)
{
	// this function loads a pcx file into a picture structure, the actual image
	// data for the pcx file is decompressed and expanded into a secondary buffer
	// within the picture structure, the separate images can be grabbed from this
	// buffer later.  also the header and palette are loaded

	long num_bytes,index;
	long count;
	long x,y;
	unsigned char data;
	pcx_header *header;
	FILE *fp;

	// open the file
	fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		printf("\nERROR: Can't open file [%s]",filename);
		return 0;
	}

	// load the header
	header = &image->header;
	fread(header, 1, 128, fp);

	header->width++;
	header->height++;

	// check to make sure this is a 256 color PCX
	if( (header->manufacturer != 10) ||
		(header->encoding != 1) ||
		(header->bits_per_pixel != 8) ||
		(header->num_color_planes != 1) ||
		(header->palette_type != 1) )
	{
		printf("\nERROR: File [%s] is not recognized as a 256 color PCX.",filename);
		fclose(fp);
		return 0;
	}

	//allocate memory for the picture + 64 empty lines
	image->buffer = malloc( (size_t)(header->height+64)*header->width );
	if(image->buffer == NULL)
	{
		printf("\nERROR: Can't allocate enough memory for the picture.");
		fclose(fp);
		return 0;
	}

	//initally clear the memory (to make those extra lines be blank)
	memset(image->buffer,0,(size_t)(header->height+64)*header->width);

	// load the data and decompress into buffer
	count=0;
	for(y=0; y < header->height; y++)
	{
		for(x=0; x < header->width; x++)
		{
			// get the first piece of data
			data = getc(fp);

			// is this a rle?
			//if ( (data>=192) && (data<=255))
			if (data>=192)
			{
				// how many bytes in run?
				num_bytes = data-192;
				x += (num_bytes-1);

				// get the actual data for the run
				data = getc(fp);

				// replicate data in buffer num_bytes times
				while( num_bytes-- > 0)
					image->buffer[count++] = data;

			} // end if rle
			else
		    {
				// actual data, just copy it into buffer at next location
				image->buffer[count++] = data;
			} // end else not rle

		} //end of x loop

		//get rid of the padding byte if there is one
		if( x < header->bytes_per_line)
			data = getc(fp);

	} //end of y loop

	//Get the Palette header (one byte, should equal 12)
	data = getc(fp);
	if(data != 12)
	{
		printf("\nERROR: Couldn't find palette header [%s]",filename);
		free(image->buffer);
		fclose(fp);

		return 0;
	}

	//get the pallete data
	for (index=0; index<256; index++)
    {
	    image->palette[index].red   = (getc(fp) >> 2);
	    image->palette[index].green = (getc(fp) >> 2);
	    image->palette[index].blue  = (getc(fp) >> 2);
    }

	//check to make sure there weren't errors while reading the file
	if(ferror(fp))
	{
		printf("\nERROR: Error reading file [%s]",filename);
		free(image->buffer);
		fclose(fp);
		return 0;
	}

	fclose(fp);

	return -1;

} // end PCX_Load

void BMP_BI_RLE8_Load(pcx_picture_ptr image,
	const bmp_header* const bmphead, const bmp_info_header* const bmpinfohead,
	FILE* fp)
{
	// BI_RLE8 decompress according to:
	// https://technet.microsoft.com/ru-ru/dd183383

	unsigned long line, i, count;
	// offset in image buffer where current line starts
	unsigned int pos;
	unsigned char ch, ch2;

	// start from bottom line
	line = bmpinfohead->biHeight;
	pos = (line-1) * bmpinfohead->biWidth;

	count = 0;
	// read all image bytes
	while (count < bmpinfohead->biSizeImage)
	{
		ch = getc(fp);
		++count;
		if (ch)
		{
			// repeat byte
			ch2 = getc(fp);
			++count;
			for (i = 0; i < ch; ++i) image->buffer[pos++] = ch2;
			continue;
		}

		// escape char
		ch = getc(fp);
		++count;
		if (ch == 0)
		{
			// End of line.

			// go one line up
			--line;
			// start of this line.
			pos = (line-1) * bmpinfohead->biWidth;
		}
		else if (ch == 1)
		{
			// End of bitmap.
			break;
		}
		else if (ch == 2)
		{
			// Delta.
			// The two bytes following the escape contain unsigned values
			// indicating the horizontal and vertical offsets of the next pixel
			// from the current position.

			ch = getc(fp);
			++count;
			// go right in the buffer
			pos += ch;

			ch = getc(fp);
			++count;
			// go given lines up
			line -= ch;
			pos -= bmpinfohead->biWidth * ch;
		}
		else
		{
			// Absolute mode.
			// The second byte represents the number of bytes that follow,
			// each of which contains the color index of a single pixel.
			ch = getc(fp);
			++count;
			for (i = 0; i < ch; ++i)
			{
				image->buffer[pos++] = getc(fp);
				++count;
			}
			if (i%2)
			{
				// Each run must be aligned on a word boundary.
				// Read and throw away the placeholder.
				ch2 = getc(fp);
				++count;
			}
		}
	}
} // end BMP_BI_RLE8_Load

int BMP_Load(char *filename, pcx_picture_ptr image)
{
	// this function loads a bmp file into a picture structure, the actual image
	// data for the bmp file is decompressed and expanded into a secondary buffer
	// within the picture structure, the separate images can be grabbed from this
	// buffer later.  also the header and palette are loaded
	FILE *fp;
	int index,i;
	pcx_header *header;
	bmp_header bmphead;
	bmp_info_header bmpinfohead;

	// open the file
	fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		printf("\nERROR: Can't open file [%s]",filename);
		return 0;
	}

	// check to see if it is a valid bitmap file
	if (fread(&bmphead, sizeof(bmp_header), 1, fp)  < 1)
	{
		printf("\nERROR: File [%s] has no correct BMP header.",filename);
		fclose(fp);
		return 0;
	}

	if (bmphead.bfType != BF_TYPE)
	{
		printf("\nERROR: File [%s] is not recognized as a BMP file.",filename);
		fclose(fp);
		return 0;
	}

	// check to see if it is a valid bitmap file
	if (fread(&bmpinfohead, sizeof(bmpinfohead), 1, fp)  < 1)
	{
		printf("\nERROR: File [%s] has no correct BMP info header.",filename);
		fclose(fp);
		return 0;
	}

	if (bmpinfohead.biBitCount != 8 || (bmpinfohead.biCompression != 0 && bmpinfohead.biCompression != 1 /*BI_RLE8*/))
	{
		printf("\nERROR: File [%s] is not a valid BMP file: 256 colors, non-compressed or BI_RLE8 supported.",filename);
		fclose(fp);
		return 0;
	}

	// seek to palette
	fseek(fp, sizeof(bmp_header) + bmpinfohead.biSize, 0);

	// initally clear the palette if there are less then 256 colors in the file
	memset(image->palette, 0, (size_t)(256 * sizeof(RGB_color)));

	// read the palette information
	for (index = 0; index<256; index++)
	{
		image->palette[index].blue = getc(fp) >> 2;
		image->palette[index].green = getc(fp) >> 2;
		image->palette[index].red = getc(fp) >> 2;
		//data=getc(fp);
		getc(fp);
	}

	header = &image->header;
	header->width = bmpinfohead.biWidth;
	header->height = bmpinfohead.biHeight;

	// allocate memory for the picture + 64 empty lines
	image->buffer = malloc( (size_t)(header->height+64) * header->width );
	if(image->buffer == NULL)
	{
		printf("\nERROR: Can't allocate enough memory for the picture.");
		fclose(fp);
		return 0;
	}

	// initally clear the memory (to make those extra lines be blank)
	memset(image->buffer,0,(size_t)(header->height+64) * header->width);

	// seek to image data
	fseek(fp, bmphead.bfOffBits, 0);

	// read the bitmap
	if (bmpinfohead.biCompression == 0)
	{
		for(index=(header->height-1) * header->width;index>=0;index-=header->width)
			for(i=0;i<header->width;i++) 
				image->buffer[index+i] = getc(fp);
	}
	else if (bmpinfohead.biCompression == 1)
	{
		// BI_RLE8
		BMP_BI_RLE8_Load(image, &bmphead, &bmpinfohead, fp);
	}

	// check to make sure there weren't errors while reading the file
	if(ferror(fp))
	{
		printf("\nERROR: Error reading file [%s]",filename);
		free(image->buffer);
		fclose(fp);
		return 0;
	}

	fclose(fp);

	return -1;
} // end BMP_Load

int TGA_Load(char *filename, pcx_picture_ptr image)
{
	// this function loads a tga file into a picture structure, the actual image
	// data for the bmp file is decompressed and expanded into a secondary buffer
	// within the picture structure, the separate images can be grabbed from this
	// buffer later.  also the header and palette are loaded
	FILE *fp;
	long index,i;
	tga_header tgahead;
	pcx_header *header;

	// open the file
	fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		printf("\nERROR: Can't open file [%s]",filename);
		return 0;
	}

	// check to see if it is a valid bitmap file
	if (fread(&tgahead, sizeof(tga_header), 1, fp)  < 1)
	{
		printf("\nERROR: File [%s] has no correct TGA header.",filename);
		fclose(fp);
		return 0;
	}


		//check to make sure there weren't errors while reading the file
	if(ferror(fp))
	{
		printf("\nERROR: Error reading file [%s]",filename);
		free(image->buffer);
		fclose(fp);
		return 0;
	}


	if (tgahead.BPP != 8 || tgahead.ImageType != 1)
	{
		printf("\nERROR: File [%s] is not a valid indexed 256 colors TGA file.",filename);
		fclose(fp);
		return 0;
	}

	header = &image->header;
	header->width = tgahead.Width;
	header->height = tgahead.Height;

  //allocate memory for the picture + 64 empty lines
	image->buffer = malloc( (size_t)(header->height+64) * header->width );
	if(image->buffer == NULL)
	{
		printf("\nERROR: Can't allocate enough memory for the picture.");
		fclose(fp);
		return 0;
	}

	//initally clear the memory (to make those extra lines be blank)
	memset(image->buffer,0,(size_t)(header->height+64) * header->width);

	// read the palette information
	for(index=0;index<256;index++)
	{
		image->palette[index].blue = getc(fp) >> 2;
		image->palette[index].green = getc(fp) >> 2;
		image->palette[index].red = getc(fp) >> 2;
	}

	// read the bitmap
	for(index=(header->height-1) * header->width;index>=0;index-=header->width)
		for(i=0;i<header->width;i++)
			image->buffer[index+i] = getc(fp);

	fclose(fp);

	return -1;
} // end TGA_Load

int PNG_Load(char *filename, pcx_picture_ptr image)
{
  unsigned error,sz,bpp;
  long i,index;
  unsigned char *pngimage;
  unsigned char* png = 0;
  size_t pngsize;
  LodePNGState state;
  unsigned int width, height;// , wal,hal;
  pcx_header *header;

  /*optionally customize the state*/
  lodepng_state_init(&state);

  // no conversion of color (to keep palette mode)
	state.decoder.color_convert = 0;

	error = lodepng_load_file(&png, &pngsize, filename);
  if (!error)  {
		error = lodepng_decode(&pngimage, &width, &height, &state, png, pngsize);
	}
	if(error) {
		printf("\nERROR: Decoder error %u: %s\n", error, lodepng_error_text(error));
		free(png);
		lodepng_state_cleanup(&state);
		free(pngimage);
		return 0;
	}

	bpp = state.info_raw.bitdepth;
	if ( (bpp  != 4) && (bpp != 8)) {
		printf("\nERROR: File [%s] is not a valid bbp value (%d bpp).",filename,bpp);
		free(png);
		lodepng_state_cleanup(&state);
		free(pngimage);
		return 0;
	}
	
	if (state.info_raw.colortype != LCT_PALETTE)
  {
		printf("\nERROR: File [%s] is not a valid indexed palette mode (mode %d).",filename,state.info_raw.colortype);
		free(png);
		lodepng_state_cleanup(&state);
		free(pngimage);
		return 0;
	}

	// read the palette information
	sz=state.info_png.color.palettesize;
	for(index=0;index<sz;index++) {
		image->palette[index].red = state.info_png.color.palette[(index*4) + 0]>>2;
		image->palette[index].green = state.info_png.color.palette[(index*4) + 1]>>2;
		image->palette[index].blue = state.info_png.color.palette[(index*4) + 2]>>2;
	}
	
	// get png information
	header = &image->header;
	header->width = width;
	header->height = height;

	printf("\nWidth %d Height %d",header->width,header->height);

  //allocate memory for the picture + 64 empty lines
	image->buffer = malloc( (size_t)(header->height+64) * header->width );
	if(image->buffer == NULL)	{
		printf("\nERROR: Can't allocate enough memory for the picture.");
		return 0;
	}

	//initally clear the memory (to make those extra lines be blank)
	memset(image->buffer,0,(size_t)(header->height+64) * header->width);

	// 4 bpps conversion
	if (bpp==4) {
	
		for (index = 0; index < header->height; index++) {
			for(i=0;i<header->width;i++)
				image->buffer[index+i] = pngimage[i +index*header->height];
		}
		/*
      // get buffer size
			*size = (wAligned / 2) * hAligned;
			// and alloc
			result = malloc(*size);

			srcPix = 0;
			for (i = 0; i < h; i++)
			{
				unsigned char *dst = &result[i * (wAligned / 2)];

				memset(dst, 0, wAligned / 2);

				for (j = 0; j < w; j++)
				{
						unsigned char v;

						if (srcPix & 1) v = (out[srcPix / 2] >> 4) & 0xF;
						else v =  (out[srcPix / 2] >> 0) & 0xF;
						srcPix++;

						if (j & 1) dst[j / 2] = (dst[j / 2] & 0x0F) | (v << 4);
						else dst[j / 2] = (dst[j / 2] & 0xF0) | (v << 0);
				}
			}
			for(;i < hAligned; i++)
				memset(&result[i * (wAligned / 2)], 0, wAligned / 2);
			*/
	}
	// 8 bpps conversion
	else {

		for (index = 0; index < header->height; index++) {
			for(i=0;i<header->width;i++) {
				image->buffer[i+(header->width*index)] = pngimage[i+(header->width*index)];
			}
		}
	}
			
	free(png);

	lodepng_state_cleanup(&state);

	free(pngimage);
	
	return -1;
} // end PNG_Load

//////////////////////////////////////////////////////////////////////////////
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
unsigned int *MakeMap(unsigned char *img, int *num_tiles, int xsize, int ysize, int tile_x, int tile_y, unsigned char optimize)
{
	unsigned int *map;
	//unsigned char blank[8*8];
	int newtiles;
	int current;	//the current tile we're looking at
	int i,t;
	int x,y;

	//allocate map
	map=(unsigned int *) malloc((size_t)tile_x*tile_y*sizeof(int));
	if(map==NULL) {
		return 0;
	}

	//clear map
	memset(map,0,tile_x*tile_y*sizeof(int));

    //I want tile #0 to be blank..
	current=0;
	t=0;
	newtiles=0;

	for(y=0;y<ysize;y++)
    {
    	for(x=0;x<xsize;x++)
	    {
    		//check for matches with previous tiles if tile_reduction on
            if (optimize)
            {
    			for(i=0;i<newtiles;i++)
	    			if( memcmp(&img[i*64],&img[current*64],64) == 0 )
		    			break;
            }
            else
                i=newtiles;

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
	    	map[y*tile_x+x] = t;
		}

		//goto the next tile
		current++;
	}

	//also return the number of new tiles
	*num_tiles = newtiles;

	return map;

}//end of MakeMap

//---------------------------------------------------------------------------
void addcomment(FILE *fp, unsigned int ratio,unsigned int size,unsigned int compress) {
	if (compress==1)
		fprintf(fp, "// rle compression %d bytes (%d%%)\n", size,ratio);
	else if (compress==2)
		fprintf(fp, "// ple compression %d bytes (%d%%)\n", size,ratio);
	else if (compress==3)
		fprintf(fp, "// dan1 compression %d bytes (%d%%)\n", size,ratio);
}

int Convert2Pic(char *filebase, unsigned char *buffer, unsigned int *tilemap, int num_tiles, int mapw, int maph, int savemap)
{
	char filenamec[256],filenameh[256];
	unsigned char bufsw1[8];
	unsigned char value, minv,maxv;
	unsigned char valtil[8];
	unsigned char *tiMem,*tiMemEncode,*coMem,*coMemEncode, *maMem, *maMemEncode;
	int x,y,t;
    int lenencode,lenencode1,lenencode2;
	FILE *fpc,*fph;

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

    tiMem = (unsigned char *) malloc(num_tiles*8);
    tiMemEncode = (unsigned char *) malloc(num_tiles*8);
    coMem = (unsigned char *) malloc(num_tiles*8);
    coMemEncode = (unsigned char *) malloc(num_tiles*8);
    maMem = (unsigned char *) malloc(mapw*maph);
    maMemEncode = (unsigned char *) malloc(mapw*maph);

	if (quietmode == 0) {
		printf("\nDecode for %d tiles...\n",num_tiles);
	}

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
			*(coMem+y+t*8)=(maxv<<4) | (minv);    // FG color | BG color 
		}
	}

	// sprites are 16x16 but we need to swap tile 2<>3  
	if (sprmode)
	{
		for(t=0;t<num_tiles;t+=4) { //loop through tiles
			memcpy(bufsw1,(tiMem+(t+1)*8),8);
			memcpy((tiMem+(t+1)*8),(tiMem+(t+2)*8),8);
			memcpy((tiMem+(t+2)*8),bufsw1,8);
		}
	}

    // Get map if possible (so not in sprite mode)
	if (tilemap!=NULL)
	{
		for (t=0;t<mapw*maph;t++) {
			// if bitmap mode, adapt it
			/*
			if (ckTiBitmapMap->Checked) {
				if (mgTil->MapData[i]>=256*2)
					*(maMem+i)=mgTil->MapData[i]-256*2;
				else if (mgTil->MapData[i]>=256)
					*(maMem+i)=mgTil->MapData[i]-256;
				else
					*(maMem+i)=mgTil->MapData[i];
			}
			else*/
				*(maMem+t)=tilemap[t];
		}
    }
	
	// write files regarding compression type
	lenencode=num_tiles*8;lenencode1=num_tiles*8;lenencode2=mapw*maph;
	if (compress==0) { // no compression
		memcpy(tiMemEncode,tiMem,lenencode);
		memcpy(coMemEncode,coMem,lenencode1);
		memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==1) { // rle compression
        lenencode=rleCompress( tiMem,tiMemEncode,num_tiles*8);
        lenencode1=rleCompress(coMem,coMemEncode,num_tiles*8);
        if (savemap)
            lenencode2=rleCompress(maMem,maMemEncode,mapw*maph);
        else
            memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==2) { // ple compression
        lenencode=pletterCompress( tiMem,tiMemEncode,num_tiles*8);
        lenencode1=pletterCompress(coMem,coMemEncode,num_tiles*8);
        if (savemap)
            lenencode2=pletterCompress(maMem,maMemEncode,mapw*maph);
        else
            memcpy(maMemEncode,maMem,lenencode2);
	}
	else if (compress==3) { // dan1 compression
        lenencode=dan1Compress( tiMem,tiMemEncode,num_tiles*8);
        lenencode1=dan1Compress(coMem,coMemEncode,num_tiles*8);
        if (savemap)
            lenencode2=dan1Compress(maMem,maMemEncode,mapw*maph);
        else
            memcpy(maMemEncode,maMem,lenencode2);
    }		
	
	// write to files
	fprintf(fpc,"// Generated by gfx2col\n\n");
	
	addcomment(fpc, ((num_tiles*8-lenencode)*100)/(num_tiles*8),lenencode,compress);
	fprintf(fpc, "const unsigned char TIL%s[%d]={\n", filebase,lenencode);
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
	
	// do that only if it is not a sprite
	if (sprmode==0) 
	{
		addcomment(fpc, ((num_tiles*8-lenencode1)*100)/(num_tiles*8),lenencode1,compress);
		fprintf(fpc, "const unsigned char COL%s[%d]={\n", filebase,lenencode1);
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
			addcomment(fpc, ((mapw*maph-lenencode1)*100)/(mapw*maph),lenencode2,compress);
			fprintf(fpc, "const unsigned char MAP%s[%d]={\n", filebase,lenencode2);
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

	// write hearder file
	fprintf(fph, "#ifndef %s_INC_\n", filebase);
	fprintf(fph, "#define %s_INC_\n\n", filebase);
	fprintf(fph, "#define SZTIL%s %d\n", filebase,lenencode);
	// do that only if it is not a sprite
	if (sprmode==0) 
	{

		fprintf(fph, "#define SZCOL%s %d\n", filebase,lenencode1);
		if (savemap)
			fprintf(fph, "#define SZMAP%s %d\n", filebase,lenencode2);
		fprintf(fph,"\n");
	}
	fprintf(fph, "extern const unsigned char TIL%s[];\n", filebase);
	// do that only if it is not a sprite
	if (sprmode==0) 
	{
		fprintf(fph, "extern const unsigned char COL%s[];\n", filebase);
		if (savemap)
			fprintf(fph, "extern const unsigned char MAP%s[];\n", filebase);
	}
	
	fprintf(fph, "\n\n");
	fprintf(fph, "#endif\n");
		
	fclose(fpc);fclose(fph);

    // free memory
    free(maMemEncode);
    free(maMem);
    free(coMemEncode);
    free(coMem);
    free(tiMemEncode);
    free(tiMem);
	
	return -1;
} //end of Convert2Pic


//---------------------------------------------------------------------------
void PrintOptions(char *str) {
	printf("\n\nUsage : gfx2col [options] bmp/pcx/tga filename ...");
	printf("\n  where filename is a 256 color PNG, BMP, PCX or TGA file");
	printf("\n   and palette is only the 1st 16 colors in bitmap.");
	
	if(str[0]!=0)
		printf("\nThe [%s] parameter is not recognized.",str);

	printf("\n\nOptions are:");
	printf("\n\n--- General options ---");
	printf("\n-c[no|rle|ple|dan]    Compression method [no]");
	printf("\n\n--- Graphic options ---");
	printf("\n-s                    Generate sprite graphics");
	printf("\n-b                    Bitmap mode (no more 256 iltes limit)");
	printf("\n\n--- Map options ---");
	printf("\n-m!                   Exclude map from output");
	printf("\n-m                    Convert the whole picture");
	printf("\n-mR!                  No tile reduction (not advised)");
	printf("\n\n--- File options ---");
	printf("\n-f[bmp|pcx|tga|png]   Convert a bmp or pcx or gta or png file [bmp]");
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
	int height, width;
	int xsize, ysize;
	pcx_picture image;

	unsigned char *buffer;
	unsigned int *tilemap;

	//FILE *fp;
	char filebase[256]="";
	char filename[256];

	int i; //, j;

    int file_type=BMP_FILE;
    int tile_reduction=1;         // 0 = no tile reduction (warning !)
    int savemap=1;                // 1 = save the map
    
	// Show something to begin :)
	if (quietmode == 0) {
		printf("\n==============================");
		printf("\n---gfx2col v"GFX2COLVERSION" "GFX2COLDATE"---");
		printf("\n------------------------------");
		printf("\n(c) 2018-2019 Alekmaul ");
		printf("\n==============================\n");
	}

	//parse the arguments
	for(i=1;i<argc;i++)	{
		if(arg[i][0]=='-') {
			if(arg[i][1]=='q') //quiet mode
			{
				quietmode=1;
			}
			else if(arg[i][1]=='s') // sprite mode
			{
				sprmode=1;
				tile_reduction=0;
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
			else if(arg[i][1]=='c') //compression method
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

			if (bmpmode)
				printf("\nBitmap mode=ON");
			else {
				printf("\nnBitmap mode=OFF");

			if (tile_reduction)
				printf("\nOptimize tilemap=ON");
			else
				printf("\nOptimize tilemap=OFF");
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


		printf("\n************************************");
	}
	fflush(stdout);

	if ( (bmpmode==1) && (width != 256) && (height != 192) )
	{
		printf("\nERROR : Image must have 256x192 pixel size in bitmap mode (%d,%d here).\n",width, height);
		return 1;
	}
		
	buffer=ArrangeBlocks( image.buffer, width, height, 8, &xsize, &ysize, 8);
	free(image.buffer);
	if(buffer==NULL)
	{
		printf("\nERROR:Not enough memory to do image operations...\n");
		return 1;
	}

	//make the tile map now
	tilemap=MakeMap(buffer, &ysize, xsize, ysize, xsize, ysize, tile_reduction );
	if(tilemap==NULL)
	{
		free(buffer);
		printf("\nERROR:Not enough memory to do tile map optimizations..\n");
		return 1;
	}

	if (ysize>0) {
		if (quietmode == 0) {
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
		if(!Convert2Pic(filebase, buffer, tilemap, ysize, width/8, height/8,savemap))
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

