/*---------------------------------------------------------------------------------


	Simple msuic demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "musicsfx.h"

//---------------------------------------------------------------------------------
// sound declaration
const u8 musicch1[]={	 					// From Smurf rescue, channel 1
	0x38,  									// rest - length 24
	0x38,  									// rest - length 24
	0xc2,0x47,0x40,0x40,0x18,0x38,			// tone freq 1575,5Hz - vol 11 swept down - length 64
	0xc2,0x55,0x40,0x40,0x18,0x38,			// tone freq 1316,0Hz - vol 11 swept down - length 64
	0xc0,0x50,0x40,0x20,					// tone freq 1398,3Hz - vol 11 - length 32
	0xc0,0x5f,0x40,0x20,					// tone freq 1177,5Hz - vol 11 - length 32
	0xc2,0x55,0x40,0x40,0x18,0x38,			// tone freq 1316,0Hz - vol 11 swept down - length 64
	0xc2,0x5f,0x40,0x40,0x18,0x38,			// tone freq 1177,5Hz - vol 11 swept down - length 64
	0xc2,0x71,0x40,0x40,0x18,0x38,			// tone freq 989,9Hz - vol 11 swept down - length 64
	0xc0,0x6b,0x40,0x20,					// tone freq 1045,4Hz - vol 11 - length 32
	0xc0,0x7f,0x50,0x20,					// tone freq 880,8Hz - vol 10 - length 32
	0xc0,0x5f,0x50,0x10,					// tone freq 1177,5Hz - vol 10 - length 16
	0x18									// repeat
};

const u8 musicch2[]={	 					// From Smurf rescue, channel 2
	0x30,									// rest - length 16
	0x80,0x1d,0x41,0x0e,					// tone freq 392,5Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0x1d,0x41,0x0e,					// tone freq 392,5Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x82,0xd6,0x40,0x18,0x15,0x38,			// tone freq 522,7Hz - vol 11 swept down - length 24
	0x80,0xd6,0x40,0x06,					// tone freq 522,7Hz - vol 11 - length 6
	0x22,									// rest - length 2
	0x80,0xd6,0x40,0x0e,					// tone freq 522,7Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0xbe,0x40,0x0e,					// tone freq 588,7Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0xaa,0x40,0x10,					// tone freq 658,0Hz - vol 11 - length 16
	0x80,0xd6,0x40,0x10,					// tone freq 522,7Hz - vol 11 - length 16
	0x80,0xaa,0x40,0x10,					// tone freq 658,0Hz - vol 11 - length 16
	0x80,0xa0,0x40,0x10,					// tone freq 699,1Hz - vol 11 - length 16
	0x82,0x8f,0x40,0x18,0x15,0x38,			// tone freq 782,2Hz - vol 11 swept down - length 24
	0x80,0x8f,0x40,0x06,					// tone freq 782,2Hz - vol 11 - length 6
	0x22,									// rest - length 2
	0x80,0x8f,0x40,0x0e,					// tone freq 782,2Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0xa0,0x40,0x10,					// tone freq 699,1Hz - vol 11 - length 16
	0x80,0xaa,0x40,0x20,					// tone freq 658,0Hz - vol 11 - length 32
	0x80,0xbe,0x40,0x10,					// tone freq 588,7Hz - vol 11 - length 16
	0x80,0xd6,0x40,0x10,					// tone freq 522,7Hz - vol 11 - length 16
	0x82,0xbe,0x40,0x18,0x15,0x38,			// tone freq 588,7Hz - vol 11 swept down - length 24
	0x80,0xbe,0x40,0x06,					// tone freq 588,7Hz - vol 11 - length 6
	0x22,									// rest - length 2
	0x80,0xbe,0x40,0x0e,					// tone freq 588,7Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0xd6,0x40,0x10,					// tone freq 522,7Hz - vol 11 - length 16
	0x80,0xbe,0x40,0x10,					// tone freq 588,7Hz - vol 11 - length 16
	0x80,0xaa,0x40,0x10,					// tone freq 658,0Hz - vol 11 - length 16
	0x80,0xbe,0x40,0x10,					// tone freq 588,7Hz - vol 11 - length 16
	0x80,0xd6,0x40,0x10,					// tone freq 522,7Hz - vol 11 - length 16
	0x82,0xbe,0x40,0x18,0x15,0x38,			// tone freq 588,7Hz - vol 11 swept down - length 24
	0x80,0xbe,0x40,0x06,					// tone freq 588,7Hz - vol 11 - length 6
	0x22,									// rest - length 2
	0x80,0xbe,0x40,0x0e,					// tone freq 588,7Hz - vol 11 - length 14
	0x22,									// rest - length 2
	0x80,0xe2,0x50,0x10,					// tone freq 495,0Hz - vol 10 - length 16
	0x80,0x1d,0x51,0x10,					// tone freq 392,5Hz - vol 10 - length 16
	0x18,									// repeat
};

const u8 musicch3[]={ 						// From Smurf rescue, channel 3
	0x40,0x55,0x30,0x10,					// tone freq 1316,0Hz - vol 12 - length 16
	0x40,0x50,0x20,0x10,					// tone freq 1398,3Hz - vol 13 - length 16
	0x40,0x40,0x30,0x10,					// tone freq 1747,8Hz - vol 12 - length 16
	0x40,0x47,0x20,0x10,					// tone freq 1575,5Hz - vol 13 - length 16
	0x40,0x50,0x20,0x08,					// tone freq 1398,3Hz - vol 13 - length 8
	0x40,0x55,0x30,0x08,					// tone freq 1316,0Hz - vol 12 - length 8
	0x40,0x5f,0x30,0x10,					// tone freq 1177,5Hz - vol 12 - length 16
	0x40,0x8f,0x30,0x10,					// tone freq 782,2Hz - vol 12 - length 16
	0x40,0x6b,0x20,0x10,					// tone freq 1045,4Hz - vol 13 - length 16
	0x40,0x5f,0x30,0x10,					// tone freq 1177,5Hz - vol 12 - length 16
	0x40,0x55,0x20,0x10,					// tone freq 1316,0Hz - vol 13 - length 16
	0x40,0x50,0x30,0x08,					// tone freq 1398,3Hz - vol 12 - length 8
	0x40,0x55,0x30,0x08,					// tone freq 1316,0Hz - vol 12 - length 8
	0x40,0x5f,0x20,0x10,					// tone freq 1177,5Hz - vol 13 - length 16
	0x40,0x55,0x30,0x10,					// tone freq 1316,0Hz - vol 12 - length 16
	0x40,0x50,0x20,0x10,					// tone freq 1398,3Hz - vol 13 - length 16
	0x40,0x47,0x30,0x08,					// tone freq 1575,5Hz - vol 12 - length 8
	0x40,0x50,0x30,0x08,					// tone freq 1398,3Hz - vol 12 - length 8
	0x40,0x55,0x20,0x10,					// tone freq 1316,0Hz - vol 13 - length 16
	0x40,0x47,0x20,0x08,					// tone freq 1575,5Hz - vol 13 - length 8
	0x68,									// rest - length 8
	0x40,0x47,0x20,0x08,					// tone freq 1575,5Hz - vol 13 - length 8
	0x68,									// rest - length 8
	0x40,0x47,0x20,0x08,					// tone freq 1575,5Hz - vol 13 - length 8
	0x68,									// rest - length 8
	0x40,0x47,0x20,0x20,					// tone freq 1575,5Hz - vol 13 - length 32
	0x40,0x50,0x20,0x08,					// tone freq 1398,3Hz - vol 13 - length 8
	0x40,0x55,0x20,0x08,					// tone freq 1316,0Hz - vol 13 - length 8
	0x40,0x5f,0x20,0x08,					// tone freq 1177,5Hz - vol 13 - length 8
	0x40,0x6b,0x40,0x08,					// tone freq 1045,4Hz - vol 11 - length 8
	0x40,0x71,0x30,0x10,					// tone freq 989,9Hz - vol 12 - length 16
	0x40,0x6b,0x40,0x10,					// tone freq 1045,4Hz - vol 11 - length 16
	0x40,0x5f,0x40,0x10,					// tone freq 1177,5Hz - vol 11 - length 16
	0x40,0x55,0x40,0x08,					// tone freq 1316,0Hz - vol 11 - length 8
	0x40,0x5f,0x40,0x08,					// tone freq 1177,5Hz - vol 11 - length 8
	0x40,0x6b,0x30,0x10,					// tone freq 1045,4Hz - vol 12 - length 16
	0x40,0x5f,0x40,0x10,					// tone freq 1177,5Hz - vol 11 - length 16
	0x40,0x55,0x40,0x10,					// tone freq 1316,0Hz - vol 11 - length 16
	0x40,0x50,0x40,0x08,					// tone freq 1398,3Hz - vol 11 - length 8
	0x40,0x55,0x40,0x08,					// tone freq 1316,0Hz - vol 11 - length 8
	0x40,0x5f,0x30,0x10,					// tone freq 1177,5Hz - vol 12 - length 16
	0x40,0x50,0x40,0x08,					// tone freq 1398,3Hz - vol 11 - length 8
	0x28,									// rest - length 8
	0x40,0x50,0x40,0x08,					// tone freq 1398,3Hz - vol 11 - length 8
	0x28,									// rest - length 8
	0x40,0x50,0x40,0x08,					// tone freq 1398,3Hz - vol 11 - length 8
	0x28,									// rest - length 8
	0x42,0x50,0x30,0x20,0x18,0x44,			// tone freq 1398,3Hz - vol 12 swept down - length 32
	0x10,									// end
};

//---------------------------------------------------------------------------------
// mandatory soundtable declaration
// For some reason the very first entry MUST be SOUNDAREA1.  SOUNDAREA1 is the lowest priority sound effect.  
// I recommend using SOUNDAREA1-SOUNDAREA3 to be music track. SOUNDAREA4-6 to be sound effects. 
const sound_t snd_table[]={
	{ musicch1,			  				SOUNDAREA1},	// sound entry 1	
	{ musicch2,			  				SOUNDAREA2},	// sound entry 2	
	{ musicch3,			  				SOUNDAREA3},	// sound entry 3	

};

const music_t mus_table[]={
	{ 512,{0x81,0x02,0x03}},
	{ 0x0000},
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
	vdp_putstring(7,6,"PUT RIGHT TO PLAY MUSIC");
	vdp_putstring(7,8,"PUT LEFT FOR STOP MUSIC");
    
    // Display screen and enable audio
    vdp_enablescr();
    vdp_enablenmi();
    
	// Wait for nothing :P
	while(1) {
		// Update display with current pad 1 values
		if (joypad_1 & PAD_RIGHT) { vdp_putstring(11,10,"MUSIC PLAY"); vdp_disablenmi(); mus_startplay(mus_table); vdp_enablenmi(); }
		if (joypad_1 & PAD_LEFT) { vdp_putstring(11,10,"MUSIC STOP"); mus_stopplay();  }

		while (joypad_1) vdp_waitvblank(1); // Wait Vblank while joy pressed 
		
		vdp_waitvblank(1); // default wait vblank
	}
	// startup module will issue a soft reboot if it comes here 
}
