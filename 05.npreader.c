#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLINE 100

/*
 * read() 함수 설명:
 * #include <unistd.h>
 * ssize_t read( int fd, void *buf, size_t nbytes );
 *
 * 파일 읽기에 성공 시 바이트 수,
 * EOF 시 0, 실패 시 -1 리턴.
 * */


// 이름 있는 파이프(fd)로부터 한 줄(NULL 문자로 끝남)을 읽어 str에 저장
// 성공하면 0보다 큰 값(읽은 바이트 수) 리턴, EOF 만나면 0 리턴
int readLine(int fd, char *str) {

	int n;
	do {
		// 한 바이트씩 읽기
		n = read(fd, str, 1);
		// 읽기 실패(-1) 또는 EOF(0)가 아니고,
		// 읽은 문자가 NULL이 아니면 계속 읽음
	} while (n > 0 && *str++ != '\0');
	// 루프 종료 조건: n <= 0 (EOF 또는 오류)이거나 '\0' 문자를 읽었을 때
	
	// read()가 0보다 큰 값을 반환했을 때만 (즉, '\0'을 만나기 전에 EOF나 오류가 없었을 때만)
	// 성공(true)으로 간주하고 0보다 큰 값을 반환 (정확히는 마지막 read의 결과인 n)
	return (n > 0);
}

int main() {

	int fd;
	char str[MAXLINE];

	// 기존 파이프 파일이 있으면 삭제 (오류 무시)
	unlink("myPipe");

	// 이름 있는 파이프 생성 (권한: 소유자/그룹 읽기쓰기)
	if (mkfifo("myPipe", 0660) == -1) {
		perror("mkfifo");
		exit(1);
	}
	
	printf("Reader: Waiting for writer...\n");

	// 파이프를 읽기 전용으로 열기(Writer가 열 때까지 대기)
	
	// **** 바로 이 부분 *****
	// 아래의 코드를 통해 fd가 읽기 전용 접근 권한을 가진 파일임을 알 수 있음
	//
	//=> 운영체제는 O_RDONLY 플래그를 보고 "myPipe" 파일에 대한
	//    읽기 전용 접근 권한을 가진 파일 디스크립터(fd)를 반환한다.
	//
	// Writer가 쓰기용(O_WRONLY)으로 열 때까지 여기서 멈춰서 기다림(Bloking)
	fd = open("myPipe", O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}
	printf("Reader: Wirter connected.\n");

	// readLine 함수가 0(EOF)을 반환할 때까지 반복
	// readLine(fd, str) 호출
	//
	// readLine 함수의 동작
	// 전달받은 fd가 읽기용인지 쓰기용인지 검사 X
	// 그냥 그 fd를 가지고 "n = read(fd, str, 1);" 코드를 실행하여 읽기를 시도
	// fd가 읽기용으로 열렸기 때문에 read()함수는 정상적으로 작동한다.
	// 만약, 실수로 쓰기 전용(O_WRONLY)으로 열린 fd가 전달되었다면,
	// read()함수는 실패하고 -1을 반환하여
	// 위의 if문이 실행됨.
	while (readLine(fd, str)) {
		// 널 문자로 끝나는 완전한 메시지를 받으면 출력
		printf("Reader received: %s \n", str);
	}
	
	// 루프 종료 (EOF 수신) 후 파이프 닫기
	close(fd);
	printf("Reader: Pipe closed. Exiting.\n");
	return 0;
}
