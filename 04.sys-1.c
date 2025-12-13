#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {

	int status;

	// C코드: system("date") 호출
	// 쉘: 쉘이 date라는 명령어를 받아서 실행
	// 결과: date는 시스템에 존재하는 유효한 명령어이므로,
	// 	 쉘이 이 프로그램을 성공적으로 실행시켰고,
	// 	 date 프로그램은 현재 날짜와 시간을 출력함.
	if ((status = system("date")) < 0) {
		perror("system() error");
		printf("Return code = %d\n", WEXITSTATUS(status));
	}

	// C코드: system("hello") 호출
	// 쉘: 쉘이 hello라는 명령어를 받아서 실행하려고 시도
	// 결과: 쉘이 PATH 환경 변수에 등록된 모든 폴더를 뒤져봤지만,
	// 	 hello라는 이름의 실행 파일을 찾지 못함.
	// 	 sh: 1: hello: not found
	// 	 -> C코드가 출력한 게 아니라,
	// 	    쉘이 "hello라는 명령어를 찾을 수 없습니다(not found)."
	// 	    라고 에러 메시지를 직접 출력한 것
	if ((status = system("hello")) < 0) {
		perror("system() error");
		printf("Return code = %d\n", WEXITSTATUS(status));
	}

	// C코드: system("who; exit 44") 호출
	// 쉘: who를 실행하고; 44를 반환하라
	// 결과: who에 해당하는 사용자 목록을 출력 후 44(종료코드)를 전달.
	if ((status = system("who; exit 44")) < 0) {
		perror("system() error");
		printf("Return code = %d\n", WEXITSTATUS(status));
	}

	return 0;
}
