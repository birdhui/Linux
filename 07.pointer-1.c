#include <stdio.h>
#include <stdlib.h>

int main() {
	int num = 42;
	int *p = &num; // num의 주소를 포인터 *p에 저장
	
	printf("주소 p: %p\n",(void*)p); // num의 주소(%p는 void* 필요)
	printf("값 p: %d\n", *p); // num의 실제 저장 값
}
