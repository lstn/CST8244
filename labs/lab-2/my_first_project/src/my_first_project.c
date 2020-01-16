#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
	puts("Hello World from QNX Neutrino RTOS!!!"); /* prints Hello World!!! */

	puts("\n@author Lucas Estienne (esti0011@algonquinlive.com)");
	puts("Something unique about myself");
	puts("");

	printf("\n");
	printf("My PID is: %d\n", getpid());
	printf("My Parent's PID is: %d\n", getppid());
	printf("\n");

	puts("Enter 'q' to quit: ");
	getchar();

	return EXIT_SUCCESS;
}
