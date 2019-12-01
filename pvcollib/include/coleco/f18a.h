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

#define sprtab_f18a  		0x2800 //f18a colour only requires 1/3 the colour data? til 0x2800
#define sprtab_f18a_2  		0x3000
#define sprtab_f18a_3	 	0x3800
#define mapvram 			0x1f00  

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

#endif
