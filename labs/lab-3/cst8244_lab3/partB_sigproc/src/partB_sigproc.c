/*
 ** partB_sigproc.c -- handles SIGUSR1
 *
 * Read:	http://beej.us/guide/bgipc/html/single/bgipc.html#signals
 * Source:	http://beej.us/guide/bgipc/examples/sigint.c
 *
 * Modified by: esti0011@algonquinlive.com
 *
 * Usage:
 *  From Momentics IDE, run program; notice PID; enter some text, but don't hit the enter key
 *  At Neutrino prompt, issue the command: kill -s SIGUSR1 <PID>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_handler(int sig);
void start_child(void);

volatile sig_atomic_t usr1Happened;
volatile pid_t ppid;


/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {

	struct sigaction act;
	sigset_t set;
	int numProc, status;
	pid_t pid;


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

	printf("Enter the number of children:  ");
	scanf ("%10d", &numProc);

	printf("PID = %d : Parent Running...\n", getpid());
	ppid = getpid();

	while (numProc > 0){
		--numProc;
		switch (pid = fork()) {
			case -1:
				perror("fork"); /* something went wrong */
				exit(1); /* parent exits */
			case 0:
				printf("PID = %d : Child Running...\n", getpid());
				while (!usr1Happened) {
					sleep(0.5);
				}
				printf("PID = %d : Child Exiting...\n", getpid());
				exit(EXIT_SUCCESS);
		}

	}


	do {
	    waitpid( pid, &status, 0 );
	} while( WIFEXITED( status ) == 0 );

	printf("PID = %d : Children finished, parent exiting.\n", getpid());
	return EXIT_SUCCESS;
}

void sig_handler(int sig) {
	if (getpid() != ppid) {
		printf("PID = %d : Child Received USR1.\n", getpid());
		++usr1Happened;
	}
}
