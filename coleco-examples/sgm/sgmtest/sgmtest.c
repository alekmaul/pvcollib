/*---------------------------------------------------------------------------------


	sgm test with no specific feature
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "sgmtest.h"

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Check if sgm is available
	sys_sgminit();

	// Put screen in text mode 2
	vdp_setmode2txt();
	
	// Put default char in VRAM and duplicate to all areas
	//  as we are going to write to line 8, it is in the second area
	vdp_setdefaultchar(FNTNORMAL);
	vdp_duplicatevram();
	vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)
	vdp_enablescr();
	
    // Check now if we were ok with SGM initialization
    if (sys_sgmok==1) {
        // Print text as we are not going to do something else
		vdp_putstring(4,5,"YEAH SGM SUPPORT IS OK !");
    }
    else {
        // Print text as we are not going to do something else
		vdp_putstring(8,5,"NO SGM SUPPORT...");
	}

	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
