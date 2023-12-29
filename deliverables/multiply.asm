;
; sum.asm
;

	.mmregs					; Include memory mapped registers to be refered to by their short name
	.sect	".text:sum"		; Tell the compiler to put this code in the correct subsection of the .text section
	.align 4				; Align the _sum function entry
	.def	_multiply			; Define the function entry

;----------------------------------------------------------------------
;   	 long multiply(short x,           => T0
;                  	   short y            => T1
;				  ) 				 	  => AC0
;----------------------------------------------------------------------
; Complete the code by filling in the question marks.

_multiply:
	; Save status registers
    pshm  ST1_55             		; Save ST1
    pshm  ST2_55					; Save ST2
    pshm  ST3_55					; Save ST3

	; Multiply x and y
	MOV T0, AC1						; Move T0 in AC1
	SFTS AC1, 16					; Shift AC1, 16 bits to the left
	MPY T1, AC1, AC0				; Multiply T1 with AC1, store result in AC0

	; Restore status registers
    popm  ST3_55       			; Restore ST1
    popm  ST2_55				; Restore ST2
    popm  ST1_55				; Restore ST3

    ret							; Exit function

    .end
