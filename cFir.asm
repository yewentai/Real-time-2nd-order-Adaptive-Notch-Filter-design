; 
;
	.mmregs					; Include memory mapped registers to be refered to by their short name
	.sect	".text:fir"		; Tell the compiler to put this code in the :fir subsection of the .text section
	.align 4				; Align the _cFir function entry
	.def	_cFir			; Define the function entry

;----------------------------------------------------------------------
;   	 short cFir(short x,           => T0
;                   short *h,           => AR0
;                   short *index        => AR1
;				    short *w,           => AR2
;                   short length,       => T1
;                   )  => T0
;----------------------------------------------------------------------
; Complete the code by filling in the question marks.

_cFir:
    pshm  ST1_55             	; Save ST1, ST2, and ST3
    pshm  ST2_55
    pshm  ST3_55
		
    or    #0x340, mmap(ST1_55)		; Set FRCT, SXMD, SATD
    bset  SMUL               		; Set SMUL;

    mov   AR0, mmap(BSA01)    		; base address for coefficients
    mov   T1,  mmap(BK03)  			; Set coefficient array size
    mov   AR2, mmap(BSA23)  		; base address for signal buffer

    ;set appropriate registers to have 2 ARx registers as circular pointers
    bset  AR0LC
    bset  AR2LC
    
    mov   #0, AR0          		; Coefficient start from h[0]
    mov   *AR1, AR2           	; Signal buffer start from w[index]
    
    mov   T0, *AR2         		; Put the new sample to signal buffer

    sub   #3, T1, T0      		; T0 = filterlength - 3
    mov   T0, CSR             	; Initialize inner loop

    mpym  *AR2+, *AR0+, AC0		; Do the 1st multiplication, store result in AC0
    							; Let *AR2 point to next sample in delay line4
    							; Let *AR0 point to next filter tap

    rpt   CSR                	; Start the inner loop
    macm  *AR2+, *AR0+, AC0		; This instruction is repeated length-2 times

    macmr *AR2, *AR0, AC0  		; Do the last operation with rounding

    mov   hi(AC0), T0   		; Save only high part to output (Q15)
    mov   AR2, *AR1       		; Update signal buffer index

    popm  ST3_55       			; Restore ST1, ST2, and ST3
    popm  ST2_55 
    popm  ST1_55	

    ret

    .end
