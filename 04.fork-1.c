#include <stdio.h>
#include <unistd.h>

int main() {
	int pid;

	printf("---- fork() 이전 ----\n");
	printf("[%d] 현재 프로세스 ID: %d\n", getpid(), getpid());
	printf("[%d] 자식 ID\n[%d] 부모 ID\n", getpid(), getppid());

	pid = fork();

	printf("---- fork() 이후 ----\n");
	printf("[%d] 자식 ID: %d\n", getpid(), pid); // 자식 프로세스 반환값: 0
	printf("[%d] 부모 ID: %d\n", getppid(), pid); // 부모 프로세스 반환값: 자식프로세스 ID

}
