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

MU 		 .set  200
LAMBDA	 .set  19661
LAMBDA2  .set  13107
											; Functions callable from C code

	.sect	".text"
	.global	_anf

;*******************************************************************************
;* FUNCTION DEFINITION: _anf_asm		                           *
;*******************************************************************************
											; int anf(int v, long *X , long *A, int *rho, unsigned int* index);
											; v: int -> T0
											; X: int* -> AR0
											; A: int* -> AR1
											; rho: int* -> AR2
											; index: int -> AR3
											;
											; T0: output argument (e in this case) must be stored in T0 at the end of code

_anf:

		PSH  mmap(ST0_55)					; Keep original values of flags
		PSH  mmap(ST1_55)
		PSH  mmap(ST2_55)

		mov   #0, mmap(ST0_55)     			; Clear all fields (OVx, C, TCx)
    	or    #4100h, mmap(ST1_55)  		; Set CPL (bit 14), SXMD (bit 8), Note that FRCT is not set since here we are not only multiplying Q15 formats								;
    	and   #07940h, mmap(ST1_55)      	; Clear BRAF, M40, SATD, C16, 54CM, ASM
		bclr  ARMS                      	; Disable ARMS bit 15 in ST2_55
    	bset  SMUL							; Ensure that saturation-on-multiplication is set

											; Add your own code here

		; allocate some stack space for local variables
		aadd #-4, SP
		; +3		;
		; +2		; +2
		; +1 		;
		; 0			; 0

		; a_i => AC3
		; k => T2

		; int a_i = *a;
		mov *AR1, T2

		; k = *index;
		mov *AR3, T1

		; Modify rho setion
		; AC0 = (long)lambda * rho[0];
    	; AC0 += 0x00004000;
    	mov #4000h, AC0
		mov *AR2, T3
		mack T3, LAMBDA, AC0, AC0
    	; AC0 >>= 15;
    	sfts AC0, #-15, AC0

    	; AC1 = (long)lambda2 * rho[1];
    	; AC1 += 0x00004000;
    	mov #4000h, AC1
    	mov *AR2(#1), T3
		mack T3, LAMBDA2, AC1, AC1
    	; AC1 >>= 15;
    	sfts AC1, #-15, AC1
    	; rho[0] = (int)(AC0 + AC1);
		add AC0, AC1
		mov AC1, *AR2


		; AC0 = (long)(*rho) * x[k];
		; AC0 += 0x00000400;
		amar *AR0, XAR4					; load the data contained in the AR0 to XAR4
		aadd T1, AR4					; index k add to address contained in XAR4 => x[k]
		mov #400h, AC0
		macm *AR2, *AR4, AC0, AC0		; (long)(*rho) * x[k]
		; AC0 >>= 11; (AC0 >> 2)
		sfts AC0, #-13, AC0
		mov AC0, T3

   		; AC0 += 0x00000400;
		mov #400h, AC0
		; AC0 = (long)a_i * (AC0 >> 2);
		macm *AR1, T3, AC0, AC0			; a(m-1)*rho(m)*s(m-1)
    	; AC0 >>= 11;
    	sfts AC0, #-11, AC0
    	
		; AC1 = -(long)a_i * x[k];
		mov XAR0, XAR4
		add T1, AR4						; index k add to address contained in XAR4 => x[k]
    	; AC1 += 0x00000100;
		mov #100h, AC1
		macm *AR1, *AR4, AC1, AC1		; a(m-1)*s(m-1)
    	; AC1 >>= 9;
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

    	; AC1 += x[k]; for e[]
		mov XAR0, XAR4
		add T1, AR4
		sub *AR4 << #4, AC1, AC1 ; subtract because we didn't negate


		; long rho_sq = (long)(*rho) * *rho;
		; rho_sq += 0x00004000;
		mov #4000h, AC2
		macm *AR2, *AR2, AC2
    	; rho_sq >>= 15;
    	sfts AC2, #-15, AC2
    	mov AC2, T3
    	; long temp = ((long)x[k] * (short)rho_sq);

		; temp += 0x00000400;
		mov #400h, AC2
		macm *AR4, T3, AC2, AC2
    	; temp >>= 11;
    	sfts AC2, #-11, AC2
    	; AC0 -= temp;
		sub AC2, AC0

		; k = (k + 1) % 3;
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

    	; x[k] = (short)(((long)y + AC0) >> 4); //v[i]
    	mov XAR0, XAR4
		add T1, AR4
		add T0, AC0
		mov AC0, AC2
		sfts AC0, #-4, AC0
		mov AC0, *AR4
    	; e = (int)(((long)x[k] << 4) + (-) (long)AC1);
		sub AC1, AC2
		mov AC2, *SP(#0)

		; k = (k + 1) % 3;
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

    	; AC0 = (long)mu * x[k];
    	mov XAR0, XAR4
		add T1, AR4
    	; AC0 += 0x00000400;
		mov #400h, AC0
		mov *AR4, T3
		mack T3, MU, AC0, AC0
    	; AC0 >>= 11;
    	sfts AC0, #-11, AC0
    	; AC0 = (long)e * (short)(AC0);
    	mov AC0, T3
    	; AC0 += 0x00004000;
    	mov #4000h, AC0
    	macm *SP(#0), T3, AC0, AC0
    	; AC0 >>= 15;
    	sfts AC0, #-15, AC0
    	; a_i += (short)((AC0 + 0x2) >> 2);           
    	add #2, AC0
		sfts AC0, #-2, AC0
    	add T2, AC0
    	mov AC0, T2

    	; k = (k + 2) % 3; this is the same as k = (k - 1) % 3
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


    	; if (a_i < -0x4000)
    	mov #-4000h, AR4
		cmp T2 < AR4, TC1
    	bcc set5, TC1
    	b continueche5
set5:
        ;      a_i = -0x4000;
		mov #-4000h, T2
		b continue6
continue5:

    	; if (a_i > 0x4000)
    	mov #4000h, AR4
    	cmp T2 > AR4, TC1
    	bcc set6, TC1
    	b continue6
set6:
		;     a_i = 0x4000;
		mov #4000h, T2
continue6:

    	; *a = a_i;
		mov T2, *AR1
   		; *index = k;
   		mov T1, *AR3
    	; return e;
    	mov *SP(#0), T0

    	aadd #4, SP

											; Clean up program and return result
		POP mmap(ST2_55)
		POP	mmap(ST1_55)					; Restore FRCT, SXMD, SATD, M_40, C
		POP	mmap(ST0_55)
                               
		RET									; Return
;*******************************************************************************
;* End of anf.asm                                              				   *
;*******************************************************************************
