#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* echo 명령어를 실행한다. */
int main() {
	printf("--> Before exec()\n");
	
	// exel(): "현재 실행중인 프로세스를 완전히 새로운 프로그램으로 덮어쓰는" 역할
	// 	   fork()가 '프로세스를 복제(분신술)'하는 것/execl()은 '프로세스에 빙의'하는 것
	
	// 아래의 코드를 통해
	// 운영체제는 현재 실행 중인 이 프로그램(컴파일된 a.out)의 메모리, 코드, 스택을 모두 지움.
	// 그리고 첫 번째 인자인 /bin/echo 프로그램을 통째로 load함.
	// 그 뒤, echo 프로그램에게 "echo", "hello", NULL을 인자로 전달하여 실행시킴.
	// 이것은 터미널에서 echo hello라고 친 것과 같음.
	// 결과: echo 프로그램이 실행되어 터미널에 hello를 출력.
	// echo 프로그램이 실행을 마치면, 프로세스는 그대로 종료됨.
	execl("/bin/echo", "echo", "hello", NULL);
	
	// execl() 함수가 성공적으로 실행되었다면(즉, /bin/echo/를 찾는 데 성공했다면),
	// 이 프로그램의 코드는 이미 echo 프로그램으로 덮어 쓰여 사라진 상태임.
	// 따라서, 아래의 printf문은 절대로 실행되지 않음.
	printf("--> After exec()\n");
	return 0;
}
