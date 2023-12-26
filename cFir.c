#include "cFir.h"

// Define DSP system memory map  
#pragma CODE_SECTION(cFir, ".text:fir");

// Direct-Form FIR Filter Implementation with circular buffer
short cFir(short x,  short *h, short *index, short *w, short length) {
/*
 * x: input sample (16-bit)
 * h: pointer to filter taps -- index 0
 * index: current index in filter delay line
 * w: pointer to delay line -- index 0
 * length: length of the FIR filter
*/
    short i, k;
    long accumulator = 0;

    k = *index;
    w[k] = x;  // Get the current data to delay line

    for (i = 0; i < length; i++)  // FIR filter processing
    {
      accumulator += *h++ * (long) w[k++];  // Q16.15 * Q16.15 = Q32.30
      k = k >= length ? 0 : k;  // Simulate circular buffer
    }

    *index = (k == 0) ? length - 1 : k - 1;  // Update circular buffer index.

    accumulator += 0x00004000;  // Round the part we'll be truncating ( Carry bit of this sum will be the LSB after conversion )
    return (short) (accumulator >> 15);  // Return filter output
}
