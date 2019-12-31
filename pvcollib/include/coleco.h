/*---------------------------------------------------------------------------------

	Copyright (C) 2018-2019

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
/*! \file coleco.h
\brief the master include file for colecovision applications.
*/

/*!
 \mainpage PVcollib Documentation


 \section intro Introduction
 Welcome to the PVcollib reference documentation.

 \section console_mngt Generic console feature
 - \ref console.h "Console management"

 \section sound_sn76489 sound engine API
 - \ref sound.h "General sound"

 \section video_tms9918 2D engine API
 - \ref video.h "General video"

 \section video_f18a F18A video device API
 - \ref f18a.h "F18A specific video"
 
 \section pad_mngt Generic pad & spinner API
 - \ref pad.h "General pad & spinner"

 \section sprite_mngt Generic sprites API
 - \ref sprite.h "General sprites"

 \section score_mngt Generic score API
 - \ref score.h "General scores"

 \section external_links Useful links
 - <a href="http://atariage.com/forums/forum/55-colecovision-programming/">AtariAge ColecoVision development forum.</a>
 - <a href="https://atariage.com/forums/topic/280138-f18a-mk2/">AtariAge F18A development topic.</a>
 
 \section special_thanks Special Thanks
 - Amy Purple (aka newcoleco) for cvlib source code, which are parts of PVcollib.
 - <a href="https://sourceforge.net/projects/sdcc/files/">Philipp Klaus Krause</a> - SDCC Release Manager. 
 - AtariAge member <a href="http://eriscreations.com/">digress</a> for his help about F18A programming on Coleco.
 - AtariAge member <a href="http://codehackcreate.com/archives/592">matthew180</a> for his help on discord about F18A device, thanks for making it real matthew ;) !.
*/


//adding the example page.
/*!
 <!-- EXAMPLES -->
    <!-- hello world -->
		\example helloworld/helloworld.c

    <!-- diamond game -->
		\example games/diamond/diamond.c

    <!-- graphic management -->
		\example graphics/backgrounds/dancompress/grafdancomp.c
		\example graphics/backgrounds/notcompress/grafnocomp.c
		\example graphics/backgrounds/plecompress/grafplecomp.c
		\example graphics/backgrounds/rlecompress/grafrlecomp.c
		\example graphics/sprites/simplesprite/simplesprite.c
		\example graphics/sprites/animatedsprite/animatedsprite.c
	
	<!-- f18a gaphic management -->
		\example graphics/f18a/f18abitmap1/f18abitmap1.c
		\example graphics/f18a/f18aecm3/f18aecm3.c
		\example graphics/f18a/f18atest/f18atest.c
	
    <!-- pad management -->
		\example input/input.c

    <!-- random numbers -->
		\example random/randvalue.c

    <!-- megacart management -->
		\example megacart/megacart.c
		
    <!-- region detection -->
		\example palntsc/palntsc.c

    <!-- score management -->
		\example scoring/scoring.c

    <!-- sound management -->
		\example audio/music/music.c
		\example audio/simplesound/ssound.c
*/

#ifndef COL_INCLUDE
#define COL_INCLUDE

#include <coleco/coltypes.h>

#include "coleco/console.h"
#include "coleco/pad.h"
#include "coleco/score.h"
#include "coleco/sound.h"
#include "coleco/sprite.h"
#include "coleco/video.h"
#include "coleco/f18a.h"

#endif // COL_INCLUDE
