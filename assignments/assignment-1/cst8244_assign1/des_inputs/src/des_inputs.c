#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <float.h>
#include <limits.h>

#include "../../des_controller/src/des.h"
void show_usage(void);

int main(int argc, char* argv[]) {

	pid_t cpid;
	int conn;
	person_t person_msg;
	controller_response_t resp;
	char event[128];

	if(argc != 2 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	cpid = atoi(argv[1]);

	// connect to des_controller
	conn = ConnectAttach(ND_LOCAL_NODE, cpid, 1, _NTO_SIDE_CHANNEL, 0);
	if(conn == -1) {
		fprintf (stderr, "ERROR: Could not connect to the des_controller. Did you specify the right PID?\n");
		exit (EXIT_FAILURE);
	}

	while(1) {
		printf("Enter the event type (ls = left scan, rs = right scan, ws = weight scale, \n"
					"lo = left open, ro = right open, lc = left closed, rc = right closed, \n"
					"gru = guard right unlock, grl = guard right lock, gll = guard left lock, \n"
					"glu = guard left unlock, exit = exit programs) \n");

		scanf("%s", event);
		// validate we have a response
		if (sizeof(resp) == 0) {
			fprintf(stderr, "ERROR: Got a zero-length response from the server.\n");
			exit(EXIT_FAILURE);
		}

		if (strcmp(event, inMessage[IN_LEFT_SCAN]) == 0){
			person_msg.state = ST_LEFT_SCAN;
			printf("Enter the Person's ID: ");
			scanf("%d", &person_msg.id);
		} else if (strcmp(event, inMessage[IN_RIGHT_SCAN]) == 0){
			person_msg.state = ST_RIGHT_SCAN;
			printf("Enter the Person's ID: ");
			scanf("%d", &person_msg.id);
		} else if (strcmp(event, inMessage[IN_WEIGHT_SCALE]) == 0) {
			printf("Enter the Person's Weight: ");
			scanf("%d", &person_msg.weight);
		} else if (strcmp(event, inMessage[IN_LEFT_OPEN]) == 0) {
			person_msg.state = ST_LEFT_OPEN;
		} else if (strcmp(event, inMessage[IN_RIGHT_OPEN]) == 0) {
			person_msg.state = ST_RIGHT_OPEN;
		} else if (strcmp(event, inMessage[IN_LEFT_CLOSED]) == 0) {
			person_msg.state = ST_LEFT_CLOSED;
		} else if (strcmp(event, inMessage[IN_RIGHT_CLOSED]) == 0) {
			person_msg.state = ST_RIGHT_CLOSED;
		} else if (strcmp(event, inMessage[IN_GUARD_RIGHT_LOCK]) == 0) {
			person_msg.state = ST_GUARD_RIGHT_LOCK;
		} else if (strcmp(event, inMessage[IN_GUARD_RIGHT_UNLOCK]) == 0) {
			person_msg.state = ST_GUARD_RIGHT_UNLOCK;
		} else if (strcmp(event, inMessage[IN_GUARD_LEFT_LOCK]) == 0) {
			person_msg.state = ST_GUARD_LEFT_LOCK;
		} else if (strcmp(event, inMessage[IN_GUARD_LEFT_UNLOCK]) == 0) {
			person_msg.state = ST_GUARD_LEFT_UNLOCK;
		} else if (strcmp(event, inMessage[IN_EXIT]) == 0) {
			person_msg.state = ST_EXIT;
		}

		// send message
		if (MsgSend(conn, &person_msg, sizeof(person_msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}


		// did we get something other than EOK?
		if (resp.statusCode != EOK) {
			printf("ERROR: Message from server: %s", resp.errorMsg);
		}
	}

	// disconnect from calc_server
	ConnectDetach(conn);
	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./des_inputs <controller-pid>\n");
}
