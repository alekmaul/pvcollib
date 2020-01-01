/*---------------------------------------------------------------------------------


	f18a test with a bitmap 
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "f18abitmap1.h"

#include "gfxs.h"									// to add definition of graphics

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Check if f18a is available
	vdp_f18ainit();

	// display a message regarding result of test
	if (vdp_f18aok==1) {
		// put screen in mode 1 with no specific f18a stuffs
		vdp_f18asetmode1(0x30);
		
		vdp_ple2vram (TILourvision, chrgen); // characters
		vdp_ple2vram (MAPourvision, chrtab); // map
		vdp_ple2vram (COLourvision, coltab); // colours

		vdp_f18asetpalette(PALourvision,64);
		
		vdp_enablescr();
	}
	else {
		// Put screen in text mode 2
		vdp_setmode2txt();
	
		// Put default char in VRAM and duplicate to all areas
		//  as we are going to write to line 8, it is in the second area
		vdp_setdefaultchar(FNTNORMAL);
		vdp_duplicatevram();
		vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)
		vdp_enablescr();
	
		// Print text as we are not going to do something else
		vdp_putstring(8,5,"NO F18A SUPPORT...");
	}

	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
