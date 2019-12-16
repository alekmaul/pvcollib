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

#ifndef GFX2COL_H
#define GFX2COL_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

#include "lodepng.h"
#include "comptool.h"

#ifndef __BUILD_VERSION
#include "config.h"
#else
#define GFX2COLVERSION __BUILD_VERSION
#define GFX2COLDATE __BUILD_DATE
#endif /* __BUILD_VERSION */


//MACROS
#define HI_BYTE(n)  (((int)n>>8) & 0x00ff)  // extracts the hi-byte of a word
#define LOW_BYTE(n) ((int)n & 0x00ff)       // extracts the low-byte of a word

#define HIGH(n)     ((int)n<<8)             // turn the char into high part of int

#endif
