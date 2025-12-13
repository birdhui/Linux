// "End of main"이 출력되는가? -> No
// signal()의 인자로 intHandler 대신 SIG_IGN을 사용하면?
// -> Ctrl + C를 입력해도 시그널이 무시되기 때문에 프로세스 종료가 안됨
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void intHandler();

int main() {
	signal(SIGINT, intHandler);
	sleep(3);
	raise(SIGINT);
	printf("End of main");
}

void intHandler(int signo){
	printf("SIGINT \n");
	printf("Sig no.: %d\n", signo);
	exit(0);
}
