//#include <aic3204.h>
//#include <dsplib.h>
//#include <stdio.h>
//#include <usbstk5515.h>
//
//#include "anf.h"
//#include "anf_cascade.h"
//
//#define SAMPLES_PER_SECOND 8000
//#define GAIN_IN_dB 10
//
//int main() {
//  // declare variables
//
//  USBSTK5515_init(); // Initializing the Processor
//  aic3204_init();    // Initializing the Audio Codec
//
//  while (1) {
//    // Read from microphone
//	aic3204_codec_read(left, right);
//
//	// Implementation goes here...
//	set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN_IN_dB);
//
//	  while (1) {
//	    // Read from microphone
//	    aic3204_codec_read(&left, &right);
//
//	    // Process the audio signal using ANF or any other algorithm
//	    e_left = anf(left);
//	    e_right = anf(right);
//
//	    // Write to line out
//	    aic3204_codec_write(e_left, e_right);
//	  }
//
//    // Write to line out
//    aic3204_codec_write(e, e);
//  }
//
//  return 0;
//}
