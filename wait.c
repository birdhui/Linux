#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int status;
	pid_t pid;

	switch (pid = fork()) {
		case -1 : /* fork failed */
			perror("fork");
			exit(1);
			break;
		case 0 : /* child process */
			printf("--> Child Process\n");
			exit(2);
			break;
		default : /* parent process */
			// wait(&status);
			//while (wait(&status) != pid)
			//  wait 함수 자체가 기다리는 함수이기 때문에, while 빼도 같은 결과값이 나옴
			//:	continue;
			printf("--> Parent process\n");
			printf("Status: %d, %x\n", status, status);
			printf("Child process Exit Status: %d\n", status >> 8);
			break;
	}
}
