#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLINE 100

int main() {

	int n, length, fd[2];
	pid_t pid;
	char message[MAXLINE], line[MAXLINE];

	// 파이프 생성
	// fd[0]: 읽기용
	// fd[1]: 쓰기용

	/*
	 * pipe(fd): fork()를 하기 전 pipe(fd)가 호출되어
	 * 	     통신 통로(파이프)가 만들어짐.
	 * 	     부모는 이 통로의 양쪽 끝(fd[0]-읽기, fd[1]-쓰기)을 알고 있음
	 * */
	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	// 자식은 부모가 알고 있는 파이프 통로의 양쪽 끝 정보도 그대로 상속됨.
	pid = fork();
	
	if (pid == 0) {
		// --- 자식 프로세스 ---
		// 자식은 쓰기만 할 것이므로 읽기용 fd를 닫음
		close(fd[0]);

		// 메시지 작성
		sprintf(message, "Hello from PID %d\n", getpid());
		// 문자열의 길이(strlen)에 1을 더함
		// + 1: 문자열의 끝을 알리는 NULL(\0)까지 함께 보내기 위함.
		// C언어에서 문자열은 \0으로 끝나야 완전한 문자열로 인식됨.
		length = strlen(message) + 1;
		
		printf("자식: 파이프에 메시지를 작성합니다.\n");

		// 데이터에 온전히 다 보내졌는지 확인하는 오류 처리 코드
		// write()가 반환한 값(실제로 쓴 바이트 수)과
		// 우리가 원래 보내려고 했던 바이트 수(length)를 비교
		//
		// 같이 않다면, 오류 처리
		if (write(fd[1], message, length) != length) {
			perror("child write error");

			// 쓰기 실패 시 쓰기용 fd 닫고 종료
			close(fd[1]);
			exit(1);
		}

		// 쓰기가 끝나면 쓰기용 fd[1]을 닫음
		// 부모에게 "더 이상 보낼 데이터 없음(EOF:End Of File)" 신호를 보냄
		close(fd[1]);
		printf("자식: 쓰기 완료 후 종료합니다.\n");
		exit(0);

	} else if (pid > 0) {
		// --- 부모 프로세스 ---
		// 부모는 읽기만 할 것이므로 쓰기용 fd를 닫음
		close(fd[1]);
		
		printf("부모: 파이프로부터 메시지를 기다립니다.\n");
		// 파이프로부터 메시지 읽기 (데이터가 올 때까지 기다림)
		// read()는 파이프로부터 메시지를 성공적으로 읽어
		// line 변수에 저장하고,
		// 읽은 바이트 수(n)를 반환한다.
		n = read(fd[0], line, MAXLINE);

		if (n < 0) {
			perror("parent read error");
			
			// 읽기 실패 시 읽기용 fd 닫고 종료
			close(fd[0]);
			exit(1);
		} else if (n == 0) {
			// read()가 0을 반환하면 파이프의 쓰기 쪽이 닫혔다는 의미(EOF)
			printf("부모: 파이프에서 EOF를 받았습니다. (자식이 쓰기 fd[1]를 닫음)\n");
		} else {
			// 읽은 메시지 출력
			printf("부모: 메시지를 받았습니다.\n");
			printf("[%d] 부모가 받은 메시지: %s", getpid(), line);
		}

		//읽기가 끝나면 읽기용 fd도 닫음
		close(fd[0]);

		// 자식 프로세스가 완전히 종료될 때까지 기다림
		// 좀비 프로세스 방지
		wait(NULL);
		printf("부모: 자식 프로세스 종료 확인 후 종료합니다.\n");

	} else {
		perror("fork failed");
		// fork 실패 시 생성했던 파이프 fd도 닫아주기
		close(fd[0]);
		close(fd[1]);
		exit(1);
	}

	// 부모만 이 줄에 도달하여 정상 종료
	return 0;
}
