#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "des.h"

void show_usage(void);

void *init_state();
void *ready_state();
void *left_scan();
void *right_scan();
void *weight_scale();
void *left_open();
void *right_open();
void *left_close();
void *right_close();
void *guard_right_lock();
void *guard_right_unlock();
void *guard_left_lock();
void *guard_left_unlock();
void *exit_state();

controller_response_t response;
person_t person;
int conn;
StateFunc statefunc = init_state;

int main(int argc, char* argv[]) {

	pid_t dpid;

	if(argc != 2 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	dpid = atoi(argv[1]);

	puts("Hello World!!!"); /* prints Hello World!!! */
	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./des_controller <display-pid>\n");
}
