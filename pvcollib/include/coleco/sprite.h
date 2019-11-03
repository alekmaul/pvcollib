/*---------------------------------------------------------------------------------

	Generic sprites functions.

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

/*! \file pad.h
    \brief coleco generic sprites support.
*/

#ifndef COL_SPRITE_H
#define COL_SPRITE_H

#include <coleco/coltypes.h>

#define SPHIDE		0xCF
#define SPHIDE1		0xCE

#define MAXSPRITE	32
#define MAXSPRITE4	(MAXSPRITE*4)
#define MAXSPRITE2	(MAXSPRITE*2)

/**
 *  \brief
 *      sprite entry :
 *			y,x     : coordinates (y,x) with y 0..191 and x 0..255
 *          pattern : pattern number of sprite
 *          colour  : colour between 0..15
 */typedef struct
{
	u8 y;
	u8 x;
	u8 pattern;
	u8 colour;
} sprite_t;

/**
 *  \brief
 *      32 sprites entries
 */
extern volatile sprite_t sprites[MAXSPRITE];	// sprites entries

/**
 *  \brief
 *      0 or 1 if sprites can be displayed or not
 */
extern volatile u8 spr_enable;

/*! \fn spr_clear(void)
	\brief remove all sprite 
	Remove all sprite from screen and init their positions
*/
extern void spr_clear(void);

/*! \fn spr_update(void)
	\brief put sprite on screen
	Put all 32 sprites on screen.
	<b>Must be call in NMI routine.</b>
*/
extern void spr_update(void);

#endif
