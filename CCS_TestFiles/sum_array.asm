;
; sum.asm
;

	.mmregs						; Include memory mapped registers to be refered to by their short name
	.sect	".text:sum_array"   ; Tell the compiler to put this code in the correct subsection of the .text section
	.align 4					; Align the _sum function entry
	.def	_sum_array			; Define the function entry

;----------------------------------------------------------------------
;   	 short sum_array(short * a,           => AR0
;                  	     short y              => T0
;				  ) 						  => T0
;----------------------------------------------------------------------
; Complete the code by filling in the question marks.

_sum_array:
	; Save status registers
    pshm  ST1_55             		; Save ST1
    pshm  ST2_55					; Save ST2
    pshm  ST3_55					; Save ST3

	mov #0, T1					; Clear register T1
	sub #1, T0					; Subtract 1 from T0
	mov T0, CSR					; Move length to repeat register


	rpt CSR						; Repeat length times
	add *AR0+, T1				; Add array value to T1 and increment position

	MOV T1, T0					; Store result in T0

	; Restore status registers
    popm  ST3_55       			; Restore ST1
    popm  ST2_55				; Restore ST2
    popm  ST1_55				; Restore ST3

    ret							; Exit function

    .end
