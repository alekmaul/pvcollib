/*---------------------------------------------------------------------------------

	Generic phoenix functions

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
 * \file phoenix.h
 * \brief contains the basic definitions for controlling the Phoenix console.
 *
 * This unit provides generic features related to Phoenix console.<br>
 *<br>
 * Here is the list of features:<br>
 * - Test phoenix console<br>
*/

#ifndef COL_F18A_INCLUDE
#define COL_F18A_INCLUDE

#include <coleco/coltypes.h>
#include <coleco/f18a.h>

/**
 *  \brief vdp_f18aok is set if f18a module is present<br>
 *	when calling vdp_f18ainit function<br>
 */
#define sys_phoenixok vdp_f18aok;

/**
 * \fn void sys_phoenixinit(void)
 * \brief Test Phoenix console<br>
 * Activate f18a and init vdp_f18aok/sys_phoenixok variable with 1 if it is ok<br>
 * Testing sys_phoenixok variable to know if on Phoenix console<br>
*/
#define sys_phoenixinit vdp_f18ainit;

#endif
