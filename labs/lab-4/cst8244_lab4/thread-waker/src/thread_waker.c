/*
 ** thread_waker.c -- wakes threads
 *
 *
 * Author: esti0011@algonquinlive.com
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *sem;

int main(void) {

	int numThreads;

	sem = sem_open("coolsem",O_CREAT,0644,1);
	if(sem == SEM_FAILED) {
		printf("sem_open");
		exit(1);
	}

	printf("thread-waker PID : %d.\n", getpid());

	do {
		printf("How many threads would you like to wake up? (enter 0 to exit): \n");
		scanf("%d", &numThreads);

		for(int i = 0; i < numThreads; i++) {
			sem_post(sem);
		}

	} while(numThreads);

	sem_close(sem);

	return EXIT_SUCCESS;
}
