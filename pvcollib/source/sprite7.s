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
	.module pvclsprite7
	
	; global from external entries / code
	.globl _spr_odd
	.globl _sprites
	
	; global from this module
	.globl _spr_getentry30r
	
	.area   _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
 
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_spr_getentry30r:
	ld		iy,#_spr_odd  
	ld		a,0(iy) 
	add		a,#0x02      
	ld		0(iy),a 
	ld		a,0(iy) 
	sub		#0x20        
	jr		c,spf30rv0    
	ld		0(iy),#0x00 

spf30rv0:
	ld		hl,#_spr_odd  
	ld		c,(hl)     
	ld		e,c        

spf30rv3:
	ld		a,e        
	sub		#0x20        
	jr		nc,spf30rv4
	ld		l,e        
	ld		h,#0x00      
	add		hl,hl      
	add		hl,hl      
	push	de         
	ld		de,#_sprites 
	add		hl,de      
	pop		de         
	ld		a,(hl)     
	sub		#0xf0							; spr hide ?
	jr		nz,spf30rv2

	ld		l,c        
	ret               

spf30rv2:
	inc		e          
	ld		c,e        
	jr		spf30rv3
    
spf30rv4:
	ld     bc,#0 							; _HEADER0 
spf30rv7:
	ld		hl,#_spr_odd  
	ld		a,b        
	sub		(hl)       
	jr		nc,spf30rv5
	ld		l,b        
	ld		h,#0x00      
	add		hl,hl      
	add		hl,hl      
	ld		de,#_sprites 
	add		hl,de      
	ld		a,(hl)     
	sub		#0xf0							; spr hide ?
	jr		nz,spf30rv6
	ld		l,c        
	ret            
   
spf30rv6:
	inc		b          
	ld		c,b        
	jr		spf30rv7

spf30rv5:
	ld		l,#0xff      

	ret       
