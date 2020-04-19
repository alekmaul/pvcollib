/*---------------------------------------------------------------------------------

	Generic video functions

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

---------------------------------------------------------------------------------*/
/**
 * \file video.h
 * \brief contains the basic definitions for controlling the video hardware.
 *
 * This unit provides generic features related to video management.<br>
 *<br>
 * Here is the list of features:<br>
 * - enable and disable screen or vdp or nmi<br>
 * - clear, fill vram <br>
 * - retreive or set some vram area<br>
 * - update vram with some specific algorithm like Run Length, Pletter or Amy Purple DAN1 compression algorithm<br>
*/

#ifndef COL_VIDEO_INCLUDE
#define COL_VIDEO_INCLUDE

#include <coleco/coltypes.h>

#define FNTNORMAL          0
#define FNTITALIC          1
#define FNTBOLD            2
#define FNTBOLD_ITALIC     (FNTITALIC | FNTBOLD)

#define chrgen  			0x0000
#define coltab  			0x2000
#define chrtab 				0x1800
#define chrtab2				0x1B00
#define sprtab  			0x3800
#define sprgen  			0x1f00

#define COLTRANSP			0
#define COLBLACK			1
#define COLMEDGREEN			2
#define COLLITGREEN			3
#define COLDRKBLUE			4
#define COLLITBLUE			5
#define COLDRKRED			6
#define COLCYAN				7
#define COLMEDRED			8
#define COLLITRED			9
#define COLDRKYELLOW		10
#define COLLITYELLOW		11
#define COLDRKGREEN			12
#define COLMAGENTA			13
#define COLGREY				14
#define COLWHITE			15

/**
 *  \fn void vdp_setreg(u8 reg,u8 val)
 *  \brief Set a value to a TMS9918 register
 *
 *  \param  reg register number
 *  \param val value to assign
*/
void vdp_setreg(u8 reg,u8 val);

/**
 *  \fn void vdp_waitvblank(u16 numtime)
 *  \brief Waits for a vertical blank interrupt a number of time
 *
 *  \param numtime number of vblank to wait
*/
void vdp_waitvblank(u16 numtime);

/**
 * \fn void vdp_enablenmi(void)
 * \brief Enable NMI interruption
 *
*/
void vdp_enablenmi(void);

/**
 * \fn void vdp_disablenmi(void)
 * \brief Disable NMI interruption
*/
void vdp_disablenmi(void);

/**
 * \fn void vdp_enablevdp(void)
 *\brief Allows VDP to work (so screen is now active)
*/
void vdp_enablevdp(void);

/**
 * \fn void vdp_disablevdp(void)
 * \brief Disallows VDP to work (so screen is not active)
*/
void vdp_disablevdp(void);

/**
 * \fn void vdp_enablescr(void)
 * \brief Allows VDP to work and NMI to be catch
*/
void vdp_enablescr(void);

/**
 * \fn void vdp_disablescr(void)
 * \brief Disallows VDP to work and NMI to occur
*/
void vdp_disablescr(void);

/**
 * \fn void vdp_setmode1txt(void)
 * \brief Activate mode 1 in text mode<br>
 * Activate Mode 1 of TMS in text mode, 16K of VRAM, sprites 16x16<br>
 * 1 VRAM (duplicated each for 8 lines) that can be populate<br>
* CHRGEN is located at $0000, COLTAB is located at $2000<br>
*/
void vdp_setmode1txt(void);

/**
 * \fn void vdp_setmode2txt(void)
 * \brief Activate mode 2 in text mode<br>
 * Activate Mode 2 of TMS in text mode, 16K of VRAM, sprites 16x16<br>
 * 3 VRAM areas (each for 8 lines) that can be populate<br>
*/
void vdp_setmode2txt(void);

/**
 * \fn void vdp_setmode2bmp(void)
 * \brief Activate mode 2 in bitmap mode<br>
 * Activate Mode 2 of TMS in bitmap mode, 16K of VRAM, sprites 16x16<br>
 * 1 VRAM area (complete screen of 24 lines) that can be populate<br>
*/
void vdp_setmode2bmp(void);

/**
 * \fn void vdp_setcharex(u8 first,u8 count, unsigned offset,u8 flags)
 * \brief  Put some chars of default font
 *
 * \param first 1st char to put
 * \param count number of chars
 * \param offset address location in video memory for the chararter patterns
 * \param flags type of font<br>
 *  FNTNORMAL for the normal font<br>
 *  FNTITALIC for the italic font<br>
 *  FNTBOLD for the bold font<br>
 *  FNTBOLD_ITALIC fot both italic and bold font<br>
*/
void vdp_setcharex(u8 first,u8 count, unsigned offset,u8 flags);

