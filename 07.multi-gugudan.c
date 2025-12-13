#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child_pid = -1;
int current_dan = 0;
#define MAX_DAN 20

void child_handler(int signo) {
	printf("[자식 PID %d] SIGINT 수신\n", getpid());

	if (current_dan <= 9) {
		printf("[자식] 구구단 실행 중이니 종료 불가 (현재 %d단)\n", current_dan);
	} else {
		printf("[자식] 9단 이상 출력 완료 (%d단). 부모와 자신을 SIGKILL\n", current_dan);

		if (kill(getppid(), SIGKILL) == -1) {
			perror("부모에게 SIGKILL 전달 실패");
		}
		exit(0);
	}
}

void gugudan() {
	signal(SIGINT, child_handler);

	for(current_dan = 1; current_dan <= MAX_DAN; current_dan++) {
		printf("\n--- %d 단 ---\n", current_dan);

		for(int i = 1; i <= 9; i++) {
			printf("%d x %d = %d\n", current_dan, i, current_dan * i);
		}
		//1단이 끝날 때마다 sleep(1)
		sleep(1);
	}
	printf("\n[자식] 모든 구구단 %d단 출력 완료. 정상 종료.\n", MAX_DAN);
	exit(0);
}

void parent_handler(int signo) {
	printf("\n[부모 PID %d] SIGINT 수신\n", getpid());

	if (child_pid != -1) {
		// 자식 프로세스에 동일한 시그널(sigint) 전달
		printf("[부모] 자식 PID %d에게 SIGINT 전달\n", child_pid);

		if (kill(child_pid, SIGINT) == -1) {
				perror("자식에게 SIGINT 전달 실패");
		}
	}
}

int main() {
	printf("[부모] 부모 PID: %d\n", getpid());

	signal(SIGINT, parent_handler);

	child_pid = fork();

	if (child_pid == -1) {
		// fork 실패
		perror("fork 실패");
		exit(1);
	} else if (child_pid == 0) {
		// 자식 프로세스
		gugudan();
	} else {
		// 부모 프로세스
		printf("[부모] 자식 PID: %d\n", child_pid);

		int status;
		waitpid(child_pid, &status, 0);

		printf("[부모] 자식 프로세스 종료 확인. 부모도 종료\n");
		exit(0);
	}

	return 0;
}
