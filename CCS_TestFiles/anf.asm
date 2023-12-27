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

	MU 		 		.set	0xC8				; 16q15, approximately 200 in decimal
	LAMBDA	 		.set	0x7333				; Approximately 0.9 in 16q15 formatt
	LAMBDA_REMAIN	.set	0x0CCD				; Approximately 0.1 in 16q15 format ( 1 - lambda)
	RHO_Q_FORMAT 	.set	0x10  				; Set RHO_Q_FORMAT to 16 in hexadecimal
	A_Q_FORMAT   	.set	0xF   				; Set A_Q_FORMAT to 15 in hexadecimal


; Functions callable from C code

	.sect	".text"
	.global	_anf

;*******************************************************************************
;* FUNCTION DEFINITION: _anf_asm		                           *
;*******************************************************************************
; int anf(int y,				=> T0  (Temporary Registers, 16-bits, latest sample)
;		  int *s,				=> AR0 (Auxiliary register, 32-bits, state buffer)
;		  int *a,				=> AR1 (Auxiliary register, 32-bits, adaptive coefficient)
; 		  unsigned int *rho,	=> AR2 (Auxiliary register, 32-bits, ratio for bandwidth adjustment)
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
		; declare variables
		aadd #-4, SP						; Modify Data Stack Pointer 16-bits => SP += -4 (signed extended) => reserve 4 bytes space for local variables in SP.

		; Modify rho[0] ( rho(m-1) )section
		mov *AR2, T1						; Mov rho[0] (rho(m-1)) to temporary register T1 for multiplication => 16q16
		mpym T1, LAMBDA, AC0				; Multiplication of 16q16 and 16q15 => in need of a 32 bit register to store the result 32q31 => MPYM
    	add #8000h, AC0    					; Add half of the LSB value to be shifted out for rounding
		sfts AC0, #-RHO_Q_FORMAT, AC0 		; Right shift AC0 by 16 bits to normalize from 32q31 to 16q15
		
		; Modify rho[1] ( rho(infinite) ) section
    	mov *AR2(#1), T1					; Indirect addressing with these registers to access memory, ARx(#1) will access the next memory location from the one currently pointed to by ARx. => rho(infinite)
		mpym T1, LAMBDA_REMAIN, AC1			; Multiplication of 16q16 and 16q15 => in need of a 32 bit register to store the result 32q31 => MPYM
		add #8000h, AC1    					; Add half of the LSB value to be shifted out for rounding
		sfts AC1, #-RHO_Q_FORMAT, AC1		; Right shift AC0 by 16 bits to normalize from 32q31 to 16q15
											; Add AC0 and AC1 to integrate rho(m) and then shift right by 1 to average
		add AC0, AC1, AC0					; AC0 = AC0 + AC1 (result of lambda * rho[0] and one_minus_lambda * rho[1]) => 16q15
		sfts AC0, #-1, AC0					; Right shift AC0 by 1 bit to average the sum => 16q16, correct?
											; Store the result back to rho[0]
		mov AC0, *AR2						; Store the updated value back into rho[0] at the location pointed to by AR2 => rho(m) 16q16

		; Calculate rho square while avoiding overflow
											; First, we need to load the latest rho value into a register
		mov *AR2, T1						; Move rho[0] into T1, rho[0] is the latest value of rho
											; Perform the multiplication of rho(m) with itself to get the square
		mpym T1, T1, AC0					; Multiply T1 with itself and store the result in AC0 (now contains rho(m)^2 in 32q32 format)
											; Normalize rho(m)^2 back to 16q16 format by shifting right by the Q-format (16 bits)
		sfts AC0, #-RHO_Q_FORMAT, AC0		; Shift right by 16 bits to normalize from 32q32 to 16q16
											; At this point, AC0 contains rho(m)^2 in 16q16 format
											; Store rho(m)^2 in a separate location if rho[0] is already being used for rho(m)
											; For example, you can store it in rho[1] or another auxiliary register or memory location
											; This is just an example, actual storage will depend on your memory/register management
		mov AC0, *AR2(#1)					; Store rho(m)^2 into the location immediately following rho[0] (rho[1] in this example) => 16q16

		; Assuming AR0 points to s[], AR1 points to a[], and AR2 points to rho[]
											; T1 contains rho(m), T2 will be used for a[m-1], and T3 for s[m-1], T0 stores the latest sample y
											; Load a[m-1] into T2
		mov *AR1, T2						; T2 = a[m-1] => 16q15
											; Load s[m-1] into T3
		mov *AR0, T3						; T3 = s[m-1] => 16q15
											; Multiply rho(m) with a[m-1]
		mpym T1, T2, AC0					; AC0 = rho(m) * a[m-1] => 16q16 * 16q15 = 32q31
											; Add rounding offset
		add #4000h, AC0						; Add half to AC0 for rounding
											; Shift right to normalize
		sfts AC0, #-A_Q_FORMAT, AC0			; Normalize AC0 to 32q16 to match with rho_square => AC0 = rho(m) * a[m-1]
											; Multiply AC0 with s[m-1]
		mpym AC0, T3, AC0					; AC0 = AC0 * s[m-1] => 32q16 * 16q15 (assuming s is in q15 format) = 32q31
											; Add rounding offset
		add #4000h, AC0						; Add half to AC0 for rounding
											; Shift right to normalize
		sfts AC0, #-A_Q_FORMAT, AC0 		; Normalize AC0 to 32q16, AC0 now contains the first term of s[m]

		; Now calculate the second term: rho(m)^2 * s[m-2]
											; First, get rho(m)^2 from where it was stored
		mov *AR2(#1), AC1					; AC1 = rho(m)^2, assuming that rho(m)^2 was stored at AR2+1
											; Load s[m-2] into T1 since rho(m) is no longer needed
		mov *AR0(#1), T1					; Load s[m-2] into T1, assuming AR0 is currently pointing to s[m-1]
											; Multiply rho(m)^2 with s[m-2]
		mpym AC1, T1, AC1					; AC1 = rho(m)^2 * s[m-2] => 16q16 * 16q15 = 32q31
											; Add rounding offset
		add #4000h, AC1						; Add half to AC1 for rounding
											; Shift right to normalize
		sfts AC1, #-A_Q_FORMAT, AC1 		; Normalize AC1 to 32q16, AC1 now contains the second term of s[m]

		; Calculate s[m] by adding y[m], the first term, and subtracting the second term
											; y[m] is in T0 => Add y[m] to the first term
		mov T0, AC2							; Move y into AC2 for conversion => 32q15
		sfts AC2, #1, AC2					; Shift y left by 1 to match the Q-format (convert y to 32q16)
		add AC2, AC0, AC0					; AC0 = AC0 + AC2 (first term of s[m] + y) => 32q16
		sub AC0, AC1, AC0					; AC0 = AC0 - AC1 (subtract second term of s[m]) => 32q16
		sfts AC0, #-RHO_Q_FORMAT, AC0		; Convert AC0 from 32q16 to all integer
		mov AC0.h, *AR0(#-1)				; Store the high 16 bits of AC0 back to s[m]

		; Calculate e(m): T1 = s[m-2], T2 = a[m-1], T3 = s[m-1]
											; Calculate a[m - 1]s[m - 1]
		mpym T2, T3, AC1					; AC1 = a[m - 1] * s[m - 1] => 32q30
		add #4000h, AC1						; Add half to AC1 for rounding
		sfts AC1, #-A_Q_FORMAT, AC1			; Normalize AC1 to 32q15
											; Normalize AC0 from 32q16 to 32q15 
		sfts AC0, #-1, AC0					; Adjust SHIFT_AMOUNT to align Q formats
											; Now subtract the normalized AC1 from AC0
		sub AC0, AC1, AC0					; AC0 = AC0 - AC1 => 32q15 => (s[m] - a[m - 1]s[m - 1])
											; Sign-extend T1 from 16q15 to 32q15 and store it in AC1 (or another temporary accumulator)
											; Assuming T1 is already in the lower 16 bits of the register
											; and the sign bit needs to be extended throughout the upper 16 bits.
		mov T1, AC1.l						; Move lower 16 bits of T1 into lower part of AC1
		sfts AC1.l, #RHO_Q_FORMAT, AC1.h	; Sign-extend lower 16 bits into upper 16 bits of AC1
											; Now AC1 contains the sign-extended version of T1 and is in 32q15 format
											; Add the 32-bit version of s[m-2] to the result
		add AC0, AC1, AC0					; AC0 = AC0 + AC1 (s[m] - a[m-1]s[m-1] + s[m-2]) => 32q15
		add #8000h, AC0						; Add half to AC0 for rounding
		sfts AC0, #-RHO_Q_FORMAT, AC0		; Normalize the result to get e(m) => e(m) needs to be in 16q15 format
		mov AC0.l, T0						; Move the lower 16 bits of AC0 to T0, which should contain e(m) in 16q15 format for return


		; Update a[m] => MU is in 16q15 format
		add AC0, AC0, AC1					; 2*e(m)
		mpym AC1, MU, AC1					; To implement (long)mu * 2e(m) => 32q30
		add #4000h, AC1						; Add half for rounding
		sfts AC1, #-A_Q_FORMAT, AC1			; Normalize AC1 to 32q15 format 
		mpym AC1, T3, AC1					; AC1 = mu * 2* e(m) * s[m-1] => 32q30
		add #4000h, AC1           			; Add half for rounding
		sfts AC1, #-A_Q_FORMAT, AC1       	; Normalize AC1 to 32q15 format
		sfts AC1, #-RHO_Q_FORMAT, AC1       ; Convert AC1 to 16q15 format.
		add T2, AC1.l, T2         			; T2 = a[m-1] + update term => 16q15 a[m]

		; Perform saturation if necessary.
		cmp T2 > #4000h, TC1				; Compare if a[m] overflow.  If the fixed-point format is Q15, then 0x7FFF effectively represents the 1 - (1/2^15)
		bcc b1, TC1							; Branch Conditionally: ; If condition TC1 is true, jump to b1
		cmp T2 < -#4000h, TC1
		bcc b2, TC1
		b routine							; Unconditional branch to routine 

b1:
		mov #4000h, T2
b2:
		mov -#4000h, T2

routine:
		; Update the value of a[m] with the new value.
		mov T2, *AR1(#1)					; Store the new a[m] back to the location pointed to by AR1.
											; Assuming AR3 holds the address where the index 'k' is stored
											; Assuming the buffer size is 3, and we need to calculate (k + 1) % 3
											; Load the current index 'k' into a temporary register T1 from the memory location pointed by AR3
		; k = (k + 1) % 3;
		mov *AR3, T1
		add #1, T1							; Increment the index
		cmp T1, #3, TC1						; Check if the incremented index is equal to 3
		bcc b3, TC1							; If the result is not equal (hence, less than 3), continue
		mov T1, *AR3						; Store the updated index 'k' back to the memory location pointed by AR3
b3:	
		mov #0, T1							; If the result was equal to 3, reset the index to 0
		mov T1, *AR3
		; Continue with the rest of the code


		
		; Clean up program and return result
		POP mmap(ST2_55)					; Restore status registers
		POP	mmap(ST1_55)
		POP	mmap(ST0_55)
                               
		RET									; Return
;*******************************************************************************
;* End of anf.asm                                              				   *
;*******************************************************************************
