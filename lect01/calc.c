#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		return 1;
	}

	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[3]);
	char op = argv[2][0];
	int result;

	if (op == '+') {
		result = num1 + num2;
		printf("%d %c %d = %d\n", num1, op, num2, result);
	} else if (op == 'x') {
		result = num1 * num2;
		printf("%d %c %d = %d\n", num1, op, num2, result);
	} else if (op == '-') {
		result = num1 - num2;
		printf("%d %c %d = %d\n", num1, op, num2, result);
	}

	return 0;
}
