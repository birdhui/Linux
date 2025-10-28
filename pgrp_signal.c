#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {

	int child, status;
	pid_t pid = fork();

	if (pid == 0) {
		// --- 자식 프로세스 ---
		// getpgrp(): 함수를 호출하는 프로세스가 속한 그룹의 PGID를 반환
		printf("PGRP of child = %d\n", getpgrp());

		// kill 신호를 받을 때까지 1초마다 대기
		while(1) {
			printf("Child is waiting...\n");
			sleep(1);
		}
		// 자식은 무한 루프 돌게 됨.
	} else if (pid > 0) {
		// --- 부모 프로세스 ---
		printf("PGRP of parent = %d\n", getpgrp());

		// 자식이 PGID를 출력하고 무한 루프에 들어갈 시간을 줌.
		sleep(5);

		// kill(-getpid(), 9)를 해보게 된다면?
		// -getpid(): 나(부모)의 PGID와 같은 그룹 전체에 9번 시그널을 보냄
		// 	      실행시 부모 자신도 종료됨.

		// pid(양수): 자식 프로세스(pid) '하나'에게 9번(SIGKILL) 시그널을 보냄
		kill(pid, 9);

		// wait() 호출하여 자식의 종료 상태를 수거하여 좀비 프로세스로 만들지 않음
		// status: 자식의 종료 코드 저장
		child = wait(&status);

		printf("[%d] Child %d is terminated \n", getpid(), pid);
		
		// status >> 8 또는 WEXITSTATUS(status)은
		// 자식 프로세스가 exit()를 통해 정상적으로 종료했을 때만
		// 그 종료 코드를 읽는 올바른 방법이다.
		//
		// 하지만, 현재 자식 프로세스는 exit()로 정상 종료한 것이 아니라,
		// 시그널 9번(SIGKILL)에 의해 비정상(강제) 종료되었다.
		// 이 경우 status 변수에는 exit 코드(2)가 아니라 시그널 번호(9)가
		// 다른 방식으로 저장된다.
		printf("\t... with status %d\n", status >> 8);

		printf("Parent is sleeping...\n");
		sleep(5);
	} else {
		perror("fork failed");
		exit(1);
	}

	return 0;
}
