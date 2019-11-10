/*---------------------------------------------------------------------------------

	Generic pad & spinner functions.

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
    \brief coleco generic pad & spinner support.
*/

#ifndef COL_PAD_H
#define COL_PAD_H

#include <coleco/coltypes.h>

#define	PAD_UP			1
#define	PAD_RIGHT		2
#define	PAD_DOWN		4
#define	PAD_LEFT		8
#define	PAD_FIRE4		16
#define	PAD_FIRE3		32
#define	PAD_FIRE2		64
#define	PAD_FIRE1		128

#define	PAD_KEY0		0
#define	PAD_KEY1		1
#define	PAD_KEY2		2
#define	PAD_KEY3		3
#define	PAD_KEY4		4
#define	PAD_KEY5		5
#define	PAD_KEY6		6
#define	PAD_KEY7		7
#define	PAD_KEY8		8
#define	PAD_KEY9		9
#define	PAD_KEYSTAR		10
#define	PAD_KEYSHARP	11
#define	PAD_KEYNONE		15

/**
 *  \brief
 *      PAD 1 and 2 values
 */
extern volatile u8 joypad_1,joypad_2;

/**
 *  \brief
 *      SPINNER 1 and 2 values
 *		/!\ spinner 1 value is reversed
 */
extern volatile char spinner_1,spinner_2;

/**
 *  \brief
 *      KEYBOARD from PAD 1 and 2 values
 */
extern volatile u8 keypad_1,keypad_2;

/*! \fn pad_resetspin(void)
	\brief reset spinners 1 & 2 values
	\return nothing
*/
void pad_resetspin(void);

/*! \fn pad_disablespin(void)
	\brief enable spinners functions 
	\return nothing
*/
void pad_disablespin(void);

/*! \fn pad_enablespin(void)
	\brief enable spinners functions to get their values
	\return nothing
*/
void pad_enablespin(void);

#endif
