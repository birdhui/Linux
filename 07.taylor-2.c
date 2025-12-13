// pthread로 병렬 수행 테일러 급수
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define N 4
#define NTHREADS 4 // 스레드 개수

typedef struct {
	int start;
	int end;
	int terms;
	double *x;
	double *result;
	
} ThreadArgcc

void *sinx_taylor_thread(void *arg) {
	ThreadArg *targ = (ThreadArg *)arg;

	for (int i=targ->start; i<targ->end; i++) {
		double value = targ->x[i];
		double numer = targ->x[i] * targ->x[i] * targ->x[i];
		double denom = 6.; // 3!
		int sign = -1;

		for (int j=1; j<=targ->terms; j++) {
			value += (double) sign * numer/denom;
			numer *= targ -> x[i] * targ -> x[i];
			denom *= (2. * (double)j+2.) * (2. * (double)j+3.);
			sign *= -1;
		}

		targ->result[i] = value;
	}
	return NULL;
}

int main() {
	double x[N] = {0, M_PI/6., M_PI/3., 0.134};
	double res[N];
	int terms = 3;

	pthread_t threads[NTHREADS];
	ThreadArg args[NTHREADS];

	int chunk = N / NTHREADS;
	int start = 0;

	//스레드 생성: 각 스레드에 서로 다른 index 구간과 공유 배열 포인터 전달
	for(int t=0; t<NTHREADS; t++) {
		args[t].start = start;
		args[t].end = (t==NTHREADS -1) ? N:start+chunk;
		args[t].terms = terms;
		args[t].x = x;
		args[t].result = res;

		pthread_create(&threads[t], NULL, sinx_taylor_thread, &args[t]);
		start = args[t].end;
	}

	// 모든 스레드가 계산 끝날 때까지 대기
	for(int t=0; t<NTHREADS; t++) {
		pthread_join(threads[t], NULL);
	}

	// 결과 출력
	for(int i=0; i<N; i++) {
		printf("sin(%.2f) by Taylor series = %f\n", x[i], res[i]);
		printf("sin(%.2f) = %f\n", x[i], sin(x[i]));
	}

	return 0;
}
