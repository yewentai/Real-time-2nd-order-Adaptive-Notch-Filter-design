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

MU 		 		.set	200					; unsigned [1,0] => 16q15
LAMBDA	 		.set	19661				; unsigned [1,0] => 16q15
LAMBDA_REMAIN  	.set	13107				; unsigned [1,0] => 16q15
RHO_Q_FORMAT 	.set	16  				; unsigned Set RHO_Q_FORMAT to 16 in hexadecimal
L_Q_FORMAT		.set	15
S_Q_FORMAT		.set	11
A_Q_FORMAT		.set	15
E_Q_FORMAT		.set	15
; Functions callable from C code

	.sect	".text"
	.global	_anf

;*******************************************************************************
;* FUNCTION DEFINITION: _anf_asm		                           *
;*******************************************************************************
; int anf(short y,				=> *SP(#02h) => expected T0 => 0x00104C => 16q11
;		  short *s,				=> *SP(#0ch),XAR0 => expected AR0 =>0x001056,57,58 => [1,-1]=> 16q11
;		  unsigned short *a,	=> *SP(#11h),XAR1 => expected AR1 => 0x001059=> 16q15
; 		  unsigned short *rho,	=> *SP(#04h),XAR2 => expected AR2 => 0x00105B => 16q16
;	      int* index	=> *SP(#0fh),XAR3 => expected AR3 =>0x00104E => 16q13
;		 );						=> T0 => short error => 16q15
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
		mpym *AR2, T1, AC0					; Multiplication of 16q16 and 16q15 => in need of a 32 bit register to store the result 32q31 => MPYM
		sfts AC0, #-L_Q_FORMAT, AC0 		; Right shift AC0 by 15 bits to normalize from 32q31 to 16q16

		; Modify rho[1] ( rho(infinite) ) section
    	mov LAMBDA_REMAIN, T1				; Indirect addressing with these registers to access memory, ARx(#1) will access the next memory location from the one currently pointed to by ARx. => rho(infinite)
		mpym *AR2(#1), T1, AC1				; Multiplication of 16q16 and 16q15 => in need of a 32 bit register to store the result 32q31 => MPYM
		sfts AC1, #-L_Q_FORMAT, AC1			; Right shift AC0 by 15 bits to normalize from 32q31 to 16q16
											; Add AC0 and AC1 to integrate rho(m) and then shift right by 1 to average
		add AC1, AC0						; AC0 = AC0 + AC1 (result of lambda * rho[0] and one_minus_lambda * rho[1]) => 16q16
		mov AC0, *AR2						; Store the updated value back into rho[0] at the location pointed to by AR2 => rho(m) 16q16

		; Assuming AR0 points to s[], AR1 points to a[], and AR2 points to rho[] => Calculat ethe [m-1] terms of s(m) and store in AC0
		amar *AR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		mov *AR3, T1						; move content index into T1, k = *index = 0 => points to m-1 => 16q13
		aadd T1, AR4						; In address phase, the content address of AR4 is added to the signed content of T1 and the result is stored in AR4 s[0]
		mpym *AR2, *AR4, AC0				; AC0 = rho(m) * s[m-1] => 16q16 * 16q11 = 32q27
		sfts AC0, #-RHO_Q_FORMAT, AC0		; Normalize AC0 to 16q11 to store in temporary register => AC0 = rho(m) * s[m-1]
											; T1 contains index, T2 will be used to store rho(m) * s[m-1], and T3 for s[m-1], T0 stores the latest sample y
		mov AC0, T2							; move rho(m) * s[m-1] (16q11) to T2
		mpym *AR1, T2, AC0					; AR1 points to a(m-1), AC0 = a[m-1] * (rho(m) * s[m-1]) => 16q15 * 16q11 = 32q26
		sfts AC0, #-A_Q_FORMAT, AC0			; Shift right to 32q11 => AC0 = a[m-1] * (rho(m) * s[m-1])

		; Calcuate the [m-1] terms of e(m) (16q15) => a(m-1)*s(m-1) => and store in AC1
		mpym *AR1, *AR4, AC1				; AC1 = a[m-1] * s[m-1] => 16q15 * 16q11 = 32q26
		sfts AC1, #-A_Q_FORMAT, AC1			; Shift right to 32q11 => AC1 = a[m-1] * s[m-1] to be subtracted from s[]

		; Increment index k and check if k = (k + 1) % 3;
		mov T1, AC2							; T1 = index k
		add #1, AC2							; Increment the index => 1
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
		aadd T1, AR4						; In address phase, added the signed content of T1 to the address of AR4 s[k] = s[m-2] = s[1]
		mov *AR2, T2						; Move rho[0] into T2, rho[0] is the latest value of rho
		mpym *AR2, T2, AC2					; Multiply T2 = rho(m) with itself and store the result in AC2 (now contains rho(m)^2 in 32q32 format)
		sfts AC2, #-RHO_Q_FORMAT, AC2		; Shift right by 16 bits to normalize from 32q16 to 16q16 in AC2
		mov AC2, T2							; Store abck the rho[m]^2 to T2
		mpym *AR4, T2, AC2					; Multiply s[m-2] with rho(m)^2 with  AC2 = s[m-2] * rho(m)^2 => 16q11 * 16q16 = 32q27
		sfts AC2, #-RHO_Q_FORMAT, AC2 		; Normalize AC2 to 16q11, AC2 now contains the second term of s[m]
											; Subtracting the second term from the first term for s[]
		sub AC2, AC0						; AC0 = a[m-1] * (rho(m) * s[m-1]) - rho(m)^2 * s[m-2] in 16q11

		; Adding the s[m-2] to AC1 for e[] => s[m-1] in 16q11 whereas e[] in 16q15 => delta of 4 bits
		sub *AR4, AC1, AC1					; Subtract s[m-2] from AC1 and Store the a[m-1] * s[m-1] - s[m-2] for e[] in AC1 (32q11)

		; Increment index k and check if k = (k + 1) % 3
		mov T1, AC2							; T1 = index k + 1 = 1
		add #1, AC2							; AC2 = k + 2 = 2
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
		aadd T1, AR4						; In address phase, added the signed content of T1 (k+2) to the address of AR4 to reach s[m] = s[2]
		add T0, AC0							; AC0 = y[m] + a[m-1] * (rho(m) * s[m-1]) - rho(m)^2 * s[m-2] => 32q11
		mov AC0, *AR4						; update the s[m] in AR4 with AC0 =>s[2] = s(m)

		; Calculate e(m) given AC0 = s[m] (32q15) and AC1 = a[m-1] * s[m-1] - s[m-2] (32q15) and stored in SP
		sub AC1, AC0						; AC1 = e[m] = s[m] - a[m-1] * s[m-1] + s[m-2] (32q11)
		psh dbl(AC0)						; The data stack pointer (SP) is decremented by 2. The content of ACx(31–16) is copied to the memory location pointed by SP and the content of ACx(15–0) is copied to the memory location pointed by SP + 1.
		;mov AC0, *SP(#0)

		; Increment index k and check if k = (k + 1) % 3
		mov T1, AC2							; AC2 = T1 = k + 2 = 2
		add #1, AC2							; Increment the index by 1 => k + 3 = 3=> k points to (m - 1)
		mov #2, T1
		cmp AC2 > T1, TC1
		mov AC2, T1							; reload incremented k = (k+3) => k now points to [m-1] => back to T1
		bcc branch3, TC1					; If the result is not equal (hence, less than 3), continue
		b m_minus_one						; Unconditional branch

branch3:
		mov #0, T1							; If the result was equal to 3, reset the index to 0 => s[0]
		b m_minus_one						; Unconditional branch

m_minus_one:
		; Update a[m] => MU is in 16q15 format
    	mov XAR0, XAR4						; Modify extended auxilary register content by computing the effective address specified by AR0 and store it in the 23-bit desitnatoin register (XARx)=> XAR4 = address of s
		aadd T1, AR4						; In address phase, added the signed content of T1 to the address of AR4 =>s[0] = s(m-1)
		mov MU, T2
		mov AC0, *AR5						; e(m) in 32q11 and 16 LSBs of the accumulator stored in AR5 16q11
		mpym *AR5, T2, AC0					; To implement mu * e(m) => 16q15 * 16q11 = 32q26
		sfts AC0, #1, AC0					; times 2 by left shifting => 2* mu * e(m)
		sfts AC0, #-E_Q_FORMAT, AC0			; Normalize AC0 to 32q11 format to align with s[]
		mov AC0, T3							; Move mu * 2e(m) to T3 => 16q11
		mpym *AR4, T3, AC0					; AC0 = mu * 2 * e(m) * s[m-1] => 16q11 * 16q11 = 32q22
											; bits delta between a and s is 4 => 15 - 11 = 4 => 11 - 4 = 7
		sfts AC0, #-7, AC0       			; Normalize AC0 to 32q15 format of 2 * mu * e(m) * s[m-1] to align with a[]
		add *AR1, AC0, AC0					; AR1 points to a(m-1) => AC1 = a[m-1] + 2 * mu * e[m] * s[m-1] (32q15)
		mov AC0, T2							; T2 = a[m] => 16q15

		; Increment index k and check if k = (k + 2) % 3 same as k = (k - 1) % 3
		mov T1, AC2							; AC2 = T1 = k = 0
		sub #1, AC2							; from m-1 to m => k = -1
		mov #0, T1
		cmp AC2 < T1, TC1
		mov AC2, T1							; reload incremented k = -1 => k now points to [m] => back to T1
		bcc branch4, TC1					; If the result is not equal, continue
		b m_final							; Unconditional branch

branch4:
		mov #2, T1							; k = 2 => points to (m) => s[2]
		b m_final

m_final:
		; Perform saturation if necessary.
		mov #8000h, AC1						; T2 = a[m] in 16q15
		cmp T2 >= AC1, TC1					; Compare if a[m] overflow
		bcc max_saturation, TC1				; Branch Conditionally: ; If condition TC1 is true, jump
		mov #-8000h, AC1
		cmp T2 <= AC1, TC1
		bcc	min_saturation, TC1

max_saturation:
		mov #7FFFh, T2						; let 1.99 (16q15) be the maximum at a[1 a(m)
		b final

min_saturation:
		mov #-7FFFh, T2						; let -1.99 (16q15) be the minimum at a[1 a(m)
		b final								; Unconditional branch

final:
		mov T2, *AR1						; Update a  => AR1 now points to a(m)
		mov T1, *AR3						; index = k = 2 => update index to let the main script know whcih index of s[] are we on now
		pop dbl(AC0)						; moves the 16-bit data memory location pointed by SP to the accumulator high part ACx(31–16) and moves the content of the 16-bit data memory location pointed by SP + 1 to the accumulator low part ACx(15–0).
		sfts AC0, #4, AC0					; 32q11 was in stack => shift left by 4 to 32q15
		mov AC0, T0							; return e => 16q15
    	;mov *SP(#0), T0

		aadd #4, SP

											; Clean up program and return result
		POP mmap(ST2_55)
		POP	mmap(ST1_55)					; Restore FRCT, SXMD, SATD, M_40, C
		POP	mmap(ST0_55)
                               
		RET									; Return
;*******************************************************************************
;* End of anf.asm                                              				   *
;*******************************************************************************
