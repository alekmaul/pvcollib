/*---------------------------------------------------------------------------------


	small game from coder Amy Purple : Diamond Dash
		made by her during 2004 MiniGame Competition 
		https://web.archive.org/web/20050204071935/http://www.ffd2.com/minigame/
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "diamond.h"

#include "dash.h"											// main file for game
#include "snds.h"											// to add definition of sounds

const u8 game_title[] = {40,41,42,43,44,45,46,47,0};		// Char array for "DIAMOND DASH!!"

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for game example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put sound table and mute all channel
	snd_settable(snd_table);
	snd_stopall();
	
	// Put screen in text mode 2 and sprite in 8x8 pixels size
	vdp_disablescr();
	
	vdp_setmode2txt();
	spr_mode8x8();
	
	// Initialise graphics and sounds 
	init_graphics();
	
	// Set a "stop point" to let the Video Chip knows that there is only 1 sprite to show
	sprites[1].y=208;
	
	// Show a new mountain & Show "DIAMOND DASH!!" in center screen 
	new_mountain(); 
	show_score();
	vdp_putstring(12,11,game_title);
	vdp_enablescr();
	
	// Wait until player press fire
	vdp_enablenmi(); sys_pause(); vdp_disablenmi();

	// Play game
	game();
	// END
	// startup module will issue a soft reboot if it comes here 
}
