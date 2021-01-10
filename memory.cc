//
//
//	memory.c
//
//	(C) R.P.Bellis 2021
//
//

#include "memory.h"

RAM::RAM(Word size) : Memory(size) {
}

ROM::ROM(Word size) : Memory(size) {
}

static Byte fread_byte(FILE *fp)
{
	char			str[3];
	long			l;

	str[0] = fgetc(fp);
	str[1] = fgetc(fp);
	str[2] = '\0';

	l = strtol(str, NULL, 16);
	return (Byte)(l & 0xff);
}

static Word fread_word(FILE *fp)
{
	Word		ret;

	ret = fread_byte(fp);
	ret <<= 8;
	ret |= fread_byte(fp);

	return ret;
}

void ROM::load_intelhex(const char *filename)
{
	FILE		*fp;
	int		done = 0;

	fp = fopen(filename, "r");
	if (!fp) {
		perror("filename");
		exit(EXIT_FAILURE);
	}

	while (!done) {
		Byte		n, t;
		Word		addr;
		Byte		b;

		(void)fgetc(fp);
		n = fread_byte(fp);
		addr = fread_word(fp);
		t = fread_byte(fp);
		if (t == 0x00) {
			while (n--) {
				b = fread_byte(fp);
				memory[addr++] = b;
			}
		} else if (t == 0x01) {
			// pc = addr;
			done = 1;
		}
		// Read and discard checksum byte
		(void)fread_byte(fp);
		if (fgetc(fp) == '\r') (void)fgetc(fp);
	}
}
