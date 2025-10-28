#include <sys/types.h> /* system.c */
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int my_system(const char *cmdstring) {

	int status;
	pid_t pid;

	// 명령어가 NULL인 경우	
	if (cmdstring == NULL) 
		return 1;

	if ((pid = fork()) < 0) {
		status = -1;	// 프로세스 생성 실패
	} else if (pid == 0) {	// 자식
		// "-c": 명령어 문자열을 인자로 받는다는 뜻
		execl("/bin/sh", "sh", "-c", cmdstring, (char *) 0);

		// execl이 실패한 경우에만 아래 코드 실행
		_exit(127);	// 쉘이 명령어를 못 찾으면 127을 반환하는 관례
	} else {
		// 자식이 끝날 때까지 기다림
		while (waitpid(pid, &status, 0) < 0)
			// EINTR: 다른 시그널에 의해 wait이 중단된 경우, 다시 wait
			if (errno != EINTR) {
				status = -1;	// waitpid() 자체에서 오류 발생
				break;
			}
	}

	return status; // 자식의 raw status 값을 반환
}
