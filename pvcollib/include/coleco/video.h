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

/*!	\file video.h
	\brief contains the basic definitions for controlling the video hardware.

*/
#ifndef COL_VIDEO_INCLUDE
#define COL_VIDEO_INCLUDE

#include <coleco/coltypes.h>

#define FNTNORMAL          0
#define FNTITALIC          1
#define FNTBOLD            2
#define FNTBOLD_ITALIC     (FNTITALIC | FNTBOLD)

/*! 
	\brief VDP table addresses 
	0x0000	Character table
	0x2000	Color of Character table
	0x1800	Map table
	0x3800	Sprite character table
	0x1b00	Sprite table
*/
#define chrgen  			0x0000
#define coltab  			0x2000
#define chrtab 				0x1800
#define sprtab  			0x3800
#define sprgen  			0x1b00

/*! 
	\brief VDP F18a table addresses 
	0x2800	Sprite location f18a
	0x3000	Sprite location f18a
	0x3800	Sprite location f18a
	0x1f00	256 bytes for extra ram
*/
#define sprtab_f18a  		0x2800 //f18a colour only requires 1/3 the colour data? til 0x2800
#define sprtab_f18a_2  		0x3000
#define sprtab_f18a_3	 	0x3800
#define mapvram 			0x1f00  

/*! 
	\brief VDP colors definition (0..15)
	0 		Transparent
	1 		Black
	2 		Medium green
	3 		Light green
	4 		Dark blue
	5 		Light blue
	6 		Dark red
	7 		Cyan
	8 		Medium red
	9 		Light red
	10 		Dark 	yellow
	11		Light yellow 
	12		Dark green
	13		Magenta
	14		Gray
	15		White
*/
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
 *  \brief
 *      vdp_f18aok is set if f18a module is present
 *		 when calling vdp_setmodef18a function.
 */
extern volatile u8 vdp_f18aok;

/*! \fn  vdp_setreg(u8 reg,u8 val)
	\brief Set a VDP register
	\param reg register number
	\param val value to assign
	Set a value to a TMS9918 register
*/
void vdp_setreg(u8 reg,u8 val);

/*! \fn  vdp_waitvblank(u16 numtime)
	\brief Wait for vblank interrupt
	\param numtime number of vblank to wait
	Waits for a vertical blank interrupt a number of time
*/
void vdp_waitvblank(u16 numtime);

/*! \fn  vdp_enablenmi(void)
	\brief Enable NMI interruption
	Allows NMI interruption to occur
*/
void vdp_enablenmi(void);

/*! \fn  vdp_disablenmi(void)
	\brief Disable NMI interruption
	Disallows NMI interruption to occur
*/
void vdp_disablenmi(void);

/*! \fn  vdp_enablevdp(void)
	\brief Enable VDP interruption
	Allows VDP to work (so screen is now active)
*/
void vdp_enablevdp(void);

/*! \fn  vdp_disablevdp(void)
	\brief Disable VDP interruption
	Disallows VDP to work (so screen is not active)
*/
void vdp_disablevdp(void);

/*! \fn  vdp_enablescr(void)
	\brief Enable VDP and NMI interruption
	Allows VDP to work and NMI to be catch
*/
void vdp_enablescr(void);

/*! \fn  vdp_disablescr(void)
	\brief Disable VDP and NMI interruption
	Disallows VDP to work and NMI to occur
*/
void vdp_disablescr(void);

/*! \fn  vdp_setmode1txt(void)
	\brief Activate mode 1 in text mode
	Activate Mode 1 of TMS in text mode, 16K of VRAM, sprites 16x16
	1 VRAM (duplicated each for 8 lines) that can be populate
	CHRGEN is located at $0000, COLTAB is located at $2000
*/
void vdp_setmode1txt(void);

/*! \fn  vdp_setmode2txt(void)
	\brief Activate mode 2 in text mode
	Activate Mode 2 of TMS in text mode, 16K of VRAM, sprites 16x16
	3 VRAM areas (each for 8 lines) that can be populate
*/
void vdp_setmode2txt(void);

