#ifndef ANF_H
#define ANF_H

/**
 * 0 < rho < 1 => unsigned 16q16 fixed-point representation from 0 up to (2^16 - 1)/(2^16), just under 1. The resolution would be 1/(2^16), which is approximately 0.0000153.
 * ANF-LMS coefficient a => -1 < a < 1 => signed 16q15 => -1 < var < 1
 * To ensure filter stability, a(t) must be clamped within the range -1 to 1 after each update.
 * int => to store both positive and negative
 * unsigned int => positive only
 */
// Define the fixed-point format for rho (0q16 in this case)
#define RHO_Q_FORMAT 16

// Define the fixed-point format for a (16q15 in this case)
#define A_Q_FORMAT 15
#define RHO_Q_FORMAT 16
#define L_Q_FORMAT 15
#define S_Q_FORMAT 11
#define A_Q_FORMAT 15
#define E_Q_FORMAT 15
// Define the step size, mu, properly scaled for the fixed-point format
// mu is always < 1, hence using 0q16 format
#define mu 0xC8 // 16q15, approximately 200 in decimal

// Define LAMBDA and ONE_MINUS_LAMBDA for 0q16 format
// Both are always <= 1, hence using 16q15 format
// 0x7333 0.9 0x4CCC 0.6
#define LAMBDA 0x4CC
// 0x0CCD 0.1 0x3333 0.4
#define ONE_MINUS_LAMBDA 0x3333

int anf(short y, short *s, unsigned short *a, unsigned short *rho, int *index);

#endif
