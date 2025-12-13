#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define MEM_SIZE (100 * 1024 * 1024)
#define PAGE_SIZE 4096

long get_minor_page_faults() {
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return usage.ru_minflt;
}

int main() {
	char *data = malloc(MEM_SIZE);
	memset(data, 1, MEM_SIZE);
	pid_t pid = fork();

	// child
	if (pid == 0) {
		long pt_before = get_minor_page_faults();
		volatile char tmp;
		
		printf("1. Minor Page Faults 읽기 전: %ld\n", pt_before);

		// 1차: 페이지 읽기 (CoW 발생 안 함)
		for(int i=0; i<MEM_SIZE; i+=PAGE_SIZE)
			tmp = data[i];

		long pt_after_read = get_minor_page_faults();

		printf("2. Minor Page Faults 읽은 후: %ld\n", pt_after_read);

		// 2차: 페이지 쓰기 (CoW 발생 시점)
		for(int i=0; i<MEM_SIZE; i+=PAGE_SIZE)
			data[i] = 2;

		long pt_after_write = get_minor_page_faults();

		// CoW로 인한 페이지 복사/폴트 관찰
		printf("3. Minor Page Faults 쓴 후: %ld\n", pt_after_write);

		free(data);
		exit(0);
	} else {
		wait(NULL);
	}
	free(data);

	return 0;
}
