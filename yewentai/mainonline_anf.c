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
    // Initialize variables
    int left, right;             // Variables to store audio input
    int e;                       // Variable to store filtered output
    int x[6] = {0};              // Circular buffer for ANF
    int a[2] = {0};              // Adaptive filter coefficients for ANF
    int rho[2] = {0};            // Rho values for ANF
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
        e = anf_cascade(left, x, a, rho, index); // Process left channel

        // Write the processed output to the line out
        aic3204_codec_write(e, e); // Writing the same output to both left and right channels
    }

    return 0;
}
