#include <stdlib.h>
#include <stdio.h>

void cleanup1() {
	printf("Cleanup 1 is called.\n");
}

void cleanup2() {
	printf("Cleanup 2 is called.\n");
}

int main() {
	// atexit(): 프로세스 종료시 수행할 작업을 예약
	// 	     예약된 순서의 역순으로 수행됨 (LIFO)
	atexit(cleanup1);
	atexit(cleanup2);

	printf("!!!\n");
	// exit(): 프로세스를 종료시키고, 상태값을 부모 프로세스에 전달
	exit(0); // or return 0;
}
