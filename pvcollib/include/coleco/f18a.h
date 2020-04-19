/*---------------------------------------------------------------------------------

	Generic f18a video functions

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
 * \file f18a.h
 * \brief contains the basic definitions for controlling the f18a video device.
 *
 * This unit provides generic features related to f18a device.<br>
 *<br>
 * Here is the list of features:<br>
 * - enable and disable f18a<br>
 * - update palette<br>
*/

#ifndef COL_F18A_INCLUDE
#define COL_F18A_INCLUDE

#include <coleco/coltypes.h>

#define sprtab_f18a  		0x2800 //f18a colour only requires 1/3 the colour data so til 0x2800
#define sprtab_f18a_2  		0x3000
#define sprtab_f18a_3	 	0x3800
#define mapvram 			0x1b00  

#define F18A_ECMS_0P		(0<<0)
#define F18A_ECMS_1B		(1<<0)
#define F18A_ECMS_2B		(2<<0)
#define F18A_ECMS_3B		(3<<0)
#define F18A_ECMT_0P		(0<<4)
#define F18A_ECMT_1B		(1<<4)
#define F18A_ECMT_2B		(2<<4)
#define F18A_ECMT_3B		(3<<4)
#define F18A_ECM_R30		(1<<6)
#define F18A_ECM_TM2		(1<<7)
#define F18A_ECM_YRE		(1<<3)

/**
 *  \brief vdp_f18aok is set if f18a module is present<br>
 *	when calling vdp_f18ainit function<br>
 */
extern volatile u8 vdp_f18aok;

/**
 * \fn void vdp_f18ainit(void)
 * \brief Activate f18a device<br>
 * Activate f18a and init vdp_f18aok variable with 1 if it is ok<br>
*/
void vdp_f18ainit(void);

/**
 * \fn void vdp_f18asetpalette(void *data,unsigned char count)
 * \brief Send a palette RGB 12bits color entries to f18a device<br>
 *
 * \param data address of data to use
 * \param count number of data <b>in words (each entry is two bytes)</b>
*/
void vdp_f18asetpalette(void *data,unsigned char count);

/**
 * \fn void vdp_f18asetpaletteentry(void *data,u8 entry,unsigned char count)
 * \brief Send a palette RGB 12bits color entries to f18a device<br>
 *
 * \param data address of data to use
 * \param offset 1st entry in the palette (0..64)
 * \param count number of data <b>in words (each entry is two bytes)</b>
*/
void vdp_f18asetpaletteentry(void *data,unsigned offset,unsigned char count);

/**
 * \fn vdp_f18asetmode1(u8 flags)
 * \brief Activate mode 1 in bitmap mode for f18a ONLY<br>
 *
 * \param flags	ecm mode definition<br>
 * F18A_ECMS_0P	or F18A_ECMS_1..3B for sprite color mode<br>
 * F18A_ECMT_0P	or F18A_ECMT_1..3B for tile color mode<br>
 * F18A_ECM_R30	for 30 lines mode <br>
 * F18A_ECM_TM2	to activate tile map 2 <br>
 * F18A_ECM_YRE	to activate Y real coordinates <br>
*/
void vdp_f18asetmode1(u8 flags);

/**
 * \fn vdp_f18asetscrollx(u8 bgnum, u8 x)
 * \brief Sets the horizontal scroll offset to the specified location
 *
 * \param bgnum	background number (1 or 2 for TL1 and TL2)
 * \param x	the horizontal scroll offset <br>
 *     (5 bits of scroll | 3 bits for planes : HTO0	HTO1	HTO2	HTO3	HTO4	HPO0	HPO1	HPO2)
*/
void vdp_f18asetscrollx(u8 bgnum, u8 x);

/**
 * \fn vdp_f18asetscrolly(u8 bgnum, u8 y)
 * \brief Sets the vertical scroll offset to the specified location
 *
 * \param bgnum	background number (1 or 2 for TL1 and TL2)
 * \param y	the vertical scroll offset <br>
 *     (5 bits of scroll | 3 bits for planes : VTO0	VTO1	VTO2	VTO3	VTO4	VPO0	VPO1	VPO2)
*/
void vdp_f18asetscrolly(u8 bgnum, u8 y);

#endif
