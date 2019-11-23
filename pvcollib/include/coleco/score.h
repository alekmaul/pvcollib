/*---------------------------------------------------------------------------------

	Generic score functions.

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
 * \file score.h
 * \brief coleco generic score support.
 *
 * This unit provides methods to manage scores.<br>
 *<br>
 * Here is the list of supported features:<br>
 * - scores with more than 65536 values<br>
 * - reset, compare, add scores<br>
 * - convert a score to a string for display purpose<br>
*/

#ifndef COL_SCORE_H
#define COL_SCORE_H

#include <coleco/coltypes.h>

/**
 * \struct score_t
 * \brief score value 
 * Can manage score with more than 65536 values (max of an unsigned)
 */
typedef struct { 
 unsigned lo;			/*!< low value of the score */
 unsigned hi;			/*!< high value of the score */
} score_t;

/**
 * \fn void sys_scoclear(score_t *sco)
 * \brief Init a score variable with value 0
 *
 * \param sco score variable to reset
*/
void sys_scoclear(score_t *sco);

/**
 * \fn void sys_scoadd(score_t *sco, unsigned value)
 * \brief Add a unsigned value to a score variable
 *
 * \param sco score variable 
 * \param value value to add to score
*/
void sys_scoadd(score_t *sco, unsigned value); 

/**
 * \fn u16 sys_scocmp(score_t *sco1,score_t *sco2)
 * \brief Compare two scores and return 1 if 1st one is lower than 2nd one
 *
 * \param sco1 score variable 
 * \param sco2 score variable 
 * \return 1 if sco1 is lower than sco2, 0 if not
*/
u16 sys_scocmp(score_t *sco1,score_t *sco2);

/**
 * \fn void sys_scocpy(score_t *sco1,score_t *sco2)
 * \brief Copy a score sco1 into another score sco2
 * 
 * \param sco1 score variable 
 * \param sco2 score variable 
*/
void sys_scocpy(score_t *sco1,score_t *sco2);

/**
 * \fn char *sys_scostr(score_t *sco, unsigned length)
 * \brief Convert a score variable to a string of specific length
 *
 * \param sco score variable 
 * \param length number of digits for displkay (max 9)
 * \return char * string value of score
*/
char *sys_scostr(score_t *sco, unsigned length);

#endif
