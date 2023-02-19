
#include <stdio.h>


void checkEndianess(char* start, int n, int len)
{
	int i;

	printf("0x%x is represented in memory as ", n);
	for (i = 0; i < len; i++) {
		printf(" %.2x", start[i]);
	}
	printf("\n");
}

void dispEndianess()
{
	unsigned int i = 1;
	char* c = (char*)&i;
	if (*c) {
		printf("Little Endian\n");
	}
	else {
		printf("Big Endian\n");
	}
}


int main()
{
	printf("Check Endianess\n");
	int n = 0x12345678;
	checkEndianess((char*)&n, n, sizeof(n));
	dispEndianess();
	
	return 0;
}

