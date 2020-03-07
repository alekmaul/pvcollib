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
	.module pvclsprite
	
	; global from external entries / code
	.globl	_vdp_putvram
	.globl	_sprites
	.globl	_spr_odd
	
	; global from this module
	.globl _spr_clear30r
	
	.area   _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
 
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_spr_clear30r:
	ld		c,#0x00      

spcv0:
	ld		de,#_sprites 
	ld		l,c        
	ld		h,#0x00      
	add		hl,hl      
	add		hl,hl      					; c*4 because of sprite definition
	add		hl,de      
	ld		(hl),#0xF0					; SPHIDE
	inc		c          
	ld		a,c        
	sub		#0x20        
	jr		c,spcv0    
	
	ld		hl,#0x80		   			; MAXSPRITE4
	push	hl         
	ld		hl,#_sprites 				; adr sprites
	push	hl         
	ld		hl,#0x1f00  				; sprgen
	push	hl         
	call	_vdp_putvram				; put really sprites on vram
	ld		hl,#0x0006   
	add		hl,sp      
	ld		sp,hl     
 
	ld 		hl,#_spr_odd  
	ld		(hl),#0x00   
	
	ret               

