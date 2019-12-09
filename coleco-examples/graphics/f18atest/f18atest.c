/*---------------------------------------------------------------------------------


	bitmap demo with less than 256 tiles
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "f18atest.h"

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
		// put screen in mode 2 bitmap adapted for f18a
		vdp_f18asetmode2bmp();

		// Put default char in VRAM and duplicate to all areas
		//  as we are going to write to line 10, it is in the second area
		vdp_setdefaultchar(FNTNORMAL);
		vdp_duplicatevram();
		vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)
		vdp_enablescr();
	
		// Print at 10,text as we are not going to do something else
		vdp_putstring(10,10,"F18A SUPPORT OK !");
	}
	else {
		// Put screen in text mode 2
		vdp_setmode2txt();
	
		// Put default char in VRAM and duplicate to all areas
		//  as we are going to write to line 10, it is in the second area
		vdp_setdefaultchar(FNTNORMAL);
		vdp_duplicatevram();
		vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)
		vdp_enablescr();
	
		// Print at 10,text as we are not going to do something else
		vdp_putstring(10,10,"NO F18A SUPPORT...");
	}

	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
