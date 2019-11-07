/*---------------------------------------------------------------------------------


	simple pal/ntsc dectection demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "palntsc.h"

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for Input example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put screen in text mode 2
	vdp_setmode2txt();
	
	// Put default char in VRAM and duplicate to all areas
	//  as we are going to write to line 10, it is in the second area
	vdp_setdefaultchar(FNTNORMAL);
	vdp_duplicatevram();
	vdp_fillvram(0x2000,0xf1,128*8); // Change color (or we will see nothing)
	vdp_enablescr();
	
	// Wait for nothing :P
	while(1) {
		// Update display with current value
		if (vid_freq==60)
			vdp_putstring(10,10,"NTSC CONSOLE");
		else if (vid_freq==50)
			vdp_putstring(10,10,"PAL CONSOLE");
		else {
			vdp_putstring(10,10,"UNKNOW");
		}
		
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
