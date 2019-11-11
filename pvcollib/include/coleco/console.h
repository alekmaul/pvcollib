/*---------------------------------------------------------------------------------

	Generic console functions.

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

/*! \file console.h
    \brief coleco generic console support.
*/

#ifndef COL_CONSOLE_H
#define COL_CONSOLE_H

#include <coleco/coltypes.h>

/**
 *  \brief
 *      buffer42 used for some functions.
 *      can be used by program too.
 */
extern u8 buffer32[32];

/**
 *  \brief
 *      vid_freq is set with video frequency (50 or 660) hz.
 */
extern volatile u8 vid_freq;

/*! \fn sys_random(void)
	\brief return a randomized number
	\return unsigned short of a randomized number
	Generate a 16 bit number 
*/
u16 sys_random(void);

/*! \fn sys_randbyte(u8 A, u8 B)
	\brief return a randomized number between A and B
	\param A minimum value of random number
	\param B maximum value of random number
	\return unsigned char of a randomized number
	Generate a 8 bit number between two numbers
*/
u8 sys_randbyte(u8 A, u8 B);


/*! \fn sys_strlen(char *text)
	\brief return the length of a string
	\param text text to analyze
	\return unsigned short of string length
	Return the length of the string
*/
u16 sys_strlen(char *text);

/*! \fn sys_utoa(unsigned value,char *buffer)
	\brief put a unsigned value into a buffer
	\param value value to convert to text
	\param buffer text that will receive vvalue converted
	Convert an unsigned value to ascii text.
	Leading zeros _are_ put in buffer
*/
void sys_utoa(unsigned value,char *buffer);

/*! \fn *sys_str(unsigned value)
	\brief return a converted value in string
	\param value number to convert
	\return pointer to converted string
	Return a converted value to a string
*/
char *sys_str(unsigned value);

#endif
