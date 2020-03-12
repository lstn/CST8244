#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "des.h"

void *init_state();
void *ready();
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
void exit_state();

controller_response_t response;
Person person;
int coid;
StateFunc statefunc;

int main(void) {
	puts("Hello World!!!"); /* prints Hello World!!! */
	return EXIT_SUCCESS;
}
