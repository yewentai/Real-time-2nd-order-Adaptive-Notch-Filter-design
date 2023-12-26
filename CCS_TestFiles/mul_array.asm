;
; sum.asm
;

	.mmregs						; Include memory mapped registers to be refered to by their short name
	.sect	".text:mul_array"   ; Tell the compiler to put this code in the correct subsection of the .text section
	.align 4					; Align the _sum function entry
	.def	_mul_array			; Define the function entry

;----------------------------------------------------------------------
;   	 long mul_array(short * a,           => AR0
;                  	    short y              => T0
;				  	) 						 => AC0
;----------------------------------------------------------------------
; Complete the code by filling in the question marks.

_mul_array:
	; Save status registers
    pshm  ST1_55             		; Save ST1
    pshm  ST2_55					; Save ST2
    pshm  ST3_55					; Save ST3


	sub #2, T0					; Subtract 2 from T0
	mov T0, BRC0				; Move length to repeat register

	MOV *AR0+ << #16, AC1   	; Move first value to HI(AC1)
	MOV #0, AC0					; Clear AC0

	RPTB loop					; Repeat BRC0 + 1 times
		MPYM *AR0+, AC1, AC0	;
loop:	SFTS AC0, 16, AC1		; Shift AC0 16 bits to the left.


	; Restore status registers
    popm  ST3_55       			; Restore ST1
    popm  ST2_55				; Restore ST2
    popm  ST1_55				; Restore ST3

    ret							; Exit function

    .end
