// 동기화로 datarace 제거하기
// 문제점: 직렬화라서 속도가 매우 느리고 비효율적임
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int acc = 0;
pthread_mutex_t mtx;

void *TaskCode(void *argument) {
	int tid = *((int*)argument);

	for(int i=0; i<10000000; i++) {
		pthread_mutex_lock(&mtx);
		acc=acc+1;
		pthread_mutex_unlock(&mtx);
	}
	return NULL;
}


int main(int argc, char *argv[]) {
	pthread_t threads[4];
	int args[4];
	int i;

	pthread_mutex_init(&mtx, NULL);

	for(i=0; i<4; ++i) {
		args[i] = i;
		pthread_create(&threads[i], NULL, TaskCode, (void *) &args[i]);
	}

	for(i=0;i<4; ++i){
		pthread_join(threads[i], NULL);
	}

	printf("직렬 동기화 후 acc: %d\n", acc);	
	return 0;

}
