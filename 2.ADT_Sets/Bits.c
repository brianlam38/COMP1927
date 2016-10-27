// BitS union

#define NBITS 1024
#define NWORDS (NBITS/32)

typedef unsigned int Word;

typedef Word BitS[NWORDS];

BitS a, b, c;

// a: 0010101010100001100110100101100100101001...
// b: 1101000010010000000100100000010001001000...
// c':1111101010110001100110100101110101101001...

void BitUnion(BitS a, BitS b, BitS c)
{
	int i;

	for (i = 0; i < NWORDS; i++) {
		c[i] = a[i] | b[i];
	}
}

void BitInsert(Bits a, int i)
{
	int whichWord = i/32;
	int whichBit  = i%32;

	a[whichWord] = a[whichWord] | (1 << whichBit);
OR
	a[whichWord] |= (1 << whichBit);
}

void BitDelete(Bits a, int i)
{
	int whichWord = i/32;
	int whichBit  = i%32;

    00000001
    00000100  << 2
    11111011  ~

    11111011  mask
    10101100  &
    --------
    10101000

	a[whichWord] = a[whichWord] & (~(1 << whichBit));
OR
	a[whichWord] &= (~(1 << whichBit));
}
