#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// 모든 쓰레드가 공유하는 전역 변수
int S[100], A[100], B[100];

void *TaskCode(void *argument) {
	
	int tid;
	tid = *((int*) argument);

	// 각 쓰레드는 자신의 ID에 맞는 25개 구간의 덧셈을 수행
	for (int i = tid * 25; i < (tid+1) * 25; i++) {
		S[i] = A[i] + B[i];
	}

	return NULL;
}

// for문으로 A[100]; B[100]; 초기화하는 코드 넣어주기

int main (int argc, char *argv[]) {
	pthread_t threads[4];
	int args[4];
	int i;

	/* 쓰레드 생성 */
	printf("Creating 4 threads...\n");
	for (i = 0; i < 4; ++i) {
		args[i] = i;	// 각 쓰레드에 0, 1, 2, 3 ID 전달
		pthread_create(&threads[i], NULL, TaskCode, (void *) & args[i]);
	}

	/* 모든 쓰레드가 끝날 때까지 대기 */
	for (i = 0; i < 4; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("All threads finished.\n\n");

	// 결과 배열 S의 내용을 출력
	printf("Calculation Results (S[i] = A[i] + B[i])\n");
	for (i = 0; i < 100; i++) {
		// 모든 결과를 다 찍으면 길어지니까 10개마다 하나씩만 출력
		if (i % 10 == 0) {
			printf("S[%d] = %d (A[%d]=%d + B[%d]=%d)\n", i, S[i], i, A[i], i, B[i]);
		}
	}

	return 0;
}
