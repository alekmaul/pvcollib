/*---------------------------------------------------------------------------------


	bitmap demo with dan3 compression
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "grafdan3comp.h"

#include "gfxs.h"									// to add definition of graphics

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put screen in text mode 2
	vdp_disablescr();
	
	vdp_setmode2txt();
	
	// Put image in vram  and duplicate to the 2nd & 3rd zone
	vdp_dan32vram (TILourvisiongfx, chrgen); // characters
	vdp_dan32vram (COLourvisiongfx, coltab); // colors
	vdp_dan32vram (MAPourvisiongfx, chrtab); // map

	vdp_duplicatevram();
	vdp_enablescr();
	
	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
