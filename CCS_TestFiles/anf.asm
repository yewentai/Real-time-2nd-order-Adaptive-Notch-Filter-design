;******************************************************************************
;* FILENAME                                                                   *
;*   anf.asm      				                                              *
;*                                                                            *
;*                                                                            *
;*----------------------------------------------------------------------------*
;*                                                                            *
;*  The rate of convergence of the filter is determined by MU                 *
;*                                                                            * 
;******************************************************************************
;/*

	 .mmregs

	MU 		 .set  0xC8
	LAMBDA	 .set  0x0x73333333
	LAMBDA2  .set  0xCCCCCCC

; Functions callable from C code

	.sect	".text"
	.global	_anf

;*******************************************************************************
;* FUNCTION DEFINITION: _anf_asm		                           *
;*******************************************************************************
; int anf(int y,				=> T0
;		  int *x,				=> AR0 (state buffer)
;		  int *a,				=> AR1 (adaptive coefficient)
; 		  int *rho,				=> AR2
;	      unsigned int* index	=> AR3
;		 );						=> T0: output argument (e in this case) must be stored in T0 at the end of code
;

_anf:

		PSH  mmap(ST0_55)					; Store original status register values on stack
		PSH  mmap(ST1_55)
		PSH  mmap(ST2_55)

		mov   #0, mmap(ST0_55)     			; Clear all fields (OVx, C, TCx)
    	or    #4100h, mmap(ST1_55)  		; Set CPL (bit 14), SXMD (bit 8);
    	and   #07940h, mmap(ST1_55)      	; Clear BRAF, M40, SATD, C16, 54CM, ASM
		bclr  ARMS                      	; Disable ARMS bit 15 in ST2_55
    	bset  SMUL							; Ensure that saturation-on-multiplication is set

		; Add your own code here
		; allocate some stack space for local variables
		aadd #-4, SP
		mov *AR1, T2
		mov *AR3, T1

    	mov #4000h, AC0
		mov *AR2, T3
		mack T3, LAMBDA, AC0, AC0
    	sfts AC0, #-15, AC0

    	mov #4000h, AC1
    	mov *AR2(#1), T3
		mack T3, LAMBDA2, AC1, AC1

    	sfts AC1, #-15, AC1
		add AC0, AC1
		mov AC1, *AR2

		amar *AR0, XAR4
		aadd T1, AR4
		mov #400h, AC0
		macm *AR2, *AR4, AC0, AC0
		sfts AC0, #-13, AC0
		mov AC0, T3

		mov #400h, AC0
		macm *AR1, T3, AC0, AC0
    	sfts AC0, #-11, AC0
		mov XAR0, XAR4
		add T1, AR4
		mov #100h, AC1
		macm *AR1, *AR4, AC1, AC1
		sfts AC1, #-9, AC1


		; k = (k + 1) % 3;
		mov T1, AC2
		add #1, AC2
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1
		bcc set1, TC1
		b continue1
set1:
		mov #0, T1
continue1:


		mov XAR0, XAR4
		add T1, AR4
		sub *AR4 << #4, AC1, AC1 ; subtract because we didn't negate

		mov #4000h, AC2
		macm *AR2, *AR2, AC2
    	sfts AC2, #-15, AC2
    	mov AC2, T3

		mov #400h, AC2
		macm *AR4, T3, AC2, AC2
    	sfts AC2, #-11, AC2
		sub AC2, AC0

		mov T1, AC2
		add #1, AC2
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1
		bcc set2, TC1
		b continue2
set2:
		mov #0, T1
continue2:

    	mov XAR0, XAR4
		add T1, AR4
		add T0, AC0
		mov AC0, AC2
		sfts AC0, #-4, AC0
		mov AC0, *AR4
		sub AC1, AC2
		mov AC2, *SP(#0)

		mov T1, AC2
		add #1, AC2
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1
		bcc set3, TC1
		b continue3
set3:
		mov #0, T1
continue3:


    	mov XAR0, XAR4
		add T1, AR4

		mov #400h, AC0
		mov *AR4, T3
		mack T3, MU, AC0, AC0

    	sfts AC0, #-11, AC0

    	mov AC0, T3

    	mov #4000h, AC0
    	macm *SP(#0), T3, AC0, AC0

    	sfts AC0, #-15, AC0

    	add #2, AC0
		sfts AC0, #-2, AC0
    	add T2, AC0
    	mov AC0, T2


		mov T1, AC2
		sub #1, AC2
		mov #0, T1
		cmp AC2 < T1, TC1
		mov AC2, T1
		bcc set4, TC1
		b continue4
set4:
		mov #2, T1
continue4:



    	mov #-4000h, AR4
		cmp T2 < AR4, TC1
    	bcc set5, TC1
    	b continue5
set5:

		mov #-4000h, T2
		b continue6
continue5:


    	mov #4000h, AR4
    	cmp T2 > AR4, TC1
    	bcc set6, TC1
    	b continue6
set6:

		mov #4000h, T2
continue6:


		mov T2, *AR1

   		mov T1, *AR3

    	mov *SP(#0), T0

    	aadd #4, SP

		
		; Clean up program and return result
		POP mmap(ST2_55)					; Restore status registers
		POP	mmap(ST1_55)
		POP	mmap(ST0_55)
                               
		RET									; Return
;*******************************************************************************
;* End of anf.asm                                              				   *
;*******************************************************************************
