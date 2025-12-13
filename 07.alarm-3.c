#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define TIMEOUT_SEC 10
#define PASS "123"

void alarm_handler(int signo) {
	// SIGALRM 발생시 Fire! 출력
	if (signo == SIGALRM) {
		printf("\nFire!");
		exit(0);
	}
}

void int_handler(int signo) {
	if (signo == SIGINT) {
		// 기존 알람 취소 후 10초로 리셋
		alarm(TIMEOUT_SEC);
		printf("\n10 sec reset...\n");
	}
}

int main() {
	char input[256];

	signal(SIGALRM, alarm_handler);
	signal(SIGINT, int_handler);

	printf("비밀번호를 %d초 내에 입력하세요.\n", TIMEOUT_SEC);
	while(1) {
		// 타이머 시작
		alarm(TIMEOUT_SEC);

		printf("> ");

		if (fgets(input, sizeof(input), stdin) == NULL) {
			continue;
		}

		input[strcspn(input, "\n")] = 0;

		// 입력 성공시 타이머 취소
		alarm(0);

		// 비밀번호 확인
		if (strcmp(input, PASS) == 0) {
			printf("Correct!\n");
			break; // 비밀번호 맞췄다면 루프 탈출 및 프로그램 종료
		} else {
			printf("InCorrect.\n");
			// 틀렸을 경우 루프 재시작
		}
	}

	return 0;
}
