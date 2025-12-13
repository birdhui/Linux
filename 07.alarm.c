// 설정된 alarm 시간 이후에 "Wake up!"을 출력하도록 수정해보자
#include <stdio.h>
#include <unistd.h>

int main() {
	
	alarm(5);

	printf("루프 시작...\n");

	while(1) {
		sleep(1);
		printf("1초 후... \n");
	}
	printf("Wake Up!\n");

}
