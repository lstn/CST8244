/*
 ** thread_factory.c -- spawns threads
 *
 *
 * Author: esti0011@algonquinlive.com
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

void sig_handler(int sig);
void* do_child_thread(void *arg);

volatile sig_atomic_t usr1Happened;
sem_t *sem;


/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {

	struct sigaction act;
	sigset_t set;
	int numThreads;
	pthread_attr_t attr;


	sigemptyset( &set );
	sigaddset( &set, SIGUSR1 );

	act.sa_handler = sig_handler;
	act.sa_mask = set;
	act.sa_flags = 0; // or SA_RESTART
	sigemptyset(&act.sa_mask);

	if (sigaction(SIGUSR1, &act, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	sem = sem_open("coolsem", O_CREAT, 0600, 0);
	if(sem == SEM_FAILED) {
		printf("sem_open");
		exit(1);
	}

	printf("Enter the number of threads to create:  ");
	scanf ("%10d", &numThreads);
	pthread_t threads[numThreads];


	for (int i = 0; i < numThreads; i++){
		pthread_attr_init( &attr );
		pthread_create( &threads[i], &attr, &do_child_thread, NULL);
		pthread_attr_destroy(&attr);
	}

	for(int j = 0; j < numThreads; j++) {
		pthread_join(threads[j], NULL);
	}

	while(!usr1Happened) {
		sleep(3);
	}

	sem_close(sem);
	return EXIT_SUCCESS;
}

void sig_handler(int sig) {
	printf("PID = %d : Received USR1.\n", getpid());
	sem_close(sem);
	++usr1Happened;
}

void* do_child_thread(void *arg) {
   printf( "Child thread %d created\n", pthread_self() );
   do {
	   sem_wait(sem);
	   printf("Child thread %d unblocked\n", pthread_self());
	   sleep(3);
   } while(!usr1Happened);
   return EXIT_SUCCESS;
}
