#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define NUM_PROC 10
#define MAX_BURST 8

struct Process {
    int original_burst;
    int burst;
    int quantum;
    int state;  // 0:ready 1:running 2:done
    int wait_time;
};

struct Process procs[NUM_PROC];
int current_proc = 0;
int quantum_base = 0;
int tick_count = 0;
volatile int all_done = 0;

void init_procs() {
    srand(time(NULL));
    int total_burst = 0;
    
    for (int i = 0; i < NUM_PROC; i++) {
        procs[i].original_burst = (rand() % MAX_BURST) + 1;
        procs[i].burst = procs[i].original_burst;
        procs[i].quantum = 0;
        procs[i].state = 0;  // ready
        procs[i].wait_time = 0;
        total_burst += procs[i].original_burst;
    }

    printf("프로세스 초기화 완료 (총 burst: %d)\n", total_burst);
}

void timer_handler(int sig) {
    tick_count++;
    printf("\n=== TICK %d ===\n", tick_count);
    
    // 현재 프로세스 실행
    if (procs[current_proc].state == 1) {
        procs[current_proc].burst--;
        procs[current_proc].quantum--;
    
    	printf("P%d 실행 (burst:%d/%d, q:%d/%d)\n", 
               current_proc, procs[current_proc].burst, procs[current_proc].original_burst,
               procs[current_proc].quantum, quantum_base);
        
        // 종료 조건
        if (procs[current_proc].burst <= 0 || procs[current_proc].quantum <= 0) {
            if (procs[current_proc].burst <= 0) {
                procs[current_proc].state = 2;  // done
                printf("P%d 완료!\n", current_proc);
            } else {
                procs[current_proc].state = 0;  // ready
            }
            
            // 다음 ready 프로세스 찾기
            int start = current_proc;

            do {
                current_proc = (current_proc + 1) % NUM_PROC;
            } while (procs[current_proc].state == 2 && current_proc != start);
            
            //  모든 프로세스 완료 체크
            int done_count = 0;

            for (int i = 0; i < NUM_PROC; i++) {
                if (procs[i].state == 2) done_count++;
            }
            
            if (done_count == NUM_PROC) {
                all_done = 1;
                printf(" 모든 %d 프로세스 완료! (총 %d 틱)\n", NUM_PROC, tick_count);
                return;  // 타이머 중단!
            }
            
            procs[current_proc].quantum = quantum_base;
            procs[current_proc].state = 1;
            printf("스위치: P%d -> P%d\n", start, current_proc);
        }
    }
    
    // 대기시간 증가
    for (int i = 0; i < NUM_PROC; i++) {
        if (procs[i].state == 0) {
            procs[i].wait_time++;
        }
    }
    
    if (!all_done) {
        alarm(1);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("사용법: %s <quantum> (예: %s 1)\n", argv[0], argv[0]);
        
	return 1;
    }
    
    quantum_base = atoi(argv[1]);
    printf("=== 라운드 로빈 스케줄러 (Q=%d) ===\n", quantum_base);
    
    init_procs();
    signal(SIGALRM, timer_handler);
    
    // 스케줄링 시작
    procs[0].quantum = quantum_base;
    procs[0].state = 1;
    alarm(1);
    
    // 완료까지 대기 (강제 종료 방지)
    while (!all_done) {
        pause();
    }
    
    // 성능 분석
    printf("\n===  성능 분석 결과 ===\n");
    printf("| P  | 초기버스트 | 대기시간 |\n");
    printf("|----|------------|---------|\n");
    
    double total_wait = 0, total_turnaround = 0;
    for (int i = 0; i < NUM_PROC; i++) {
        int turnaround = procs[i].original_burst + procs[i].wait_time;
        printf("| P%-1d | %-8d | %-7d |\n", i, procs[i].original_burst, procs[i].wait_time);
        total_wait += procs[i].wait_time;
        total_turnaround += turnaround;
    }
    
    printf("|----|------------|---------|\n");
    printf("| **평균** | **%.1f** | **%.1f** |\n", 
           total_wait/NUM_PROC, total_turnaround/NUM_PROC);
    printf("\n분석: Q=%d에서 평균 대기시간 %.1f \n", 
           quantum_base, total_wait/NUM_PROC);
    
    return 0;
}

