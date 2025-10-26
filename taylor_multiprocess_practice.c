/* 자식 프로세스를 여러 개 생성하여
 * 테일러 급수 프로그램을 병렬로 수행 
 *
 * - fork()와 wait() 사용
 * - 데이터 전달을 위해 파이프 사용
 *   자식의 결과값을 부모에 전달  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define N 4
#define MAXLINE 100

// 주석 처리 벗기기
void sinx_taylor(int num_elements, int terms, double* x, double* result) {
	
	// int child_id, pid;
	int fd[2*N];	// 읽기 쓰기를 N개만큼 생성
	int length;
	char message[MAXLINE], line[MAXLINE];
	for(int i=0; i<num_elements; i++) {
		// child_id = i;
		pipe(fd + 2* i);
		// pid = fork();
		// break;
	// }

		if (pid == 0) {	// child
		// int i = cild_id;
		
		close(fd[2*i]);
		
		double value = x[i];
		double numer = x[i] * x[i] * x[i];
		double denom = 6.; // 3!
		int sign = -1;

		for (int j=1; j<=terms; j++) {
			value += (double)sign * numer / denom;
			numer *= x[i] * x[i];
			denom *= (2.*(double)j+2.) * (2.*(double)j+3);
			sign *= -1;
		}
		result[i] = value;
		sprintf(message, "%lf", result[i]);
		length = strlen(message)+1;
		write(fd[2*i+1], message, length); 
		exit(0);

	} else {	// parent
		close(fd[2*i+1]);

		int n = read(fd[2*i], line, MAXLINE);
		result[i] = atof(line);	// string을 int로 변환해주는 함수
	}	// 기존 코드에서 else 문만 벗기고 안에 코드는 그대로 두기 (else 코드 삭제하고 밑에 코드로)
	// parent
	for(int i=0; i<num_elements; i++) {

	int status;
	wait(&status);

	int child_id = status >> 8;
	int n = read(fd[2*i], line, MAXLINE);
	result[child_id] = atof(line);	// string을 int로 변환해주는 함수
	}
}

int main() {
	
	double x[N] = { 0, M_PI/6., M_PI/3., 0.134 };
	double res[N];

	sinx_taylor(N, 3, x, res);
	for (int i = 0; i < N; i++) {
		printf("sin(%.2f) by Taylor series = %f\n", x[i], res[i]);
		printf("sin(%2.f) = %f\n", x[i], sin(x[i]));
	}



	return 0;
}
