/*---------------------------------------------------------------------------------


	megacart demo with 3 banks
	images from Amy Purple post (newcoleco) here:
		https://atariage.com/forums/topic/268244-data-compression-benchmark-dan-pletter-zx7-exomizer-pucrunch-megalz/
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "megacart.h"

#include "gfxsb1.h"									// to add definition of graphics
#include "gfxsb2.h"									// to add definition of graphics
#include "gfxsb3.h"									// to add definition of graphics

// Switch of the banks
#define SWITCH_IN_BANK1		{ dummy=(*(volatile unsigned char*) 0xFFF8); } // b1 with 16k
#define SWITCH_IN_BANK2		{ dummy=(*(volatile unsigned char*) 0xFFF9); } // b2 with 16k
#define SWITCH_IN_BANK3		{ dummy=(*(volatile unsigned char*) 0xFFFA); } // b3 with 16k
#define SWITCH_IN_BANK4		{ dummy=(*(volatile unsigned char*) 0xFFFB); } // b4 with 16k
#define SWITCH_IN_BANK5		{ dummy=(*(volatile unsigned char*) 0xFFFC); } // b5 with 16k
#define SWITCH_IN_BANK6		{ dummy=(*(volatile unsigned char*) 0xFFFD); } // b6 with 16k
#define SWITCH_IN_BANK7		{ dummy=(*(volatile unsigned char*) 0xFFFE); } // b7 with 16k
#define SWITCH_IN_BANK8		{ dummy=(*(volatile unsigned char*) 0xFFFF); } // b8 with 16k

// var for bank switching
unsigned char dummy;

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void initvramIMG1(void) {
	// Disable Interrupt 
	vdp_waitvblank(1);
	vdp_disablescr();
	
	SWITCH_IN_BANK1;
	
	// Put image in vram  
	vdp_dan12vram (TILimage1gfx, chrgen); // characters
	vdp_dan12vram (COLimage1gfx, coltab); // colors
	vdp_dan12vram (MAPimage1gfx, chrtab); // map

	// enable interrupt
	vdp_enablescr(); 
    vdp_enablenmi();
}

//---------------------------------------------------------------------------------
void initvramIMG2(void) {
	// Disable Interrupt 
	vdp_waitvblank(1);
	vdp_disablescr();
	
	SWITCH_IN_BANK2;
	
	// Put image in vram  
	vdp_dan12vram (TILimage2gfx, chrgen); // characters
	vdp_dan12vram (COLimage2gfx, coltab); // colors
	vdp_dan12vram (MAPimage2gfx, chrtab); // map

	// enable interrupt
	vdp_enablescr(); 
    vdp_enablenmi();
}

//---------------------------------------------------------------------------------
void initvramIMG3(void) {
	// Disable Interrupt 
	vdp_waitvblank(1);
	vdp_disablescr();
	
	SWITCH_IN_BANK3;
	
	// Put image in vram  
	vdp_dan12vram (TILimage3gfx, chrgen); // characters
	vdp_dan12vram (COLimage3gfx, coltab); // colors
	vdp_dan12vram (MAPimage3gfx, chrtab); // map

	// enable interrupt
	vdp_enablescr(); 
    vdp_enablenmi();
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put screen in bitmapmode 2
	vdp_disablescr();
	
	vdp_setmode2bmp();
	
    // Enable screen 
	vdp_enablescr();
	
	// Put 1st image
	initvramIMG1();
	
	// Wait for nothing :P
	while(1) {
		// change image regarding joypad
	    if ( (joypad_1 & PAD_RIGHT) ) initvramIMG1();
		if ( (joypad_1 & PAD_LEFT) ) initvramIMG2();
		if ( (joypad_1 & PAD_FIRE1) ) initvramIMG3();

		// Wait Vblank
		while (joypad_1) { vdp_waitvblank(1); }
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
