/*
 ** partA_sighandler.c -- handles SIGUSR1
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

void sig_handler(int sig);

volatile sig_atomic_t usr1Happened;


/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {

	struct sigaction act;
	sigset_t set;

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

	printf("PID = %d : Running...\n", getpid());

	while (!usr1Happened) {
		sleep(0.5);
	}

	printf("PID = %d : Exiting.\n", getpid());

	return EXIT_SUCCESS;
}

void sig_handler(int sig) {
	printf("PID = %d : Received USR1.\n", getpid());
	++usr1Happened;
}
