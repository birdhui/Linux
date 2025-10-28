#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* 자식 프로세스를 생성하여 echo 명령어를 실행한다. */
int main() {
	printf("Parent process\n");

	if (fork() == 0) {
		printf("[%d] Child process \n", getpid());
		execl("/bin/echo", "echo", "hello", NULL);
	
		// execl이 실패했을 경우 실행됨
		// perror(): print error
		perror("execl failed");
		printf("End of child process \n");
		
		// 부모가 어차피 종료 코드를 무시하는데,
		// 자식이 exit(1)을 호출할 필요가 있음?
		// 이유: 만약 execl이 실패했을 경우 자식 프로세스는 죽지 않고
		// 	 계속 실행됨.
		// 	 비상 정지 버튼을 만들어 놓는 거임
		exit(1);
	} else {
// wait(&status): "자식이 끝날 때까지 기다리고, 끝나면 status 변수에 종료 코드를 저장해주세요."
// wait(NULL): "자식이 끝날 때까지 기다리고, 종료 코드는 저장할 필요 없이 그냥 버려주세요."
		wait(NULL); // 동기화
		printf("End of parent process \n");
	}
	return 0;
}
