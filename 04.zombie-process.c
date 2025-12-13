#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * 좀비 프로세스 예제:
 * 자식 프로세스가 먼저 종료되고, 부모는 wait()를 호출하지 않아
 * 좀비 상태가 되는 코드
 * */
int main() {
	pid_t pid = fork();

	if (pid == 0) {
		// --- 자식 프로세스 ---
		printf("[%d] 자식 프로세스입니다. 1초 후에 종료합니다.\n", getpid());
		exit(0); // 자식은 1초도 안되서 바로 종료됨
	} else if (pid > 0) {
		// --- 부모 프로세스 ---
		printf("[%d] 부모 프로세스입니다. 자식[%d]을 생성했습니다.\n", getpid(), pid);

		// wait() 호출 X
		// 부모는 자식 수거를 하지 않고 10초간 잠이 듦
		printf("부모: 20초 동안 잠이 듭니다.\n");
		printf("이 시간 동안 자식[%d]은 '좀비' 상태입니다.\n", pid);
		printf("다른 터미널을 열고 [ps -ef | grep %d] 명령어로 확인해보세요.\n", pid);

		sleep(20);

		// 20초 후 부모가 wait()를 호출하며 좀비 수거.
		printf("부모: 20초가 지나 잠에서 깼습니다. 이제 wait()를 호출하여 좀비 프로세스를 수거합니다. \n");
		wait(NULL); // 여기서 좀비 프로세스 정리됨.
		printf("부모: 좀비 수거 완료. 3초 후 종료합니다. \n");

		sleep(3);
	} else {
		perror("fork failed");
		exit(1);
	}

	printf("부모: 프로그램 종료. \n");

	return 0;
}
