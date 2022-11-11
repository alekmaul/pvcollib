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

/*! 
  \brief definition of music area.
	music_duration, [NBR-1 sounds to start | sound_no1 in 6 bits ], sound_no2*, sound_no2*, sound_no2*
	So, for 1 channel, channel_data will have directly the sound entry (ex: 0x01),
	for 2 channels, it will be 0x41,0x02 for example
	for 3 channels, it will be 0x81,02,03
	for 4 channels (maximum), it will be 0xc1,02,03,04
	these sound_no are not essential, it depends on the value of NBR.
	IF music_duration = 0000, END MARKER.
	IF music_duration > 7FFF, SET MUSIC TABLE POINTER TO THIS NEW LOCATION TO CONTINUE.
*/
typedef struct
{
 unsigned music_duration;
 char channel_data[];
} music_t;


/**
 * \brief
 * put 1 to snd_mute to disable sound update.
 */
extern volatile u8 snd_mute;

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

/*
 * \fn mus_startplay(void *mudata);
 * \brief play a music specified but mudata.
 *
 * \param mudata entry point of music data (see music_t)
 * \return nothing
*/
void mus_startplay(void *mudata);

/*
 * \fn mus_nextplay(void *mudata);
 * \brief stop the current music and play the music specified but mudata.
 *
 * \param mudata entry point of music data (see music_t)
 * \return nothing
*/
void mus_nextplay(void *mudata);

/**
 * \fn mus_stopplay (void)
 * \brief Stop the current music.
 *
 * \return nothing
*/
void mus_stopplay(void);

/**
 * \fn mus_update (void)
 * \brief This routine can be called at the end of the NMI routine, in project code.
 * To use this routine, the program should not use the first 4 SOUNDAREAs to play sound effects, because they are reserved for the music.
 * Because the first 4 AREAs are low priotity and because of that the sound effect in AREAs 5+ will play while continuing playing logically the music in the background.
 *
 * \return nothing
*/
void mus_update(void);

#endif
