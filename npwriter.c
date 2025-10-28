#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLINE 100

int main() {

	int fd, length, i;
	char message[MAXLINE];

	// 보낼 메시지 생성 ("Hello from PID [자신의 PID]")
	sprintf(message, "Hello from PID [%d]", getpid());
	length = strlen(message) + 1;	// 널 문자 포함 길이

	printf("Wirter: Waiting for reader to open the pipe...\n");
	
	// Reader가 파이프를 열 때까지 기다리며 쓰기용으로 열기 시도
	do {
		// "myPipe"를 쓰기 전용(O_WRONLY)으로 열기 시도
		fd = open("myPipe", O_WRONLY);

		// open 실패 시 (-1 반환), 1초 대기 후 재시도
		if (fd == -1) {

			// Reader가 아직 mkfifo()를 실행 안했거나, open()을 안했을 수 있음
			sleep(1);
		}
		
	// fd가 -1인 동안 (open 실패한 동안) 계속 반복
	} while (fd == -1);

	printf("Writer: Reader connected. Sending messages...\n");
	

	// 메시지를 3번 반복해서 파이프에 쓰기
	for (i = 0; i < 3; i++) {
		printf("Writer: Sending message %d...\n", i+1);
		
		if (write(fd, message, length) != length) {
			perror("write error");
			close(fd); // 오류 시 fd 닫기
			exit(1);
		}
		// 3초 대기
		sleep(3);
	}

	// 쓰기가 끝나면 파이프 닫기 (EOF 신호 전달)
	printf("Writer: Closing pipe. \n");

	close(fd);
	return 0;
}
