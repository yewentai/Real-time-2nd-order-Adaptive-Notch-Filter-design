#include <usbstk5515.h>
#include <usbstk5515_i2c.h>
#include <aic3204.h>
#include <stdio.h>

#include "band_pass.h"

#define BPL 61;

short FIR(char index, Int16 * w) {
    long y = 0;

    int i = 0;
    // FIR Filter Code here...
    for (i = 0;
            i < 61;
            i++) {
        y += (long)w[60 - ((i + index) % 61)]*(long)BP[i];
    }

    y += 0x00004000; // Round the part we'll be truncating ( Carry bit of this sum will be the LSB after conversion )
    return (short)(y >> 15); // Conversion: 32Q30 --> 16Q15.
}

int main(void) {

    USBSTK5515_init();

    FILE *fpIn, *fpOut;

    fpIn = fopen("..\\data\\input.pcm", "rb");  // Read file pointer
    fpOut = fopen("..\\data\\output.pcm", "wb");  // Write file pointer

    if (fpIn == NULL) {
        printf("Can't open input file\n");
        exit(0);
    }

    //we need a buffer of size BPL
    Int8 data[2];
    data[0] = data[1] = 0;
    Int16 buffer[61];
    memset((void*) buffer, 0, sizeof(buffer));

    char index = 0;
    while (fread(&data[0], sizeof(*data), 1, fpIn) == 1) {
        fread(&data[1], sizeof(*data), 1, fpIn);
        index++;
        index = index % 61;
        //little indian
        Int16 actual_data = (((Int16)data[1]) << 8) | (Int16)data[0];
        buffer[index] = actual_data;

        short out = FIR(index, buffer);
        data[1] = (char)(0xFF & out);
        data[0] = (char)((0xFF00 & out) >> 8);

        fwrite(&data[0], sizeof(*data), 1, fpOut);
        fwrite(&data[1], sizeof(*data), 1, fpOut);
    }
    printf("Filtering complete!");
}
