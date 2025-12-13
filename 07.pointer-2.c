#include <stdio.h>

int main() {
	int *pnum, num = 0;
	pnum = &num;
	printf("num: %d\n", num);

	*pnum = 10;
	printf("num: %d\n", num);
	printf("pnum: %p\n", (void*)pnum);

	return 0;
}
