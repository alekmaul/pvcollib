/*---------------------------------------------------------------------------------


	bitmap demo with less than 256 tiles
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "grafnocomp.h"

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
	vdp_putvram (chrgen,TILourvision,SZTILourvision); // characters
	vdp_putvram (coltab,COLourvision,SZCOLourvision); // colors
	vdp_putvram (chrtab,MAPourvision,SZMAPourvision); // map

	vdp_duplicatevram();
	vdp_enablescr();
	
	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
