/*---------------------------------------------------------------------------------


	Simple console 'hello world' demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "helloworld.h"

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for Helloworld example
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
	vdp_enablevdp();
	
	// Change color (or we will see nothing)
	vdp_fillvram(0x2000,0xf1,128*8);
	
	// Print at 10,text as we are not going to do something else
	vdp_putstring(10,10,"HELLO WORLD!");
	
	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
