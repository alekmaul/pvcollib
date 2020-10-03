/*---------------------------------------------------------------------------------


	Simple sound demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "ssound.h"

//---------------------------------------------------------------------------------
// sound declaration
const u8 dummysound[]={ 			// no sound, for soundtable, see below
	0x90
};

const u8 dksound[]={ 				// from Donkey Kong sound item 9
	0x81,0x3d,0x00,0x50,0x11,0x01,  //  tone freq 1833,8Hz swept down - vol 15 (max) - length 80
	0x90							// end
};

const u8 btsound[]={ 				// from Burger time sound item 12
	0x00,0x00,0x02,0x05,			// periodic noise - freq 125 Hz - vol 15 (max) - length 5
	0x10							// end
};

const u8 shsound[]={ 				// from spy hunter sound item 13
	0x02,0x26,0x1e,0x1d,0x24,		// white noise - freq 874 Hz - vol 13 swept down - length 30
	0x10							// end
};

const u8 sfsound[]={ 				// from space fury sound item 4
	0x43,0x97,0x00,0x08,0x11,
		0x2d,0x1f,0x11,				// tone freq 740,8Hz swept down - vol 15 (max) swept down - length 8
	0x10							// end
/*
*/
};

//---------------------------------------------------------------------------------
// mandatory soundtable declaration
// For some reason the very first entry MUST be SOUNDAREA1.  SOUNDAREA1 is the lowest priority sound effect.  
// I recommend using SOUNDAREA1-SOUNDAREA3 to be music track. SOUNDAREA4-6 to be sound effects. 
const sound_t snd_table[]={
	{ dummysound,		  				SOUNDAREA1},	// sound entry 1	

	{ dksound,			  				SOUNDAREA4},	// sound entry 2	
	{ btsound,			  				SOUNDAREA5},	// sound entry 3	
	{ shsound,			  				SOUNDAREA6},	// sound entry 4	
	{ sfsound,			  				SOUNDAREA4},	// sound entry 5	
};

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for Input example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put sound table and mute all channel
	snd_settable(snd_table);
	snd_stopall();

	// Put screen in text mode 2
	vdp_setmode2txt();
	
	// Put default char in VRAM and duplicate to all areas
	//  as we are going to write to line 10, it is in the second area
	vdp_setdefaultchar(FNTNORMAL);
	vdp_duplicatevram();
	vdp_fillvram(0x2000,0xf1,128*8); // Change color (or we will see nothing)
    vdp_putstring(7,6,"PUSH PAD FOR SOUNDS");
	
    // display screen and enable sound
    vdp_enablescr();
    vdp_enablenmi();
	
	// Wait for nothing :P
	while(1) {
		// Update display with current pad 1 values
		if (joypad_1 & PAD_RIGHT) { vdp_putstring(9,10,"DK SND"); snd_startplay(2); }
		if (joypad_1 & PAD_LEFT)  { vdp_putstring(9,10,"BT SND"); snd_startplay(3); }
		if (joypad_1 & PAD_DOWN)  { vdp_putstring(9,10,"SH SND"); snd_startplay(4); }
		if (joypad_1 & PAD_UP)    { vdp_putstring(9,10,"SF SND"); snd_startplay(5); }

		while (joypad_1) vdp_waitvblank(1); // Wait Vblank while joy pressed 
		
		vdp_waitvblank(1); // default wait vblank
	}
	// startup module will issue a soft reboot if it comes here 
}
