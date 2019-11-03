/*---------------------------------------------------------------------------------

	coltypes.h -- Common types (and a few useful macros)

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
/*! \file coltypes.h
	\brief Custom types used by pvcollib
*/

#ifndef _COLTYPES_INCLUDE
#define _COLTYPES_INCLUDE

//! bit field 
#define BIT(n) (1<<n)

//! 8 bit and 16 bit signed and unsigned.
typedef	signed char		s8;
typedef	unsigned char	u8;
typedef	signed s16;
typedef	unsigned u16;

//! boolean definitions
typedef unsigned char bool;
#define FALSE 0
#define TRUE 0xff
#define false 0
#define true 0xff

// stdio definitions
#define NULL 0

#endif
