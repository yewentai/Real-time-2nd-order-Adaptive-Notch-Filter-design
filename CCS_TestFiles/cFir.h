#define  NUM_TAPS   48

// Ordering of parameters is important for ASM:
// x -> T0
// *h -> AR0
// *index -> AR1
// *w -> AR2 (has to be AR2 since we use two circular buffers)
// length -> T1
short cFir(short x,  short *h, short *index, short *w, short length);
