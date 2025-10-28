#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int status;
	pid_t pid;

	// pid=fork() = switch (ex.pid=1235)로 실행됨
	switch (pid = fork()) {
		case -1 : /* fork failed */
			perror("fork");
			exit(1);
			break;
		case 0 : /* child process */
			printf("--> Child Process\n");
			exit(2); // exit(): 프로세스 종료후 -> 상태값(status - 2)를 부모 프로세스에 전달
			break; // exit() 때문에 영원히 실행되지 않음
		default : /* parent process */
			wait(&status);
			// while (wait(&status) != pid)
			//  wait 함수 자체가 기다리는 함수이기 때문에, while 빼도 같은 결과값이 나옴
			//:	continue;
			printf("--> Parent process\n");
			printf("Status: %d, %x\n", status, status); // 10진수, 16진수
			printf("Child process Exit Status: %d\n", status >> 8);
			break;
	}
}