/*! \fn  vdp_setcharex(u8 first,u8 count, unsigned offset,u8 flags)
	\brief  Put some chars of default font
	\param first 1st char to put
	\param count number of chars
	\param offset address location in video memory for the chararter patterns
	\param flags type of font
	Put some chars of default font in VRAM with the flags :
		FNTNORMAL for the normal font
		FNTITALIC for the italic font
		FNTBOLD for the bold font
		FNTBOLD_ITALIC fot both italic and bold font
*/
void vdp_setcharex(u8 first,u8 count, unsigned offset,u8 flags);

/*! \fn  vdp_putchar (u8 x, u8 y, char value)
	\brief  Put a single char on screen
	\param x colum to print
	\param y line to print
	\param value ascii value of char to print
	Print a char on screen
*/
void vdp_putchar (u8 x, u8 y, char value);

/*! \fn  vdp_getchar (u8 x, u8 y)
	\brief  Get a single char from screen
	\param x column of the char
	\param y line of the char
	\return char value from screen
	Get a char from screen
*/
u8  vdp_getchar (u8 x, u8 y);

/*! \fn  vdp_getarea (void *table, u8 x, u8 y, u8 width, u8 height)
	\brief  Get an area of chars from screen
	\param table area in ram to save chars
	\param x column of the char
	\param y line of the char
	\param width length of x char to get
	\param height length of y char to get
	Get an area of chars from screen
*/
void vdp_getarea (void *table, u8 x, u8 y, u8 width, u8 height);

/*! \fn  vdp_setdefaultchar (u8 flags)
	\brief Put default font
	\param flags	character format
	Put default font in VRAM with the flags :
		FNTNORMAL for the normal font
		FNTITALIC for the italic font
		FNTBOLD for the bold font
		FNTBOLD_ITALIC fot both italic and bold font
*/
void vdp_setdefaultchar(u8 flags);

/*! \fn  vdp_setmode2bmp(void)
	\brief Activate mode 2 in bitmap mode
	Activate Mode 2 of TMS in bitmap mode, 16K of VRAM, sprites 16x16
	1 VRAM area (complete screen of 24 lines) that can be populate
*/
void vdp_setmode2bmp(void);

/*! \fn  vdp_putstring(void)
	\brief Display a string
	\param x column x
	\param y line y
	\param text text to display
	Put a string on screen at coordinates x,y
*/
void vdp_putstring(u8 x, u8 y, char* text);

/*! \fn  vdp_fillvram(offset,u8 value,count)
	\brief Change VRAM with a specific value
	\param offset address in VRAM
	\param value value to set
	\param count number of time we set value
	Fill the VRAM with a specific value
*/
void vdp_fillvram(u16 offset,u8 value,u16 count);

/*! \fn  vdp_duplicatevram(void)
	\brief Duplicate vram for patterm
	Put 1st area of pattern vram to the 2nd and 3rd one
*/
void vdp_duplicatevram(void);

/*! \fn  vdp_putvram(unsigned offset,void *data,unsigned count)
	\brief Change VRAM with some data
	\param offset address in VRAM of 1st data to fill
	\param data address of data to use
	\param count number of data
	Fill the VRAM with some non compressed data 
*/
void vdp_putvram (unsigned offset,void *data,unsigned count);

/*! \fn  vdp_rle2vram(void *rledata,unsigned offset)
	\brief Change VRAM with some RLE compressed data
	\param rledata address of data to use
	\param offset address in VRAM of 1st data to fill
	Fill the VRAM with some RLE compressed data 
*/
void *vdp_rle2vram (void *rledata,unsigned offset);

/*! \fn  vdp_ple2vram(void *pledata, unsigned offset)
	\brief Change VRAM with some PLE compressed data
	\param pledata address of data to use
	\param offset address in VRAM of 1st data to fill
	Fill the VRAM with some PLE compressed data 
*/
void vdp_ple2vram(void *pledata, unsigned offset);

/*! \fn  vdp_dan2vram(void *dandata, unsigned offset)
	\brief Change VRAM with some DAN1 compressed data
	\param dandata address of data to use
	\param offset address in VRAM of 1st data to fill
	Fill the VRAM with some DAN1 compressed data 
*/
void vdp_dan2vram(void *dandata, unsigned offset);

#endif
