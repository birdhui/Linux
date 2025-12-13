// 1. "End of main"이 출력되는가?
// 2. signal()의 인자로 intHandler 대신 SIG_IGN을 사용하면?
// => ctrl+c에 대한 인터럽트 시그널은 무시 [해결] ctrl+z 해야 종료됨
// 시그널 2번 발생 시 프로세스 종료하도록 하려면?
// 3. 시그널이 2번 발생했을 때 프로세스 종료하도록 하려면? (아래 코드는 3번)
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void intHandler();

/* 인터럽트 시그널을 처리한다. */
int main() {

	signal(SIGINT, intHandler);
	// sleep(3);
	// raise(SIGINT);
	// printf("End of main \n");

	while(1) {
		pause();
	}

	return 0;
}

void intHandler(int signo) {
	static int count = 0;
	count++; // 카운트 증가
	printf("SIGINT %d회\n", count);
	
	if (count >= 2) {
		printf("2번째 SIGINT 종료\n");
		exit(0);
	} else {
		printf("1번째 SIGINT 무시(한번 더 누르세요)\n");
		// signal(SIGINT, SIG_IGN); // SIGINT 1번 무시
	}
}
