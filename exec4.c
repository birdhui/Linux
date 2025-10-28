#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int child, status;
	pid_t pid = fork();

	if (pid == 0) {
		// 자식 프로세스
		char *argv[3];
		argv[0] = "echo";
		argv[1] = "Bonjours";
		argv[2] = NULL;

		// execvp(const char *file, char *const argv[])
		// execvp(실행할 프로그램의 이름, 프로그램에 전달한 배열)
		execvp(argv[0], argv);

		// execp가 (빙의에) 실패했을 때 아래 코드 실행
		fprintf(stderr, "%s: Error in exec\n", argv[1]);
		exit(1);
	} else if (pid > 0) {
		// 부모 프로세스
		child = wait(&status);
		printf("[%d] Child %d is terminated \n", getpid(), pid);
		printf("\t... with status %d \n", status >> 8);
	} else {
		// fork 실패
		perror("fork");
		exit(1);
	}

	return 0;
}
