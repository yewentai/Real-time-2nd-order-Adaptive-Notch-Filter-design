#include <aic3204.h>
#include <dsplib.h>
#include <stdio.h>
#include <usbstk5515.h>

#include "anf.h"
#include "anf_cascade.h"

#define SAMPLES_PER_SECOND 8000
#define GAIN_IN_dB 10

int main()
{
    // Initialize variables =>我拿去跑了，有幾個點我寫在底下
    int left, right;             // Variables to store audio input =>用Int16, 然後在aic3204.h裡加上 #ifndef Uint16 typedef unsigned short Uint16; #endif  #ifndef Int16 typedef short Int16;#endi
    int e;                       // Variable to store filtered output
    int x[6] = {0};              // Circular buffer for ANF =>這個我試過了，{0} DSP板子只會initialize 1個0.很笨，需要{0,0,0,0,0,0}
    int a[2] = {0};              // Adaptive filter coefficients for ANF =>這個我試過了，{0} DSP板子只會initialize 1個0.很笨，需要{0,0}，而且需要給一個小的值，不然他這個term會直接變消失，我現在用{0x2000, 0x2000}
    int rho[2] = {0};            // Rho values for ANF =>這個我試過了，{0} DSP板子只會initialize 1個0.很笨，需要{0,0}，而且需要給一個值，不然他這個term會直接變消失，我現在用{0x6666, 0x6666}
    unsigned int index[2] = {0}; // Index for circular buffer

    // Initialize the Processor and Audio Codec
    USBSTK5515_init();
    aic3204_init();
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN_IN_dB);

    while (1)
    {
        // Read audio input from the microphone
        aic3204_codec_read(&left, &right);

        // Here you can process the audio signal using ANF or any other algorithm.
        // For example, using the anf_cascade function:
        e = anf_cascade(left, x, a, rho, index); // Process left channel => 改成e = anf_cascade(（int)left, &x, &a, &rho, &index)

        // Write the processed output to the line out
        aic3204_codec_write(e, e); // Writing the same output to both left and right channels
    }

    return 0;
}
