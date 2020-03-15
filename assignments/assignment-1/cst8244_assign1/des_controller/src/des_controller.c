#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <process.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "des.h"

void show_usage(void);

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

controller_response_t resp;
person_t person_msg;
int conn;
StateFunc statefunc = ready_state;

int main(int argc, char* argv[]) {

	pid_t dpid;
	int chan, rec;

	if(argc != 2 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	dpid = atoi(argv[1]);

	// create channel
	chan = ChannelCreate (0);
	if (chan == -1) {
		fprintf (stderr, "ERROR: Could not create channel.\n");
		exit (EXIT_FAILURE);
	}

	// connect to des_display
	conn = ConnectAttach(ND_LOCAL_NODE, dpid, 1, _NTO_SIDE_CHANNEL, 0);
	if(conn == -1) {
		fprintf (stderr, "ERROR: Could not connect to the des_display. Did you specify the right PID?\n");
		exit (EXIT_FAILURE);
	}

	// print pid
	printf("The controller is running as PID: %d \n",getpid());
	person_msg.id = 0;
	person_msg.weight = 0;
	person_msg.state = ST_READY;

	while(person_msg.state != ST_STOP) {
		rec = MsgReceive(chan, (void*)&person_msg, sizeof(person_msg), NULL);

		// printf("%d - %d - %d\n", person_msg.id, person_msg.weight, person_msg.state);
		if (person_msg.state == ST_EXIT) {
			exit_state();
		}

		statefunc = (StateFunc)(*statefunc)();

		// send answer back
		resp.statusCode = EOK;
		MsgReply(rec, EOK, &resp, sizeof(resp));
	}
	sleep(0.5);

	ChannelDestroy(chan);
	ConnectDetach(conn);
	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./des_controller <display-pid>\n");
}

void *ready_state() {

	if (person_msg.state == ST_LEFT_SCAN) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return left_scan;
	}

	else if (person_msg.state == ST_RIGHT_SCAN) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
		}
		return right_scan;
	}
	return ready_state;
}

void *left_scan() {
	if (person_msg.state == ST_GUARD_LEFT_UNLOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}

	return left_scan;
}

void *right_scan() {
	if (person_msg.state == ST_GUARD_RIGHT_UNLOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}
	return right_scan;
}

void *weight_scale() {
	if (person_msg.state == ST_LEFT_CLOSED) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
		}
		return left_close;
	} else if (person_msg.state == ST_RIGHT_CLOSED) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
		}
		return right_close;
	}
	return weight_scale;
}

void *left_open() {
	if (person_msg.state == ST_WEIGHT_SCALE) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return weight_scale;
	}
	return left_open;

	if (person_msg.state == ST_LEFT_CLOSED) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return left_close;
	}
	return left_open;
}

void *right_open() {
	if (!person_msg.weight) {
		if (person_msg.state == ST_WEIGHT_SCALE) {
			if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
				fprintf(stderr, "ERROR: Could not send message.\n");
				exit(EXIT_FAILURE);
			}
			return weight_scale;
		}
		return right_open;
	}

	if (person_msg.state == ST_RIGHT_CLOSED) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return right_close;
	}
	return right_open;
}

void *left_close() {
	if (person_msg.state == ST_GUARD_LEFT_LOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return guard_left_lock;
	}
	return left_close;
}

void *right_close() {
	if (person_msg.state == ST_GUARD_RIGHT_LOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return guard_right_lock;
	}
	return right_close;
}

void *guard_right_lock() {
	if (person_msg.state == ST_GUARD_LEFT_UNLOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}

	return guard_right_lock;
}

void *guard_right_unlock() {
	if (person_msg.state == ST_RIGHT_OPEN) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return right_open;
	}

	return guard_right_unlock;
}

void *guard_left_lock() {
	if (person_msg.state == ST_GUARD_RIGHT_UNLOCK) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}

	return guard_left_lock;
}

void *guard_left_unlock() {
	if (person_msg.state == ST_LEFT_OPEN) {
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}
		return left_open;
	}

	return guard_left_unlock;
}

void *exit_state() {
	person_msg.state = ST_EXIT;
	if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
		fprintf(stderr, "ERROR: Could not send message.\n");
		exit(EXIT_FAILURE);
	}
	sleep(5);
	person_msg.state = ST_STOP;
	return exit_state;
}

