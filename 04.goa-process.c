#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
 * 고아 프로세스 예제:
 * 부모 프로세스가 먼저 종료되어,
 * 자식이 '고아'가 되고
 * 'init' 프로세스(PID 1)에 입양되는 코드
 * */
int main() {
	pid_t pid = fork();

	if (pid == 0) {
		// --- 자식 프로세스 ---
		printf("[%d] 자식 프로세스입니다. 현재 저의 부모는 [%d]입니다.\n", getpid(), getppid());

		// 자식은 10초 동안 잠이 듭니다.
		// 이 시간 동안 부모는 종료됩니다..
		printf("자식: 10초간 잠이 듭니다. 그동안 부모 프로세스는 종료됩니다.\n");
		sleep(10);

		// 잠에서 깬 후, 다시 부모 PID를 확인합니다.
		printf("자식: 잠에서 깼습니다.\n");
		printf("자식: 저의 새 부모는 [%d](init 프로세스)입니다.\n", getppid());
		printf("자식: 고아가 되었지만, 5초 더 일하다가 종료합니다.\n");

		sleep(5);
		printf("자식: 종료.\n");
		exit(0);
	} else if (pid > 0) {
		// --- 부모 프로세스 ---
		printf("[%d] 부모 프로세스입니다. 자식을 [%d] 만들었습니다.\n", getpid(), pid);

		// wait() 없이 2초 만에 바로 종료됨
		printf("부모: 2초 후에 자식을 기다리지 않고 먼저 종료합니다.\n");
		
		sleep(2);
		printf("부모: 종료.\n");
		exit(0); // 부모가 자식보다 훨씬 먼저 종료됨. (dead)
	} else {
		perror("fork failed");
		exit(1);
	}

	// 부모/자식이 모두 exit()하므로 이 코드는 실행되지 않습니다.
	return 0;
}