/**
 * \fn void vdp_putchar (u8 x, u8 y, char value)
 * \brief  Put a single char on screen
 *
 * \param x colum to print
 * \param y line to print
 * \param value ascii value of char to print
*/
void vdp_putchar (u8 x, u8 y, char value);

/**
 * \fn u8  vdp_getchar (u8 x, u8 y)
 * \brief  Get a single char from screen
 *
 * \param x column of the char
 * \param y line of the char
 * \return char value from screen
*/
u8  vdp_getchar (u8 x, u8 y);

/**
 * \fn void vdp_getarea (void *table, u8 x, u8 y, u8 width, u8 height)
 * \brief  Get an area of chars from screen
 *
 * \param table area in ram to save chars
 * \param x column of the chars
 * \param y line of the chars
 * \param width length of x chars to get
 * \param height length of y chars to get
*/
void vdp_getarea (void *table, u8 x, u8 y, u8 width, u8 height);

/**
 * \fn void vdp_putarea (void *table, u8 x, u8 y, u8 width, u8 height)
 * \brief  Put an area of chars to screen
 *
 * \param table area in ram to load chars
 * \param x column of the chars
 * \param y line of the chars
 * \param width length of x chars to put
 * \param height length of y chars to put
*/
void vdp_putarea (void *table, u8 x, u8 y, u8 width, u8 height);

/**
 * \fn  void vdp_setdefaultchar(u8 flags)
 * \brief Put default font
 *
 * \param flags	character format<br>
 * FNTNORMAL for the normal font<br>
 * FNTITALIC for the italic font<br>
 * FNTBOLD for the bold font<br>
 * FNTBOLD_ITALIC fot both italic and bold font<br>
*/
void vdp_setdefaultchar(u8 flags);

/**
 * \fn  void vdp_putstring(u8 x, u8 y, char* text)
 * \brief Put a string on screen at coordinates x,y
 *
 * \param x column x
 * \param y line y
 * \param text text to display
*/
void vdp_putstring(u8 x, u8 y, char* text);

/**
 * \fn void vdp_fillvram(u16 offset,u8 value,u16 count)
 * \brief  Change VRAM with a specific value
 *
 * \param offset  address in VRAM
 * \param value  value to set
 * \param count  number of time we set value
*/
void vdp_fillvram(u16 offset,u8 value,u16 count);

/**
 * \fn void vdp_duplicatevram(void)
 * \brief Put 1st area of pattern vram to the 2nd and 3rd one
*/
void vdp_duplicatevram(void);

/**
 * \fn void vdp_putvram (unsigned offset,void *data,unsigned count)
 * \brief Fill the VRAM with some non compressed data 
 *
 * \param offset address in VRAM of 1st data to fill
 * \param data address of data to use
 * \param count number of data
*/
void vdp_putvram (unsigned offset,void *data,unsigned count);

/**
 * \fn void vdp_putvram (unsigned offset,void *data,unsigned count)
 * \brief Get from VRAM to a RAM pointer with some non compressed data 
 *
 * \param offset address in VRAM of 1st data to get
 * \param data address of RAM to use
 * \param count number of data
*/
void vdp_getvram (unsigned offset,void *data,unsigned count);

/**
 * \fn void *vdp_rle2vram (void *rledata,unsigned offset)
 * \brief Fill the VRAM with some RLE compressed data 
 * 
 * \param rledata address of data to use
 * \param offset address in VRAM of 1st data to fill
*/
void *vdp_rle2vram (void *rledata,unsigned offset);

/**
 * \fn void vdp_ple2vram(void *pledata, unsigned offset)
 * \brief Put data in VRAM with some Pletter compressed data 
 *
 * \param pledata address of data to use
 * \param offset address in VRAM of 1st data to fill
*/
void vdp_ple2vram(void *pledata, unsigned offset);

/**
 * \fn void vdp_dan2vram(void *dandata, unsigned offset)
 * \brief Put data in VRAM with some DAN1 compressed data 
 *
 * \param dandata address of data to use
 * \param offset address in VRAM of 1st data to fill
*/
void vdp_dan2vram(void *dandata, unsigned offset);


/*! \fn vdp_blocknmi 
	\brief set no_nmi flag to avoid nmi 
*/
#define vdp_blocknmi()  { __asm__("\tpush hl\n\tld hl,#_no_nmi\n\tset 0,(hl)\n\tpop hl"); }

/*! \fn vdp_releasenmi 
	\brief reset no_nmi flag to allow nmi 
*/
#define vdp_releasenmi() { __asm__("\tpush hl\n\tld hl,#_no_nmi\n\tbit 7,(hl)\n\tjp z,.+6\n\tcall _nmi_direct\n\tres 0,(hl)\n\tpop hl"); }

#endif
