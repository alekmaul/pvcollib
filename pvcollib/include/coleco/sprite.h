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
/**
 * \file sprite.h
 * \brief coleco generic sprites support.
 *
 * This unit provides methods to display / remove sprites on screen.<br>
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
 */
typedef struct
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

/**
 * \fn void spr_clear(void)
 * \brief Remove all sprite from screen and init their positions
*/
void spr_clear(void);

/**
 * \fn void spr_clear30r(void)
 * \brief Remove all sprite from screen and init their positions in F18a 30 rows mode
*/
void spr_clear30r(void);

/*! \fn spr_update(void)
	\brief put sprite on screen
	Put all 32 sprites on screen.
	<b>Must be call in NMI routine.</b>
*/
void spr_update(void);

/*! \fn spr_updatefast(void)
	\brief put sprite on screen without checking order
	Put all 32 sprites on screen.
	<b>Must be call in NMI routine.</b>
*/
void spr_updatefast(void);

/*! \fn spr_getentry(void)
	\brief get a sprite id in sprite list 
	\return sprite entry (0..31)
	Must be used to allocate sprite because we swap sprite each NMI to <b>avoid 4 sprites limit per line.</b><br>
	<b>Warning, it uses 0xcf Y attribute to know if sprite is used or not.</b>
*/
u8 spr_getentry(void);

/*! \fn spr_getentry30r(void)
	\brief get a sprite id in sprite list 
	\return sprite entry (0..31)
	Must be used to allocate sprite because we swap sprite each NMI to <b>avoid 4 sprites limit per line.</b><br>
	<b>Warning, it uses 0xf0 Y attribute to know if sprite is used or not.</b>
*/
u8 spr_getentry30r(void);


/*! \fn spr_mode8x8(void)
	\brief put sprite in 8x8 pix mode
	Put all 32 sprites on screen in 8x8 pixels depth.
*/
void spr_mode8x8(void);

/*! \fn spr_mode16x16(void)
	\brief put sprite in 16x16 pix mode
	Put all 32 sprites on screen in 16x16 pixels depth.
*/
void spr_mode16x16(void);

/*! \fn spr_set(id,xp,yp,col,pat) 
	\brief sets an sprite entry to the supplied values
    \param id the sprite number to be get [0 - 31]
    \param xp the x location of the sprite in pixels
    \param yp the y location of the sprite in pixels
    \param col the sprite color (0 to 15)
    \param pat the pattern number of sprite (must be a multiple of 4)
*/
#define spr_set(id,  xp, yp, col, pat) \
	{ \
	sprites[id].x=xp; \
	sprites[id].y=yp; \
	sprites[id].colour=col; \
	sprites[id].pattern=pat; \
	}

/*! \fn spr_setxy(id,xp,yp) 
	\brief sets an sprite coordinate to the supplied values
    \param id the sprite number to be get [0 - 31]
    \param xp the x location of the sprite in pixels
    \param yp the y location of the sprite in pixels
*/
#define spr_setxy(id, xp, yp) \
	{ \
	sprites[id].x=xp; \
	sprites[id].y=yp; \
	}

/*! \fn spr_setx(id) 
	\brief set the x sprite coordinate 
    \param id the sprite number to be get [0 - 31]
    \param xp the x location of the sprite in pixels
*/
#define spr_setx(id,xp) { sprites[id].x=xp; }

/*! \fn spr_sety(id,yp) 
	\brief set the y sprite coordinate 
    \param id the sprite number to be get [0 - 31]
    \param yp the y location of the sprite in pixels
*/
#define spr_sety(id,yp) { sprites[id].y=yp; }

/*! \fn spr_setpat(id) 
	\brief set the sprite pattern
    \param id the sprite number to be get [0 - 31]
    \param pat the pattern of the sprite (multiple of 4)
*/
#define spr_setpat(id,pat) { sprites[id].pattern=pat; }

/*! \fn spr_setcol(id) 
	\brief set the sprite color
    \param id the sprite number to be get [0 - 31]
    \param col the sprite color (0 to 15)
*/
#define spr_setcol(id,col) { sprites[id].colour=col; }

/*! \fn spr_getx(id) 
	\brief get the x sprite coordinate 
    \param id the sprite number to be get [0 - 31]
*/
#define spr_getx(id) (sprites[id].x)

/*! \fn spr_gety(id) 
	\brief get the y sprite coordinate 
    \param id the sprite number to be get [0 - 31]
*/
#define spr_gety(id) (sprites[id].y)

#endif
