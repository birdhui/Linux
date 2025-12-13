// gcc할때 '-lpthread' 뒤에 붙여야 함.
// datarace를 막기 위해 Mutex lock in Pthread 추가함.
// 현재 문제점: "시간종속적"이다. (코드를 실행할 때마다 값이 달라짐)
// Mutex를 사용하더라도 1개의 스레드가 임계구역(탈의실)에 들어가 있으면
// 나머지 3개의 스레드는 하염없이 기다리기만 하기 때문에
// 병렬적인 프로그래밍이 아니기에 병렬프로그래밍이 필요하다.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int acc = 0;
pthread_mutex_t mtx;

void *TaskCode(void *argument) {
	
	int tid;
	tid = *((int*) argument);
	int partial_acc = 0;	

	for (int i = 0; i < 10000000; i++) {

		partial_acc =  partial_acc + 1;	
		// acc = acc + 1;
	
	}

	pthread_mutex_lock(&mtx);
	acc += partial_acc;
	pthread_mutex_unlock(&mtx);

	return NULL;
}

int main (int argc, char *argv[]) {
	pthread_t threads[4];
	int args[4];
	int i;

	pthread_mutex_init(&mtx, NULL);

	/* create all threads */
	for (i = 0; i < 4; ++i) {
		args[i] = i;
		pthread_create(&threads[i], NULL, TaskCode, (void *) & args[i]);
	}

	/* wait for all threads to complete */
	for (i = 0; i < 4; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("병렬 동기화 후 acc: %d\n", acc);
	return 0;
}
