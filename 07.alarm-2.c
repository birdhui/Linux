#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void alarm_Handler();

int main() {
	signal(SIGALRM, alarm_Handler);

	alarm(5);

	printf("루프 시작... \n");

	while(1) {
		sleep(1);
		printf("1 sec... \n");
	}

	printf("End of main \n");
}

void alarm_Handler(int signo) {
	printf("SIGALRM\n");
	printf("Wake Up!!!\n");
	exit(0);
}

