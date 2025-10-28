#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int status;
	pid_t pid;

	// waitpid(): wait() 함수의 "고급 버전"
	// 	      wait()가 단순히 "아무 자식이나 끝날 때까지 기다리기"라면,
	// 	      waitpid()는 "특정 자식을, 특별한 방식으로 기다리기"가 가능하다.
	//
	// 	      - 특정 자식(PID)를 골라서 기다릴 수 있음
	// 	      - option을 통해 "어떻게" 기다릴지 방식을 정할 수 있음
	//
	// 	      [option]
	// 	      - 0
	// 	      waitpid()는 wait()과 거의 똑같이 작동한다.
	// 	      즉, 부모 프로세스는 자식이 종료될 때까지 다른 일을 전혀 못하고
	// 	      그 자리에서 멈춰서 기다린다. (Bloking).
	//
	// 	      - WHOHANG
	// 	      waitpid()를 "기다리지 않는" 함수로 바꿔준다.
	// 	      자식이 종료되었으면: 자식의 PID를 반환
	// 	      자식이 아직 실행 중이면: 0을 즉시 반환 (기다리지 않음)
	// 	      오류가 나면: -1을 반환
	//
	// 	      - WUNTRACED
	// 	      waitpid()는 자식이 "종료"될 때만 반환된다.
	// 	      하지만, 이 옵션을 사용하면 자식이 종료되지 않았더라도
	// 	      "일시 정지"상태가 되었을 때도 반환된다.
	//
	// 	      - WCONTINUED
	// 	      WUNTRACED의 반대.
	// 	      "일시 정지"되었던 자식이 SIGCOUNT 신호를 받아
	// 	     다시 "실행"될 때 부모에게 알려줌
	//
	// 	     - WNOWAIT
	// 	     waitpid()와 똑같이 자식의 상태를 확인해서 반환하지만,
	// 	     자식을 수거(reap)하지 않는다.
	if ((pid = fork()) < 0) { /* fork failed */
		perror("fork");
		exit(1);
	}

	if (pid == 0) { /* child process */
		printf("--> Child process (PID: %d) starts sleeping for 3 seconds...\n", getpid());
		sleep(3);
		exit(3);
	} else {
		printf("--> Parent process (PID: %d) starts waiting...\n", getpid());
	
		// WNOHANG: 자식(pid)이 끝나지 않았으면 0을 반환하며 기다리지 않음.
		// 자식이 끝날 때까지 0이 반환되므로 루프가 돔.
		while (waitpid(pid, &status, WNOHANG) == 0) {
		printf("Parent stil wait...\n");
		sleep(1); // 1초마다 자식이 끝났는지 확인
		}
	}

	// 루프가 끝났다는 것은 waitpid가 0이 아닌 값(자식 PID)을 반환했다는 뜻.
	// 즉, 자식이 정상적으로 종료됨.

	printf("Child Exit Status %d\n", status >> 8);
}
