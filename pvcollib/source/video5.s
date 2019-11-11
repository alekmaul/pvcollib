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
	.module pvclvideo5
	
	; global from external entries / code
	.globl  _nmi_flag
	
	; global from this module
	.globl _vdp_waitvblank
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_waitvblank:
	pop     hl
	pop     de
	push    de
	push    hl
	ld      a,(#0x73c4)       ; check if NMI enabled
	and     #0x20
	jr      z,vwvb3
vwvb1:              
	ld      a,e             ; NMI enabled, check _nmi_flag
	or      d
	ret     z
	xor     a
	ld      (_nmi_flag),a
vwvb2:      
	ld      a,(_nmi_flag)
	or      a
	jr      z,vwvb2
	dec     de
	jr      vwvb1
vwvb3:      
	call    #0x1fdc           ; NMI disabled, check VDP status
vwvb4:      
	ld      a,e
	or      d
	ret     z
vwvb5:      
	call    #0x1fdc
	rlca
	jr      nc,vwvb5
	dec     de
	jr      vwvb4
