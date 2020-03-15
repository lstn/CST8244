#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <float.h>
#include <limits.h>
#include <unistd.h>

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
	while(person_msg.state != ST_STOP) {
		rec = MsgReceive(chan, (void*)&person_msg, sizeof(person_msg), NULL);

		switch (person_msg.state) {
			case ST_LEFT_SCAN:
			case ST_RIGHT_SCAN:
				printf("%s %d \n", outMessage[OUT_SCAN], person_msg.id);
				break;
			case ST_WEIGHT_SCALE:
				printf("%s %d \n", outMessage[OUT_WEIGHT_SCALE], person_msg.weight);
				break;
			case ST_LEFT_OPEN:
				printf("%s \n", outMessage[OUT_LEFT_OPEN]);
				break;
			case ST_RIGHT_OPEN:
				printf("%s \n", outMessage[OUT_RIGHT_OPEN]);
				break;
			case ST_LEFT_CLOSED:
				printf("%s \n", outMessage[OUT_LEFT_CLOSED]);
				break;
			case ST_RIGHT_CLOSED:
				printf("%s \n", outMessage[OUT_RIGHT_CLOSED]);
				break;
			case ST_GUARD_RIGHT_LOCK:
				printf("%s \n", outMessage[OUT_GUARD_RIGHT_LOCK]);
				break;
			case ST_GUARD_RIGHT_UNLOCK:
				printf("%s \n", outMessage[OUT_GUARD_RIGHT_UNLOCK]);
				break;
			case ST_GUARD_LEFT_LOCK:
				printf("%s \n", outMessage[OUT_GUARD_LEFT_LOCK]);
				break;
			case ST_GUARD_LEFT_UNLOCK:
				printf("%s \n", outMessage[OUT_GUARD_LEFT_UNLOCK]);
				break;
			case ST_EXIT:
				printf("%s \n", outMessage[OUT_EXIT]);
				printf(outMessage[OUT_STOP]);
				break;
			default:
				printf("Invalid input\n");
				break;
		}

		// send answer back
		MsgReply(rec, EOK, &resp, sizeof(resp));
	}
	sleep(0.5);

	if (resp.statusCode != EOK) {
		printf("ERROR: %s", resp.errorMsg);
	}

	// destroy channel
	ChannelDestroy(chan);
	return EXIT_SUCCESS;
}
