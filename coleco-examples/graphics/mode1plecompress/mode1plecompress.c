/*---------------------------------------------------------------------------------


	Graphic mode 1 example with a bitmap 
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "mode1plecompress.h"

#include "gfxs.h"									// to add definition of graphics

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	vdp_disablescr();

	// put screen in mode 1 
	vdp_setmode1txt();
	
	vdp_ple2vram (TILourvisiongfx, chrgen); // characters
	vdp_putvram (coltab,COLourvisiongfx,32); // colors (not compressed as it is only 32 bytes)
	vdp_ple2vram (MAPourvisiongfx, chrtab); // map
	
	vdp_enablescr();

	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
