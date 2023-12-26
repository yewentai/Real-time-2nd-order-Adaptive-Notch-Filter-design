#include <stdio.h>
#include <usbstk5515.h>
#include "usbstk5515_i2c.h"
#include <stdlib.h>
#include <aic3204.h>
#include <dsplib.h>

#include "anf.h"
#define SAMPLES_PER_SECOND 8000
#define GAIN_IN_dB 10

int main() {
  // declare variables
  int s[3] = {0,0,0};    // State vector X
  int a[2] = {0x2000, 0x2000};
  unsigned int rho[2] = {0x6666,0x6666};  // Initial values for rho(t), rho(Inf)
  int y, e;  // input sample y, error signal sample e
  unsigned int index = 0;
  Int16 left, right;  // Variables to store mic two channels data
  char temp[2];

  USBSTK5515_init(); // Initializing the Processor
  aic3204_init();    // Initializing the Audio Codec
  set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN_IN_dB);

	FILE* fpOut = fopen("..\\data\\outpureal_codec.pcm", "wb");
  if (fpOut == NULL) {
    printf("Can't open output file. Exiting.\n");
    return 1;
  }

  // while (1) {
  //   // Read from microphone
	//   aic3204_codec_read(&left, &right);

	//   // Implementation goes here...
  //   y = left;  // Assuming you want to process the left channel
  //   // For example, using the anf_cascade function:
  //   e = anf(&y, &s, &a, &rho, &index); // Process left channel
  //   // Write to output file for evaluation
  //   // Convert 16-bit integer back to 2 bytes in little-endian format
  //   temp[0] = (short) (e & 0x00FF);
  //   temp[1] = (short) (e & 0xFF00) >> 8;
  //   fwrite(temp, sizeof(char), 2, fpOut);
  //   // Write to line out
  //   aic3204_codec_write(e, e);
  // }
  
  /** the following code is for testing only
   * mic iniput directly to output. it shows that there was no real stereo input
   * given that both with filter anf and without filter ( mic read to mic write) are both flat in spectrum
  */

  int seconds = 10; // Duration for which you want to run the loop
  int totalSamples = SAMPLES_PER_SECOND * seconds; // Total samples to process

  for (int i = 0; i < totalSamples; i++) {
    // Read from microphone
    aic3204_codec_read(&left, &right);

    // Implementation goes here...
    y = left;  // Assuming you want to process the left channel
    // Comment out anf function to debug if microphone is working properly
    e = anf(y, &s, &a, &rho, &index); // Process left channel
    // Write to output file for evaluation
    // Convert 16-bit integer back to 2 bytes in little-endian format
    temp[0] = (short) (e & 0x00FF);
    temp[1] = (short) (e & 0xFF00) >> 8;
    fwrite(temp, sizeof(char), 2, fpOut);
    // Write to line out
    aic3204_codec_write(e, e);
  }
  fclose(fpOut);
  return 0;
}