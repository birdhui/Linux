#include <stdlib.h>
#include <stdio.h>

void cleanup1() {
	printf("Cleanup 1 is called.\n");
}

void cleanup2() {
	printf("Cleanup 2 is called.\n");
}

int main() {
	atexit(cleanup1);	// atexit()에 등록된 함수 수행 (등록의 역순)
	atexit(cleanup2);

	printf("!!!\n");
	exit(0); // or return 0;
}
