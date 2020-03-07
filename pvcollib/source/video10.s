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
	.module pvclvideo10
	
	; global from external entries / code
	.globl _vdp_enablevdp
	.globl _vdp_disablevdp
	;.globl  _vdp_enablenmi
	;.globl _vdp_disablenmi
    .globl _nmi_direct
	.globl _spr_clear
	.globl _spr_enable
	.globl _vdp_waitvblank
    .globl _no_nmi
	
	; global from this module
	.globl _vdp_enablescr
	.globl _vdp_disablescr
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
_vdp_enablescr:
	call 	_vdp_enablevdp
    push    hl
    ld      hl,#_no_nmi
    bit     7,(hl)
    jp      z,.+6
    call    _nmi_direct
    res     0,(hl)
	ld		hl,#_spr_enable
	ld		(hl),#00   
    pop     hl

	ret
	
_vdp_disablescr:
	push	hl
	ld 		hl,#1
	push	hl
	call 	_vdp_waitvblank
	pop		af
	call 	_spr_clear
	call 	_vdp_disablevdp
    ld      hl,#_no_nmi
    set     0,(hl)
	ld		hl,#_spr_enable
	ld		(hl),#00   
    pop     hl

	ret
