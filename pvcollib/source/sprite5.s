;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2019
;		Alekmaul 
;
;	This software is provided 'as-is', without any express or implied
;	warranty.  In no event will the authors be held liable for any
;	damages arising from the use of this software.
;
;	Permission is granted to anyone to use this software for any
;	purpose, including commercial applications, and to alter it and
;	redistribute it freely, subject to the following restrictions:
;
;	1.	The origin of this software must not be misrepresented; you
;		must not claim that you wrote the original software. If you use
;		this software in a product, an acknowledgment in the product
;		documentation would be appreciated but is not required.
;	2.	Altered source versions must be plainly marked as such, and
;		must not be misrepresented as being the original software.
;	3.	This notice may not be removed or altered from any source
;		distribution.
;
;---------------------------------------------------------------------------------
	.module pvclsprite5
	
	; global from external entries / code
	.globl _sprites
	
	; global from this module
	.globl _spr_updatefast
	
	
	.area   _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
 
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_spr_updatefast:
	di                
	xor		a          
	out		(#0xbf),a    							; 00000000 -> lower value of address
	ld		a,#0x5f      
	out		(#0xbf),a    							; 01011111 -> 0xBF Write data to 0x1F 00
	ei                
	ld		hl,#_sprites 
	ld		bc,#0x80be   							; write 4*32 data
	di                
	otir              
	ei                
	ret               
