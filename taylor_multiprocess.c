/*
 * 자식 프로세스를 여러 개 생성하여
 * 테일러 급수 프로그램을 병렬로 수행
 *
 * - fork()와 wait() 사용
 * - 데이터 전달을 위해 파이프 사용
 * 자식의 결과값을 부모에 전달
 */

#include <stdio.h>
#include <stdlib.h>     // exit(), atof()
#include <string.h>     // strlen()
#include <unistd.h>     // fork(), pipe(), write(), read(), close()
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait()
#define _USE_MATH_DEFINES
#include <math.h>       // M_PI, sin()

#define N 4             // 병렬로 처리할 작업의 개수 (프로세스 개수)
#define MAXLINE 100

/*
 * 테일러 급수 계산 함수
 */
void sinx_taylor_parallel(int num_elements, int terms, double* x, double* result) {
    
    int fd[2 * N];        // N개의 파이프를 위한 파일 디스크립터 배열 (N*2)
    pid_t pids[N];        // 생성된 N개의 자식 프로세스 ID를 저장할 배열
    char message[MAXLINE];  // 파이프로 주고받을 메시지 버퍼

    // 자식 프로세스 생성
    for (int i = 0; i < num_elements; i++) {
        
        // (1) i번째 자식과 통신할 파이프 생성
        //     fd[2*i]   : 읽기용 (부모가 사용)
        //     fd[2*i+1] : 쓰기용 (자식이 사용)
        if (pipe(fd + 2 * i) < 0) {
            perror("pipe");
            exit(1);
        }

        // (2) 자식 프로세스 생성
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }

        // (3) 자식 프로세스가 수행할 작업
        if (pids[i] == 0) { // 나 = 자식 프로세스
            
            close(fd[2 * i]);

            // 테일러 급수 계산 (i번째 작업만 수행)
            double value = x[i];
            double numer = x[i] * x[i] * x[i];
            double denom = 6.; // 3!
            int sign = -1;

            for (int j = 1; j <= terms; j++) {
                value += (double)sign * numer / denom;
                numer *= x[i] * x[i];
                denom *= (2. * (double)j + 2.) * (2. * (double)j + 3.);
                sign *= -1;
            }

            // (4) 계산 결과를 문자열로 변환하여 부모에게 파이프로 전송
            sprintf(message, "%lf", value);
            write(fd[2 * i + 1], message, strlen(message) + 1);

            close(fd[2 * i + 1]);
            
            // (5) 자식 프로세스 작업 완료 후 종료
            exit(0); 
        
        } else {
            close(fd[2 * i + 1]);
        }
    }

    // 프로세스 동기화
    for (int i = 0; i < num_elements; i++) {
        int status;
        wait(&status);
    }

    for (int i = 0; i < num_elements; i++) {
        char line[MAXLINE];
        int n = read(fd[2 * i], line, MAXLINE);
        
        // atof: 문자열(string)을 double로 변환
        result[i] = atof(line); 
        
        close(fd[2 * i]);
    }
}

int main() {
    double x[N] = { 0, M_PI / 6., M_PI / 3., 0.134 };
    double res[N];

    // 병렬 함수 호출
    sinx_taylor_parallel(N, 3, x, res);

    printf("병렬 계산 결과\n");
    for (int i = 0; i < N; i++) {
        printf("sin(%.2f) by Taylor = %f\n", x[i], res[i]);
        printf("sin(%.2f) by sin() = %f\n\n", x[i], sin(x[i]));
    }

    return 0;
}

