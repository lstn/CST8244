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

	person_t person_msg;
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
		rec = MsgReceive(chan, (void*)&person_msg, sizeof(person_msg), NULL);

		if (rec == 0) {
			switch (person_msg.state) {
				case ST_LEFT_SCAN:
				case ST_RIGHT_SCAN:
					printf("%s %d \n", outMessage[OUT_SCAN], person_msg.id);
					break;
				case ST_WEIGHT_SCALE:
					printf("%s %d \n", outMessage[OUT_WEIGHT_SCALE], person_msg.id);
					break;
				case ST_LEFT_OPEN:
					printf(outMessage[OUT_LEFT_OPEN]);
					break;
				case ST_RIGHT_OPEN:
					printf(outMessage[OUT_RIGHT_OPEN]);
					break;
				case ST_LEFT_CLOSED:
					printf(outMessage[OUT_LEFT_CLOSED]);
					break;
				case ST_RIGHT_CLOSED:
					printf(outMessage[OUT_RIGHT_CLOSED]);
					break;
				case ST_GUARD_RIGHT_LOCK:
					printf(outMessage[OUT_GUARD_RIGHT_LOCK]);
					break;
				case ST_GUARD_RIGHT_UNLOCK:
					printf(outMessage[OUT_GUARD_RIGHT_UNLOCK]);
					break;
				case ST_GUARD_LEFT_LOCK:
					printf(outMessage[OUT_GUARD_LEFT_LOCK]);
					break;
				case ST_GUARD_LEFT_UNLOCK:
					printf(outMessage[OUT_GUARD_LEFT_UNLOCK]);
					break;
				case ST_EXIT:
					printf(outMessage[OUT_EXIT]);
					printf(outMessage[OUT_STOP]);
					return EXIT_SUCCESS;
				default:
					printf("Invalid Command\n");
					break;
			}

		}
		// send answer back
		MsgReply(rec, EOK, &resp, sizeof(resp));
	}

	if (resp.statusCode != EOK) {
		printf("ERROR: %s", resp.errorMsg);
	}

	// destroy channel
	ChannelDestroy(chan);
	return EXIT_SUCCESS;
}
