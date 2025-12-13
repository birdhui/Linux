#include <stdio.h>

int main() {
    int sum = 0;
    int i; // 초기화되지 않은 변수 (잠재적 버그)
    
    // 1부터 5까지 더하려고 함
    for (i = 1; i <= 5; i++) { 
        sum += i;
    }
    
    printf("1부터 5까지의 합은: %d\n", sum);
    
    // 일부러 넣은 버그: i 값을 잘못 사용
    if (i > 5) {
        printf("i가 예상보다 큽니다: %d\n", i); // 이 부분에서 i 값이 얼마일까요?
    }
    
    return 0;
}
