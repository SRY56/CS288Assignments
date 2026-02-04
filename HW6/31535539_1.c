#include <stdio.h>
#include <ctype.h>

int main (int argc, char *argv[]) {
	unsigned char *p;

	// print argv variable
	printf("argv      | ");
	p = (unsigned char *)&argv;
	for (int i = 0; i < 8; i++) {
		printf("%02x ", p[i]);
	}
	printf("| %p\n", (void *)argv);

	printf("\n");

	// print argv[0], argv[1] and so on
	for (int i = 0; i < argc; i++) {
		printf("argv[%d]   | ", i);
		p = (unsigned char *)&argv[i];
		for (int j = 0; j < 8; j++) {
			printf("%02x ", p[j]);
		}
		printf("| %p\n", (void *)argv[i]);
	}

	printf("\n");

	// print command line arguments
	// find starting address before first argement
	unsigned char *start = (unsigned char *)argv[0];

	// align down to teh nearest 8-byte boundry
	unsigned long addr = (unsigned long)start;
	addr = (addr / 8) * 8;

	// find ending address
	unsigned char *end = (unsigned char *)argv[argc - 1];
	while (*end != '\0') {
		end++;
	}
	end++;
	
	addr = (unsigned long)end;
	if (addr % 8 != 0) {
		addr = ((addr/8) + 1) * 8;
	}
	end = (unsigned char *)addr;

	p = start;
	while (p < end) {
		printf("|");
		for (int i = 0; i < 8 && p + i < end; i++) {
			unsigned char byte = p[i];
			if (isprint(byte)) {
				printf("%02x(%c) ", byte, byte);
			} else if (byte == '\0') {
				printf("%02x(\\0) ", byte);
			} else if (byte = '\n') {
				printf("%20x(\\n) ", byte);
			} else if (byte = '\t') {
				printf("%02x(\\t) ", byte);
			} else if (byte == ' ') {
				printf("%02x( ) ", byte);
			} else {
				printf("%02x    ", byte);
			}
		}
		printf(" |%p\n", (void *)p);
		p += 8;
	}

	return 0;
}
