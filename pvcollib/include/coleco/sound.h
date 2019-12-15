/*---------------------------------------------------------------------------------

	Generic sound functions.

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
 * \file sound.h
 * \brief coleco generic sound support.
*/

#ifndef COL_SOUND_H
#define COL_SOUND_H

#include <coleco/coltypes.h>

/*! 
	\brief definition of sound area
*/
typedef struct
{
 void *sound_data;
 unsigned sound_area;
} sound_t;

/*! 
	\brief sound areas 1 to 6 available pour channels
*/
#define SOUNDAREA1  0x702b
#define SOUNDAREA2  0x702b+10
#define SOUNDAREA3  0x702b+20
#define SOUNDAREA4  0x702b+30
#define SOUNDAREA5  0x702b+40
#define SOUNDAREA6  0x702b+50

/**
 * \fn snd_settable (void *snd_table)
 * \brief define the sound table used for playing sound
 *
 * \param snd_table address of sound table
 * \return nothing
*/
void snd_settable (void *snd_table);

/*
 * \fn snd_startplay (u8 sound_number)
 * \brief play a sound specified but sound_number
 *
 * \param sound_number aid of sound in sound table
 * \return nothing
*/
void snd_startplay(u8 sound_number);

/**
 * \fn snd_stopplay (u8 sound_number)
 * \brief stop a sound specified but sound_number
 *
 * \param sound_number aid of sound in sound table
 * \return nothing
*/
void snd_stopplay(u8 sound_number);

/**
 * \fn snd_stopall (void)
 * \brief mute all channels
 *
 * \return nothing
*/
void snd_stopall(void);

/**
 * \fn snd_isplaying (u8 sound_number)
 * \brief retrieve if channel is playing or not
 *
 * \param sound_number aid of sound in sound table
 * \return 0xff is sound_number is still playing, 0x00 if not
*/
u8 snd_isplaying(u8 sound_number);

#endif
