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

MU 		 		.set	200
LAMBDA	 		.set	52428
LAMBDA_REMAIN  	.set	13107
RHO_Q_FORMAT 	.set	16  				; Set RHO_Q_FORMAT to 16 in hexadecimal
A_Q_FORMAT   	.set	15   				; Set A_Q_FORMAT to 15 in hexadecimal
											; Functions callable from C code

	.sect	".text"
	.global	_anf

;*******************************************************************************
;* FUNCTION DEFINITION: _anf_asm		                           *
;*******************************************************************************
; int anf(int y,				=> *SP(#02h) => expected T0 => 0x00104C
;		  int *s,				=> *SP(#0ch),XAR0 => expected AR0 =>0x001056,57,58
;		  int *a,				=> *SP(#11h),XAR1 => expected AR1 => 0x001059, 5A
; 		  int *rho,				=> *SP(#04h),XAR2 => expected AR2 => 0x00105B
;	      unsigned int* index	=> *SP(#0fh),XAR3 => expected AR3 =>0x00104E
;		 );						=> T0
; T0: output argument (e in this case) must be stored in T0 at the end of code

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
		; declare variables

		aadd #-4, SP

		; Modify rho[0] ( rho(m-1) )section
		mov LAMBDA, T1
		mpym *AR2, T1, AC0				; Multiplication of 16q16 and 16q16 => in need of a 32 bit register to store the result 32q32 => MPYM
    	add #4000h, AC0    					; Add half of the LSB value to be shifted out for rounding
		sfts AC0, #-RHO_Q_FORMAT, AC0 		; Right shift AC0 by 16 bits to normalize from 32q32 to 16q16

		; Modify rho[1] ( rho(infinite) ) section
    	mov LAMBDA_REMAIN, T1					; Indirect addressing with these registers to access memory, ARx(#1) will access the next memory location from the one currently pointed to by ARx. => rho(infinite)
		mpym *AR2(#1), T1, AC1					; Multiplication of 16q16 and 16q16 => in need of a 32 bit register to store the result 32q32 => MPYM
		add #4000h, AC1    					; Add half of the LSB value to be shifted out for rounding
		sfts AC1, #-RHO_Q_FORMAT, AC1		; Right shift AC0 by 16 bits to normalize from 32q32 to 16q16
											; Add AC0 and AC1 to integrate rho(m) and then shift right by 1 to average
		add AC1, AC0						; AC0 = AC0 + AC1 (result of lambda * rho[0] and one_minus_lambda * rho[1]) => 16q16
		mov AC0, *AR2						; Store the updated value back into rho[0] at the location pointed to by AR2 => rho(m) 16q16

		; Assuming AR0 points to s[], AR1 points to a[], and AR2 points to rho[] => Calculat ethe [m-1] terms of s(m) and store in AC0
		amar *AR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		mov *AR3, T1						; move content index into T1, k = *index => points to m-1
		aadd T1, AR4						; In address phase, the content of AR4 is added to the signed content of T1 and the result is stored in AR4 s[k]
		mpym *AR2, *AR4, AC0				; AC0 = rho(m) * s[m-1] => 16q16 * 16q15 = 32q31
		add #4000h, AC0						; Add rounding offset, half to AC0, for rounding
		sfts AC0, #-RHO_Q_FORMAT, AC0		; Normalize AC0 to 16q15 to store in temporary register => AC0 = rho(m) * s[m-1]
											; T1 contains index, T2 will be used to store rho(m) * s[m-1], and T3 for s[m-1], T0 stores the latest sample y
		mov AC0, T2							; move rho(m) * s[m-1] (16q15) to T2
		mpym *AR1, T2, AC0					; AC0 = a[m-1] * (rho(m) * s[m-1]) => 16q15 * 16q15 = 32q30
		add #4000h, AC0						; Add half to AC0 for rounding
		sfts AC0, #-A_Q_FORMAT, AC0			; Shift right to 32q15 => AC0 = a[m-1] * (rho(m) * s[m-1])

		; Calcuate the [m-1] terms of e(m) => a(m-1)*s(m-1) => and store in AC1
		mpym *AR1, *AR4, AC1				; AC1 = a[m-1] * s[m-1] => 16q15 * 16q15 = 32q30
		add #4000h, AC1						; Add half to AC1 for rounding
		sfts AC1, #-A_Q_FORMAT, AC1			; Shift right to 32q15 => AC1 = a[m-1] * s[m-1]

		; Increment index k and check if k = (k + 1) % 3;
		mov T1, AC2							; T1 = index k
		add #1, AC2							; Increment the index
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1							; reload incremented k = (k+1) => k now points to [m-2] => back to T1
		bcc branch1, TC1					; If the result is not equal (hence, less than 3), continue
		b m_minus_two						; Unconditional branch
branch1:
		mov #0, T1							; If the result was equal to 3, reset the index to 0

m_minus_two:
		; Find s(m-2) and add to existing terms of s(m) => T1 now contains k = (m-2) => AC2 contains the rho[m]^2 * s[m-2]
		mov XAR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		add T1, AR4						; In address phase, added the signed content of T1 to the address of AR4 s[k] = s[m-2]
		mov *AR2, T2						; Move rho[0] into T2, rho[0] is the latest value of rho
		mpym *AR2, T2, AC2					; Multiply T2 = rho(m) with itself and store the result in AC2 (now contains rho(m)^2 in 32q30 format)
		sfts AC2, #-A_Q_FORMAT, AC2			; Shift right by 16 bits to normalize from 32q30 to 32q15 in AC2
		mov AC2, T2							; Store abck the rho[m]^2 to T2
		mpym *AR4, T2, AC2					; Multiply s[m-2] with rho(m)^2 with  AC2 = s[m-2] * rho(m)^2 => 16q15 * 16q15 = 32q30
		add #4000h, AC2						; Add half to AC2 for rounding
		sfts AC2, #-A_Q_FORMAT, AC2 		; Normalize AC2 to 16q15, AC2 now contains the second term of s[m]
											; Subtracting the second term from the first term for s[]
		sub AC2, AC0						; AC0 = a[m-1] * (rho(m) * s[m-1]) - rho(m)^2 * s[m-2] in 32q15

		; Adding the s[m-2] to AC1 for e[]
		sub *AR4, AC1, AC1						; Subtract s[m-2] from AC1 and Store the a[m-1] * s[m-1] - s[m-2] for e[] in AC1 (32q15)

		; Increment index k and check if k = (k + 1) % 3
		mov T1, AC2							; T1 = index k + 1
		add #1, AC2							; AC2 = k + 2
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1							; reload incremented k = (k+2) => k now points to [m] => back to T1
		bcc branch2, TC1					; If the result is not equal (hence, less than 3), continue
		b m_current							; Unconditional branch

branch2:
		mov #0, T1							; If the result was equal to 3, reset the index to 0

m_current:
		; Add y[m] to AC0 where AC0 = a[m-1] * (rho(m) * s[m-1]) - rho(m)^2 * s[m-2]
		mov XAR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		add T1, AR4						; In address phase, added the signed content of T1 (k+2) to the address of AR4 to reach s[m]
		add T0, AC0							; AC0 = y[m] + a[m-1] * (rho(m) * s[m-1]) - rho(m)^2 * s[m-2] => 32q15
		mov AC0, *AR4						; update the s[m] in AR4 with AC0

		; Calculate e(m) given AC0 = s[m] (32q15) and AC1 = a[m-1] * s[m-1] - s[m-2] (32q15) and stored in SP
		sub AC1, AC0						; AC1 = e[m] = s[m] - a[m-1] * s[m-1] + s[m-2] (32q15)
		psh dbl(AC0)						; The data stack pointer (SP) is decremented by 2. The content of AC1(31–16) is copied to the memory location pointed by SP and the content of AC1(15–0) is copied to the memory location pointed by SP + 1.

		; Increment index k and check if k = (k + 1) % 3
		mov T1, AC2							; AC2 = T1 = k + 2
		add #1, AC2							; Increment the index by 1 => k + 3 => k points to (m - 1)
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1							; reload incremented k = (k+3) => k now points to [m-1] => back to T1
		bcc branch3, TC1					; If the result is not equal (hence, less than 3), continue
		b m_minus_one						; Unconditional branch

branch3:
		mov #0, T1							; If the result was equal to 3, reset the index to 0

m_minus_one:
		; Update a[m] => MU is in 16q15 format
    	mov XAR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		add T1, AR4						; In address phase, added the signed content of T1 to the address of AR4
		mov MU, T2
		mov AC0, *AR5						; 2*e(m) in 32q15 and 16 LSBs of the accumulator stored in AR5
		mpym *AR5, T1, AC0					; To implement (long)mu * 2e(m) => 32q31
		add #4000h, AC0						; Add half for rounding
		sfts AC0, #-A_Q_FORMAT, AC0			; Normalize AC0 to 32q16 format
		mov AC0, T3							; Move mu * 2e(m) to T3
		mpym *AR4, T3, AC0					; AC0 = mu * 2 * e(m) * s[m-1] => 32q31
		add #4000h, AC0           			; Add half for rounding
		sfts AC0, #-A_Q_FORMAT, AC0       	; Normalize AC0 to 32q16 format of mu * 2* e(m) * s[m-1]
		add *AR1, AC0, AC0					; AC1 = a[m-1] + 2 * mu * e[m] * s[m-1] (32q16)
		sfts AC0, #-1, AC0					; Normalize AC1 = a[m] back to 32q15

		; Increment index k and check if k = (k + 2) % 3 same as k = (k - 1) % 3
		mov T1, AC2							; AC2 = T1 = k + 3
		sub #1, AC2							; from m-1 to m
		mov #0, T1
		cmp AC2 < T1, TC1
		mov AC2, T1							; reload incremented k = (k+5) => k now points to [m] => back to T1
		bcc branch4, TC1					; If the result is not equal, continue
		b m_final							; Unconditional branch

branch4:
		mov #2, T1

m_final:
		; Perform saturation if necessary.
		mov #4000h, T2						; T2 = a[m] in 16q15
		cmp AC0 > T2, TC1					; Compare if a[m] overflow.  If the fixed-point format is Q15, then 0x7FFF effectively represents the 1 - (1/2^15)
		bcc max_saturation, TC1				; Branch Conditionally: ; If condition TC1 is true, jump
		mov #-4000h, T2						; T2 = a[m] in 16q15
		cmp AC0 < T2, TC1
		bcc min_saturation, TC1
		mov AC0, T2
		b final								; Unconditional branch

max_saturation:
		mov #4000h, T2
		b final

min_saturation:
		mov #-4000h, T2
		b final

final:
		mov T2, *AR1						; update a[m]
		mov T1, *AR3						; update index
		pop dbl(AC0)						; moves the 16-bit data memory location pointed by SP to the accumulator high part ACx(31–16) and moves the content of the 16-bit data memory location pointed by SP + 1 to the accumulator low part ACx(15–0).
		mov AC0, T0							; return eaadd

		aadd #4, SP

											; Clean up program and return result
		POP mmap(ST2_55)
		POP	mmap(ST1_55)					; Restore FRCT, SXMD, SATD, M_40, C
		POP	mmap(ST0_55)
                               
		RET									; Return
;*******************************************************************************
;* End of anf.asm                                              				   *
;*******************************************************************************
