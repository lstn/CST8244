#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <float.h>
#include <limits.h>

#include "../../des_controller/src/des.h"

int main(void) {

	person_t person;
	controller_response_t resp;
	int chan, rec;

	// create channel
	chan = ChannelCreate (0);
	if (chan == -1) {
		fprintf (stderr, "ERROR: Could not create channel.\n");
		exit (EXIT_FAILURE);
	}

	// print pid
	printf("The display is running as PID: %d\n", getpid());

	// loop
	while(1) {
		rec = MsgReceive(chan, (void*)&msg, sizeof(msg), NULL);

		if (rcvid == 0) {
			switch (person.state) {
				case ST_LEFT_SCAN:
					break;
				case ST_RIGHT_SCAN:
					break;
				case ST_WEIGHT_SCALE:
					break;
				case ST_LEFT_OPEN:
					break;
				case ST_RIGHT_OPEN:
					break;
				case ST_LEFT_CLOSED:
					break;
				case ST_RIGHT_CLOSED:
					break;
				case ST_GUARD_RIGHT_LOCK:
					break;
				case ST_GUARD_RIGHT_UNLOCK:
					break;
				case ST_GUARD_LEFT_LOCK:
					break;
				case ST_GUARD_LEFT_UNLOCK:
					break;
				case ST_EXIT:
					return EXIT_SUCCESS;
					break;
				default:
					printf("Invalid Command\n");
					break;
			}

		}
		// send answer back
		MsgReply(rec, EOK, &resp, sizeof(resp));
	}

	if (resp.error != EOK) {
		printf("ERROR: %s", resp.errorMsg);
	}

	// destroy channel
	ChannelDestroy(chan);
	return EXIT_SUCCESS;
}
