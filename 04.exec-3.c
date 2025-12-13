#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/* 자식 프로세스를 생성하여 echo 명령어를 실행한다. */
int main() {
	pid_t pid = fork();

	if (pid == -1) {
		// fork 실패한 경우
		perror("fork failed");
		exit(1);

	} else if (pid == 0) {
		// 자식 프로세스
		char *argv[3];
		argv[0] = "echo";	// 관례적으로 프로그램 이름
		argv[1] = "hello";	// 실제 인자
		argv[2] = NULL;		// 배열의 끝을 알리는 NULL

		// execl(): 경로, 프로그램, 배열, NULL 4가지 전달
		// execv(): 경로와 '배열' 2가지만 전달.
		execv("/bin/echo", argv);

		// execv 실패한 경우
		perror("exec failed");
		exit(1);
	} else {
		// 부모 프로세스
		wait(NULL);
		printf("End of parent process: child finished. \n");
	}

	return 0;
}
