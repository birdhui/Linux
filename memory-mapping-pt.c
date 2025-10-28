#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int fd;
	void *addr;
	struct stat statbuf;
	pid_t pid;

	// 명령줄 인자 확인 (파일 이름이 주어졌는지)
	if (argc != 2) {
		fprintf(stderr,
				"Usage: %s filename\n", argv[0]);
		exit(1);
	}

	// 파일 정보 가져오기
	if (stat(argv[1], &statbuf) == -1) {
		perror("stat");
		exit(1);
	}

	// 파일 읽고 쓰기 모드로 열기
	if ((fd = open(argv[1], O_RDWR)) == -1) {
		perror("open");
		exit(1);
	}

	// 파일을 메모리에 매핑하기
	addr = mmap(NULL, statbuf.st_size,	// 파일 크기만큼 매핑
			PROT_READ|PROT_WRITE,	// 읽고 쓰기 가능하게
			MAP_SHARED,		// 변경 사항 공유 (IPC 핵심)
			fd,			// 열린 파일 디스크립터
			(off_t)0);		// 파일 시작부터 매핑

	// mmap 실패 시 오류 처리
	if (addr == MAP_FAILED) {
		perror("mmap");
		close(fd); // 열었던 파일 디스크립터 닫기
		exit(1);
	}
	close(fd);

	// ---- fork() 및 공유 메모리 접근 코드 시작 ----
	
	pid = fork();

	/*
	printf("%s", addr);

	if (munmap(addr, statbuf.st_size) == -1) {
		perror("munmap");
		exit(1);
	}
	*/

	switch (pid) {
		case -1:
			perror("fork");
			munmap(addr, statbuf.st_size);
			exit(1);
		case 0:
			// 자식 프로세스
			printf("1. Child Process (PID %d) sees: %s", getpid(), (char *)addr);
			sleep(1); // 부모가 다음 단계로 가기 전에 잠시 대기
			
			// 공유 메모리의 첫 번째 글자를 'x'로 변경
			((char *)addr)[0] = 'x';
			printf("2. Child Process modified. Sees: %s", (char *)addr);
			sleep(2); // 부모가 수정 후 읽을 시간을 줌
			
			printf("3. Child Process sees parent's change?: %s", (char *)addr);

			// 자식 프로세스는 여기서 자신의 역할을 마치고 종료해야 함
			// 아래 munmap은 부모만 하도록 한다.
			exit(0);

		default:
			// 부모 프로세스
			printf("1. Parent process (PID: %d) sees: %s", getpid(), (char *)addr);
			sleep(2); // 자식이 수정 후 읽을 시간을 줌
			
			printf("2. Parent process sees child's change: %s", (char *)addr);
			// 공유 메모리의 두 번째 글자를 'y'로 변경
			((char *)addr)[1] = 'y';
			printf("3. Parent process modified. Sees: %s", (char *)addr);

			// 자식 프로세스가 종료될 때까지 기다림
			wait(NULL);
			break;
	}
	// ---- fork 및 공유 메모리 접근 코드 끝 ----
	
	// 부모 프로세스만 이 아래 코드 실행 (자식은 위에서 exit(0))
	printf("\nParent: Child finished. Final content in shared memory:\n%s\n", (char *)addr);

	// 메모리 매핑 해제
	printf("Parent: Inmapping memory...\n");
	if (munmap(addr, statbuf.st_size) == -1) {
		perror("munmap");
		exit(1);
	}
	printf("Parent: Memory unmapped. Exiting\n");

	return 0; // 부모 프로세스 정상 종료
}
